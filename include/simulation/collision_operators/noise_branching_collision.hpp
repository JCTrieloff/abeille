/*
 * Abeille Monte Carlo Code
 * Copyright 2019-2023, Hunter Belanger
 * Copyright 2021-2022, Commissariat à l'Energie Atomique et aux Energies
 * Alternatives
 *
 * hunter.belanger@gmail.com
 *
 * This file is part of the Abeille Monte Carlo code (Abeille).
 *
 * Abeille is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Abeille is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Abeille. If not, see <https://www.gnu.org/licenses/>.
 *
 * */
#ifndef NOISE_BRANCHING_COLLISION_H
#define NOISE_BRANCHING_COLLISION_H

#include <materials/material.hpp>
#include <simulation/collision_operators/collision_operator.hpp>
#include <simulation/collision_operators/noise_fission_operator.hpp>
#include <simulation/particle.hpp>
#include <tallies/tallies.hpp>
#include <utils/russian_roulette.hpp>

#include <cmath>
#include <complex>

class NoiseBranchingCollision {
 public:
  NoiseBranchingCollision() = default;

  void collision(Particle& p, MaterialHelper& mat,
                 ThreadLocalScores& thread_scores) const {
    // Sample a nuclide for the collision
    std::pair<const Nuclide*, MicroXSs> nuclide_info = mat.sample_nuclide(p.E(), p.rng);
    const Nuclide* nuclide = nuclide_info.first;
    MicroXSs xs = nuclide_info.second;

    // Get noise parameters. We assume that the optional must be filled here
    const double eta = mat.noise_params()->eta;
    const double omega = mat.noise_params()->omega;

    // Score kabs
    const double k_abs_scr = p.wgt() * xs.nu_total * xs.fission / xs.total;
    thread_scores.k_abs_score += k_abs_scr;

    // Make all fission neutrons
    fission_operator.fission(p, xs, *nuclide);

    // Make a copy of the particle when doing noise transport, modifying
    // the weight of the copy as well
    noise_copy(p, xs, eta);

    // Implicit capture
    p.set_weight(p.wgt() * (1. - (xs.absorption + xs.noise_copy) / xs.total));
    p.set_weight2(p.wgt2() * (1. - (xs.absorption + xs.noise_copy) / xs.total));

    // Roulette
    russian_roulette(p);

    // Scatter particle
    if (p.is_alive()) {
      // Perform scatter with nuclide
      scatter(p, *nuclide, xs);

      if (p.E() < settings::min_energy) {
        p.kill();
      }
    }
  }

 private:
  NoiseFissionOperator fission_operator;

  void scatter(Particle& p, const Nuclide& nuclide, const MicroXSs& xs) const {
    ScatterInfo sinfo = nuclide.sample_scatter(p.E(), p.u(), xs, p.rng);

    if (sinfo.yield == 0.) {
      // this scatter had a yield of 0... we have no choice but to kill
      // the particle now.
      p.kill();
      return;
    }

    // If yield is an integral quantity, we produce secondary neutrons in the
    // secondary bank, to keep weights from being too high and causing
    // variance issues.
    if (std::floor(sinfo.yield) == sinfo.yield && sinfo.yield != 1.) {
      int n = static_cast<int>(sinfo.yield) - 1;
      for (int i = 0; i < n; i++) {
        // Sample outgoing info
        ScatterInfo ninfo = nuclide.sample_scatter_mt(sinfo.mt, p.E(), p.u(),
                                                      xs.energy_index, p.rng);

        p.make_secondary(ninfo.direction, ninfo.energy, p.wgt(), p.wgt2());
      }

      // This is set to 1 so that we have the correct weight for the last
      // neutron that we make outside this if block
      sinfo.yield = 1.;
    }

    p.set_direction(sinfo.direction);
    p.set_energy(sinfo.energy);
    p.set_weight(p.wgt() * sinfo.yield);
    p.set_weight2(p.wgt2() * sinfo.yield);
  }

  void noise_copy(Particle& p, const MicroXSs& xs, double eta) const {
    std::complex<double> weight_copy{p.wgt(), p.wgt2()};
    if (xs.noise_copy / xs.total + RNG::rand(p.rng) >= 1.) {
      std::complex<double> yield{1., -1. / eta};
      weight_copy *= yield;
      p.make_secondary(p.u(), p.E(), weight_copy.real(), weight_copy.imag());
    }
  }
};

#endif
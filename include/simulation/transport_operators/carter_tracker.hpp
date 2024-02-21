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
#ifndef CARTER_TRACKER_H
#define CARTER_TRACKER_H

#include <tallies/tallies.hpp>

#include <PapillonNDL/cross_section.hpp>
#include <PapillonNDL/energy_grid.hpp>

#include <memory>
#include <vector>

class CarterTracker {
 public:
  CarterTracker(const std::vector<double>& mgxs);
  CarterTracker(const std::vector<double>& energy,
                const std::vector<double>& xs);

  void transport(Particle& p, Tracker& trkr, MaterialHelper& mat,
                 ThreadLocalScores& thread_scores) const;

  bool exact_cancellation_compatible() const { return true; }

  bool track_length_compatible() const { return false; }

  void write_output_info(const std::string& base) const;

 private:
  std::shared_ptr<pndl::EnergyGrid> EGrid;
  std::shared_ptr<pndl::CrossSection> Esmp;
};

#endif

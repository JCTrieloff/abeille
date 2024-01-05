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
#ifndef MODIFIED_FIXED_SOURCE_H
#define MODIFIED_FIXED_SOURCE_H

#include <simulation/simulation.hpp>

class ModifiedFixedSource : public Simulation {
 public:
  ModifiedFixedSource(std::shared_ptr<IParticleMover> i_pm,
                      std::vector<std::shared_ptr<Source>> src)
      : Simulation(i_pm, src) {}
  ~ModifiedFixedSource() = default;

  void initialize() override final;
  void run() override final;
  void premature_kill() override final;

  void set_nparticles(std::size_t np) { nparticles = np; }
  void set_nbatches(std::size_t nb) { nbatches = nb; }
  void add_source(std::shared_ptr<Source> src);

 private:
  std::vector<std::shared_ptr<Source>> sources{};
  std::size_t batch = 0;
  std::size_t nparticles, nbatches;

  void check_time(std::size_t batch);
  bool out_of_time(std::size_t batch);
  void print_header();
  void generation_output(std::size_t batch);
};

#endif

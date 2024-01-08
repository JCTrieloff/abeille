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
#ifndef SURFACE_TRACKER_H
#define SURFACE_TRACKER_H

#include <materials/material_helper.hpp>
#include <simulation/particle.hpp>
#include <simulation/tracker.hpp>
#include <tallies/tallies.hpp>

#include <memory>

class SurfaceTracker {
 public:
  SurfaceTracker() = default;

  void transport(Particle& p, Tracker& trkr, MaterialHelper& mat,
                 ThreadLocalScores& thread_scores) const;

  bool exact_cancellation_compatible() const { return false; }

  bool track_length_compatible() const { return true; }

  void write_output_info(const std::string& base) const;
};

#endif
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
#ifndef SURFACE_H
#define SURFACE_H

#include <utils/direction.hpp>
#include <utils/position.hpp>

#include <yaml-cpp/yaml.h>

#include <cstdint>
#include <string>
#include <memory>

enum BoundaryType { Vacuum, Reflective, Normal };

class Surface {
 public:
  Surface(BoundaryType bound, uint32_t i_id, std::string i_name);
  virtual ~Surface() = default;

  virtual int sign(const Position& r, const Direction& u) const = 0;

  virtual double distance(const Position& r, const Direction& u,
                          bool on_surf) const = 0;

  virtual Direction norm(const Position& r) const = 0;

  BoundaryType boundary() const;
  uint32_t id() const;
  std::string name() const;

 protected:
  BoundaryType boundary_;
  uint32_t id_;
  std::string name_;

};  // Surface

// Makes any type of surface from a surface yaml node
std::shared_ptr<Surface> make_surface(const YAML::Node& surface_node);

#endif

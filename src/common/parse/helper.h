/*
 * PpM - Pong Plus More - A pong clone full of surprises written with C++11.
 * Copyright (C) 2013  Luke San Antonio
 *
 * You can contact me (Luke San Antonio) at lukesanantonio@gmail.com!
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <array>
#include <tuple>

#define DECLARE_PROPERTY_VALUES(size, ...)\
  constexpr static const int property_values_size = size;\
  constexpr static const std::array<const char*, property_values_size>\
  property_values = {__VA_ARGS__}
#define DEFINE_PROPERTY_VALUES(type)\
  constexpr const std::array<const char*, type::property_values_size>\
  type::property_values

#define DEFINE_TEMPLATE_PROPERTY_VALUES(type)\
  template <typename T>\
  constexpr const std::array<const char*, type<T>::property_values_size>\
  type<T>::property_values

#define DECLARE_PROPERTIES_TUPLE(...)\
  using properties_tuple = std::tuple<__VA_ARGS__>
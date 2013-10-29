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
#include "ConcreteServer.h"
#include "common/center.hpp"
namespace pong
{
  ConcreteServer::ConcreteServer(uint16_t width, uint16_t height) noexcept :
                     first_paddle_({0, {0, 0}, 200, 30}, 0),
                     second_paddle_({0, {0, static_cast<double>(height - 30)},
                                     200, 30}, 0),
                     ball_({1, {static_cast<double>(width / 2),
                               static_cast<double>(height / 2)}, 25},
                           {0,1}){}

  PaddleID ConcreteServer::makePaddle()
  {
    //This horrible function returns 1 then 2 then 0.
    //First client gets the first paddle.
    //Second client gets the second paddle.
    //Third+ clients get no paddle.
    if(id_impl_ == 1) std::get<0>(this->first_paddle_).id = 1;
    else if(id_impl_ == 2) std::get<0>(this->second_paddle_).id = 2;
    else return 0;
    return id_impl_++;
  }
  void ConcreteServer::setPaddleDestination(PaddleID id,
                                            decltype(Paddle::pos.x) x) noexcept
  {
    //A kind of cool, hidden feature of using two ifs rather than an if-else:
    //If neither paddle has been claimed by a client, one can make both move
    //to the same place at the same time! Usefullness: None. Reason: Crap
    //design with this paddle business.
    if(std::get<0>(this->first_paddle_).id == id)
    {
      std::get<1>(this->first_paddle_) = x;
    }
    if(std::get<0>(this->second_paddle_).id == id)
    {
      std::get<1>(this->second_paddle_) = x;
    }
  }
  std::vector<Paddle> ConcreteServer::paddles() const noexcept
  {
    return {std::get<0>(this->first_paddle_),
            std::get<0>(this->second_paddle_)};
  }
  std::vector<Ball> ConcreteServer::balls() const noexcept
  {
    return {std::get<0>(this->ball_)};
  }
  void ConcreteServer::step()
  {
    std::get<0>(this->first_paddle_).pos.x = std::get<1>(this->first_paddle_);
    std::get<0>(this->second_paddle_).pos.x =
                                             std::get<1>(this->second_paddle_);
    std::get<0>(this->ball_).pos.x += std::get<1>(this->ball_).x;
    std::get<0>(this->ball_).pos.y += std::get<1>(this->ball_).y;
  }
};

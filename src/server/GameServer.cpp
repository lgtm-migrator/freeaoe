/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  <copyright holder> <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "GameServer.h"
#include <communication/TunnelToClient.h>
#include <mechanics/Unit.h>
#include <data/DataManager.h>

GameServer::GameServer()
{

}

GameServer::GameServer(const GameServer& other)
{

}

GameServer::~GameServer()
{

}

void GameServer::addClient(TunnelToClient* client)
{
  client_ = client;
}

void GameServer::update()
{
  while (client_->commandAvailable())
  {
    client_->getCommand()->execute(this);
  }
}

/*
Unit* GameServer::createUnit()
{
  Unit *unit = new Unit(unit_id_counter_ ++);
  units_[unit_id_counter_] = unit;
  
  return unit;
}
*/

bool GameServer::spawnUnit(void* player, sf::Uint32 unit_id_, sf::Uint32 x_pos, sf::Uint32 y_pos)
{
  Unit *unit = new Unit(unit_id_counter_ ++);
  units_[unit_id_counter_] = unit;
  
  unit->setPos(x_pos, y_pos);
  
  unit->setData(DataManager::Inst()->getUnit(unit_id_));
}


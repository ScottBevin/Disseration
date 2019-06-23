/**************************************************************************************************
 *
 *	XCS Crowd Simulation Software
 *	Author: Scott Bevin
 *	Web: www.gameXcore.co.uk
 *	Email: scott@gameXcore.co.uk
 *	All code (c)2010 Scott Bevin, all rights reserved.
 *	
 *	This software is part of the disssertation work entitled "Real time crowd simulations incorporating individual
 *	agent personality models and group interactions" submitted to The University of Bolton by Scott Bevin in partial
 *	fulfilment of the requirements of The University of Bolton for the degree of Bachelor of Science in May 2010.
 *	
 **************************************************************************************************/

 /************************************************************************

 This file is part of "Real time crowd simulations incorporating individual agent personality models and group interactions".

 "Real time crowd simulations incorporating individual agent personality 
 models and group interactions" is free software: you can redistribute 
 it and/or modify it under the terms of the GNU General Public License 
 as published by the Free Software Foundation, either version 3 of the 
 License, or (at your option) any later version.

 "Real time crowd simulations incorporating individual agent personality models and group interactions" is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with "Real time crowd simulations incorporating individual agent personality models and group interactions".  
 If not, see <http://www.gnu.org/licenses/>.

************************************************************************/

// All code (c)2010 Scott Bevin, all rights reserved.

#pragma once

#include <map>
#include <string>
#include <cassert>

namespace XF
{
	class GameEntity;
	class EntityComponentMakerBase;
	class EntityComponent;

	/**
		Factory to create agent components
		@author Scott Bevin
		*/
	class EntityComponentFactory
	{
	private:
		/**
			Returns the map which holds all the makers
			*/
		static std::map<std::string, EntityComponentMakerBase*>* MakersList();

	public:
		EntityComponentFactory();
		~EntityComponentFactory();

		/**
			Registers a maker into the list
			*/
		static void RegisterMaker(std::string id, EntityComponentMakerBase* maker);

		/**
			Unregisters a maker form the list
			*/
		static void UnregisterMaker(std::string id);

		/**
			Gets the correct maker and uses it to create a new EntityComponent
			*/
		static EntityComponent* CreateComponent(std::string id, GameEntity* owner);
	};
}

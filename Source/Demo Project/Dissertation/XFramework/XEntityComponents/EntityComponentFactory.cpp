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

#include "EntityComponentFactory.h"
#include "EntityComponentMaker.h"

#include "XUtilities/Logger.h"

namespace XF
{
	EntityComponentFactory::EntityComponentFactory()
	{
	}

	EntityComponentFactory::~EntityComponentFactory()
	{
	}

	void EntityComponentFactory::RegisterMaker(std::string id, EntityComponentMakerBase* maker)
	{
		std::map<std::string, EntityComponentMakerBase*> *makersList = MakersList();

		assert(makersList->find(id) == makersList->end());

		makersList->insert(std::make_pair(id, maker));


	}

	void EntityComponentFactory::UnregisterMaker(std::string id)
	{
		std::map<std::string, EntityComponentMakerBase*> *makersList = MakersList();

		assert(makersList->find(id) != makersList->end());

		makersList->erase(id);

	}

	EntityComponent* EntityComponentFactory::CreateComponent(std::string id, GameEntity* owner)
	{
		std::map<std::string, EntityComponentMakerBase*> *makersList = MakersList();

		std::map<std::string, EntityComponentMakerBase*>::iterator it = makersList->find(id);

		if(it == makersList->end())
		{
			SSELOG << "ERROR: Component not registered: " << id; // chris, the assert crashed it in xutility so i thought this might be more helpful
			return NULL;
		}

		return (*it).second->MakeComponent(owner);
	}

	std::map<std::string, EntityComponentMakerBase*>* EntityComponentFactory::MakersList()
	{
		static std::map<std::string, EntityComponentMakerBase*> *makersList 
			= new std::map<std::string, EntityComponentMakerBase*>();

		return makersList;
	}
}

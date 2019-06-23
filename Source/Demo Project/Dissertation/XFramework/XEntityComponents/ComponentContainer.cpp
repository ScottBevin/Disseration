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


#include "ComponentContainer.h"
#include "EntityComponent.h"
#include "EntityComponentFactory.h"

namespace XF
{
	ComponentContainer::ComponentContainer() : mComponentsInitialized(false)
	{
	}

	ComponentContainer::ComponentContainer(ComponentContainer &copy) : mComponentsInitialized(false)
	{
		for(unsigned int i = 0; i < copy.mComponents.size(); ++i)
		{
			EntityComponent* thisComp = copy.mComponents[i];

			EntityComponent* newComp = EntityComponentFactory::CreateComponent(thisComp->ComponentName(), (GameEntity*)this);

			newComp->CopyPropertys(thisComp);

			AddComponent(newComp);
		}
	}

	ComponentContainer::~ComponentContainer()
	{
		for(unsigned int i = 0; i < mComponents.size(); ++i)
		{
			EntityComponent* thisComp = mComponents[i];
			delete mComponents[i];
		}
	}

	void ComponentContainer::AddComponent(XF::EntityComponent *newComponent)
	{
		mComponents.push_back(newComponent);

		if(mComponentsInitialized)
			newComponent->Initialize();
	}

	void ComponentContainer::UpdateComponents(double dT)
	{
		std::list<int> killList;

		for(unsigned int i = 0; i < mComponents.size(); ++i)
		{
			EntityComponent* thisComp = mComponents[i];

			if(!thisComp->ShouldKill())
				thisComp->Update(dT);
			else
			{				
				mComponents[i] = mComponents[mComponents.size() - 1];

				mComponents.pop_back();

				delete thisComp;
			}
		}
	}

	void ComponentContainer::HandleInputComponents(double dT, XF::Input* input)
	{
		for(unsigned int i = 0; i < mComponents.size(); ++i)
		{
			EntityComponent* thisComp = mComponents[i];

			if(!thisComp->ShouldKill())
				thisComp->HandleInput(dT, input);
		}
	}

	bool ComponentContainer::MessageComponents(const XF::Message& message)
	{
		bool messageHandled = false;

		int size = (int)mComponents.size();

		for(int i = 0; i < size; ++i)
		{
			EntityComponent* thisComp = mComponents[i];

			if(!thisComp->ShouldKill())
				messageHandled = (thisComp->HandleMessage(message) || messageHandled); //this will make it return true if one returns true, which one do we really want??
		}

		return messageHandled;
	}

	void ComponentContainer::InitializeComponents()
	{
		mComponentsInitialized = true;

		for(unsigned int i = 0; i < mComponents.size(); ++i)
		{
			EntityComponent* thisComp = mComponents[i];

			thisComp->Initialize();
		}
	}
}
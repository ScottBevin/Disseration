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
#include "XPropertys/PropertyContainer.h"
#include "XGameEntities/GameEntity.h"
#include "XMessaging/Message.h"

namespace XF
{
	class EntityComponent: public PropertyContainer
	{
	private:
		std::string mComponentName;
		bool mInitialized;
		bool mKillComponent;

		std::string mErrorMessage;

	protected:
		GameEntity *mOwner;

		void SetErrorMessage(std::string val) { mErrorMessage = val; }

		virtual bool OnInitialize(){return true;}
		virtual void OnUpdate(double dT){}
		virtual void OnHandleInput(double dT, XF::Input *input){}
		virtual bool OnHandleMessage(const XF::Message& message){return false;}
		virtual void OnShutdown(){}

	public:
		void MessageSelf(XF::MessageSubject::ENUM sub, void* data = NULL) //chris
		{
			mOwner->MessageSelf(sub,  data);
		}
		EntityComponent(GameEntity *owner, std::string idName);
		virtual ~EntityComponent();

		void Initialize();
		void Update(double dT){ OnUpdate(dT); }
		void HandleInput(double dT, XF::Input *input){OnHandleInput(dT, input);}
		bool HandleMessage(const XF::Message& message){ return OnHandleMessage(message); }
		void Shutdown()
		{
			if(mInitialized)//chris
				OnShutdown(); 
			mInitialized = false;
		}

		std::string ComponentName(){return mComponentName;}

		GameEntity* Owner(){ return mOwner; }
		bool Initialized(){return mInitialized;}

		void CopyPropertys(EntityComponent *component);

		void Kill(){mKillComponent = true;}
		bool ShouldKill(){return mKillComponent;}
	};
}

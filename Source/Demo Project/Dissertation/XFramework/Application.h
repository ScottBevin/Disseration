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
#include <list>
#include "XInput\Input.h"
#include "XSystemComponents\XOgre\OgreRenderer.h"
#include "XTiming\Timer.h"

namespace XF
{
	class SystemComponent;

	class Application
	{
	protected:
		virtual bool OnInitialise(){return true;}
		virtual void OnHandleInput(double dT, Input *input){}
		virtual void OnUpdate(double dT){}
		virtual void OnShutDown(){}
		float mDeltaScale;
	private:
		
		bool mQuitGame;
		OgreRenderer *mOgre;

		Timer *mTimer;
		Input *mInput;

		typedef std::list<SystemComponent*> GameComponentList;
		typedef std::list<SystemComponent*>::iterator GameComponentListIt;
		typedef std::list<SystemComponent*>::reverse_iterator GameComponentListRevIt;

		typedef std::map<std::string, SystemComponent*> GameComponentMap;
		typedef std::map<std::string, SystemComponent*>::iterator GameComponentMapIt;

		GameComponentMap *mGameComponents;
		GameComponentList mGameComponentsList;

		bool InitializeFramework();

		void InitializeComponents();

		bool InitializeGraphics(std::string windowTitle, std::string resourceFile, bool initialiseResources);

		void AddDefaultComponents();

	public:
		Application();
		virtual ~Application();

		bool Initialize(std::string windowTitle, std::string resourceFile, bool initialiseResources);

		void Run();
		void ShutDown();

		void QuitGame() { mQuitGame = true; }

		void AddComponent(SystemComponent *component, std::string componentName);
		SystemComponent* GetComponent(std::string componentName);
		void RemoveComponent(SystemComponent *component);
		void RemoveComponent(std::string componentName){RemoveComponent(GetComponent(componentName));}

		OgreRenderer *GetRenderer(){return mOgre;}
	};
}

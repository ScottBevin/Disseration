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

#include "Application.h"
#include "XSystemComponents\SystemComponent.h"
#include "XTiming\WindowsTimer.h"
#include "XInput\OISInput.h"
#include "XUtilities\Logger.h"
#include "XMessaging\PostOffice.h"
#include "XSystemComponents\XOgre\RendererGameComponent.h"
#include "XSystemComponents\XCEGui\CEGUIComponent.h"
#include "XUtilities\GameSettings.h"
#include "XGameEntities\GameEntityFactory.h"
#include "XSystemComponents\XDebugging\DebugVariableTracker.h"
#include "XUtilities/QuickAndDirtyOgreDebugRenderer.h"
#include "XUtilities/Random.h"

namespace XF
{
#ifndef X_CONSOLE_POSSIBLE
#define X_CONSOLE_POSSIBLE
#endif

	Application::Application()
	{
		mQuitGame = false;
		mOgre = NULL;
		mTimer = NULL;
		mInput = NULL;
		mGameComponents = new GameComponentMap();
		mDeltaScale = 1.0f;
	}

	Application::~Application()
	{
	}
	
	bool Application::InitializeFramework()
	{
		mTimer = new WindowsTimer();

		size_t windowHnd;

		Random::Seed();

		mOgre->GetRenderWindow()->getCustomAttribute("WINDOW", &windowHnd);
		mInput = new OISInput(windowHnd);	
		mInput->Initialize();

		Logger::GetInstance()->Log("XApplicaion: framework initialized");
		return true;
	}

	bool Application::InitializeGraphics(std::string windowTitle, std::string resourceFile, bool initialiseResources)
	{

		mOgre = new OgreRenderer();

		if(!mOgre->Initialize(windowTitle, resourceFile)) 
		{
			Logger::GetInstance()->Log("XApplicaion: Ogre setup failed to initialize");
			return false;
		}

		if(initialiseResources)
			mOgre->InitializeAllResources(5);

		Logger::GetInstance()->Log("XApplicaion: Graphics initialized");
		return true;
	}

	void Application::InitializeComponents()
	{
		QuickAndDirtyOgreDebugRenderer::InitializeStaticVersion(mOgre);

		SSELOG << "XApplication: Initializing components: ";

		GameComponentListIt it;

		for(it = mGameComponentsList.begin(); it != mGameComponentsList.end(); ++it)
		{
			SystemComponent *comp = (*it);

			ILOG << "XApplication: Init component: " << typeid(*comp).name();

			if(!comp->Initialised())
				comp->Initialise();
		}
	}

	void Application::Run()
	{
		while(!mQuitGame)
		{
			//Ogre::WindowEventUtilities::messagePump(); //enables window going in/out of focus (Chris)

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MSG msg;
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
			{
				 if (msg.message == WM_QUIT)
					 this->ShutDown();
				 else 
				 {
					 TranslateMessage(&msg);
					 DispatchMessage(&msg);
				 }
			}
#endif

			mTimer->update();

			double dT = mTimer->getDeltaTime() * mDeltaScale;
			
			GameComponentMapIt it;

			mInput->Update();

			QuickAndDirtyOgreDebugRenderer::GetInstance()->Begin();

			OnHandleInput(dT, mInput);

			for(it = mGameComponents->begin(); it != mGameComponents->end(); ++it)
				(*it).second->HandleInput(dT, mInput);

			OnUpdate(dT);

			for(it = mGameComponents->begin(); it != mGameComponents->end(); ++it)
				(*it).second->Update(dT);

			QuickAndDirtyOgreDebugRenderer::GetInstance()->End();

			mOgre->GetRoot()->renderOneFrame();			
		}

		SSELOG << "XApplication: Exiting Run Loop";
	}

	void Application::ShutDown()
	{
		SSELOG << "Shutting down XFramework";

		delete mTimer;
		delete mInput;

		GameComponentListRevIt it;

		for(it = mGameComponentsList.rbegin(); it != mGameComponentsList.rend(); ++it)
		{
			(*it)->ShutDown();
			delete (*it);
		}

		delete mGameComponents;

		GameSettings::ShutDown();

		XF_LOGGER->ShutDown();
		delete XF_LOGGER;

		mOgre->ShutDown();
		delete mOgre;

		QuickAndDirtyOgreDebugRenderer::KillStaticVersion();
	}

	void Application::AddComponent(SystemComponent *component, std::string componentName)
	{
		if(mGameComponents->find(componentName) == mGameComponents->end())
		{
			mGameComponents->insert(std::make_pair(componentName, component));
			mGameComponentsList.push_back(component);

			ILOG << "XFramework: Component added - " << componentName;
		}
	}

	SystemComponent* Application::GetComponent(std::string componentName)
	{
		GameComponentMapIt it = mGameComponents->find(componentName);

		if(it != mGameComponents->end())
		{
			return it->second;
		}

		return NULL;
	}

	void Application::RemoveComponent(SystemComponent *component)
	{
		GameComponentMapIt it;

		for(it = mGameComponents->begin(); it != mGameComponents->end(); ++it)
		{
			if((*it).second == component)
			{
				ILOG << "XApplication: Removing component - " << it->first;

				mGameComponents->erase(it);
				mGameComponentsList.remove(component);
				
				delete it->second;
			}
		}
	}

	bool Application::Initialize( std::string windowTitle, std::string resourceFile, bool initialiseResources)
	{
		SSELOG << "Initializing XFramework";

		GameSettings::Initialise("./Settings.cfg");
		GameEntityFactory::Initialize("./EntityScripts.cfg");

		if(! InitializeGraphics(windowTitle, resourceFile, initialiseResources)) 
			return false;

		if(! InitializeFramework()) 
			return false;

		AddDefaultComponents();

		if(!OnInitialise())
		{
			SSELOG << "XApplicaion: " << "OnInitialise() returned false, shutting down.";
			return false;
		}

		InitializeComponents();

		SSELOG << "XFramework initialized";
		return true;
	}

	void Application::AddDefaultComponents()
	{
		this->AddComponent(new PostOffice(this), "Post Office");

		this->AddComponent(new DebugVariableTracker(this), "DebugVariableTracker");
		this->AddComponent(new RendererGameComponent(this, GetRenderer()), "Renderer");
		this->AddComponent(new CEGUIComponent(this, GetRenderer()->GetRenderWindow(), GetRenderer()->GetDefaultSceneManager()), "GUI");
	}
}

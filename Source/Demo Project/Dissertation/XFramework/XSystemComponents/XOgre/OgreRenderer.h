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

#include <Ogre.h>

namespace XF
{
	class OgreRenderer
	{
	private:
		Ogre::Root *mRoot;
		Ogre::RenderWindow *mWindow;
		Ogre::SceneManager *mDefaultSceneMgr;
		Ogre::Camera *mDefaultCamera;
		Ogre::Viewport *mDefaultViewport;

		bool CreateRoot();
		void DefineResources(std::string resourceFile);
		bool CreateRendersystem();
		void CreateRenderWindow(std::string windowTitle);
		void CreateDefaultScene();

	public:
		OgreRenderer();
		~OgreRenderer();

		bool Initialize(std::string windowTitle, std::string resourceFile);
		void InitializeAllResources(int mipLevel);
		void ShutDown();

		Ogre::Root* GetRoot(){ return mRoot; }
		Ogre::RenderWindow* GetRenderWindow(){ return mWindow; }
		Ogre::SceneManager* GetDefaultSceneManager(){ return mDefaultSceneMgr;}
		Ogre::Camera* GetDefaultCamera(){return mDefaultCamera;}
		Ogre::Viewport* GetDefaultViewport(){return mDefaultViewport;}
	};
}

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

#include "OgreRenderer.h"
#include "../../XUtilities/Logger.h"

#include <string>

namespace XF
{
	OgreRenderer::OgreRenderer()
	{
		mRoot = NULL;
		mWindow = NULL;
		mDefaultSceneMgr = NULL;
		mDefaultCamera = NULL;
		mDefaultViewport = NULL;
	}

	OgreRenderer::~OgreRenderer()
	{
	}

	bool OgreRenderer::Initialize(std::string windowTitle, std::string resourceFile)
	{
		if(!CreateRoot()) return false;

		DefineResources(resourceFile);

		if(!CreateRendersystem()) return false;

		CreateRenderWindow(windowTitle);

		CreateDefaultScene();

		return true;
	}

	void OgreRenderer::ShutDown()
	{
		mWindow->removeViewport(mDefaultViewport->getZOrder());
		mDefaultSceneMgr->destroyCamera(mDefaultCamera);
		mRoot->destroySceneManager(mDefaultSceneMgr);
		mWindow->destroy();
		mRoot->shutdown();

		delete mRoot;
		mRoot = NULL;
	}

	bool OgreRenderer::CreateRoot()
	{
		try
		{
			mRoot = new Ogre::Root();
		}
		catch(...)
		{
			return false;
		}

		return (mRoot != NULL);
	}

	void OgreRenderer::DefineResources(std::string resourceFile)
	{
		std::string secName, typeName, archName;

		Ogre::ConfigFile cf;
		cf.load(resourceFile);

		Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

		while(seci.hasMoreElements())
		{
			secName = seci.peekNextKey();

			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator i;

			for(i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				archName = i->second;

				SSELOG << "<XF::OgreRenderer>: Adding resource location (Typename: " << typeName << ", Archname: " << archName;

				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
			}
		}
	}

	bool OgreRenderer::CreateRendersystem()
	{
		if(/*!mRoot->restoreConfig() &&*/ 
			!mRoot->showConfigDialog())
			return false;

		return true;
	}

	void OgreRenderer::CreateRenderWindow(std::string windowTitle)
	{
		mWindow = mRoot->initialise(true, windowTitle);
	}

	void OgreRenderer::InitializeAllResources(int mipLevel)
	{
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}

	void OgreRenderer::CreateDefaultScene()
	{
		mDefaultSceneMgr = mRoot->createSceneManager(Ogre::ST_EXTERIOR_FAR, "Default SceneManager");
		mDefaultCamera = mDefaultSceneMgr->createCamera("Default Camera");
		//mDefaultCamera->setProjectionType(Ogre::ProjectionType::PT_ORTHOGRAPHIC);
		mDefaultCamera->setNearClipDistance(0.001f);
		mDefaultViewport = mWindow->addViewport(mDefaultCamera);
	}
}

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
#include <CEGUI/CEGUI.h>
#include <OgreCEGUIRenderer.h>
#include "CEGUI/CEGUIWindow.h"
#include <ois/OIS.h>
#include <string>
#include "CEGUI/CEGUIWindowManager.h"
#include "../SystemComponent.h"
#include "XMessaging/Messenger.h"
#include "XUtilities/Logger.h"

#ifndef DEFAULT_CEGUI_SKIN
	#define DEFAULT_CEGUI_SKIN "TaharezLook"	
#endif
#ifndef DEFAULT_CEGUI_FONT
	#define DEFAULT_CEGUI_FONT "BlueHighway-10"
#endif


namespace XF
{
	class CEGUIComponent : public XF::SystemComponent, 
							public OIS::KeyListener,
							public OIS::MouseListener,
							public XF::Messenger
	{
	private:
		

		CEGUI::WindowManager *mWindowManager;	
		CEGUI::OgreCEGUIRenderer *mRenderer;
		CEGUI::System *mSystem;

		CEGUI::String mCurrentStyle;
		CEGUI::String mCurrentFont;
		
		Ogre::RenderWindow* mOgreRenderWindow;
		Ogre::SceneManager* mSceneManager;

		OIS::Keyboard *mKeyboard;
		OIS::InputManager *mInput;
		OIS::Mouse *mMouse;

		std::map<CEGUI::String, CEGUI::Window* > mSheets;

		float mLastWheelZ;	

		std::vector<CEGUI::String> mLoadedThemes;
		std::vector<CEGUI::String> mLoadedLayOuts;
	
		std::map<CEGUI::String,float> mFadeInList;
		std::map<CEGUI::String,float> mFadeOutList;
		typedef std::map<CEGUI::String,float>::iterator FadeIter;

		CEGUI::Window* RootWindow;

	public:
		
		
		CEGUIComponent(XF::Application *ownerApp, Ogre::RenderWindow* ogreRenderWindow, Ogre::SceneManager*sceneManager) 
															: SystemComponent(ownerApp),
															  mOgreRenderWindow(ogreRenderWindow),
															  mSceneManager(sceneManager)
		{}
			
		~CEGUIComponent();


		void OnInitialise();
		void OnShutDown();
		void OnUpdate(double dT);	
		void OnHandleInput(double dT, XF::Input *input);

		// MouseListener
			virtual bool mouseMoved(const OIS::MouseEvent &arg);
			virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
			virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
			CEGUI::MouseButton ConvertButton(OIS::MouseButtonID buttonID);

		// KeyListener
			virtual bool keyPressed(const OIS::KeyEvent &arg);
			virtual bool keyReleased(const OIS::KeyEvent &arg);
		
		
		//scheme stuff
			void LoadScheme(CEGUI::String scheme = DEFAULT_CEGUI_SKIN);
			void SetFont(CEGUI::String font = DEFAULT_CEGUI_FONT);
			void LoadSchemeAndSetFont(CEGUI::String scheme, CEGUI::String font);

		//window stuff
			void SetFocus(CEGUI::Window* window);
			void SetFocus(CEGUI::String tag)
			{
				SetFocus(mWindowManager->getWindow(tag));
			}
		//void SetSheetWindowLayout(CEGUI::String tag);
		
		
		//sheet stuff
			/*remove sheet via name*/
			bool AddSheet(CEGUI::String file, CEGUI::String toParentSheetName = "RootWindow");
			/*add sheet directly*/
			bool AddSheet(CEGUI::Window* sheet, CEGUI::String toParentSheetName= "RootWindow");
			/*remove sheet via name*/
			void KillWindow(CEGUI::String name);
			/*remove sheet directly*/
			void KillWindow(CEGUI::Window* sheet);
			void HideSheet(CEGUI::String tag);
			void ShowSheet(CEGUI::String tag);
			void ToggleSheetVisibilty(CEGUI::String tag);
			void CEGUIComponent::ChangeSetSheet(CEGUI::String tag);

		//window utilities
			/*Link and event with a window, some usefull ones are CEGUI::FrameWindow::EventCloseClicked, CEGUI::PushButton::EventClicked,CEGUI::Listbox::EventChildAdded*/ 
			void CEGUIComponent::LinkEvent( CEGUI::String windowName, CEGUI::Event::Subscriber event, CEGUI::String type = CEGUI::Window::EventMouseClick  );
			void CEGUIComponent::LinkEvent( CEGUI::Window* window, CEGUI::Event::Subscriber event, CEGUI::String type );
			

			CEGUI::Window* GetWindow(CEGUI::String windowName);
			void ShowWindow(CEGUI::String tag);
			void HideWindow(CEGUI::String tag);
			void ToggleWindowVisibilty(CEGUI::String tag);
		
		//Functions to create things
			/*Create Blank sheet*/
			CEGUI::Window* CreateBlankSheet(CEGUI::String tag , CEGUI::String toParentSheetName = "RootWindow");
			CEGUI::Listbox* CreateListBox( CEGUI::String name , CEGUI::String schemeName );
			CEGUI::Window* CreateImage( CEGUI::String name , CEGUI::String image, CEGUI::String imageset );
			CEGUI::Window* CreateStaticText(CEGUI::String name, CEGUI::String text = "");
			CEGUI::FrameWindow* CreateFrameWindow( CEGUI::String name , CEGUI::String schemeName = DEFAULT_CEGUI_SKIN);
			CEGUI::Window* CreateAControl( CEGUI::String name , CEGUI::String type, CEGUI::String schemeName = DEFAULT_CEGUI_SKIN);

			CEGUI::Window* CreateViewPortWindow(CEGUI::String WindowTag, 
												CEGUI::String name, 
												Ogre::Root * ogreRoot, 
												Ogre::SceneManager *sceneManager,
												Ogre::Camera* cameraViewPoint,
												float x, float y, 
												float width, float height,																	
												CEGUI::Event::Subscriber* clickEvent = NULL,
												float texResX = 512, float texResY = 512);

			CEGUI::ButtonBase* CreateButtonA(CEGUI::String buttonName, 
			CEGUI::String buttonText,
			float width = 100, float height = 20,
			float x = 0, float y = 0,
			CEGUI::Event::Subscriber* clickEvent = NULL);

			
			CEGUI::Window* CEGUIComponent::CreatePictureButton( CEGUI::String buttonName, 
																CEGUI::String buttonImage,
				                                                float width = 1, float height =1, 
																float x =0, float y =0, 
																CEGUI::Event::Subscriber* clickEvent = NULL , 
																CEGUI::String buttonImageSetName = "CustomButtonSet");
			CEGUI::ButtonBase* CreateButtonB(CEGUI::String buttonName, 
				CEGUI::String buttonText,
				float width = 0.1, float height = 0.02,
				float x = 0, float y = 0,
				CEGUI::Event::Subscriber* clickEvent = NULL);

			CEGUI::ButtonBase* CreateButtonC(CEGUI::String buttonName, 
				CEGUI::String buttonText,
				float width = 100, float height = 20,
				float x = 0, float y = 0,
				CEGUI::Event::Subscriber* clickEvent = NULL);
			
	

		//handle message
			bool OnHandleMessage(const XF::Message &message);

		//mouse stuff
			void SetMouseImageForOverWindow(CEGUI::String imageName, CEGUI::String style = DEFAULT_CEGUI_SKIN);
			void SetMouseImageForNotOverWindow(CEGUI::String imageName, CEGUI::String style = DEFAULT_CEGUI_SKIN);
			void SetMouseImages(CEGUI::String imageName, CEGUI::String style = DEFAULT_CEGUI_SKIN);
		
		
		//event stuff
			bool FakeMousePressed();
			bool FakeMouseMoved(float x, float y);
			bool FakeMouseReleased();
			void CreateCloseEvent(CEGUI::String windowName, CEGUI::Event::Subscriber event);	

			/*checks window Type*/
			static bool IsMouseOnGui()
			{
				CEGUI::Window *window = CEGUI::System::getSingletonPtr()->getWindowContainingMouse();
				return (window && (window->getType() != "DefaultWindow"));
			}
			/*checks window name*/
			static bool IsMouseOnGuiB()
			{
				CEGUI::Window *window = CEGUI::System::getSingletonPtr()->getWindowContainingMouse();
				return (window && (window->getName() != "RootWindow"));
			}
			void CEGUIComponent::AttatchChildToParent(CEGUI::String child, CEGUI::String parent);
		
			
			CEGUI::Window* CEGUIComponent::LoadLayout(CEGUI::String file);
			CEGUI::ButtonBase* CEGUIComponent::CreateButtonD( CEGUI::String buttonName, CEGUI::String buttonText, CEGUI::UDim width /*= 100*/, CEGUI::UDim height /*= 20*/, CEGUI::UDim x /*= 0*/, CEGUI::UDim y /*= 0*/, CEGUI::Event::Subscriber* clickEvent /*= NULL*/ );
			void FadeIn( std::string window, float fadeInRate);
			void CEGUIComponent::FadeOut( std::string window, float fadeInRate );
			void CEGUIComponent::RemoveEvent( CEGUI::String windowName, CEGUI::String type );
			void CEGUIComponent::KillAllWindows();
			};
}

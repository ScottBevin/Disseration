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


#ifdef _DEBUG
#include <iosfwd>
#endif



#include <assert.h>
#include <OgreCEGUIRenderer.h>
#include <Ogre.h>
#include <cegui/CEGUI.h>
#include "CEGUIComponent.h"
#include "XUtilities/Logger.h"

using namespace Ogre;

namespace XF
{
	CEGUIComponent::~CEGUIComponent()
	{
		mWindowManager->destroyAllWindows();
	}
	
	void CEGUIComponent::LoadScheme( CEGUI::String scheme )
	{
		
		for(int i = 0; i < (int)mLoadedThemes.size(); ++i)
		{
			if(mLoadedThemes[i] == scheme)
			{
				DLOG << "attempted to load the same scheme twice!";
				return;
			}
		}
		std::string readable = scheme.c_str();
		mCurrentStyle = scheme;
		
		readable+=".scheme";
		CEGUI::String schemeFilePath = readable.c_str();
		CEGUI::String imageset = mCurrentStyle;

		CEGUI::SchemeManager::getSingleton().loadScheme(schemeFilePath);

		mLoadedThemes.push_back(scheme);
	    readable = mLoadedThemes[0].c_str();

		//set cursor for when cursor is not over a CEGUI window
 		mSystem->setDefaultMouseCursor((CEGUI::utf8*)imageset.c_str(),(CEGUI::utf8*)"MouseTarget");
		//set cursor to the same for when it is over a CEGUI window
		CEGUI::MouseCursor::getSingleton().setImage((CEGUI::utf8*)imageset.c_str(),(CEGUI::utf8*)"MouseArrow"); // hmm this doesnt seem to be working
	
	}
	/*this calls both set mouse for over window, and set mouse for not over window*/
	void CEGUIComponent::SetMouseImages( CEGUI::String imageName, CEGUI::String style /*= DEFAULT_CEGUI_SKIN*/)
	{
		mSystem->setDefaultMouseCursor(style,imageName);
		CEGUI::MouseCursor::getSingleton().setImage(style,imageName); // hmm this doesnt seem to be working

	}
	void CEGUIComponent::SetMouseImageForOverWindow( CEGUI::String imageName, CEGUI::String style /*= DEFAULT_CEGUI_SKIN*/)
	{
		mSystem->setDefaultMouseCursor(style,imageName);
	}

	void CEGUIComponent::SetMouseImageForNotOverWindow( CEGUI::String imageName, CEGUI::String style /*= DEFAULT_CEGUI_SKIN*/)
	{
		CEGUI::MouseCursor::getSingleton().setImage(style,imageName);
	}

	void CEGUIComponent::SetFont( CEGUI::String font /*= DEFAULT_CEGUI_FONT*/ )
	{
		std::string readable = font.c_str();
	
		if(CEGUI::FontManager::getSingletonPtr()->isFontPresent(mCurrentFont))
		{
			mCurrentFont = font;
			mSystem->setDefaultFont(mCurrentFont);			
		}
		else
		{
			SSELOG << "could not load " << mCurrentFont << "!";
		}		

	}


	void CEGUIComponent::LoadSchemeAndSetFont( CEGUI::String scheme, CEGUI::String font /*= DEFAULT_CEGUI_FONT*/ )
	{
		LoadScheme(scheme);
		SetFont(font);
	}


	void CEGUIComponent::LinkEvent( CEGUI::String windowName, CEGUI::Event::Subscriber event, CEGUI::String type )
	{
		CEGUI::Window* window = mWindowManager->getWindow(windowName);
		window->subscribeEvent(type,event);
	}
	void CEGUIComponent::RemoveEvent( CEGUI::String windowName, CEGUI::String type )
	{
		CEGUI::Window* window = mWindowManager->getWindow(windowName);
		window->removeEvent(type);
	}
	void CEGUIComponent::LinkEvent( CEGUI::Window*  window, CEGUI::Event::Subscriber event, CEGUI::String type )
	{
		window->subscribeEvent(type,event);
	}



	CEGUI::Window* CEGUIComponent::CreateViewPortWindow(CEGUI::String WindowTag, 
														CEGUI::String name, 
														Ogre::Root * ogreRoot, 
														Ogre::SceneManager *sceneManager,
														Ogre::Camera* cameraViewPoint,
														float x, float y, 
														float width, float height,									
														CEGUI::Event::Subscriber* clickEvent,
														float texResX , float texResY) 
	{
		
		CEGUI::String  imageName = name+"-Image";
		CEGUI::String  textureName = name+"-Texture";
		CEGUI::String imageSetName = name+"-ImageSet";
		//create a manual texture of the render target
		RenderTexture * tex = ogreRoot->getTextureManager()->createManual(textureName.c_str(), 
															ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
															TEX_TYPE_2D,
															texResX,texResY,
															0,
															PF_R8G8B8,
															TU_RENDERTARGET)->getBuffer()->getRenderTarget();


		Viewport *v = tex->addViewport(cameraViewPoint);
		v->setOverlaysEnabled(false);
		v->setClearEveryFrame(true);
		v->setBackgroundColour(ColourValue::Black);

		CEGUI::Texture *cTex = mRenderer->createTexture((CEGUI::utf8*)textureName.c_str());

		CEGUI::Imageset *imageSet = CEGUI::ImagesetManager::getSingleton().createImageset((CEGUI::utf8*)imageSetName.c_str(), cTex);
		imageSet->defineImage(imageName, 
			CEGUI::Point(0.0f, 0.0f),
			CEGUI::Size(cTex->getWidth(), cTex->getHeight()),
			CEGUI::Point(0.0f,0.0f));

		CEGUI::String staticImageStyle = mCurrentStyle+"/StaticImage";

		CEGUI::Window *viewPortWindow = mWindowManager->createWindow((CEGUI::utf8*)(staticImageStyle.c_str()) ,WindowTag+"/"+name);
		viewPortWindow->setSize(CEGUI::UVector2(CEGUI::UDim(width, 0), CEGUI::UDim(height,0)));

		viewPortWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(x, 0), CEGUI::UDim(y, 0)));
		
		if(clickEvent)
		{
			viewPortWindow->subscribeEvent(CEGUI::PushButton::EventClicked,*clickEvent);
			delete clickEvent;
		}
		

		viewPortWindow->setProperty("Image", 
			CEGUI::PropertyHelper::imageToString(&imageSet->getImage((CEGUI::utf8*)imageName.c_str())));


		mWindowManager->getWindow(WindowTag)->addChildWindow(viewPortWindow);
		return viewPortWindow;


	}

	/*Create a button, width and hieght are in absolute and x and y are in scale*/
	CEGUI::ButtonBase* CEGUIComponent::CreateButtonA( CEGUI::String buttonName, 
												CEGUI::String buttonText, 
												float width /*= 100*/, float height /*= 20*/, 
												float x /*= 0*/, float y /*= 0*/, 
												CEGUI::Event::Subscriber* clickEvent /*= NULL*/ )
	
	{
		std::string buttonSkinType = DEFAULT_CEGUI_SKIN;
		buttonSkinType +="/Button";

		CEGUI::ButtonBase *button = (CEGUI::ButtonBase *)mWindowManager->createWindow(buttonSkinType,buttonName);
		button->setText(buttonText);
		button->setSize(CEGUI::UVector2(CEGUI::UDim(0, width), CEGUI::UDim(0, height)));
		button->setPosition(CEGUI::UVector2(CEGUI::UDim(x, 0), CEGUI::UDim(y, 0)));
		button->subscribeEvent(CEGUI::PushButton::EventClicked,clickEvent);	

		return button;

	}
	/*Create a button, width, height, x and y are in scale*/
	CEGUI::ButtonBase* CEGUIComponent::CreateButtonB( CEGUI::String buttonName, 
		CEGUI::String buttonText, 
		float width /*= 0.1*/, float height /*= 0.02*/, 
		float x /*= 0*/, float y /*= 0*/, 
		CEGUI::Event::Subscriber* clickEvent /*= NULL*/ )

	{
		std::string buttonSkinType = DEFAULT_CEGUI_SKIN;
		buttonSkinType +="/Button";

		CEGUI::ButtonBase *button = (CEGUI::ButtonBase *)mWindowManager->createWindow(buttonSkinType,buttonName);
		button->setText(buttonText);
		button->setSize(CEGUI::UVector2(CEGUI::UDim(width, 0), CEGUI::UDim(height, 0)));
		button->setPosition(CEGUI::UVector2(CEGUI::UDim(x, 0), CEGUI::UDim(y, 0)));
		button->subscribeEvent(CEGUI::PushButton::EventClicked,clickEvent);	

		return button;

	}
	/*Create a button, width, height, x and y are in absolute*/
	CEGUI::ButtonBase* CEGUIComponent::CreateButtonC( CEGUI::String buttonName, 
		CEGUI::String buttonText, 
		float width /*= 100*/, float height /*= 20*/, 
		float x /*= 0*/, float y /*= 0*/, 
		CEGUI::Event::Subscriber* clickEvent /*= NULL*/ )

	{
		std::string buttonSkinType = DEFAULT_CEGUI_SKIN;
		buttonSkinType +="/Button";

		CEGUI::ButtonBase *button = (CEGUI::ButtonBase *)mWindowManager->createWindow(buttonSkinType,buttonName);
		button->setText(buttonText);
		button->setSize(CEGUI::UVector2(CEGUI::UDim(0, width), CEGUI::UDim(0,height)));
		button->setPosition(CEGUI::UVector2(CEGUI::UDim( 0,x), CEGUI::UDim( 0,y)));
		button->subscribeEvent(CEGUI::PushButton::EventClicked,clickEvent);	

		return button;
	}

	CEGUI::ButtonBase* CEGUIComponent::CreateButtonD( CEGUI::String buttonName, 
		CEGUI::String buttonText, 
		CEGUI::UDim width /*= 100*/, CEGUI::UDim height /*= 20*/, 
		CEGUI::UDim x /*= 0*/, CEGUI::UDim y /*= 0*/, 
		CEGUI::Event::Subscriber* clickEvent /*= NULL*/ )

	{
		std::string buttonSkinType = DEFAULT_CEGUI_SKIN;
		buttonSkinType +="/Button";

		CEGUI::ButtonBase *button = (CEGUI::ButtonBase *)mWindowManager->createWindow(buttonSkinType,buttonName);
		button->setText(buttonText);
		button->setSize(CEGUI::UVector2(width, height));
		button->setPosition(CEGUI::UVector2(x, y));
		button->subscribeEvent(CEGUI::PushButton::EventClicked,clickEvent);	

		return button;

	}

	/*Create a picture button, width, height, x and y are in scale*/
	CEGUI::Window* CEGUIComponent::CreatePictureButton( CEGUI::String buttonName,  
		CEGUI::String buttonImage,
		float width /*= 100*/, float height /*= 20*/, 
		float x /*= 0*/, float y /*= 0*/, 
		CEGUI::Event::Subscriber* clickEvent /*= NULL*/ ,
		CEGUI::String buttonImageSetName /*= "CustomButtonSet"*/)

	{
		std::string buttonSkinType = DEFAULT_CEGUI_SKIN;
		buttonSkinType +="/ImageButton";

		CEGUI::PushButton *button = (CEGUI::PushButton *)mWindowManager->createWindow("TaharezLook/ImageButton",buttonName);
		std::string readable = "set:";
		readable += buttonImageSetName.c_str();
		readable += " image:";
		readable += buttonImage.c_str();
		readable += "/btnNormal";

							
		button->setProperty("NormalImage",readable.c_str());
		button->setProperty("HoverImage","set:"+buttonImageSetName+" image:"+buttonImage+"/btnHover");
		button->setProperty("PushedImage","set:"+buttonImageSetName+" image:"+buttonImage+"/btnPushed");
		button->setProperty("DisabledImage","set:"+buttonImageSetName+" image:"+buttonImage+"/btnDisabled");
		

		button->setPosition(CEGUI::UVector2(CEGUI::UDim(x, 0), CEGUI::UDim(y, 0)));
		button->setSize(CEGUI::UVector2(CEGUI::UDim(width, 0), CEGUI::UDim(height, 0)));
		
		if(clickEvent)
			button->subscribeEvent(CEGUI::Window::EventMouseClick,clickEvent);	

		return button;

	}
	void  CEGUIComponent::AttatchChildToParent(CEGUI::String child, CEGUI::String parent)
	{
		this->GetWindow(parent)->addChildWindow(child);
	}


	
	CEGUI::Window* CEGUIComponent::CreateBlankSheet(CEGUI::String tag, CEGUI::String toParentSheetName /*= "RootWindow"*/)
	{	
		if(mSheets.find(tag) != mSheets.end())
		{
			DLOG << "ERROR: Tried to create the same sheet twice!";
			return NULL;
		}

		CEGUI::Window *sheet = mWindowManager->createWindow("DefaultGUISheet", tag);

		sheet->setMousePassThroughEnabled(true);//this function tells the window to ignore mouse events and pass it through to windows behind it
		mSheets[tag] = sheet;
		sheet->setVisible(true);
		mSheets[toParentSheetName]->addChildWindow(sheet);

		DLOG << "blank window layout added: with tag " <<tag;
		
		return sheet;
	}

	CEGUI::Listbox* CEGUIComponent::CreateListBox( CEGUI::String name , CEGUI::String schemeName )
	{
		return (CEGUI::Listbox*)mWindowManager->createWindow(schemeName+"/ListBox", name);	
	}

	CEGUI::FrameWindow* CEGUIComponent::CreateFrameWindow( CEGUI::String name , CEGUI::String schemeName/*= DEFAULT_CEGUI_SKIN*/  )
	{
		return (CEGUI::FrameWindow*)mWindowManager->createWindow(schemeName+"/FrameWindow", name);

		
	}
	CEGUI::Window* CEGUIComponent::CreateAControl( CEGUI::String name , CEGUI::String type, CEGUI::String schemeName/*= DEFAULT_CEGUI_SKIN*/)
	{
		return mWindowManager->createWindow(schemeName+"/"+type, name);

		
	}	


	
	/*
	void CEGUIComponent::AttatchSheetToSheet(CEGUI::String tag, CEGUI::String tag)
		{
			AttatchWindowToWindow(mSheets[tag],mSheets[tag]);
		}
		void CEGUIComponent::AttatchWindowToWindow( CEGUI::String childName,CEGUI::String parentName)
		{
			CEGUI::Window* windowParent = mWindowManager->getWindow(parentName);
			CEGUI::Window* windowChild = mWindowManager->getWindow(childName);
			windowParent->addChildWindow(windowChild);
		}
		void CEGUIComponent::AttatchWindowToWindow(CEGUI::String windowChild,CEGUI::Window* windowParent )
		{
			windowParent->addChildWindow(windowChild);
		}*/

	CEGUI::Window* CEGUIComponent::LoadLayout(CEGUI::String file)
	{
		for(int i = 0; i < (int)mLoadedLayOuts.size(); ++i)
		{
			if(mLoadedLayOuts[i] == file)
			{
				SSELOG << "attempted to load the layout twice!";
				return NULL;
			}
		}
		mLoadedLayOuts.push_back(file);
		return mWindowManager->loadWindowLayout(file); 
	}
	bool CEGUIComponent::AddSheet(CEGUI::Window* sheet, CEGUI::String toParentSheetName /*= "RootWindow"*/)
	{
		CEGUI::String tag = sheet->getName();

		if(mSheets.find(tag) != mSheets.end())
			return false;

		
	
		mSheets[tag] = sheet;

		if(mSheets.find(toParentSheetName) == mSheets.end())
		{
			LOG << "attempting to attach to an unknown parent";
			assert("attempting to attach to an unknown parent");
			return false;
		}
		mSheets[toParentSheetName]->addChildWindow(sheet);
		
		DLOG << "window layout added: with tag"<<tag<<" to parent " << toParentSheetName;
		return true;
	}
	
	bool CEGUIComponent::AddSheet(CEGUI::String file, CEGUI::String toParentSheetName/*= "RootWindow"*/)
	{
		
		for(int i = 0; i < (int)mLoadedLayOuts.size(); ++i)
		{
			if(mLoadedLayOuts[i] == file)
			{
				SSELOG << "attempted to load the layout twice!";
				return false;
			}
		}
		mLoadedLayOuts.push_back(file);
		CEGUI::Window* sheet = mWindowManager->loadWindowLayout(file); 
		

		sheet->setMousePassThroughEnabled(true);

		if(sheet)
			return AddSheet(sheet,toParentSheetName);		
		else
		{
			DLOG << "Unable To Load " << file;
			return false;
		}

		
		
		
	}
	void CEGUIComponent::KillAllWindows()
	{
		mWindowManager->destroyAllWindows();
	}
	void CEGUIComponent::KillWindow( CEGUI::String name )
	{
		mWindowManager->destroyWindow(name);
		
	}
	void CEGUIComponent::KillWindow( CEGUI::Window* sheet )
	{
		KillWindow(sheet->getName());
	}

	CEGUI::Window* CEGUIComponent::GetWindow( CEGUI::String windowName )
	{
		return mWindowManager->getWindow(windowName);
	}
	void CEGUIComponent::ToggleWindowVisibilty(CEGUI::String tag)
	{
		mWindowManager->getWindow(tag)->setVisible(!mWindowManager->getWindow(tag)->isVisible());
	}
	void CEGUIComponent::HideWindow(CEGUI::String tag)
	{
		mWindowManager->getWindow(tag)->setVisible(false);
	};
	void CEGUIComponent::ShowWindow(CEGUI::String tag)
	{
		mWindowManager->getWindow(tag)->setVisible(true);
	};
	void CEGUIComponent::ToggleSheetVisibilty(CEGUI::String tag)
	{
		mSheets[tag]->setVisible(!mSheets[tag]->isVisible());
	}
	void CEGUIComponent::HideSheet(CEGUI::String tag)
	{
		mSheets[tag]->setVisible(false);
	};
	void CEGUIComponent::ShowSheet(CEGUI::String tag)
	{
		if(mSheets.find(tag) != mSheets.end())
		{
			mSheets[tag]->setVisible(true);
		}
		else
		{
			DLOG << "Attemtping to make a sheet that doesnt exist visible";
		}
	};

	void CEGUIComponent::ChangeSetSheet(CEGUI::String tag)
	{			
		mSystem->setGUISheet(mSheets[tag]);		
	}
	void CEGUIComponent::OnInitialise()
	{	
		size_t windowHnd;
		std::ostringstream windowHndStr;
		OIS::ParamList pl;

		

		mOgreRenderWindow->getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << windowHnd;
		pl.insert( std::make_pair( std::string("WINDOW"), windowHndStr.str()  ) );	
		
	

		mRenderer = new CEGUI::OgreCEGUIRenderer(mOgreRenderWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneManager);
		mSystem = new CEGUI::System(mRenderer);

		mInput = OIS::InputManager::createInputSystem(pl);

		mKeyboard = static_cast<OIS::Keyboard*>(mInput->createInputObject(OIS::OISKeyboard, true));
		mMouse = static_cast<OIS::Mouse*>(mInput->createInputObject(OIS::OISMouse, true));

		mKeyboard->setEventCallback(this);
		mMouse->setEventCallback(this);

		

		LoadSchemeAndSetFont(DEFAULT_CEGUI_SKIN,DEFAULT_CEGUI_FONT);
		mWindowManager = CEGUI::WindowManager::getSingletonPtr();

		this->SubscribeToMailingList(XF::MailingList::GUI_SUBSYSTEM);
		RootWindow = CreateBlankSheet("RootWindow");
		mSystem->setGUISheet(RootWindow);

	#ifdef _DEBUG
		// tell us a lot about what is going on (see CEGUI.log in the working directory)
		CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);
	#endif
		
	}
	void CEGUIComponent::OnHandleInput(double dT, XF::Input *input)
	{

	}
	void CEGUIComponent::OnShutDown()
	{
		delete mKeyboard;
		delete mMouse;
		this->KillAllWindows();
		UnsubscribeFromMailingList(XF::MailingList::GUI_SUBSYSTEM);
		LOG << "CEGUI component shut down";
	}
	void CEGUIComponent::OnUpdate(double dT)
	{
		//fade stuff
		mKeyboard->capture();
		mMouse->capture();

		FadeIter it = mFadeInList.begin();

		std::vector<FadeIter> toErase;
		for(it; it != mFadeInList.end();++it)
		{
			CEGUI::String name = (it->first);
			float fadeSpeed = (it->second)*dT;		
			float alpha = mWindowManager->getWindow(name)->getAlpha();
			mWindowManager->getWindow(name)->setAlpha(alpha+fadeSpeed);
			if(mWindowManager->getWindow(name)->getAlpha() > 1)
			{
				mWindowManager->getWindow(name)->setAlpha(1);
				toErase.push_back(it);
	
			}	
			
		}
		for(unsigned int i = 0;  i < toErase.size(); ++i)
		{
			mFadeInList.erase(toErase[i]);
		}
		toErase.clear();
		it = mFadeOutList.begin();
		
		for(it; it != mFadeOutList.end();++it)
		{
			CEGUI::String name = (it->first);
			float fadeSpeed = (it->second)*dT;
			float alpha = mWindowManager->getWindow(name)->getAlpha();
			mWindowManager->getWindow(name)->setAlpha(alpha-fadeSpeed);
			if(mWindowManager->getWindow(name)->getAlpha() < 0)
			{
				mWindowManager->getWindow(name)->setAlpha(0);
				mWindowManager->getWindow(name)->setVisible(false);
				toErase.push_back(it);
			}
			
		}
		for(unsigned int i = 0; i < toErase.size(); ++i)
		{
			mFadeInList.erase(toErase[i]);
		}
		

	}

	// KeyListener
	bool CEGUIComponent::keyPressed(const OIS::KeyEvent &arg)
	{

		mSystem->injectKeyDown(arg.key);
		mSystem->injectChar(arg.text);

		return true;
	}

	bool CEGUIComponent::keyReleased(const OIS::KeyEvent &arg)
	{
		mSystem->injectKeyUp(arg.key);
		return true;
	}
	// MouseListener
	bool CEGUIComponent::mouseMoved(const OIS::MouseEvent &arg)
	{
		mSystem->injectMouseMove(arg.state.X.rel,arg.state.Y.rel);	
		mSystem->injectMouseWheelChange(arg.state.Z.rel);


		return true;
	}

	bool CEGUIComponent::FakeMouseMoved(float x, float y)
	{
		mSystem->injectMouseMove(x,y);	
		return true;
	}
	bool CEGUIComponent::FakeMouseReleased()
	{
		mSystem->injectMouseButtonUp(ConvertButton(OIS::MB_Left));

		return true;
	}
	bool CEGUIComponent::FakeMousePressed()
	{
		
		mSystem->injectMouseButtonDown(ConvertButton(OIS::MB_Left));
		

		return true;
	}
	bool CEGUIComponent::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		mSystem->injectMouseButtonDown(ConvertButton(id));
		
		
		return true;
	}

	bool CEGUIComponent::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		mSystem->injectMouseButtonUp(ConvertButton(id));
		


		return true;
	}
	//stupid convertor
	CEGUI::MouseButton CEGUIComponent::ConvertButton(OIS::MouseButtonID buttonID)
	{
		switch (buttonID)
		{
		case OIS::MB_Left:
			return CEGUI::LeftButton;

		case OIS::MB_Right:
			return CEGUI::RightButton;

		case OIS::MB_Middle:
			return CEGUI::MiddleButton;
			
		default:
			return CEGUI::LeftButton;
		}
	}
	//------------------------------------------------------------------
	bool CEGUIComponent::OnHandleMessage(const XF::Message &message)
	{
		if(message.Subject == XF::MessageSubject::PLEASE_SEND_GUI)
		{
			PostAMessage(
				message.SenderID,
				XF::MessageSubject::SENT_GUI,
				this);

			return true;
		}

		return false;
	}

	void CEGUIComponent::SetFocus( CEGUI::Window* window )
	{
		window->activate();
	}

	void CEGUIComponent::FadeIn( std::string window, float fadeInRate )
	{
		CEGUI::String key =window.c_str(); 
		mFadeInList[key] = fadeInRate;
		mWindowManager->getWindow(key)->setVisible(true);
	}
	void CEGUIComponent::FadeOut( std::string window, float fadeInRate )
	{
		CEGUI::String key =window.c_str(); 
		mFadeOutList[key] = fadeInRate;
	}

	CEGUI::Window* CEGUIComponent::CreateImage( CEGUI::String name, CEGUI::String image, CEGUI::String imageset )
	{
		std::string buttonSkinType = DEFAULT_CEGUI_SKIN;
		buttonSkinType += "/StaticImage";

		CEGUI::Window *imageWindow = mWindowManager->createWindow(buttonSkinType, name);
		imageWindow->setProperty("BackgroundEnabled", "false");
		imageWindow->setProperty("FrameEnabled", "false");
		imageWindow->setProperty("Image","set:"+imageset+" image:"+image);
		//imageWindow->setProperty("Image", image);

		return imageWindow;
	}

	CEGUI::Window* CEGUIComponent::CreateStaticText( CEGUI::String name, CEGUI::String text /*= ""*/ )
	{
		std::string buttonSkinType = DEFAULT_CEGUI_SKIN;
		buttonSkinType += "/StaticText";

		CEGUI::Window *textWindow = mWindowManager->createWindow(buttonSkinType, name);
		textWindow->setProperty("BackgroundEnabled", "false");
		textWindow->setProperty("FrameEnabled", "false");

		textWindow->setText(text);

		return textWindow;
	}
}


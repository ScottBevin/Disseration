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

#include "DemoScreen.h"

#include <ois/OIS.h>
#include <math.h>

DemoScreen::DemoScreen() : XF::GameScreen(false), mMakingObstacle(NULL), mSelectingAgents(false)
{
}

DemoScreen::~DemoScreen()
{
}

void DemoScreen::OnInitialise()
{
	mAutoTestsOn = false;

	RegisterDebugVariables();

	InitializeGUI();

	CreateSelectionRectangle();

	//CreateFloorPlane();

	CreateCrowdSim();

	CreateCamera();

	RegisterDemo("Auto FPS tests", XF::Delegate1<void, std::string>::FromMethod<DemoScreen, &DemoScreen::AutoTests>(this), "");

	RegisterDemo("Demo 1", XF::Delegate1<void, std::string>::FromMethod<DemoScreen, &DemoScreen::Demo1>(this), "");
	RegisterDemo("Demo 2", XF::Delegate1<void, std::string>::FromMethod<DemoScreen, &DemoScreen::Demo2>(this), "");
	RegisterDemo("Demo 3", XF::Delegate1<void, std::string>::FromMethod<DemoScreen, &DemoScreen::Demo3>(this), "");
	RegisterDemo("Demo 4", XF::Delegate1<void, std::string>::FromMethod<DemoScreen, &DemoScreen::Demo4>(this), "");
	RegisterDemo("Demo 5", XF::Delegate1<void, std::string>::FromMethod<DemoScreen, &DemoScreen::Demo5>(this), "");
	RegisterDemo("Demo 6", XF::Delegate1<void, std::string>::FromMethod<DemoScreen, &DemoScreen::Demo6>(this), "");
	RegisterDemo("Demo 7", XF::Delegate1<void, std::string>::FromMethod<DemoScreen, &DemoScreen::Demo7>(this), "");
	RegisterDemo("Demo 8", XF::Delegate1<void, std::string>::FromMethod<DemoScreen, &DemoScreen::Demo8>(this), "");
	RegisterDemo("Demo 9", XF::Delegate1<void, std::string>::FromMethod<DemoScreen, &DemoScreen::Demo9>(this), "");
	RegisterDemo("Demo 10", XF::Delegate1<void, std::string>::FromMethod<DemoScreen, &DemoScreen::Demo10>(this), "");
	RegisterDemo("Demo 11", XF::Delegate1<void, std::string>::FromMethod<DemoScreen, &DemoScreen::Demo11>(this), "");
	RegisterDemo("Demo 12", XF::Delegate1<void, std::string>::FromMethod<DemoScreen, &DemoScreen::Demo12>(this), "");
}

void DemoScreen::OnHandleInput(double dT, XF::Input *input)
{
	if(mAutoTestsOn) 
	{
		if(input->IsKeyRelease(OIS::KC_SPACE))
			mAutoTestsOn = false;
		else
			return;
	}

	HandleCameraControls(input, dT);

	mBulletTime = input->IsKeyDown(OIS::KC_SPACE);

	HandleSimInteraction(input);
}

void DemoScreen::OnUpdate(double dT)
{
	if(mAutoTestsOn)
	{
		HandleAutoFPSTesting(dT);

	}

	mCamera->Update(dT);

	double simUpdateTime = dT;

	if(mBulletTime) simUpdateTime *= 0.1f;

	mCrowdSim->UpdateSim(simUpdateTime);

	mCrowdSimDebugRender->UpdateRender(dT);

	UpdateTrackedVariables();

}

void DemoScreen::OnShutDown()
{
	XF::DebugVariableTracker::UnregisterVariable("Agent count");
	XF::DebugVariableTracker::UnregisterVariable("Collisions");

	delete mCrowdSimDebugRender;
	delete mCrowdSim;
	delete mCamera;
	delete mMakingObstacle;
}

void DemoScreen::OnAgentReachedPositionHigh(XCS::Agent *agent, XCS::Vector2f &pos)
{
	agent->SteeringComponent().Target(XCS::Vector2f(
		XF::Random::NextFloat(-75.0f, 75.0f),
		XF::Random::NextFloat(-75.0f, 75.0f)));
}

void DemoScreen::OnAgentReachedPositionMedium(XCS::Agent *agent, XCS::Vector2f &pos)
{
	agent->SteeringComponent().Target(XCS::Vector2f(
		XF::Random::NextFloat(-125.0f, 125.0f),
		XF::Random::NextFloat(-125.0f, 125.0f)));
}

void DemoScreen::OnAgentReachedPositionLow(XCS::Agent *agent, XCS::Vector2f &pos)
{
	agent->SteeringComponent().Target(XCS::Vector2f(
		XF::Random::NextFloat(-200.0f, 200.0f),
		XF::Random::NextFloat(-200.0f, 200.0f)));
}

bool DemoScreen::OnHandleMessage( const XF::Message &message )
{
	switch(message.Subject)
	{
	case XF::MessageSubject::SENT_GUI:
		{
			XF::CEGUIComponent* gui = static_cast<XF::CEGUIComponent*>(message.Data);
			if(gui)
			{
				mGUI = gui;
			}
			return true;
		}
	}
	return false;
}

XCS::Agent* DemoScreen::CreateAgent( XCS::Vector2f cPos, XCS::Vector2f goal )
{
	XCS::Agent* agent = mCrowdSim->AddAgent(
		cPos,					// start pos
		goal,					// target pos
		XF::Random::NextFloat(),		// openness
		XF::Random::NextFloat(),		// consciousnesses
		XF::Random::NextFloat(),		// extroversion
		XF::Random::NextFloat(),		// agreeableness
		XF::Random::NextFloat());		// neuroticism
	return agent;
}

XCS::Agent* DemoScreen::CreateAgent( XCS::Vector2f cPos, XCS::Vector2f goal, float o, float c, float e, float a, float n )
{
	XCS::Agent* agent = mCrowdSim->AddAgent(
		cPos,					// start pos
		goal,					// target pos
		o,		// openness
		c,		// consciousnesses
		e,		// extroversion
		a,		// agreeableness
		n);		// neuroticism
	return agent;
}
bool DemoScreen::ClickSim( XF::Vector2 &clickPos )
{
	CEGUI::MouseCursor * mouse = CEGUI::MouseCursor::getSingletonPtr();

	Ogre::Camera *cam = mRenderer->GetDefaultCamera();
	Ogre::Ray ray = cam->getCameraToViewportRay(
		mouse->getPosition().d_x / mRenderer->GetRenderWindow()->getWidth(), 
		mouse->getPosition().d_y / mRenderer->GetRenderWindow()->getHeight());

	Ogre::Plane p(0.0f, 1.0f, 0.0f, 0.0f);
	std::pair<bool, Ogre::Real> pair = ray.intersects(p);

	if(pair.first)
	{
		XF::Vector3 pos = ray.getOrigin() + (ray.getDirection().normalisedCopy() * pair.second);

		clickPos = XF::Vector2(pos.x, pos.z);

		return true;
	}
	else
		return false;
}

void DemoScreen::OnAgentReachedPosition0( XCS::Agent *agent, XCS::Vector2f &pos )
{
	XCS::Vector2f oldTarget = agent->SteeringComponent().Target();
	oldTarget.mX = -oldTarget.mX;
	oldTarget.mY = -oldTarget.mY;

	agent->SteeringComponent().Target(oldTarget);
}

void DemoScreen::RegisterDebugVariables()
{
	XF::DebugVariableTracker::RegisterVariable(XF::PropertySerializer::TYPE_INT, "Agent count");
	XF::DebugVariableTracker::RegisterVariable(XF::PropertySerializer::TYPE_INT, "Collisions");
}

void DemoScreen::InitializeGUI()
{
	this->PostAMessage(
		XF::MailingList::GUI_SUBSYSTEM,
		XF::MessageSubject::PLEASE_SEND_GUI,
		NULL);

	mDemoSheet = mGUI->CreateBlankSheet("demoSheet");

	mRootDemoWindow = mGUI->CreateFrameWindow("Demos");
	mRootDemoWindow->setCloseButtonEnabled(false);
	mRootDemoWindow->setText("Demos");
	mRootDemoWindow->setWidth(CEGUI::UDim(1.0f, 0.0f));
	mRootDemoWindow->setYPosition(CEGUI::UDim(0.5f, 0.0f));

	mDemoSheet->addChildWindow(mRootDemoWindow);
}

void DemoScreen::CreateSelectionRectangle()
{
	Ogre::MaterialPtr SelectRect = Ogre::MaterialManager::getSingleton().create("SelectRectMaterial", "debugger");
	SelectRect->setReceiveShadows(false);
	SelectRect->getTechnique(0)->setLightingEnabled(false);
	mSelectRect = mRenderer->GetDefaultSceneManager()->getRootSceneNode()->createChildSceneNode("SelectRect");
	Ogre::ManualObject *manObj = mRenderer->GetDefaultSceneManager()->createManualObject("SelectRect");
	manObj->begin("SelectRectMaterial", Ogre::RenderOperation::OT_LINE_STRIP);
	manObj->colour(1.0f, 1.0f, 1.0f);
	manObj->position(-0.5f, 0.0f, 0.5f);
	manObj->position(0.5f, 0.0f, 0.5f);
	manObj->position(0.5f, 0.0f, -0.5f);
	manObj->position(-0.5f, 0.0f, -0.5f);
	manObj->position(-0.5f, 0.0f, 0.5f);
	manObj->end();
	mSelectRect->attachObject(manObj);
	mSelectRect->setVisible(false);
}

void DemoScreen::CreateFloorPlane()
{
	// create floor plane
	Ogre::MeshManager::getSingleton().createPlane(
		"Plane.mesh", 
		"Custom", 
		Ogre::Plane(XF::Vector3::UNIT_Z, XF::Vector3::ZERO),
		400,
		400,
		1,
		1);

	Ogre::Entity* floor = mRenderer->GetDefaultSceneManager()->createEntity("Floor", "Plane.mesh");
	floor->setMaterialName("FloorMaterial");
	Ogre::SceneNode* groundNode = mRenderer->GetDefaultSceneManager()->getRootSceneNode()->createChildSceneNode();
	groundNode->attachObject(floor);
	groundNode->setOrientation( Ogre::Quaternion( Ogre::Radian( -Ogre::Math::HALF_PI ), XF::Vector3::UNIT_X) );
	groundNode->setPosition(0.0f, -0.4f, 0.0f);
	groundNode->setVisible(true);
}

void DemoScreen::CreateCrowdSim()
{
	mCrowdSim = new XCS::CrowdSimulator(
		XF_GAMESETTINGS->Property<float>("AGENT_MIN_LOOK_DISTANCE"),
		XF_GAMESETTINGS->Property<float>("AGENT_MAX_LOOK_DISTANCE"),
		XF_GAMESETTINGS->Property<float>("AGENT_MIN_DESIRED_VELOCITY"),
		XF_GAMESETTINGS->Property<float>("AGENT_MAX_DESIRED_VELOCITY"),
		XF_GAMESETTINGS->Property<float>("AGENT_MIN_SHYNESS"),
		XF_GAMESETTINGS->Property<float>("AGENT_MAX_SHYNESS"));
	mCrowdSimDebugRender = new CrowdSimDebugRendererMkII(mRenderer, mCrowdSim);
}

void DemoScreen::HandleCameraControls( XF::Input * input, double dT )
{
	if(input->IsMouseButtonDown(XF::MouseButtons::mbMiddle))
	{
		mCamera->AddToPitch(-input->GetMouseMovementY() * dT, false);
		mCamera->AddToYaw(input->GetMouseMovementX() * dT, false);
	}

	mCamera->AddToDistance(input->GetMouseScrollChange() * 1.0f);
}

void DemoScreen::HandleAutoFPSTesting( double dT )
{
	int fps = XF::FPSTracker::GetLastFPS();

	if(fps < mMinFPS)
		mMinFPS = fps;

	if(fps > mMaxFPS)
		mMaxFPS = fps;

	if((mTimeTillNextTest -= dT) < 0.0f)
	{
		mTimeTillNextTest = mTestTime;

		mFPSLogs[mSetType][mIteration][mAgentCount].mMax = mMaxFPS;
		mFPSLogs[mSetType][mIteration][mAgentCount].mMin = mMinFPS;
		mFPSLogs[mSetType][mIteration][mAgentCount].mAverage = (mMaxFPS + mMinFPS) * 0.5f;

		mMinFPS = XF::MathHelper::Infinity();
		mMaxFPS = -XF::MathHelper::Infinity();

		mAgentCount++;

		if(mAgentCount < mMaxAgentCount)
		{
			ILOG << "Increasing agent count to " << mAgentCount;

			if(mSetType == 0)
			{
				for(int i = 0; i < 40; ++i)
				{
					XCS::Agent* agent1 = CreateAgent(
						XCS::Vector2f(XF::Random::NextFloat(-200.0f, 200.0f), XF::Random::NextFloat(-200.0f, 200.0f)),
						XCS::Vector2f(XF::Random::NextFloat(-200.0f, 200.0f), XF::Random::NextFloat(-200.0f, 200.0f)));

					agent1->SteeringComponent().OnTargetReached(
						XCS::Delegate2<void, XCS::Agent*, XCS::Vector2f&>::FromMethod<DemoScreen, &DemoScreen::OnAgentReachedPositionLow>(this));
				}
			}
			else if(mSetType == 1)
			{
				for(int i = 0; i < 40; ++i)
				{
					XCS::Agent* agent1 = CreateAgent(
						XCS::Vector2f(XF::Random::NextFloat(-125.0f, 125.0f), XF::Random::NextFloat(-125.0f, 125.0f)),
						XCS::Vector2f(XF::Random::NextFloat(-125.0f, 125.0f), XF::Random::NextFloat(-125.0f, 125.0f)));

					agent1->SteeringComponent().OnTargetReached(
						XCS::Delegate2<void, XCS::Agent*, XCS::Vector2f&>::FromMethod<DemoScreen, &DemoScreen::OnAgentReachedPositionMedium>(this));
				}
			}
			else if(mSetType == 2)
			{
				for(int i = 0; i < 40; ++i)
				{
					XCS::Agent* agent1 = CreateAgent(
						XCS::Vector2f(XF::Random::NextFloat(-75.0f, 75.0f), XF::Random::NextFloat(-75.0f, 75.0f)),
						XCS::Vector2f(XF::Random::NextFloat(-75.0f, 75.0f), XF::Random::NextFloat(-75.0f, 75.0f)));

					agent1->SteeringComponent().OnTargetReached(
						XCS::Delegate2<void, XCS::Agent*, XCS::Vector2f&>::FromMethod<DemoScreen, &DemoScreen::OnAgentReachedPositionHigh>(this));
				}
			}
		}
		else
		{
			ILOG << "Finished iteration " << mIteration;

			mIteration++;
			mCrowdSim->KillAll();
			mAgentCount = 0;

			if(mIteration < mMaxIterations)
			{
				ILOG << "Starting iteration " << mIteration;
			}
			else
			{
				ILOG << "Finished set " << mSetType;
				mSetType++;
				mIteration = 0;

				if(mSetType < mMaxSets)
				{
					ILOG << "Starting set " << mSetType;
				}
				else
				{
					ILOG << "Finished auto test - dumping";

					mAutoTestsOn = false;

					for(int i = 0; i < mMaxSets; ++i)
					{
						ILOG << "Set " << i;

						float min = 0; 
						float max = 0;
						float av = 0;

						for(int j = 0; j < mMaxIterations; ++j)
						{
							ILOG << " Iteration " << j;

							std::string minStr, mMaxStr, mAvStr;

							for(int k = 0; k < mMaxAgentCount; ++k)
							{
								minStr += XF::Utilities::ToString(mFPSLogs[i][j][k].mMin) + ", ";
								mMaxStr += XF::Utilities::ToString(mFPSLogs[i][j][k].mMax) + ", ";
								mAvStr += XF::Utilities::ToString(mFPSLogs[i][j][k].mAverage) + ", ";

								min += mFPSLogs[i][j][k].mMin;
								max += mFPSLogs[i][j][k].mMax;
								av += mFPSLogs[i][j][k].mAverage;
							}

							ILOG << "  Min: " << minStr;
							ILOG << "  Max: " << mMaxStr;
							ILOG << "  Average: " << mAvStr;
							ILOG << "  Totals: Min: " << min / mMaxAgentCount << " Max: " << max / mMaxAgentCount << " Average: " << av / mMaxAgentCount;
						}
					}
				}
			}
		}
	}
}

void DemoScreen::HandleSimInteraction( XF::Input * input )
{
	if(!XF::CEGUIComponent::IsMouseOnGui())
	{
		if(input->IsMouseButtonClick())
		{
			XF::Vector2 cPos;

			if(ClickSim(cPos))
			{
				if(input->IsKeyDown(OIS::KC_O))
				{
					mMakingObstacle = mCrowdSim->AddObstacle(XCS::Vector2f(cPos.x, cPos.y), XCS::Vector2f(cPos.x, cPos.y));
				}
				else if(!input->IsKeyPress(OIS::KC_A) && !input->IsKeyDown(OIS::KC_A) &&
					!input->IsKeyPress(OIS::KC_T) && !input->IsKeyDown(OIS::KC_T))
				{
					mSelectingAgents = true;
					mSelectionStartPos = cPos;
					mSelectRect->setVisible(true);

					mSelectRect->setPosition(
						mSelectionStartPos.x + ((cPos.x - mSelectionStartPos.x) * 0.5f),
						0.0f,
						mSelectionStartPos.y + ((cPos.y - mSelectionStartPos.y) * 0.5f));

					mSelectRect->setScale((cPos.x - mSelectionStartPos.x), 1.0f, (cPos.y - mSelectionStartPos.y));
				}
			}
		}

		if(input->IsMouseButtonDown())
		{
			XF::Vector2 cPos;

			if(ClickSim(cPos))
			{
				if(input->IsKeyDown(OIS::KC_A))
				{
					XCS::Agent* agent = CreateAgent(
						XCS::Vector2f(cPos.x, cPos.y) + (XCS::Vector2f(XF::Random::NextFloat(), XF::Random::NextFloat())), 
						XCS::Vector2f(cPos.x, cPos.y) + (XCS::Vector2f(XF::Random::NextFloat(), XF::Random::NextFloat())));

					mAgentsBeingCreated.push_back(agent);
				}
			}
		}

		if(input->IsMouseButtonRelease() && input->IsKeyDown(OIS::KC_Z))
		{
			XF::Vector2 cPos;

			if(ClickSim(cPos))
			{
				CreateAgent(XCS::Vector2f(cPos.x, cPos.y), XCS::Vector2f(cPos.x, cPos.y));
			}
		}
	}

	if(input->IsMouseMoving())
	{
		XF::Vector2 cPos;

		if(ClickSim(cPos))
		{
			if(mMakingObstacle)
			{
				mMakingObstacle->EndPoint(XCS::Vector2f(cPos.x, cPos.y));
			}
			if(mSelectingAgents)
			{
				mSelectRect->setPosition(
					mSelectionStartPos.x + ((cPos.x - mSelectionStartPos.x) * 0.5f),
					0.0f,
					mSelectionStartPos.y + ((cPos.y - mSelectionStartPos.y) * 0.5f));

				mSelectRect->setScale((cPos.x - mSelectionStartPos.x), 1.0f, (cPos.y - mSelectionStartPos.y));
			}
		}
	}

	if(input->IsMouseButtonRelease())
	{
		XF::Vector2 cPos;

		if(ClickSim(cPos))
		{
			if(mMakingObstacle)
			{
				mMakingObstacle = NULL;
			}
			if(input->IsKeyDown(OIS::KC_T))
			{
				std::vector<XCS::Entity*> selectedAgents = mCrowdSimDebugRender->SelectedEntities();

				for(std::vector<XCS::Entity*>::iterator it = selectedAgents.begin();
					it != selectedAgents.end(); ++it)
				{
					if((*it)->Type() == XCS::Entity::AGENT)
						((XCS::Agent*)(*it))->SteeringComponent().Target(XCS::Vector2f(cPos.x, cPos.y));
				}
			}

			if(mAgentsBeingCreated.size() > 0)
			{
				mCrowdSimDebugRender->SetSelectedEntitys(mAgentsBeingCreated);
				mAgentsBeingCreated.clear();
			}

			if(mSelectingAgents)
			{
				float minX = XF::MathHelper::Min(cPos.x, mSelectionStartPos.x);
				float minY = XF::MathHelper::Min(cPos.y, mSelectionStartPos.y);
				float maxX = XF::MathHelper::Max(cPos.x, mSelectionStartPos.x);
				float maxY = XF::MathHelper::Max(cPos.y, mSelectionStartPos.y);

				std::vector<XCS::Entity*> selected = mCrowdSim->SelectEntities(
					XCS::Vector2f(minX, minY), XCS::Vector2f(maxX, maxY));

				for(unsigned int i = 0; i < selected.size(); ++i)
					selected[i]->Log();

				mCrowdSimDebugRender->SetSelectedEntitys(selected);
				mSelectingAgents = false;
				mSelectRect->setVisible(false);
			}
		}
	}

	if(input->IsKeyPress(OIS::KC_DELETE))
	{
		std::vector<XCS::Entity*> selectedAgents = mCrowdSimDebugRender->SelectedEntities();

		mCrowdSimDebugRender->SetSelectedEntitys(NULL);

		for(std::vector<XCS::Entity*>::iterator it = selectedAgents.begin();
			it != selectedAgents.end(); ++it)
		{
			mCrowdSim->KillEntity(*it);
		}
	}

	if(input->IsKeyPress(OIS::KC_K))
	{
		mCrowdSim->KillAll();
	}
}

void DemoScreen::UpdateTrackedVariables()
{
	XF::DebugVariableTracker::UpdateVariable("Agent count", XF::Utilities::ToString(mCrowdSim->NumAgents()));
	XF::DebugVariableTracker::UpdateVariable("Collisions", XF::Utilities::ToString(mCrowdSim->GetQuadTree()->Collisions()));
}

bool DemoScreen::RunDemoButton( const CEGUI::EventArgs &args )
{
	const CEGUI::WindowEventArgs& we = static_cast<const CEGUI::WindowEventArgs&>(args);

	for(int i = 0; i < mDemos.size(); ++i)
	{
		if(mDemos[i].mButton == (CEGUI::ButtonBase*)we.window)
		{
			mCrowdSim->KillAllAgents();

			mDemos[i].mOnClick(mDemos[i].mExtraData);
		}
	}

	return true;
}

void DemoScreen::RegisterDemo( std::string text, XF::Delegate1<void, std::string> onClick, std::string extraInfo )
{
	Demo newDemo;

	newDemo.mButton = mGUI->CreateButtonD("DemoButton" + text, text,
		CEGUI::UDim(0.18f, 0.0f), CEGUI::UDim(0.0f, 20.0f), CEGUI::UDim(0.0f, 0.0f), CEGUI::UDim(0.0f, 0.0f), 
		new CEGUI::Event::Subscriber(&DemoScreen::RunDemoButton, this));

	mRootDemoWindow->addChildWindow(newDemo.mButton);

	newDemo.mExtraData = extraInfo;
	newDemo.mOnClick = onClick;

	mDemos.push_back(newDemo);

	for(int i = 0; i < mDemos.size(); ++i)
	{
		mDemos[i].mButton->setXPosition(CEGUI::UDim(0.01f + (0.198 * (i % 5)), 0.0f));
		mDemos[i].mButton->setYPosition(CEGUI::UDim(0.0f, 25.0f + (25.0f * (int)(i / 5))));
	}

	mRootDemoWindow->setHeight(CEGUI::UDim(0.0f, 60.0f + (25.0f * (int)(mDemos.size() / 5))));
}

void DemoScreen::Demo1( std::string extraInfo )
{
	for(int i = 0; i < 50; ++i)
	{
		CreateAgent(
			XCS::Vector2f(-20.0f, -125.0f + i * 5.0f),
			XCS::Vector2f(20.0f, -125.0f + i * 5.0f));

		CreateAgent(
			XCS::Vector2f(20.0f, -125.0f + i * 5.0f),
			XCS::Vector2f(-20.0f, -125.0f + i * 5.0f));
	}
}

void DemoScreen::Demo2( std::string extraInfo )
{
	for(int i = 0; i < 5; ++i)
	{
		for(int j = 0; j < 10; ++j)
		{
			CreateAgent(
				XCS::Vector2f(-20.0f - (j * 5.0f),	-7.5f + i * 3.0f),
				XCS::Vector2f(20.0f + (j * 5.0f),	-7.5f + i * 3.0f),
				0.5f,
				0.0f,
				1.0f,
				0.0f,
				0.0f);
		}
	}

	for(int i = 0; i < 5; ++i)
	{
		for(int j = 0; j < 10; ++j)
		{
			CreateAgent(
				XCS::Vector2f(20.0f + (j * 5.0f),	-7.5f + i * 3.0f),
				XCS::Vector2f(-20.0f - (j * 5.0f),	-7.5f + i * 3.0f),
				0.0f,
				1.0f,
				0.0f,
				0.5f,
				1.0f);
		}
	}
}

void DemoScreen::Demo3( std::string extraInfo )
{
	for(int i = 0; i < 5; ++i)
	{
		for(int j = 0; j < 10; ++j)
		{
			CreateAgent(
				XCS::Vector2f(-20.0f - (j * 6.0f),	-6.0f + i * 6.0f),
				XCS::Vector2f(100.0f + (j * 6.0f),	-6.0f + i * 6.0f),
				0.5f,
				0.0f,
				1.0f,
				0.0f,
				0.0f);
		}
	}

	for(int i = 0; i < 5; ++i)
	{
		for(int j = 0; j < 10; ++j)
		{
			CreateAgent(
				XCS::Vector2f(20.0f + (j * 6.0f),	-5.0f + i * 6.0f),
				XCS::Vector2f(-100.0f - (j * 6.0f),	-5.0f + i * 6.0f),
				0.0f,
				1.0f,
				0.0f,
				0.5f,
				1.0f);
		}
	}
}

void DemoScreen::Demo4( std::string extraInfo )
{
	for(int i = 0; i < 5; ++i)
	{
		for(int j = 0; j < 10; ++j)
		{
			CreateAgent(
				XCS::Vector2f(-20.0f - (j * 6.0f),	-6.0f + i * 6.0f),
				XCS::Vector2f(100.0f + (j * 6.0f),	-6.0f + i * 6.0f)
				);
		}
	}

	for(int i = 0; i < 5; ++i)
	{
		for(int j = 0; j < 10; ++j)
		{
			CreateAgent(
				XCS::Vector2f(20.0f + (j * 6.0f),	-5.0f + i * 6.0f),
				XCS::Vector2f(-100.0f - (j * 6.0f),	-5.0f + i * 6.0f)
				);
		}
	}
}

void DemoScreen::Demo5( std::string extraInfo )
{

	for(int i = 0; i < 5; ++i)
	{
		for(int j = 0; j < 10; ++j)
		{
			CreateAgent(
				XCS::Vector2f(-20.0f - (j * 6.0f),	-6.0f + i * 6.0f),
				XCS::Vector2f(100.0f + (j * 6.0f),	-6.0f + i * 6.0f),
				1.0f, 0.5f,0.5f,0.5f,0.5f
				);
		}
	}

	for(int i = 0; i < 5; ++i)
	{
		for(int j = 0; j < 10; ++j)
		{
			CreateAgent(
				XCS::Vector2f(20.0f + (j * 6.0f),	-5.0f + i * 6.0f),
				XCS::Vector2f(-100.0f - (j * 6.0f),	-5.0f + i * 6.0f),
				1.0f,0.5f,0.5f,0.5f,0.5f
				);
		}
	}
}

void DemoScreen::Demo6( std::string extraInfo )
{

	float numAgents = 100;
	float angle = 0.0f;
	float angleChange = XF::MathHelper::TwoPi() / numAgents;
	float distance = 200.0f;

	for(int i = 0; i < numAgents; ++i)
	{
		XCS::Vector2f pos(cos(angle) * distance, sin(angle) * distance);
		XCS::Vector2f target(cos(XF::MathHelper::Pi() + angle), sin(XF::MathHelper::Pi() + angle));
		target *= distance;

		CreateAgent(
			pos,
			target,
			0.5f,
			0.5f,
			0.5f,
			0.5f,
			0.5f);

		angle += angleChange;
	}
}

void DemoScreen::Demo7( std::string extraInfo )
{

	float numAgents = 100;
	float angle = 0.0f;
	float angleChange = XF::MathHelper::TwoPi() / numAgents;
	float distance = 40.0f;

	for(int i = 0; i < numAgents; ++i)
	{
		XCS::Vector2f pos(cos(angle) * distance, sin(angle) * distance);
		XCS::Vector2f target(0.0f);//(cos(XF::MathHelper::Pi() + angle), sin(XF::MathHelper::Pi() + angle));
		target *= distance;

		if(XF::MathHelper::IsEven(i))
		{
			CreateAgent(
				pos,
				target,
				0.5f,
				0.5f,
				1.0f,
				0.5f,
				0.5f);
		}
		else
		{
			CreateAgent(
				pos,
				target,
				0.5f,
				0.5f,
				0.0f,
				0.5f,
				0.5f);
		}

		angle += angleChange;
	}
}

void DemoScreen::Demo8( std::string extraInfo )
{
	for(int i = 0; i < 40; ++i)
	{
		XCS::Agent* agent1 = CreateAgent(
			XCS::Vector2f(XF::Random::NextFloat(-200.0f, 200.0f), XF::Random::NextFloat(-200.0f, 200.0f)),
			XCS::Vector2f(XF::Random::NextFloat(-200.0f, 200.0f), XF::Random::NextFloat(-200.0f, 200.0f)));

		agent1->SteeringComponent().OnTargetReached(
			XCS::Delegate2<void, XCS::Agent*, XCS::Vector2f&>::FromMethod<DemoScreen, &DemoScreen::OnAgentReachedPositionLow>(this));
	}
}

void DemoScreen::Demo9( std::string extraInfo )
{
	for(int i = 0; i < 40; ++i)
	{
		XCS::Agent* agent1 = CreateAgent(
			XCS::Vector2f(XF::Random::NextFloat(-125.0f, 125.0f), XF::Random::NextFloat(-125.0f, 125.0f)),
			XCS::Vector2f(XF::Random::NextFloat(-125.0f, 125.0f), XF::Random::NextFloat(-125.0f, 125.0f)));

		agent1->SteeringComponent().OnTargetReached(
			XCS::Delegate2<void, XCS::Agent*, XCS::Vector2f&>::FromMethod<DemoScreen, &DemoScreen::OnAgentReachedPositionMedium>(this));
	}
}

void DemoScreen::Demo10( std::string extraInfo )
{
	for(int i = 0; i < 40; ++i)
	{
		XCS::Agent* agent1 = CreateAgent(
			XCS::Vector2f(XF::Random::NextFloat(-75.0f, 75.0f), XF::Random::NextFloat(-75.0f, 75.0f)),
			XCS::Vector2f(XF::Random::NextFloat(-75.0f, 75.0f), XF::Random::NextFloat(-75.0f, 75.0f)));

		agent1->SteeringComponent().OnTargetReached(
			XCS::Delegate2<void, XCS::Agent*, XCS::Vector2f&>::FromMethod<DemoScreen, &DemoScreen::OnAgentReachedPositionHigh>(this));
	}
}

void DemoScreen::Demo11( std::string extraInfo )
{
	XCS::Agent* agent1 = CreateAgent(XCS::Vector2f(-30.0f, 0.0f), XCS::Vector2f(30.0f, 0.0f));
	agent1->SteeringComponent().OnTargetReached(
		XCS::Delegate2<void, XCS::Agent*, XCS::Vector2f&>::FromMethod<DemoScreen, &DemoScreen::OnAgentReachedPosition0>(this));

	XCS::Agent* agent2 = CreateAgent(XCS::Vector2f(30.0f, 0.0f), XCS::Vector2f(-30.0f, 0.0f));	
	agent2->SteeringComponent().OnTargetReached(
		XCS::Delegate2<void, XCS::Agent*, XCS::Vector2f&>::FromMethod<DemoScreen, &DemoScreen::OnAgentReachedPosition0>(this));
}

void DemoScreen::Demo12( std::string extraInfo )
{
	for(int i = 0; i < 40; ++i)
	{
		XCS::Agent* agent1 = CreateAgent(
			XCS::Vector2f(XF::Random::NextFloat(-75.0f, 75.0f), XF::Random::NextFloat(-75.0f, 75.0f)),
			XCS::Vector2f(XF::Random::NextFloat(-75.0f, 75.0f), XF::Random::NextFloat(-75.0f, 75.0f)));

		agent1->SteeringComponent().OnTargetReached(
			XCS::Delegate2<void, XCS::Agent*, XCS::Vector2f&>::FromMethod<DemoScreen, &DemoScreen::OnAgentReachedPositionHigh>(this));
	}
}

void DemoScreen::AutoTests( std::string extraInfo )
{
	ILOG << "Begin automatic FPS tests";
	ILOG << "Press SPACE to cancel";

	mFPSLogs.clear();

	mAutoTestsOn = true;

	mSetType = 0;
	mIteration = 0;
	mAgentCount = 0;

	mMinFPS = XF::MathHelper::Infinity();
	mMaxFPS = -XF::MathHelper::Infinity();

	mMaxSets = 3;
	mMaxIterations = 20;
	mMaxAgentCount = 20;
	mTestTime = 20.0f;

	mTimeTillNextTest = mTestTime;

	mFPSLogs.resize(mMaxSets);

	for(int i = 0; i < mMaxSets; ++i)
	{
		mFPSLogs[i].resize(mMaxIterations);

		for(int j = 0; j < mMaxIterations; ++j)
		{
			mFPSLogs[i][j].resize(mMaxAgentCount);
		}
	}
}

void DemoScreen::CreateCamera()
{
	mCamera = new XF::OrbitalCamera(mRenderer);
	mCamera->Pitch(-1.0f);
	mCamera->Distance(100.0f);
}
void XCS::Agent::PrintAgentDetailsToLog(XCS::Agent *agent)
{
	LOG << "XCS::Agent :"
		<< " {O: " << agent->Personality().O
		<< " C: " << agent->Personality().C
		<< " E: " << agent->Personality().E
		<< " A: " << agent->Personality().A
		<< " N: " << agent->Personality().N << "}";

	LOG << " LD: " << agent->Personality().LookDistance
		<< ", DV: " << agent->Personality().DesiredVelocity
		<< ", SH: " << agent->Personality().Shyness
		<< ", TH: " << agent->Personality().Threat;
	LOG << " TT: " << agent->Personality().ThreatTolerance
		<< ", ST: " << agent->Personality().Stubbornness
		<< ", AC: " << agent->Personality().AvoidCongestion
		<< ", TC: " << agent->Personality().TargetChange;
}
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
#include "../XCS/CrowdSimulator.h"
#include "../CrowdSimDebugRendererMkII.h"

#include <Ogre.h>
#include <vector>

#include "XFramework.h"
#include <list>

struct FPSLog
{
	float mMin;
	float mAverage;
	float mMax;
};

struct Demo
{
	CEGUI::ButtonBase* mButton;
	XF::Delegate1<void, std::string> mOnClick;
	std::string mExtraData;
};

class DemoScreen :
	public XF::GameScreen
{
private:
	typedef std::vector<FPSLog> FullSetOfFPSLogs;
	typedef std::vector<FullSetOfFPSLogs> FPSLoggerIteration;
	std::vector<FPSLoggerIteration> mFPSLogs;

	float mMinFPS, mMaxFPS;

	int mMaxSets, mMaxIterations, mMaxAgentCount;
	int mSetType, mIteration, mAgentCount;

	bool mAutoTestsOn;
	float mTimeTillNextTest, mTestTime;

	XCS::CrowdSimulator *mCrowdSim;
	CrowdSimDebugRendererMkII *mCrowdSimDebugRender;
	XCS::LineObstacle* mMakingObstacle;
	XF::OrbitalCamera *mCamera;

	std::vector<XCS::Entity*> mAgentsBeingCreated;

	bool mSelectingAgents;
	XF::Vector2 mSelectionStartPos;

	bool mBulletTime;
	XF::CEGUIComponent* mGUI;

	Ogre::SceneNode* mSelectRect;

	CEGUI::Window* mDemoSheet;
	CEGUI::FrameWindow* mRootDemoWindow;
	std::vector<Demo> mDemos;
	bool RunDemoButton( const CEGUI::EventArgs &args );
	void RegisterDemo(std::string text, XF::Delegate1<void, std::string> onClick, std::string extraInfo);

	XCS::Agent* CreateAgent(XCS::Vector2f cPos, XCS::Vector2f goal);
	XCS::Agent* CreateAgent(XCS::Vector2f cPos, XCS::Vector2f goal, float o, float c, float e, float a, float n );

	void OnAgentReachedPositionHigh(XCS::Agent *agent, XCS::Vector2f &pos);
	void OnAgentReachedPositionMedium(XCS::Agent *agent, XCS::Vector2f &pos);
	void OnAgentReachedPositionLow(XCS::Agent *agent, XCS::Vector2f &pos);
	void OnAgentReachedPosition0(XCS::Agent *agent, XCS::Vector2f &pos);

	void CreateCrowdSim();
	void CreateFloorPlane();
	void CreateSelectionRectangle();
	void InitializeGUI();
	void RegisterDebugVariables();

	void HandleSimInteraction( XF::Input * input );
	void HandleCameraControls( XF::Input * input, double dT );

	void UpdateTrackedVariables();
	void HandleAutoFPSTesting( double dT );

	void Demo1(std::string extraInfo);
	void Demo2(std::string extraInfo);
	void Demo3(std::string extraInfo);
	void Demo4(std::string extraInfo);
	void Demo5(std::string extraInfo);
	void Demo6(std::string extraInfo);
	void Demo7(std::string extraInfo);
	void Demo8(std::string extraInfo);
	void Demo9(std::string extraInfo);
	void Demo10(std::string extraInfo);
	void Demo11(std::string extraInfo);
	void Demo12(std::string extraInfo);

	void AutoTests(std::string extraInfo);

public:
	void OnInitialise();

	void CreateCamera();
	void OnHandleInput(double dT, XF::Input *input);
	void OnUpdate(double dT);
	void OnShutDown();
	bool OnHandleMessage(const XF::Message &message);

	bool ClickSim( XF::Vector2 &clickPos );

public:
	DemoScreen();
	~DemoScreen();
};

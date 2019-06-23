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

#include "DissertationApplication.h"

#include "GameScreens/DemoScreen.h"

void OnDebugVariableRegistered(std::string name)
{
	LOG << "Debug variable registered: " << name;
}

void OnDebugVariableChanged(std::string name, std::string value)
{
	LOG << "Debug variable " << name << " changed value to: " << value;
}

void OnDebugVariableUnregistered(std::string name)
{
	LOG << "Debug variable unregistered: " << name;
}

DissertationApplication::DissertationApplication()
{
}

DissertationApplication::~DissertationApplication()
{
}

bool DissertationApplication::OnInitialise()
{
	//XF_LOGGER->OnLogMessage.RegisterMethod<DissertationApplication, &DissertationApplication::LogToCout>(this);

	//XF::DebugVariableTracker::OnVariableRegisteredEvent.RegisterFunction<&OnDebugVariableRegistered>();
	//XF::DebugVariableTracker::OnVariableChangedEvent.RegisterFunction<&OnDebugVariableChanged>();
	//XF::DebugVariableTracker::OnVariableUnregisteredEvent.RegisterFunction<&OnDebugVariableUnregistered>();

	this->AddComponent(new XF::CEGuiDebuggingTools(this, "TaharezLook"), "XCEGuiDebuggingTools");
	AddComponent(new XF::FPSTracker(this), "FPSTracker");

	XF::GameScreenManagerComponent* gsm = new XF::GameScreenManagerComponent(this);
	this->AddComponent(gsm, "Screen manager");	
	gsm->AddNewScreen(new DemoScreen());

	return true;
}

void DissertationApplication::OnHandleInput(double dT, XF::Input *input)
{
	if(input->IsKeyPress(1))
		QuitGame();
}

void DissertationApplication::OnUpdate(double dT)
{
}

void DissertationApplication::OnDraw()
{
}

void DissertationApplication::OnShutDown()
{

}

void DissertationApplication::LogToCout( std::string message )
{
	std::cout << message << "\n";
}
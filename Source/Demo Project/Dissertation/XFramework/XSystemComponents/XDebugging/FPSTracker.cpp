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

#include "FPSTracker.h"
#include "DebugVariableTracker.h"

namespace XF
{
	float FPSTracker::smLastFPS = 0.0f;

	FPSTracker::FPSTracker(Application *ownerApp)
		: SystemComponent(ownerApp)
	{
	}

	FPSTracker::~FPSTracker()
	{
	}

	void FPSTracker::OnInitialise()
	{
		for(int i = 0; i < XF_FPS_TRACKER_ACCURACY; ++i)
			mFPSValues[i] = 0.0f;

		DebugVariableTracker::RegisterVariable(PropertySerializer::TYPE_FLOAT, "FPS");
	}

	void FPSTracker::OnUpdate(double dT)
	{
		for(int i = 0; i < XF_FPS_TRACKER_ACCURACY - 1; ++i)
			mFPSValues[i] = mFPSValues[i + 1];

		mFPSValues[XF_FPS_TRACKER_ACCURACY - 1] = 1.0f / dT;

		float fps = WorkoutFPS();

		smLastFPS = fps;

		DebugVariableTracker::UpdateVariable("FPS", PropertySerializer::ToString(fps));
	}

	void FPSTracker::OnShutDown()
	{
		DebugVariableTracker::UnregisterVariable("FPS");
	}

	float FPSTracker::WorkoutFPS()
	{
		float fps = 0.0f;

		for(int i = 0; i < XF_FPS_TRACKER_ACCURACY; ++i)
			fps += mFPSValues[i];

		return (fps /= XF_FPS_TRACKER_ACCURACY);
	}
}

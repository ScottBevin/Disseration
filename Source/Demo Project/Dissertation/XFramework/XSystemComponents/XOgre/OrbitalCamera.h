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

#include "../../XUtilities/MathHelper.h"
#include "OgreRenderer.h"
#include "../../XFrameworkTypedefsAndDefines.h"
namespace XF
{
	class OrbitalCamera
	{
	private:
		Ogre::SceneNode *mTargetNode;
		Ogre::SceneNode *mCameraNode;
		Ogre::Camera *mCamera;

		std::string mName;

		XF::OgreRenderer *mRenderer;

		float mTightness;

		float mDistance, mDesiredDistance;

		float mYaw, mDesiredYaw;
		float mPitch, mDesiredPitch;
		float mRoll, mDesiredRoll;

		XF::Vector3 mDesiredTargetPos;

		void UpdatePosition( float dT );
		void UpdateDistance( float dT );
		void UpdateOrientation( float dT );

	public:
		OrbitalCamera(XF::OgreRenderer *renderer);
		~OrbitalCamera();

		float Tightness() const { return mTightness; }
		void Tightness(float val) { mTightness = val; }

		float Distance() const { return mDistance; }
		void Distance(float val, bool instantUpdate = false);
		void AddToDistance(float val, bool instantUpdate = false){Distance(mDesiredDistance + val, instantUpdate);}

		float Yaw() const { return mYaw; }
		void Yaw(float val, bool instantUpdate = false);
		void AddToYaw(float val, bool instantUpdate = false){Yaw(mDesiredYaw + val, instantUpdate);}

		float Pitch() const { return mPitch; }
		void Pitch(float val, bool instantUpdate = false);
		void AddToPitch(float val, bool instantUpdate = false){Pitch(mDesiredPitch + val, instantUpdate);}

		float Roll() const { return mRoll; }
		void Roll(float val, bool instantUpdate = false);
		void AddToRoll(float val, bool instantUpdate = false){Roll(mDesiredRoll + val, instantUpdate);}

		XF::Vector3 FocalPoint(){return mTargetNode->getPosition();}
		void FocalPoint(XF::Vector3 val, bool instantUpdate = false);
		void AddToFocalPoint(XF::Vector3 val, bool instantUpdate = false){FocalPoint(mDesiredTargetPos + val, instantUpdate);}
		void AddToFocalPointRelative(XF::Vector3 val, bool instantUpdate = false);//chris

		void Update(float dT);
	};
}

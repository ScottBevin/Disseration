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

#include "OrbitalCamera.h"

#include "../XDebugging/DebugVariableTracker.h"
#include "../../XUtilities/Utilities.h"

namespace XF
{
	OrbitalCamera::OrbitalCamera(XF::OgreRenderer *renderer)
	{
		mRenderer = renderer;

		mCamera = mRenderer->GetDefaultCamera();

		mName = "OrbitalCamera: " + XF::Utilities::CreateRandomString();

		mTargetNode = mRenderer->GetDefaultSceneManager()->getRootSceneNode()->createChildSceneNode(mName + "_target");
		mCameraNode = mTargetNode->createChildSceneNode(mName);

		mCameraNode->setAutoTracking(true, mTargetNode);

		mCamera->detatchFromParent();
		mCameraNode->attachObject(mCamera);

		mTightness = 0.8f;

		mDistance = mDesiredDistance = 200.0f;

		mYaw = mDesiredYaw = 0.0f;
		mPitch = mDesiredPitch = 0.0f;
		mRoll = mDesiredRoll = 0.0f;

		mDesiredTargetPos = mTargetNode->getPosition();
	}

	OrbitalCamera::~OrbitalCamera()
	{
		mCameraNode->detachAllObjects();

		mRenderer->GetDefaultSceneManager()->destroySceneNode(mName);
		mRenderer->GetDefaultSceneManager()->destroySceneNode(mName + "_target");
	}

	void OrbitalCamera::Update( float dT )
	{
		mTargetNode->yaw(Ogre::Radian(1.0f * dT));

		UpdatePosition(dT);

		UpdateOrientation(dT);

		UpdateDistance(dT);
	}

	void OrbitalCamera::UpdatePosition( float dT )
	{
		XF::Vector3 displacement;
		XF::Vector3 pos = mTargetNode->getPosition();

		displacement = (mDesiredTargetPos - pos) * mTightness;
		mTargetNode->translate(displacement * dT);
	}

	void OrbitalCamera::UpdateOrientation( float dT )
	{
		mYaw =		XF::MathHelper::Lerp(mYaw, mDesiredYaw, mTightness * dT);
		mPitch =	XF::MathHelper::Lerp(mPitch, mDesiredPitch, mTightness * dT);
		mRoll =		XF::MathHelper::Lerp(mRoll, mDesiredRoll, mTightness * dT);

		Ogre::Matrix3 mat;
		mat.FromEulerAnglesYXZ(Ogre::Radian(mYaw), Ogre::Radian(mPitch) ,Ogre::Radian(0.0f));
		Ogre::Quaternion targetOrientation;
		targetOrientation.FromRotationMatrix(mat);

		mat.FromEulerAnglesXYZ(Ogre::Radian(0.0f), Ogre::Radian(0.0f), Ogre::Radian(mRoll));
		Ogre::Quaternion camOrientation;
		camOrientation.FromRotationMatrix(mat);

		//mCameraNode->setOrientation(camOrientation);
		mTargetNode->setOrientation(targetOrientation);
	}

	void OrbitalCamera::UpdateDistance( float dT )
	{
		mDistance = XF::MathHelper::Lerp(mDistance, mDesiredDistance, mTightness * dT);
		XF::Vector3 vec = (mCameraNode->getPosition() - mTargetNode->getPosition()).normalisedCopy() * mDistance;

		mCameraNode->setPosition(XF::Vector3(0.0f, 0.0f, mDistance));
	}

	void OrbitalCamera::Yaw( float val, bool instantUpdate /*= false*/ )
	{
		mDesiredYaw = val;

		if(instantUpdate)
		{
			mYaw = val;
		}
	}

	void OrbitalCamera::Pitch( float val, bool instantUpdate /*= false*/ )
	{
		mDesiredPitch = val;

		if(instantUpdate)
		{
			mPitch = val;
		}
	}

	void OrbitalCamera::Roll( float val, bool instantUpdate /*= false*/ )
	{
		mDesiredRoll = val;

		if(instantUpdate)
		{
			mRoll = val;
		}
	}

	void OrbitalCamera::Distance( float val, bool instantUpdate /*= false*/ )
	{
		mDesiredDistance = val;

		if(instantUpdate)
		{
			mDistance = val;
		}
	}

	void OrbitalCamera::FocalPoint( XF::Vector3 val, bool instantUpdate /*= false*/ )
	{
		mDesiredTargetPos = val;

		if(instantUpdate)
		{
			mTargetNode->setPosition(val);
		}
	}

	void OrbitalCamera::AddToFocalPointRelative( XF::Vector3 val, bool instantUpdate /*= false*/ )
	{	
		//mDesiredTargetPos = Vector3::ZERO;
		mDesiredTargetPos.z += sin(mYaw)*val.z;
		mDesiredTargetPos.x += cos(mYaw)*val.z;

		mDesiredTargetPos.z += sin(mYaw+90)*val.x;
		mDesiredTargetPos.x += cos(mYaw+90)*val.x;
		if(instantUpdate)
		{	
			mTargetNode->setPosition(mDesiredTargetPos);
		}
	}
}
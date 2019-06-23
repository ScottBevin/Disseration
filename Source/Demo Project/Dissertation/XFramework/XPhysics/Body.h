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
#include "XDelegates/Delegate.h"
#include "../XFrameworkTypedefsAndDefines.h"

namespace XF
{
	class Body
	{
	private:
		float mOldRadius, mRadius;
		XF::Vector3 mPosition, mOldPosition;
		XF::Vector3 mVelocity;
		int mCollisionCatagory;

		XF::Delegate2<bool, Body*, Body*> mOnCollision;

	public:
		Body(float x, float y, float z, float radius) : 
			mPosition(x, y, z), mRadius(radius),
				mOldPosition(x, y, z), mOldRadius(radius), mCollisionCatagory(-1){}
		Body(): 
			mPosition(0.0f, 0.0f, 0.0f), mRadius(0.0f),
			mOldPosition(0.0f, 0.0f, 0.0f), mOldRadius(0.0f), mCollisionCatagory(-1){}
		Body(XF::Vector3 pos, float radius): 
			mPosition(pos), mRadius(radius),
			mOldPosition(pos), mOldRadius(radius), mCollisionCatagory(-1){}
		Body(Body &body): 
			mPosition(body.mPosition), mRadius(body.mRadius),
			mOldPosition(body.mOldPosition), mOldRadius(body.mOldRadius), mCollisionCatagory(body.mCollisionCatagory){}
		~Body() {}

		void Position(float x, float y, float z)
		{
			mOldPosition = mPosition;
			mPosition = XF::Vector3(x, y, z);
		}

		void Position(XF::Vector3 pos)
		{
			mOldPosition = mPosition;
			mPosition = pos;
		}

		XF::Vector3 Position(){ return mPosition; }
		float X(){return mPosition.x;}
		float Y(){return mPosition.y;}
		float Z(){return mPosition.z;}

		XF::Vector3 Velocity(){ return mVelocity; }
		void Velocity(XF::Vector3 velocity){ mVelocity = velocity; }

		float Radius(){ return mRadius; }
		void Radius(float radius){ mOldRadius = mRadius; mRadius = radius; }

		bool HasMoved(){ return !(mOldPosition != mPosition && mOldRadius == mRadius); }

		void OnCollision(XF::Delegate2<bool, Body*, Body*> function){mOnCollision = function;}
		XF::Delegate2<bool, Body*, Body*> OnCollision(){ return mOnCollision; }

		int CollisionCatagory(){ return mCollisionCatagory; }
		void CollisionCatagory(int catagory){ mCollisionCatagory = catagory; }
	};
}
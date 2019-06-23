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

#ifndef AGENTSTEERINGCOMPONENT_H
#define AGENTSTEERINGCOMPONENT_H

#include "Delegates/Delegate.h"
#include "Vector2.h"

namespace XCS
{
	class Agent;

	class AgentSteeringComponent
	{
	private:
		const float mDecelerationTweaker;

		const float mSeekWeight;
		const float mFleeWeight;
		const float mArriveWeight;
		const float mMinDistToTargetSquared;

	public:
		enum SteeringBehaviours
		{
			NONE	= 0x00000,
			SEEK	= 0x00002,
			FLEE	= 0x00004,
			ARRIVE	= 0x00008,
		};

	private:
		enum Deceleration
		{
			SLOW = 3,
			NORMAL = 2,
			FAST = 1,
		};

	private:
		Agent *mOwningAgent;
		int mFlags;
		Vector2f mTargetPos, mAgentPosition, mAgentVelocity;
		float mDecelerationInverse;
		Delegate2<void, Agent*, Vector2f&> mOnTargetReached;
		bool mAtTarget;

		Vector2f Seek();
		Vector2f Flee();
		Vector2f Arrive();

		Vector2f CalculatedPriorityWeighted();
		bool AccumulateForce(Vector2f &runningTotal, Vector2f &forceToAdd);
		bool TargetReached();

	public:
		AgentSteeringComponent(Agent *owningAgent);
		~AgentSteeringComponent();

		Vector2f CalculateForce();

		bool IsOn(SteeringBehaviours behaviour){return (mFlags & behaviour) == behaviour;}
		void TurnOn(SteeringBehaviours behaviour){mFlags |= behaviour;}
		void TurnOff(SteeringBehaviours behaviour){if(IsOn(behaviour)) mFlags ^= behaviour;}

		void DecelerationType(Deceleration dec)
		{
			mDecelerationInverse = 1.0f / ((float)dec * mDecelerationTweaker);
		}

		Vector2f Target(){return mTargetPos;}
		void Target(Vector2f val){mTargetPos = val;}
		void OnTargetReached(Delegate2<void, Agent*, Vector2f&> func){mOnTargetReached = func;}
	};
}

#endif
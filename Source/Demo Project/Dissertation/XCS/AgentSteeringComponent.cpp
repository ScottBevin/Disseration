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

#include "AgentSteeringComponent.h"

#include "Agent.h"

namespace XCS
{
	AgentSteeringComponent::AgentSteeringComponent(Agent *owningAgent) : 
		mOwningAgent(owningAgent), 
		mFlags(0), 
		mDecelerationTweaker(0.3f),
		mSeekWeight(0.3f),
		mFleeWeight(0.3f),
		mArriveWeight(0.4f),
		mMinDistToTargetSquared(250.0f),
		mAtTarget(false)
{
	DecelerationType(Deceleration::NORMAL);
}

AgentSteeringComponent::~AgentSteeringComponent()
{
}

Vector2f AgentSteeringComponent::Seek()
{
	return (
		((mTargetPos - mAgentPosition).Normalise() * mOwningAgent->Personality().DesiredVelocity)	//Desired velocity
		- mAgentVelocity);
}

Vector2f AgentSteeringComponent::Flee()
{
	return (
		((mAgentPosition - mTargetPos).Normalise() * mOwningAgent->Personality().DesiredVelocity)	//Desired velocity
		- mAgentVelocity);
}

Vector2f AgentSteeringComponent::Arrive()
{
	Vector2f toTarget = mTargetPos - mAgentPosition;
	float dist = toTarget.Length(); //distance to target

	if(dist > 0)
	{
		float speed = std::min(dist * mDecelerationInverse, mOwningAgent->Personality().DesiredVelocity);
		return ((toTarget * (speed / dist)) //desired velocity 
			- mAgentVelocity);
	}

	return Vector2f::ZERO();
}

bool AgentSteeringComponent::AccumulateForce(Vector2f &runningTotal, Vector2f &forceToAdd)
{
	float magnitudeRemaining = mOwningAgent->Personality().DesiredVelocity - runningTotal.Length();

	if(magnitudeRemaining <= 0.0f) return false;

	if(forceToAdd.Length() < magnitudeRemaining)
		runningTotal += forceToAdd;
	else
		runningTotal += (forceToAdd.Normalise() * magnitudeRemaining);

	return true;
}

Vector2f AgentSteeringComponent::CalculateForce()
{
	mAgentPosition = mOwningAgent->Position();
	mAgentVelocity = mOwningAgent->Velocity();

	Vector2f steeringForce = CalculatedPriorityWeighted();

	if(!mAtTarget)
	{
		if(TargetReached())
		{
			mAtTarget = true;

			if(mOnTargetReached)
				mOnTargetReached(mOwningAgent, mTargetPos);
		}
	}
	else
	{
		if(!TargetReached())
			mAtTarget = false;
	}

	return steeringForce;
}

Vector2f AgentSteeringComponent::CalculatedPriorityWeighted()
{
	Vector2f steeringForce = Vector2f::ZERO();
	Vector2f force;

	if(IsOn(SteeringBehaviours::ARRIVE))
	{
		force = Arrive() * mArriveWeight;

		if(!AccumulateForce(steeringForce, force)) return steeringForce;
	}

	if(IsOn(SteeringBehaviours::SEEK))
	{
		force = Seek() * mSeekWeight;

		if(!AccumulateForce(steeringForce, force)) return steeringForce;
	}

	if(IsOn(SteeringBehaviours::FLEE))
	{
		force = Flee() * mFleeWeight;

		if(!AccumulateForce(steeringForce, force)) return steeringForce;
	}

	return steeringForce;
}

bool AgentSteeringComponent::TargetReached()
{
	return ((mTargetPos - mAgentPosition).SquaredLength() <= mMinDistToTargetSquared);
}
}

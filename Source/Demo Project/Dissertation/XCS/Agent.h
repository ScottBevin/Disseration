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

#ifndef AGENT_H
#define AGENT_H

#include "Entity.h"

#include <algorithm>
#include <list>

#include "AgentSteeringComponent.h"
#include "VelocityObstacle.h"

namespace XCS
{
	struct AgentPersonalityDescription
	{
		float O, C, E, A, N;
		float LookDistance;
		float DesiredVelocity;
		float Shyness;
		float Threat;
		float ThreatTolerance;
		float Stubbornness;
		float AvoidCongestion;
		float TargetChange;
	};

	class Agent : public Entity
	{
	private:
		Vector2f mHeadingVector, mNewVelocity, mSteeringVelocity;
		AgentPersonalityDescription mPersonality;

		int mNoVelSamples;
		AgentSteeringComponent mSteering;

		VelocityObstacle mVo;
		std::list<Vector2f> mGeometryEdges;

		// Group stuff
		Agent* mLeader;
		Vector2f mLeaderOffset;

		static int smNextID;
		int mAgentID;
		std::vector<Entity*> mThisFramesObstacles;

		void SortObstaclesByDistance();

	public:
		Agent(AgentPersonalityDescription personality);
		~Agent();

		void SetThisFramesObstacles(std::vector<Entity*> valVec){mThisFramesObstacles = valVec;}
		std::vector<Entity*> GetThisFramesObstacles(){ return mThisFramesObstacles;}

		void ApplySteeringForceToAgent();
		void AvoidOtherAgentsSamplingMethod(double dT);
		void AvoidOtherAgentsSamplingMethodSlow(double dT);
		void AvoidOtherAgentsGeometricMethod(double dT);
		void FinalizeVelocity();

		void Update(double dT);

		AgentPersonalityDescription Personality() const { return mPersonality; }

		AgentSteeringComponent& SteeringComponent(){return mSteering;}

		void SetLeader(Agent* leader, Vector2f offset){mLeader = leader; mLeaderOffset = offset;}

		Vector2f SteeringVelocity() const { return mSteeringVelocity; }
		Vector2f HeadingVector() const { return mHeadingVector; }

		VelocityObstacle GetVelocityObstacle() const { return mVo; }

		void PrintAgentDetailsToLog(XCS::Agent *agent);
		std::vector<std::pair<Vector2f, Vector2f> > mVelocitysSampled;
	};
}

#endif
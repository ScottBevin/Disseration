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

#ifndef CROWDSIMULATOR_H
#define CROWDSIMULATOR_H

#include <vector>
#include <Ogre.h>

#include "Delegates/Event.h"
#include "Agent.h"
#include "QuadTree.h"
#include "LineObstacle.h"

namespace XCS
{
	class CrowdSimulator
	{
	private:
		std::vector<Agent*> mAgents;
		std::vector<LineObstacle*> mObstacles;

		QuadTree *mSpatialPart;
		
		float mMinLookDist, mMaxLookDist;
		float mMinDesVel, mMaxDesVel;
		float mMinShyness, mMaxShyness;

		float GenerateLookDistance( float openness, float concienciousness, float extroversion, float agreeableness, float neuroticism );
		float GenerateDesiredVelocity( float openness, float concienciousness, float extroversion, float agreeableness, float neuroticism );
		float GenerateShyness( float openness, float concienciousness, float extroversion, float agreeableness, float neuroticism );
		float GenerateAC( float openness, float concienciousness, float extroversion, float agreeableness, float neuroticism );
		float GenerateStubbornness( float openness, float concienciousness, float extroversion, float agreeableness, float neuroticism );
		float GenerateTargetChange( float openness, float concienciousness, float extroversion, float agreeableness, float neuroticism );
		float GenerateThreat( float openness, float concienciousness, float extroversion, float agreeableness, float neuroticism );
		float GenerateTolerance( float openness, float concienciousness, float extroversion, float agreeableness, float neuroticism );

	public:
		Event1<void, Agent*> mOnAddAgentEvent;
		Event1<void, Agent*> mOnKillAgentEvent;

		Event1<void, LineObstacle*> mOnAddObstacleEvent;
		Event1<void, LineObstacle*> mOnKillObstacleEvent;

	public:
		CrowdSimulator(
			float minLookDist,
			float maxLookDist,
			float minDesiredVelocity,
			float maxDesiredVelocity,
			float minShyness,
			float maxShyness);
		~CrowdSimulator();

		Agent* AddAgent(
			Vector2f position, 
			Vector2f goal, 
			float openness,
			float concienciousness,
			float extroversion,
			float agreeableness,
			float neuroticism);

		void KillAgent(Agent* agent);

		LineObstacle* AddObstacle(Vector2f startPos, Vector2f endPos );
		void KillObstacle(LineObstacle* obstacle);

		void KillEntity(Entity* entity);

		void KillAll();
		void KillAllAgents();

		void UpdateSim(double dT);

		std::vector<Agent*> GetAllAgents(){return mAgents;}
		std::vector<LineObstacle*> GetAllObstacles(){return mObstacles;}

		Agent* SelectAgent(Vector2f pos);
		std::vector<Agent*> SelectAgents( Vector2f min, Vector2f max);

		LineObstacle* SelectObstacle(Vector2f pos);

		std::vector<Entity*> SelectEntities( Vector2f min, Vector2f max);
		Entity* SelectEntities(Vector2f pos);

		unsigned int NumAgents(){return mAgents.size();}
		unsigned int NumObstacles(){return mObstacles.size();}

		QuadTree * GetQuadTree() const { return mSpatialPart; }
	};
}

#endif
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

#include "CrowdSimulator.h"
#include "Math.h"
#include <omp.h>

namespace XCS
{
	CrowdSimulator::CrowdSimulator(
		float minLookDist,
		float maxLookDist,
		float minDesiredVelocity,
		float maxDesiredVelocity,
		float minShyness,
		float maxShyness)
	{
		mSpatialPart = new QuadTree();

		mMinLookDist = minLookDist;
		mMaxLookDist = maxLookDist;
		mMinDesVel = minDesiredVelocity;
		mMaxDesVel = maxDesiredVelocity;
		mMinShyness = minShyness;
		mMaxShyness = maxShyness;
	}

	CrowdSimulator::~CrowdSimulator()
	{
		for(int i = 0; i < (int)mAgents.size(); ++i)
		{
			delete mAgents[i];
		}

		for(int i = 0; i < (int)mObstacles.size(); ++i)
		{
			delete mObstacles[i];
		}

		delete mSpatialPart;
	}

	Agent* CrowdSimulator::AddAgent(
		Vector2f position, 
		Vector2f goal, 
		float openness,
		float concienciousness,
		float extroversion,
		float agreeableness,
		float neuroticism)
	{

		Math::Clamp(openness, 0.0f, 1.0f);
		Math::Clamp(concienciousness, 0.0f, 1.0f);
		Math::Clamp(extroversion, 0.0f, 1.0f);
		Math::Clamp(agreeableness, 0.0f, 1.0f);
		Math::Clamp(neuroticism, 0.0f, 1.0f);

		AgentPersonalityDescription persDesc;

		persDesc.O = openness;
		persDesc.C = concienciousness;
		persDesc.E = extroversion;
		persDesc.A = agreeableness;
		persDesc.N = neuroticism;

		persDesc.DesiredVelocity = GenerateDesiredVelocity(openness, concienciousness, extroversion, agreeableness, neuroticism);
		persDesc.LookDistance = GenerateLookDistance(openness, concienciousness, extroversion, agreeableness, neuroticism);
		persDesc.Shyness = GenerateShyness(openness, concienciousness, extroversion, agreeableness, neuroticism);
		persDesc.AvoidCongestion = GenerateAC(openness, concienciousness, extroversion, agreeableness, neuroticism);
		persDesc.Stubbornness = GenerateStubbornness(openness, concienciousness, extroversion, agreeableness, neuroticism);
		persDesc.TargetChange = GenerateTargetChange(openness, concienciousness, extroversion, agreeableness, neuroticism);
		persDesc.Threat = GenerateThreat(openness, concienciousness, extroversion, agreeableness, neuroticism);
		persDesc.ThreatTolerance = GenerateTolerance(openness, concienciousness, extroversion, agreeableness, neuroticism);

		Agent* newAgent = new Agent(persDesc);

		newAgent->Position(position);

		newAgent->SteeringComponent().Target(goal);
		newAgent->SteeringComponent().TurnOn(AgentSteeringComponent::SteeringBehaviours::ARRIVE);

		mSpatialPart->AddEntity(newAgent);
		mAgents.push_back(newAgent);
		mOnAddAgentEvent.Invoke(newAgent);

		return newAgent;
	}

	void CrowdSimulator::KillAgent(Agent* agent)
	{
		mSpatialPart->RemoveEntity(agent);

		std::vector<Agent*>::iterator it = std::find(mAgents.begin(), mAgents.end(), agent);

		if(it != mAgents.end())
		{
			mAgents.erase(it);
		}

		mOnKillAgentEvent.Invoke(agent);

		delete agent;
	}


	void CrowdSimulator::UpdateSim(double dT)
	{
		mSpatialPart->SetUp();
		mSpatialPart->TagAllCollidingAgents();

		int loopMax = (int)mAgents.size();

#pragma omp parallel for 
		for(int i = 0; i < loopMax; ++i)
		{
			Agent* agent = mAgents[i];

			agent->SetThisFramesObstacles(mSpatialPart->EntitiesNearEntity(agent, agent->Personality().LookDistance));
			agent->ApplySteeringForceToAgent();
			agent->AvoidOtherAgentsSamplingMethod(dT);
			agent->FinalizeVelocity();
		}

#pragma omp parallel for
		for(int i = 0; i < loopMax; ++i)
		{
			mAgents[i]->Update(dT);
		}
	}

	LineObstacle* CrowdSimulator::AddObstacle( Vector2f startPos, Vector2f endPos )
	{
		LineObstacle* newObstacle = new LineObstacle(startPos, endPos);

		mSpatialPart->AddEntity(newObstacle);

		mObstacles.push_back(newObstacle);

		mOnAddObstacleEvent.Invoke(newObstacle);

		return newObstacle;
	}

	void CrowdSimulator::KillObstacle( LineObstacle* obstacle )
	{
		mSpatialPart->RemoveEntity(obstacle);

		std::vector<LineObstacle*>::iterator it = std::find(mObstacles.begin(), mObstacles.end(), obstacle);

		if(it != mObstacles.end())
		{
			mObstacles.erase(it);
		}

		mOnKillObstacleEvent.Invoke(obstacle);

		delete obstacle;
	}

	std::vector<Agent*> CrowdSimulator::SelectAgents( Vector2f min, Vector2f max)
	{
		std::vector<Agent*> retVec;

		mSpatialPart->SetUp();
		std::vector<Entity*> intersectAgents = mSpatialPart->EntitiesInArea(min, max);

		for(unsigned int i = 0; i < intersectAgents.size(); ++i)
		{
			if(intersectAgents[i]->Type() == Entity::AGENT)
				retVec.push_back((Agent*)intersectAgents[i]);
		}

		return retVec;
	}

	Agent* CrowdSimulator::SelectAgent(Vector2f pos)
	{
		mSpatialPart->SetUp();
		std::vector<Entity*> intersectAgents = mSpatialPart->EntitiesInArea(Vector2f(pos.mX, pos.mY), 0.1f);

		if(intersectAgents.size() > 0 && intersectAgents[0]->Type() == Entity::AGENT)
			return (Agent*)intersectAgents[0];
		else
			return NULL;
	}

	LineObstacle* CrowdSimulator::SelectObstacle( Vector2f pos )
	{
		mSpatialPart->SetUp();
		std::vector<Entity*> intersectAgents = mSpatialPart->EntitiesInArea(Vector2f(pos.mX, pos.mY), 0.1f);

		if(intersectAgents.size() > 0 && intersectAgents[0]->Type() == Entity::OBSTACLE)
			return (LineObstacle*)intersectAgents[0];
		else
			return NULL;
	}

	std::vector<Entity*> CrowdSimulator::SelectEntities(  Vector2f min, Vector2f max)
	{
		mSpatialPart->SetUp();
		return mSpatialPart->EntitiesInArea(min, max);
	}

	Entity* CrowdSimulator::SelectEntities( Vector2f pos )
	{
		mSpatialPart->SetUp();
		std::vector<Entity*> intersectAgents = mSpatialPart->EntitiesInArea(Vector2f(pos.mX, pos.mY), 0.1f);

		if(intersectAgents.size() > 0 && intersectAgents[0]->Type())
			return (LineObstacle*)intersectAgents[0];
		else
			return NULL;
	}

	void CrowdSimulator::KillEntity( Entity* entity )
	{
		if(entity->Type() == Entity::AGENT)
			KillAgent((Agent*)entity);
		else
			KillObstacle((LineObstacle*)entity);
	}

	void CrowdSimulator::KillAll()
	{
		KillAllAgents();

		int count = mObstacles.size();
		for(int i = count - 1; i >= 0; --i)
			KillObstacle(mObstacles[i]);
	}

	float CrowdSimulator::GenerateDesiredVelocity( float openness, float concienciousness, float extroversion, float agreeableness, float neuroticism )
	{
		float value = 0.5f;

		value += Math::Lerp(-0.3f, 0.0f, Math::PercentageFromTo(openness, 0.0f, 0.5f));
		value += Math::Lerp(0.1f, 0.2f, concienciousness);
		value += Math::Lerp(-0.3f, 0.2f, extroversion);
		value += Math::Lerp( 0.0f, -0.1f, Math::PercentageFromTo(agreeableness, 0.5f, 1.0f));
		//value += Math::Lerp(-0.3f, 0.0f, Math::PercentageFromTo(neuroticism, 0.0f, 0.5f));

		Math::Clamp(value, 0.0f, 1.0f);
		return Math::Lerp(mMinDesVel, mMaxDesVel, value);
	}

	float CrowdSimulator::GenerateShyness( float openness, float concienciousness, float extroversion, float agreeableness, float neuroticism )
	{
		float value = 0.5f;

		value += Math::Lerp(-0.3f, 0.0f, Math::PercentageFromTo(openness, 0.0f, 0.5f));
		value += Math::Lerp(0.0f, -0.3f, Math::PercentageFromTo(openness, 0.5f, 1.0f));

		//value += Math::Lerp(0.1f, 0.2f, concienciousness);
		value += Math::Lerp(0.2f, -0.3f, extroversion);
		value += Math::Lerp( 0.0f, -0.2f, Math::PercentageFromTo(agreeableness, 0.5f, 1.0f));
		value += Math::Lerp( 0.0f, 0.2f, Math::PercentageFromTo(neuroticism, 0.5f, 1.0f));

		Math::Clamp(value, 0.0f, 1.0f);
		return Math::Lerp(mMinShyness, mMaxShyness, value);
	}

	float CrowdSimulator::GenerateAC( float openness, float concienciousness, float extroversion, float agreeableness, float neuroticism )
	{
		float value = 0.5f;

		value += Math::Lerp(0.0f, -0.1f, Math::PercentageFromTo(openness, 0.5f, 1.0f));
		//value += Math::Lerp(0.1f, 0.2f, concienciousness);
		value += Math::Lerp( 0.2f, -0.2f, extroversion);
		value += Math::Lerp( 0.0f, -0.1f, Math::PercentageFromTo(agreeableness, 0.5f, 1.0f));
		value += Math::Lerp( 0.2f, 0.0f, Math::PercentageFromTo(neuroticism, 0.0f, 0.5f));
		value += Math::Lerp( 0.0f, 0.2f, Math::PercentageFromTo(neuroticism, 0.0f, 0.5f));

		Math::Clamp(value, 0.0f, 1.0f);
		return value;
	}

	float CrowdSimulator::GenerateStubbornness( float openness, float concienciousness, float extroversion, float agreeableness, float neuroticism )
	{
		float value = 0.5f;

		value += Math::Lerp(0.2f, -0.2f, openness);
		value += Math::Lerp(0.1f, -0.1f, concienciousness);
		//value += Math::Lerp( 0.1f, -0.2f, extroversion);
		value += Math::Lerp( 0.4f, -0.3f, agreeableness);
		value += Math::Lerp(-0.2f, 0.0f, Math::PercentageFromTo(neuroticism, 0.0f, 0.5f));
		value += Math::Lerp( 0.0f, -0.2f, Math::PercentageFromTo(neuroticism, 0.0f, 0.5f));	

		Math::Clamp(value, 0.0f, 1.0f);
		return value + 0.5f;
	}

	float CrowdSimulator::GenerateTargetChange( float openness, float concienciousness, float extroversion, float agreeableness, float neuroticism )
	{
		float value = 0.5f;

		//value += Math::Lerp(0.2f, -0.2f, openness);
		value += Math::Lerp(0.4f, -0.2f, concienciousness);
		//value += Math::Lerp( 0.1f, -0.2f, extroversion);
		//value += Math::Lerp( 0.4f, -0.3f, agreeableness);
		//value += Math::Lerp(-0.2f, 0.0f, Math::PercentageFromTo(neuroticism, 0.0f, 0.5f));

		Math::Clamp(value, 0.0f, 1.0f);
		return Math::Lerp(mMinDesVel, mMaxDesVel, value);
	}

	float CrowdSimulator::GenerateThreat( float openness, float concienciousness, float extroversion, float agreeableness, float neuroticism )
	{
		float value = 0.5f;

		//value += Math::Lerp(0.2f, -0.2f, openness);
		value += Math::Lerp(0.2f, 0.0f, Math::PercentageFromTo(concienciousness, 0.0f, 0.5f));
		//value += Math::Lerp( 0.1f, -0.2f, extroversion);
		value += Math::Lerp( 0.3f, -0.0f, Math::PercentageFromTo(agreeableness, 0.0f, 0.5f));
		//value += Math::Lerp(-0.2f, 0.0f, Math::PercentageFromTo(neuroticism, 0.0f, 0.5f));

		Math::Clamp(value, 0.0f, 1.0f);
		return value;
	}

	float CrowdSimulator::GenerateTolerance( float openness, float concienciousness, float extroversion, float agreeableness, float neuroticism )
	{
		float value = 0.5f;

		value += Math::Lerp(-0.2f, 0.2f, openness);
		//value += Math::Lerp(0.2f, 0.0f, Math::PercentageFromTo(concienciousness, 0.0f, 0.5f));
		value += Math::Lerp( 0.2f, 0.2f, extroversion);
		value += Math::Lerp( 0.0f, 0.2f, Math::PercentageFromTo(agreeableness, 0.5f, 1.0f));
		value += Math::Lerp( 0.2f, -0.2f, neuroticism);

		Math::Clamp(value, 0.0f, 1.0f);
		return value;
	}
	float CrowdSimulator::GenerateLookDistance( float openness, float concienciousness, float extroversion, float agreeableness, float neuroticism )
	{
		float value = 0.5f;

		value += Math::Lerp(-0.3f, 0.3f, openness);

		value += Math::Lerp(-0.3f, 0.0f, Math::PercentageFromTo(concienciousness, 0.0f, 0.5f));

		Math::Clamp(value, 0.0f, 1.0f);
		return Math::Lerp(mMinLookDist, mMaxLookDist, value);
	}

	void CrowdSimulator::KillAllAgents()
	{
		int count = mAgents.size();
		for(int i = count - 1; i >= 0; --i)
			KillAgent(mAgents[i]);
	}
}
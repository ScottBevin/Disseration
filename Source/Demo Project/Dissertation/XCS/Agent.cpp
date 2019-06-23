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

#include "Agent.h"
#include "LineObstacle.h"
#include "Math.h"

namespace XCS
{
	int Agent::smNextID = 0;

	Agent::Agent(AgentPersonalityDescription personality)
		: Entity(AGENT, 1.0f), mSteering(this), mLeader(NULL)
	{
		mAgentID = smNextID;
		smNextID++;

		mNoVelSamples = 5;

		mPersonality = personality;
	}

	Agent::~Agent()
	{
	}

	void Agent::Update(double dT)
	{
		mVelocity = mNewVelocity;
		mPosition += mVelocity * dT;

		mHeadingVector = mVelocity.NormalisedCopy();
	}

	void Agent::SortObstaclesByDistance()
	{
		std::sort(mThisFramesObstacles.begin(), mThisFramesObstacles.end());
	}

	void Agent::ApplySteeringForceToAgent()
	{
		if(mLeader)
			mSteering.Target(mLeader->Position() + mLeaderOffset);

		mSteeringVelocity = mSteering.CalculateForce();
	}

	void Agent::AvoidOtherAgentsSamplingMethod(double dT)
	{
		mVelocitysSampled.clear();

		float minPenalty = Math::Infinity();
		float minTimePenatly, minDistancePenalty, minTimeToCollision;

		Vector2f candidateVelocity;
		Vector2f searchCenter = Vector2f::ZERO();

		Vector2f bestSearchCenter = Vector2f::ZERO();

		for(int i = 0; i < mNoVelSamples; ++i)
		{
			int iters = 8;

			if(i == 0) 
			{
				candidateVelocity = mSteeringVelocity;
				iters = 1;
			}

			searchCenter = bestSearchCenter;

			for(int j = 0; j < iters; ++j)
			{
				Vector2f velOffset = Vector2f::ZERO();

				if(i != 0)
				{
					float desVelScaled = mPersonality.DesiredVelocity / i;
					desVelScaled *= 0.5f;

					float angle = (Math::TwoPi() / mNoVelSamples) * i + (Math::TwoPi() / iters) * j;

					velOffset = Vector2f(cos(angle) * desVelScaled, sin(angle) * desVelScaled);

					candidateVelocity = searchCenter + velOffset;
				}		

				mVelocitysSampled.push_back(std::make_pair(searchCenter, candidateVelocity));

				//distance between candidate velocity and preferred
				float dV;

				if(mColliding)
					dV = 0;
				else
					dV = (candidateVelocity - mSteeringVelocity).Length();

				// find the smallest time to collision
				//time to collision
				float cT = Math::Infinity();

				for(unsigned int k = 0; k < mThisFramesObstacles.size(); ++k)
				{
					if(mThisFramesObstacles[k] == this)
						continue;

					Entity* otherAgent = mThisFramesObstacles[k];
					Vector2f abVector;
					float agentCollisionTime;

					if(otherAgent->Type() == Entity::AGENT)
					{
						Agent* agent = (Agent*)otherAgent;

						abVector = 2 * candidateVelocity - mVelocity - agent->Velocity();

						float ThreatModifier = (agent->mPersonality.Threat - mPersonality.ThreatTolerance);
						Math::Clamp(ThreatModifier, 0.0f, 1.0f);

						double time = 
							Math::RayDiskTimeToCollision(
							mPosition, 
							abVector, 
							agent->Position(), 
							mRadius + agent->Radius() + (agent->Radius() * ThreatModifier), 
							mColliding);

						if(mColliding)
						{
							agentCollisionTime = -ceil(time / dT);
							agentCollisionTime -= candidateVelocity.SquaredLength() / Math::Sqr(mPersonality.DesiredVelocity);
						}
						else
						{
							agentCollisionTime = time;
						}
					}
					else if(otherAgent->Type() == Entity::OBSTACLE)
					{
						LineObstacle* obs = (LineObstacle*)otherAgent;

						float time1, time2, timeA, timeB;

						time1 = Math::RayDiskTimeToCollision(mPosition, candidateVelocity, obs->StartPoint(), (mRadius * 1.0f), mColliding);
						time2 = Math::RayDiskTimeToCollision(mPosition, candidateVelocity, obs->EndPoint(), (mRadius * 1.0f), mColliding);
						timeA = Math::RayLineTimeToCollision(mPosition, candidateVelocity, obs->StartPoint() + (mRadius * 1.0f) * obs->Normal(), obs->EndPoint() + (mRadius * 1.0f) * obs->Normal(), mColliding);
						timeB = Math::RayLineTimeToCollision(mPosition, candidateVelocity, obs->StartPoint() - (mRadius * 1.0f) * obs->Normal(), obs->EndPoint() - (mRadius * 1.0f) * obs->Normal(), mColliding);

						if(mColliding)
						{
							float time = std::max(std::max(std::max(time1, time2), timeA), timeB);
							agentCollisionTime = -ceil(time / dT);
							agentCollisionTime -= candidateVelocity.SquaredLength() / Math::Sqr(mPersonality.DesiredVelocity);
						}
						else
						{
							float time = std::min(std::min(std::min(time1, time2), timeA), timeB);

							if(time < dT || Math::Sqr(time) < candidateVelocity.SquaredLength() / Math::Sqr(mPersonality.DesiredVelocity))
							{
								agentCollisionTime = time;
							}
							else
							{
								agentCollisionTime = Math::Infinity();
							}
						}
					}

					if(agentCollisionTime < cT)
					{
						cT = agentCollisionTime;

						if((mPersonality.Shyness / cT) + (dV * mPersonality.Stubbornness) >= minPenalty)
							break;
					}
				}

				float timePen = (mPersonality.Shyness / cT);

				if(cT == Math::Infinity()) timePen = 0.0f;

				float distancePen = (dV * mPersonality.Stubbornness);
				float penalty = timePen + distancePen;

				if(penalty < minPenalty)
				{
					minTimeToCollision = cT;
					minDistancePenalty = distancePen;
					minTimePenatly = timePen;
					minPenalty = penalty;
					mNewVelocity = candidateVelocity;

					if(i != 0)
						bestSearchCenter = (searchCenter + velOffset) * 0.5f;
				}
			}
		}

		if(minTimeToCollision <= 0.5f && minTimeToCollision >= 0.1f)
			mNewVelocity = Vector2f::ZERO();
	}

	void Agent::AvoidOtherAgentsSamplingMethodSlow(double dT)
	{
		if(mColliding)
		{
			mNewVelocity = -mNewVelocity;
			mPosition += mNewVelocity * dT;
		}

		{
			float minPenalty = Math::Infinity();
			float minTimePenatly, minDistancePenalty, minTimeToCollision;

			Vector2f candidateVelocity;

			float angle = 0.0f;
			float angleChange = Math::TwoPi() / mNoVelSamples;

			for(int i = 0; i < 400; ++i)
			{
				if(i == 0) 
				{
					candidateVelocity = mSteeringVelocity;
				}
				else
				{
					do 
					{
						candidateVelocity = Vector2f(2.0f * rand() - RAND_MAX, 2.0f * rand() - RAND_MAX);

					} while(candidateVelocity.SquaredLength() > Math::Sqr((float)RAND_MAX));

					candidateVelocity *= (mPersonality.DesiredVelocity / RAND_MAX);			
				}		

				//distance between candidate velocity and preferred
				float dV;

				if(mColliding)
					dV = 0;
				else
					dV = (candidateVelocity - mSteeringVelocity).Length();

				// find the smallest time to collision
				//time to collision
				float cT = Math::Infinity();

				for(int j = 0; j < mThisFramesObstacles.size(); ++j)
				{
					if(mThisFramesObstacles[j] == this)
						continue;

					Entity* otherAgent = mThisFramesObstacles[j];
					Vector2f abVector;
					float agentCollisionTime;

					if(otherAgent->Type() == Entity::AGENT)
					{
						Agent* agent = (Agent*)otherAgent;

						abVector = 2 * candidateVelocity - mVelocity - agent->Velocity();

						float ThreatModifier = (agent->mPersonality.Threat - mPersonality.ThreatTolerance);
						Math::Clamp(ThreatModifier, 0.0f, 1.0f);

						float time = 
							Math::RayDiskTimeToCollision(
							mPosition, 
							abVector, 
							agent->Position(), 
							mRadius + agent->Radius() + (agent->Radius() * ThreatModifier), 
							mColliding);

						if(mColliding)
						{
							agentCollisionTime = -ceil(time / dT);
							agentCollisionTime -= candidateVelocity.SquaredLength() / Math::Sqr(mPersonality.DesiredVelocity);
						}
						else
						{
							agentCollisionTime = time;
						}
					}
					else if(otherAgent->Type() == Entity::OBSTACLE)
					{
						LineObstacle* obs = (LineObstacle*)otherAgent;

						float time1, time2, timeA, timeB;

						time1 = Math::RayDiskTimeToCollision(mPosition, candidateVelocity, obs->StartPoint(), mRadius, mColliding);
						time2 = Math::RayDiskTimeToCollision(mPosition, candidateVelocity, obs->EndPoint(), mRadius, mColliding);
						timeA = Math::RayLineTimeToCollision(mPosition, candidateVelocity, obs->StartPoint() + mRadius * obs->Normal(), obs->EndPoint() + mRadius * obs->Normal(), mColliding);
						timeB =Math::RayLineTimeToCollision(mPosition, candidateVelocity, obs->StartPoint() - mRadius * obs->Normal(), obs->EndPoint() - mRadius * obs->Normal(), mColliding);

						if(mColliding)
						{
							float time = std::max(std::max(std::max(time1, time2), timeA), timeB);
							agentCollisionTime = -ceil(time / dT);
							agentCollisionTime -= candidateVelocity.SquaredLength() / Math::Sqr(mPersonality.DesiredVelocity);
						}
						else
						{
							float time = std::min(std::min(std::min(time1, time2), timeA), timeB);

							if(time < dT || Math::Sqr(time) < candidateVelocity.SquaredLength() / Math::Sqr(mPersonality.DesiredVelocity))
							{
								agentCollisionTime = time;
							}
							else
							{
								agentCollisionTime = Math::Infinity();
							}
						}
					}

					if(agentCollisionTime < cT)
					{
						cT = agentCollisionTime;

						if((mPersonality.Shyness / cT) + (dV * mPersonality.Stubbornness) >= minPenalty)
							break;
					}
				}

				float timePen = (mPersonality.Shyness / cT);
				float distancePen = (dV * mPersonality.Stubbornness);
				float penalty = timePen + distancePen;

				if(penalty < minPenalty)
				{
					minTimeToCollision = cT;
					minDistancePenalty = distancePen;
					minTimePenatly = timePen;
					minPenalty = penalty;
					mNewVelocity = candidateVelocity;
				}
			}

			if(minTimeToCollision <= 0.5f && minTimeToCollision >= 0.1f)
				mNewVelocity = Vector2f::ZERO();
		}
	}

	void Agent::FinalizeVelocity()
	{
		int collisions = 1;

		for(unsigned int i = 0; i < mThisFramesObstacles.size(); ++i)
		{
			if(Math::DoesLineIntersectCircle(
				mPosition,
				mNewVelocity,
				mThisFramesObstacles[i]->Position(),
				mThisFramesObstacles[i]->Radius() + mRadius))
				// 		float dot = (mVelocity).dotProduct(mThisFramesObstacles[i]->Position() - mPosition);
				// 
				// 		if(dot >= -Math::Pi() && dot <= Math::Pi())
			{
				collisions++;
			}
		}

		mNewVelocity /= (float)collisions;
	}

	void Agent::AvoidOtherAgentsGeometricMethod( double dT )
	{
		mVo.Clear();
		mVo.Initialise(mPosition, mVelocity, mRadius);

		for(unsigned int i = 0; i < mThisFramesObstacles.size(); ++i)
		{
			Entity* obs = mThisFramesObstacles[i];

			mVo.AddObstacle(obs->Position(), obs->Velocity(), obs->Radius());
		}

		if(!mVo.DoesAgentVelocityLieInsideObstacle(mSteeringVelocity)) 
		{
			mNewVelocity = mSteeringVelocity;
		}
		else
		{
			std::vector<Vector2f> allPossibilities = mVo.ComputeAllPoints();

			Vector2f bestVelocity = mSteeringVelocity;
			float bestMagicNumber = Math::Infinity();

			for(unsigned int i = 0; i < allPossibilities.size(); ++i)
			{
				float newMagicNumber = (allPossibilities[i].Length() + mSteeringVelocity.Length()) * 0.5f;

				if(newMagicNumber < bestMagicNumber)
				{
					newMagicNumber = bestMagicNumber;
					bestVelocity = allPossibilities[i];
				}
			}

			bestVelocity.Normalise();
			bestVelocity *= mPersonality.DesiredVelocity;
			mNewVelocity = bestVelocity;
		}
	}
}
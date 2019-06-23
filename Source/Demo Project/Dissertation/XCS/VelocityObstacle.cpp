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

#include "VelocityObstacle.h"
#include "Math.h"

namespace XCS
{
	VelocityObstacle::VelocityObstacle()
	{

	}

	VelocityObstacle::~VelocityObstacle(void)
	{
	}

	void VelocityObstacle::Clear()
	{
		mCones.clear();
	}

	void VelocityObstacle::AddObstacle( const Vector2f &obstaclePos, const Vector2f &obstacleVelocity, const float &obstacletRadius )
	{
		mCones.push_back(VOCone(mAgentPos, mAgentVelocity, mAgentRadius, obstaclePos, obstacleVelocity, obstacletRadius));
	}

	std::vector<Vector2f> VelocityObstacle::ComputeAllPoints()
	{
		std::vector<Vector2f> retVec;

		for(unsigned int i = 0; i < mCones.size(); ++i) // for each cone
		{
			std::vector<Vector2f> cone1Verts = mCones[i].Vertices(); // get its vertices

			for(unsigned int j = 0; j < cone1Verts.size() - 1; ++j) // the for each line
			{
				retVec.push_back(cone1Verts[j]); // add the start point to the return list

				for(unsigned int k = i + 1; k < mCones.size(); ++k) // then for each remaining cone
				{
					std::vector<Vector2f> cone2Verts = mCones[k].Vertices(); // get its vertices

					for(unsigned int l = 0; l < cone1Verts.size() - 1; ++l) // for each of its lines
					{
						Vector2f intersectPoint;

						if(Math::LineLineIntersection(
							cone1Verts[j],
							cone1Verts[j + 1],
							cone2Verts[l],
							cone2Verts[l + 1],
							intersectPoint))	//if the two intersect
						{
							retVec.push_back(intersectPoint); // add that point as well
						}
					}
				}
			}
		}

		return retVec;
	}

	void VelocityObstacle::Initialise( const Vector2f &agentPos, const Vector2f &agentVelocity, const float &agentRadius )
	{
		mAgentPos = agentPos;
		mAgentRadius = agentRadius;
		mAgentVelocity = agentVelocity;
	}

	bool VelocityObstacle::DoesAgentVelocityLieInsideObstacle(Vector2f vel)
	{
		for(unsigned int i = 0; i < mCones.size(); ++i)
		{
			std::vector<Vector2f> verts = mCones[i].Vertices();

			for(unsigned int j = 0; j < verts.size() - 1; ++j)
			{
				Vector2f intPoint;

				if(Math::LineLineIntersection(verts[j], verts[j + 1], Vector2f(0.0f), (vel), intPoint))
					return true;
			}
		}

		return false;
	}
}
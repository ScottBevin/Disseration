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

#include "VOCone.h"

namespace XCS
{
	VOCone::VOCone(
		Vector2f agentPos,
		Vector2f agentVelocity,
		float agentRadius,
		Vector2f obstaclePos,
		Vector2f obstacleVelocity,
		float obstacleRadius
		)
	{
		Vector2f offset = obstaclePos - agentPos; // get obstacles position in local space

		Vector2f offsetPerp = offset.Perpendicular();	// Get the perp vector
		offsetPerp.Normalise();	// make sure its normalised

		Vector2f directionToPoint1 = offset + (offsetPerp * (obstacleRadius + agentRadius));
		mVertices.push_back(directionToPoint1);
		mVertices.push_back(directionToPoint1.NormalisedCopy() * (obstacleVelocity.Length()));

		//mVertices.push_back(offset.normalisedCopy() * (agentRadius + obstacleRadius)); // root point

		Vector2f directionToPoint2 = offset + (offsetPerp * -(obstacleRadius + agentRadius));
		mVertices.push_back(directionToPoint2.NormalisedCopy() * (obstacleVelocity.Length()));
		mVertices.push_back(directionToPoint2);


		Vector2f averageVelocity = (obstacleVelocity + agentVelocity) / 2.0f;

		mVertices[0] += averageVelocity;
		mVertices[1] += averageVelocity;
		mVertices[2] += averageVelocity;
		mVertices[3] += averageVelocity;
	}

	VOCone::~VOCone()
	{
	}
}

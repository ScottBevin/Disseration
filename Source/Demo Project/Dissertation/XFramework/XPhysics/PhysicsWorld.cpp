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

#include "PhysicsWorld.h"

namespace XF
{
	PhysicsWorld::PhysicsWorld(float x, float y, float w, float h)
	{
		PhysicsWorld(x, y, w, h, 10, 100);
	}

	PhysicsWorld::PhysicsWorld(float x, float y, float w, float h, unsigned int targetQuadDepth, unsigned int targetQuadObjects)
	{
		mQuadTree = new QuadTree(x, y, w, h, targetQuadDepth, targetQuadObjects);
	}

	PhysicsWorld::~PhysicsWorld()
	{

	}

	void PhysicsWorld::Update(float dT)
	{

	}

	Body* PhysicsWorld::CreateBody(XF::Vector3 pos, float radius)
	{
		Body* newBody = new Body(pos, radius);

		mAllBodies.push_back(newBody);
		mQuadTree->AddBody(newBody);

		return newBody;
	}

	void PhysicsWorld::RemoveBody(Body* body)
	{
		mAllBodies.remove(body);
		mQuadTree->RemoveBody(body);
	}

	void PhysicsWorld::UpdateAllBodies(float dT)
	{
		for(std::list<Body*>::iterator it = mAllBodies.begin();
			it != mAllBodies.end(); ++it)
		{
			Body* body = *it;

			body->Position(body->Position() + (body->Velocity() * dT));

			if(body->HasMoved())
			{
				mQuadTree->RemoveBody(body);
				mQuadTree->AddBody(body);
			}
		}
	}
}
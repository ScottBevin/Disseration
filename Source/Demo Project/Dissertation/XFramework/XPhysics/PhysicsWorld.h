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

#include <list>

#include "Body.h"
#include "XQuadTree\QuadTree.h"
#include "../XFrameworkTypedefsAndDefines.h"

namespace XF
{
	class PhysicsWorld
	{
	private:
		std::list<Body*> mAllBodies;
		QuadTree* mQuadTree;

		void UpdateAllBodies(float dT);

	public:
		PhysicsWorld(float x, float y, float w, float h);
		PhysicsWorld(float x, float y, float w, float h, unsigned int targetQuadDepth, unsigned int targetQuadObjects);
		~PhysicsWorld();

		void Update(float dT);

		Body* CreateBody(XF::Vector3 pos, float radius);
		void RemoveBody(Body* body);
	};
}

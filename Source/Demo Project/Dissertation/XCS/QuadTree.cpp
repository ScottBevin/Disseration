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

#include "QuadTree.h"

#include "Math.h"
#include "Agent.h"
#include "LineObstacle.h"

namespace XCS
{
	QuadTree::QuadTree() 
		: mTree(Vector2f(-200.0f), Vector2f(200.0f), 10, 1, 3)
	{

	}

	QuadTree::~QuadTree()
	{
	} 

	void QuadTree::AddEntity( Entity* agent )
	{
		mEntities.push_back(agent);
	}

	void QuadTree::RemoveEntity( Entity* agent )
	{
		//std::vector<Entity*>::iterator it = std::find(mEntities.begin(), mEntities.end(), agent);
		std::swap(agent, mEntities.back());
		mEntities.pop_back();
	}

	void QuadTree::SetUp()
	{
		mTree.Clear();

		unsigned int size = mEntities.size();
		for(unsigned int i = 0; i < size; ++i)
		{
			mTree.InsertEntity(mEntities[i]);
		}

		for(std::map<QuadTreeNode*, Entity*>::iterator it = mTempEntities.begin();
			it != mTempEntities.end(); ++it)
		{
			delete it->second;
		}

		mTempEntities.clear();

		NodeIterator it(&mTree);

		while(!it.End())
		{
			QuadTreeNode* node = it.Next();

			if(node->IsOvercrowded())
			{
				Entity* entity = new Entity(Entity::AGENT, node->Width() * 0.5f);
				entity->Position(Vector2f(node->Center()));

				mTempEntities.insert(std::make_pair(node, entity));
			}
		}
	}

	void QuadTree::TagAllCollidingAgents()
	{
		mCollisions = 0;

		for(unsigned int i = 0; i < mEntities.size(); ++i)
			mEntities[i]->Colliding(false);

		for(unsigned int i = 0; i < mEntities.size(); ++i)
		{
			if(mEntities[i]->Colliding() ||
				mEntities[i]->Type() != Entity::AGENT) continue;

			Agent* agent = (Agent*)mEntities[i];

			CircularSpatialNodeIterator it2(&mTree, agent->Position(), agent->Radius());	

			while(!it2.End())
			{
				QuadTreeNode* node = it2.Next();

				std::vector<Entity*> nodesEnts = node->Entities();

				for(unsigned int j = 0; j < nodesEnts.size(); ++j)
				{
					Entity* thisent = nodesEnts[j];

					if(thisent->Colliding() || thisent == mEntities[i]) continue;

					if(thisent->Type() == Entity::AGENT)
					{
						if(Math::DoesCircleIntersectCircle(thisent->Position(), thisent->Radius(), agent->Position(), agent->Radius()))
						{
							thisent->Colliding(true);
							agent->Colliding(true);

							mCollisions += 2;
						}
					}
					else
					{
						LineObstacle* obs = (LineObstacle*)thisent;
						if(Math::DoesLineSegmentIntersectCircle(obs->StartPoint(), obs->EndPoint(), agent->Position(), agent->Radius()))
						{
							thisent->Colliding(true);
							agent->Colliding(true);
							mCollisions += 2;
						}
					}
				}
			}
		}
	}

	std::vector<Entity*> QuadTree::EntitiesInArea( Vector2f pos, float radius )
	{
		std::vector<Entity*> retVec;

		CircularSpatialNodeIterator it2(&mTree, pos, radius);	

		while(!it2.End())
		{
			QuadTreeNode* node = it2.Next();

			std::vector<Entity*> nodesEnts = node->Entities();

			for(unsigned int i = 0; i < nodesEnts.size(); ++i)
			{
				Entity* ent = nodesEnts[i];

				if(Math::DoesCircleIntersectCircle(ent->Position(), ent->Radius(), pos, radius))
					retVec.push_back(ent);
			}
		}

		return retVec;
	}

	std::vector<Entity*> QuadTree::EntitiesInArea( Vector2f min, Vector2f max )
	{
		std::vector<Entity*> retVec;

		RectangularSpatialNodeIterator it2(&mTree, min, max);	

		while(!it2.End())
		{
			QuadTreeNode* node = it2.Next();

			std::vector<Entity*> nodesEnts = node->Entities();

			for(unsigned int i = 0; i < nodesEnts.size(); ++i)
			{
				Entity* ent = nodesEnts[i];

				if(Math::DoesCircleIntersectRectangle(ent->Position(), ent->Radius(), min, max))
					retVec.push_back(ent);
			}
		}

		return retVec;
	}

	std::vector<Entity*> QuadTree::EntitiesNearEntity( Entity* agent, float radius )
	{
		return EntitiesInArea(agent->Position(), radius);
	}
}
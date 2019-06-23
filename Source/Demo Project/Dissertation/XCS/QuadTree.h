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

#ifndef QUADTREE_H
#define QUADTREE_H

#include <stack>
#include <map>

#include "QuadTreeNode.h"

namespace XCS
{
	class QuadTree
	{
	private:
		std::vector<Entity*> mEntities;
		std::map<QuadTreeNode*, Entity*> mTempEntities;
		int mCollisions;

		QuadTreeNode mTree;

	public:
		QuadTree();
		~QuadTree();

		void AddEntity(Entity* agent);
		void RemoveEntity(Entity* agent);

		void SetUp();

		void TagAllCollidingAgents();

		std::vector<Entity*> EntitiesInArea(Vector2f pos, float radius);
		std::vector<Entity*> EntitiesInArea( Vector2f min, Vector2f max );
		std::vector<Entity*> EntitiesNearEntity(Entity* agent, float radius);

		XCS::QuadTreeNode* RootNode() { return &mTree; }

		int Collisions() const { return mCollisions; }

		class NodeIterator
		{
		private:
			std::stack<QuadTreeNode*> mNodes;

		public:
			NodeIterator(QuadTreeNode* startNode)
			{
				mNodes.push(startNode);
			}

			QuadTreeNode* Next()
			{
				QuadTreeNode* top = mNodes.top();
				mNodes.pop();

				if(top->HasChildren())
				{
					std::vector<QuadTreeNode*> nodes = top->ChildNodes();

					mNodes.push(nodes[3]);
					mNodes.push(nodes[2]);
					mNodes.push(nodes[1]);
					mNodes.push(nodes[0]);
				}

				return top;
			}

			bool End()
			{
				return mNodes.empty();
			}
		};

		class CircularSpatialNodeIterator
		{
		private:
			std::stack<QuadTreeNode*> mNodes;
			Vector2f mPos;
			float mRadius;

		public:
			CircularSpatialNodeIterator(QuadTreeNode* startNode, Vector2f pos, float radius)
			{
				mPos = pos;
				mRadius = radius;

				if(startNode->NodeIntersectsCircle(pos, radius))
					mNodes.push(startNode);
			}

			QuadTreeNode* Next()
			{
				QuadTreeNode* top = mNodes.top();
				mNodes.pop();

				if(top->HasChildren())
				{
					std::vector<QuadTreeNode*> nodes = top->ChildNodes();

					if(nodes[3]->NodeIntersectsCircle(mPos, mRadius)) mNodes.push(nodes[3]);
					if(nodes[2]->NodeIntersectsCircle(mPos, mRadius)) mNodes.push(nodes[2]);
					if(nodes[1]->NodeIntersectsCircle(mPos, mRadius)) mNodes.push(nodes[1]);
					if(nodes[0]->NodeIntersectsCircle(mPos, mRadius)) mNodes.push(nodes[0]);
				}

				return top;
			}

			bool End()
			{
				return mNodes.empty();
			}
		};

		class RectangularSpatialNodeIterator
		{
		private:
			std::stack<QuadTreeNode*> mNodes;
			Vector2f mMin, mMax;

		public:
			RectangularSpatialNodeIterator(QuadTreeNode* startNode, Vector2f min, Vector2f max)
			{
				mMin = min;
				mMax = max;

				if(startNode->NodeIntersectsRectangle(mMin, mMax))
					mNodes.push(startNode);
			}

			QuadTreeNode* Next()
			{
				QuadTreeNode* top = mNodes.top();
				mNodes.pop();

				if(top->HasChildren())
				{
					std::vector<QuadTreeNode*> nodes = top->ChildNodes();

					if(nodes[3]->NodeIntersectsRectangle(mMin, mMax)) mNodes.push(nodes[3]);
					if(nodes[2]->NodeIntersectsRectangle(mMin, mMax)) mNodes.push(nodes[2]);
					if(nodes[1]->NodeIntersectsRectangle(mMin, mMax)) mNodes.push(nodes[1]);
					if(nodes[0]->NodeIntersectsRectangle(mMin, mMax)) mNodes.push(nodes[0]);
				}

				return top;
			}

			bool End()
			{
				return mNodes.empty();
			}
		};
	};
}

#endif
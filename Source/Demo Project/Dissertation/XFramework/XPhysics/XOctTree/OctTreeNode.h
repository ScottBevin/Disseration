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
#include <vector>
#include <algorithm>

#include "../Body.h"
#include "../AABB.h"

namespace XF
{
	class OctTreeNode
	{
	private:
		unsigned int mTargetObjects;
		unsigned int mMaxDepth;

		std::list<Body*> mObjects;

		OctTreeNode *mParentNode;
		std::vector<OctTreeNode*> mChildNodes;

		AABB mAABB;
		unsigned int mDepth;

			/**
			attempts to place the object into the proper node from this point downwards, 
			this function is not capable of propagating upwards through the tree.
			*/
		bool PlaceBodyIntoNode(Body* body);

		/**
			spawns 8 children node
			*/
		void SplitNode();

		/**
			deletes all the children of this node
			*/
		void DeleteChildNodes();

	public:
		OctTreeNode(OctTreeNode &copyNode);
		OctTreeNode(OctTreeNode *parent, int depth, float x, float y, float z, float w, float h, float d);
		~OctTreeNode();


		/**
			this function will add an object into the tree, subdividing if necessary and possible
			*/
		bool AddBody(Body* body);

		/**
			remove object will remove an object and unnecessary nodes
			*/
		bool RemoveBody(Body* body);

		/**
			counts the number of objects held by the tree from this node downwards
		*/
		int GetNoOfObjects();

		int GetNoOfObjectsThisNodeHolds(){ return (int)mObjects.size(); }

		bool IsRootNode(){ return (mParentNode == NULL); }

		bool HasChildNodes() { return mChildNodes[0] != NULL; }
		std::vector<OctTreeNode*> GetChildNodes(){return std::vector<OctTreeNode*>(mChildNodes);}

		int TargetObjects(){ return mTargetObjects; }
		void TargetObjects(int targetObjects){ mTargetObjects = targetObjects; }

		int MaxDepth(){ return mMaxDepth; }
		void MaxDepth(int maxDepth){ mMaxDepth = maxDepth; }
	};
}
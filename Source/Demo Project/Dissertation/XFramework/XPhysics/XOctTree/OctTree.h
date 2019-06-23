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

#include "OctTreeNode.h"

namespace XF
{
	class OctTree
	{
	private:
		OctTreeNode *mRootNode;

	public:
		OctTree(OctTree &copyTree);
		OctTree(float x, float y, float z, float width, float height, float depth, int targetObjects, int maxHeirachyDepth);
		~OctTree();

		/**
		this function will add an object into the tree, subdividing if necessary and possible
		*/
		bool AddBody(Body* body){ return mRootNode->AddBody(body); }

		/**
		remove object will remove an object and unnecessary nodes
		*/
		bool RemoveBody(Body* body){ return mRootNode->RemoveBody(body);}

		OctTreeNode* Root(){ return mRootNode; }

		/**
		counts the number of objects held by the tree from this node downwards, this one uses the iterator to count, 
		enabling a test for whether it works yarrrrrrrrrrrrrrrrr
		*/
		int GetNoOfObjects();
		
		int TargetObjects(){ return mRootNode->TargetObjects(); }
		int MaxDepth(){ return mRootNode->MaxDepth(); }
	};
}
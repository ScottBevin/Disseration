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

#include <stack>
#include <list>
#include <vector>
#include <algorithm>

#include "OctTreeNode.h"

namespace XF
{
	class OctTreeIterator
	{
	private:
		std::stack<OctTreeNode*> mCurrentNodes;
		std::list<OctTreeNode*> mVisitedNodes;

	public:
		OctTreeIterator(OctTreeNode *startNode)
		{
			mCurrentNodes.push(startNode);
		}
		~OctTreeIterator(){}

		bool End(){return (mCurrentNodes.size() <= 0);}
		OctTreeNode* CurrentNode(){return mCurrentNodes.top();}

		void Increment()
		{
			if(CurrentNode()->HasChildNodes() 
				&& std::find(mVisitedNodes.begin(), mVisitedNodes.end(), CurrentNode()) == mVisitedNodes.end())
			{
				std::vector<OctTreeNode*> children = CurrentNode()->GetChildNodes();

				for(int i = 0; i < 8; ++i)
				{
					mCurrentNodes.push(children[i]);
				}
			}
			else
			{
				mVisitedNodes.push_back(CurrentNode());
				mCurrentNodes.pop();
			}
		}
	};
}
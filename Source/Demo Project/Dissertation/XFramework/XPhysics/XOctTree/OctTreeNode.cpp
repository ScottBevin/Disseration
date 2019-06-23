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

#include "OctTreeNode.h"

namespace XF
{
	OctTreeNode::OctTreeNode(
		OctTreeNode *parent, int depth, float x, float y, 
		float z, float w, float h, float d)
	{
		mChildNodes.resize(8);
		for(int i = 0; i < 8; ++i)
			mChildNodes[i] = NULL; 

		mAABB = AABB(x, y, z, x + w, y + h, z + d);
		mDepth = depth;

		mParentNode = parent;

		if(parent != NULL)
		{
			mTargetObjects = parent->mTargetObjects;
			mMaxDepth = parent->mMaxDepth;
		}
	}

	OctTreeNode::OctTreeNode(OctTreeNode &copyNode)
	{
		for(int i = 0; i < 8; ++i)
			if(copyNode.mChildNodes[i] != NULL)
				mChildNodes[i] = new OctTreeNode(*copyNode.mChildNodes[i]);

		mAABB = AABB(copyNode.mAABB);
		mDepth = copyNode.mDepth;

		mParentNode = copyNode.mParentNode;

		mObjects = std::list<Body*>(copyNode.mObjects);
	}

	OctTreeNode::~OctTreeNode()
	{
		DeleteChildNodes();
	}

	void OctTreeNode::DeleteChildNodes()
	{
		for(int i = 0; i < 8; ++i)
		{
			delete mChildNodes[i];
			mChildNodes[i] = NULL;
		}
	}

	bool OctTreeNode::AddBody(Body* body)
	{
		bool placed = PlaceBodyIntoNode(body);

		if(!HasChildNodes() &&
			(int)mObjects.size() >= mTargetObjects &&
			mDepth <= mMaxDepth)
		{
			SplitNode();
		}

		return placed;
	}

	bool OctTreeNode::PlaceBodyIntoNode(Body* body)
	{
		if(HasChildNodes())
		{
			for(int i = 0; i < 8; ++i)
			{
				if(mChildNodes[i]->mAABB.Contains(body->X(), body->Y(), body->Z(), body->Radius()))
				{
					return mChildNodes[i]->AddBody(body);
				}
			}
		}

		if(mAABB.Contains(body->X(), body->Y(), body->Z(), body->Radius()))
		{		
			mObjects.push_back(body);
			return true;
		}

		return false;
	}

	void OctTreeNode::SplitNode()
	{
		std::list<Body*> tempBodyList(mObjects);
		mObjects.clear();

		//nwf
		mChildNodes[0] = new OctTreeNode(this, mDepth + 1, 
			mAABB.MinX(), 
			mAABB.MinY() + mAABB.Height() * 0.5f, 
			mAABB.MinZ(), 
			mAABB.Width() * 0.5f, mAABB.Height() * 0.5f, mAABB.Depth() * 0.5f);

		//nef
		mChildNodes[1] = new OctTreeNode(this, mDepth + 1, 
			mAABB.MinX() + mAABB.Width() * 0.5f, 
			mAABB.MinY() + mAABB.Height() * 0.5f, 
			mAABB.MinZ(), 
			mAABB.Width() * 0.5f, mAABB.Height() * 0.5f, mAABB.Depth() * 0.5f);

		//swf
		mChildNodes[2] = new OctTreeNode(this, mDepth + 1, 
			mAABB.MinX(), 
			mAABB.MinY(), 
			mAABB.MinZ(), 
			mAABB.Width() * 0.5f, mAABB.Height() * 0.5f, mAABB.Depth() * 0.5f);

		//sef
		mChildNodes[3] = new OctTreeNode(this, mDepth + 1, 
			mAABB.MinX() + mAABB.Width() * 0.5f, 
			mAABB.MinY(), 
			mAABB.MinZ(), 
			mAABB.Width() * 0.5f, mAABB.Height() * 0.5f, mAABB.Depth() * 0.5f);

		//nwb
		mChildNodes[4] = new OctTreeNode(this, mDepth + 1, 
			mAABB.MinX(), 
			mAABB.MinY() + mAABB.Height() * 0.5f, 
			mAABB.MinZ() + mAABB.Depth() * 0.5f, 
			mAABB.Width() * 0.5f, mAABB.Height() * 0.5f, mAABB.Depth() * 0.5f);

		//neb
		mChildNodes[5] = new OctTreeNode(this, mDepth + 1, 
			mAABB.MinX() + mAABB.Width() * 0.5f, 
			mAABB.MinY() + mAABB.Height() * 0.5f, 
			mAABB.MinZ() + mAABB.Depth() * 0.5f, 
			mAABB.Width() * 0.5f, mAABB.Height() * 0.5f, mAABB.Depth() * 0.5f);

		//swb
		mChildNodes[6] = new OctTreeNode(this, mDepth + 1, 
			mAABB.MinX(), 
			mAABB.MinY(), 
			mAABB.MinZ() + mAABB.Depth() * 0.5f, 
			mAABB.Width() * 0.5f, mAABB.Height() * 0.5f, mAABB.Depth() * 0.5f);

		//seb
		mChildNodes[7] = new OctTreeNode(this, mDepth + 1, 
			mAABB.MinX() + mAABB.Width() * 0.5f, 
			mAABB.MinY(), 
			mAABB.MinZ() + mAABB.Depth() * 0.5f, 
			mAABB.Width() * 0.5f, mAABB.Height() * 0.5f, mAABB.Depth() * 0.5f);

		for(std::list<Body*>::iterator it = tempBodyList.begin(); 
			it != tempBodyList.end(); ++it)
		{
			PlaceBodyIntoNode(*it);
		}
	}

	bool OctTreeNode::RemoveBody(Body* body)
	{
		if(mAABB.Contains(body->X(), body->Y(), body->Z(), body->Radius()))
		{
			std::list<Body*>::iterator it = std::find(mObjects.begin(), mObjects.end(), body);

			if(it != mObjects.end())
			{
				mObjects.remove(body);
				return true;
			}
		}

		if(HasChildNodes())
		{
			for(int i = 0; i < 8; ++i)
			{
				if(mChildNodes[i]->RemoveBody(body))
					return true;
			}
		}

		return false;
	}

	int OctTreeNode::GetNoOfObjects()
	{
		int count = 0;

		count += (int)mObjects.size();

		if(HasChildNodes())
		{
			for(int i = 0; i < 8; ++i)
			{
				count += mChildNodes[i]->GetNoOfObjects();
			}
		}

		return count;
	}
}
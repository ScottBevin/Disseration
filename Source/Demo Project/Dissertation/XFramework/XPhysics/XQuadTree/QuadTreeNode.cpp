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

#include "QuadTreeNode.h"

namespace XF
{
	bool QuadTreeNode::smTreeStructureChangedThisFrame = false;

	QuadTreeNode::QuadTreeNode(QuadTreeNode &copyNode)
	{
		for(int i = 0; i < 4; ++i)
			if(copyNode.mChildNodes[i] != NULL)
				mChildNodes[i] = new QuadTreeNode(*copyNode.mChildNodes[i]);

		mRect = RectangleF(copyNode.mRect);
		mDepth = copyNode.mDepth;

		mParentNode = copyNode.mParentNode;
		mObjects = std::list<Body*>(copyNode.mObjects);
	}

	QuadTreeNode::QuadTreeNode(QuadTreeNode *parent, int depth, float x, float y, float w, float h)
	{
		mChildNodes.resize(4);

		for(int i = 0; i < 4; ++i)
			mChildNodes[i] = NULL;

		mRect = RectangleF(x, y, w, h);
		mDepth = depth;

		mParentNode = parent;

		if(parent != NULL)
		{
			mTargetObjects = parent->mTargetObjects;
			mMaxDepth = parent->mMaxDepth;
		}
	}

	QuadTreeNode::~QuadTreeNode()
	{
		DeleteChildNodes();
	}

	bool QuadTreeNode::PlaceBodyIntoNode( Body* body )
	{
		if(HasChildNodes())
		{
			for(int i = 0; i < 4; ++i)
			{
				if(mChildNodes[i]->mRect.Contains(body->X(), body->Z(), body->Radius()))
				{
					return mChildNodes[i]->AddBody(body);
				}
			}
		}

		if(mRect.Contains(body->X(), body->Z(), body->Radius()))
		{
			mObjects.push_back(body);
			return true;
		}

		return false;
	}

	void QuadTreeNode::DeleteChildNodes()
	{
		smTreeStructureChangedThisFrame = true;

		for(int i = 0; i < 4; ++i)
		{
			delete mChildNodes[i];
			mChildNodes[i] = NULL;
		}
	}

	bool QuadTreeNode::AddBody( Body* body )
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

	bool QuadTreeNode::RemoveBody( Body* body )
	{
		if(mRect.Contains(body->X(), body->Y(), body->Radius()))
		{
			std::list<Body*>::iterator it = std::find(mObjects.begin(), mObjects.end(), body);

			if(it != mObjects.end())
			{
				mObjects.erase(it);
				
				if(mObjects.size() <= 0 &&
					GetNoOfObjects() <= 0)
				{
					DeleteChildNodes();
				}

				return true;
			}
		}

		if(HasChildNodes())
		{
			for(int i = 0; i < 4; ++i)
			{
				if(mChildNodes[i]->RemoveBody(body))
				{
					if(GetNoOfObjects() - mObjects.size() <= 0)
					{
						DeleteChildNodes();
					}

					return true;
				}
			}
		}

		return false;
	}

	int QuadTreeNode::GetNoOfObjects()
	{
		int count = 0;

		count += (int)mObjects.size();

		if(HasChildNodes())
		{
			for(int i = 0; i < 4; ++i)
			{
				count += mChildNodes[i]->GetNoOfObjects();
			}
		}

		return count;
	}

	void QuadTreeNode::SplitNode()
	{
		smTreeStructureChangedThisFrame = true;

		std::list<Body*> tempBodyList(mObjects);
		mObjects.clear();

		// bottom left
		mChildNodes[0] = new QuadTreeNode(
			this,
			mDepth + 1,
			mRect.X(),
			mRect.Y(),
			mRect.Width() * 0.5f,
			mRect.Height() * 0.5f);

		// bottom right
		mChildNodes[1] = new QuadTreeNode(
			this,
			mDepth + 1,
			mRect.X() + mRect.Width() * 0.5f,
			mRect.Y(),
			mRect.Width() * 0.5f,
			mRect.Height() * 0.5f);

		// top left
		mChildNodes[2] = new QuadTreeNode(
			this,
			mDepth + 1,
			mRect.X(),
			mRect.Y() + mRect.Height() * 0.5f,
			mRect.Width() * 0.5f,
			mRect.Height() * 0.5f);

		// top right
		mChildNodes[3] = new QuadTreeNode(
			this,
			mDepth + 1,
			mRect.X() + mRect.Width() * 0.5f,
			mRect.Y() + mRect.Height() * 0.5f,
			mRect.Width() * 0.5f,
			mRect.Height() * 0.5f);

		for(std::list<Body*>::iterator it = tempBodyList.begin(); 
			it != tempBodyList.end(); ++it)
		{
			PlaceBodyIntoNode(*it);
		}
	}

	void QuadTreeNode::Render( Ogre::ManualObject *manObj )
	{
		RenderNode(manObj);

		if(HasChildNodes())
		{
			for(int i = 0; i < 4; ++i)
			{
				mChildNodes[i]->Render(manObj);
			}
		}
	}

	void QuadTreeNode::RenderNode( Ogre::ManualObject *manObj )
	{
		manObj->position(mRect.X(), 0, mRect.Y());
		manObj->position(mRect.X() + mRect.Width(), 0, mRect.Y());

		manObj->position(mRect.X() + mRect.Width(), 0, mRect.Y());
		manObj->position(mRect.X() + mRect.Width(), 0, mRect.Y() + mRect.Height());

		manObj->position(mRect.X() + mRect.Width(), 0, mRect.Y() + mRect.Height());
		manObj->position(mRect.X(), 0, mRect.Y() + mRect.Height());

		manObj->position(mRect.X(), 0, mRect.Y() + mRect.Height());
		manObj->position(mRect.X(), 0, mRect.Y());
	}

	void QuadTreeNode::CollideObjectWithNode(std::list<Body*>::iterator it)
	{
		Body* body = *it;

		for(std::list<Body*>::iterator it2 = it; 
			it2 != mObjects.end(); ++it2)
		{
			Body* body2 = *it2;

			if(body != body2 &&
				(body->Position().squaredDistance(body2->Position()) <= 
				(body->Radius() + body2->Radius()) * (body->Radius() + body2->Radius())))
			{
				XF::Delegate2<bool, Body*, Body*> collisionFunc = body->OnCollision();

				if(!collisionFunc || collisionFunc(body, body2))
				{
					// TODO: Insert penetration solution here
				}
			}
		}

		if(HasChildNodes())
		{
			for(int i = 0; i < 4; ++i)
			{
				mChildNodes[i]->CollideObjectWithNode(it);
			}
		}
	}

	void QuadTreeNode::CollideAllBodies()
	{
		for(std::list<Body*>::iterator it = mObjects.begin();
			it != mObjects.end(); ++it)
		{
			CollideObjectWithNode(it);
		}
	}
}
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

#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include <vector>
#include "Entity.h"

namespace XCS
{
	class QuadTreeNode
	{
	private:
		std::vector<Entity*> mMyEntities;
		std::vector<QuadTreeNode*> mChildNodes;

		int mMaxDepth, mDepth, mTargetEntities;
		bool mChildrenInUse;
		Vector2f mMin, mMax;

		bool PlaceInThisNode(Entity* entity)
		{
			if(NodeContainsCircle(entity->Position(), entity->Radius()))
			{
				mMyEntities.push_back(entity);

				SubdivideCell();

				return true;
			}
			else
				return false;
		}

		void SubdivideCell()
		{
			if(!HasChildren() && !IsAtMaxDepth()&& IsOvercrowded())
			{
				if(mChildNodes[0] == NULL)
				{
					float width = mMax.mX - mMin.mX;
					float widthDiv2 = width  * 0.5f;
					float height = mMax.mY - mMin.mY;
					float heightDiv2 = height * 0.5f;

					mChildNodes[0] = new QuadTreeNode(Vector2f(mMin.mX,					mMin.mY),				Vector2f(mMin.mX + widthDiv2,	mMin.mY + heightDiv2),	mMaxDepth, mDepth + 1, mTargetEntities);
					mChildNodes[1] = new QuadTreeNode(Vector2f(mMin.mX + widthDiv2,		mMin.mY),				Vector2f(mMin.mX + width,		mMin.mY + heightDiv2),	mMaxDepth, mDepth + 1, mTargetEntities);
					mChildNodes[2] = new QuadTreeNode(Vector2f(mMin.mX,					mMin.mY + heightDiv2),	Vector2f(mMin.mX + widthDiv2,	mMin.mY + height),		mMaxDepth, mDepth + 1, mTargetEntities);
					mChildNodes[3] = new QuadTreeNode(Vector2f(mMin.mX + widthDiv2,		mMin.mY + heightDiv2),	Vector2f(mMin.mX + width,		mMin.mY + height),		mMaxDepth, mDepth + 1, mTargetEntities);
				}

				mChildrenInUse = true;

				std::vector<Entity*> ents(mMyEntities);
				mMyEntities.clear();

				for(unsigned int i = 0; i < ents.size(); ++i)
				{
					InsertEntity(ents[i]);
				}
			}
		}

	public:
		QuadTreeNode(Vector2f min, Vector2f max, int maxDepth, int depth, int targetEntities) 
		{
			mMin = min;
			mMax = max;
			mDepth = depth;
			mTargetEntities = targetEntities;
			mMaxDepth = maxDepth;

			mChildrenInUse = false;
			mChildNodes.resize(4);

			mChildNodes[0] = NULL;
			mChildNodes[1] = NULL;
			mChildNodes[2] = NULL;
			mChildNodes[3] = NULL;
		}
		~QuadTreeNode(){}

		std::vector<QuadTreeNode*> ChildNodes() const { return mChildNodes; }
		std::vector<Entity*> Entities() const { return mMyEntities; }

		Vector2f Max() const { return mMax; }
		Vector2f Min() const { return mMin; }

		float Width(){ return mMax.mX - mMin.mX;}
		float Height(){ return mMax.mY - mMin.mY;}
		Vector2f Center(){return mMin + ((mMax - mMin) *0.5f);}

		bool NodeContainsCircle(Vector2f position, float radius)
		{
			return	mMin.mX < position.mX - radius &&
				mMin.mY < position.mY - radius &&
				mMax.mX > position.mX + radius &&
				mMax.mY > position.mY + radius;
		}

		bool NodeIntersectsCircle(Vector2f position, float radius)
		{
			return	mMin.mX < position.mX + radius &&
				mMin.mY < position.mY + radius &&
				mMax.mX > position.mX - radius &&
				mMax.mY > position.mY - radius;
		}

		bool NodeIntersectsRectangle(Vector2f min, Vector2f max)
		{
			return	mMin.mX < max.mX &&
				mMin.mY < max.mY &&
				mMax.mX > min.mX &&
				mMax.mY > min.mY;
		}

		bool IsOvercrowded() 
		{
			return((int)CompleteNumEntities() > (mTargetEntities * (NumChildNodes() + 1)));
		}

		bool IsAtMaxDepth() 
		{
			return mDepth >= mMaxDepth ;
		}

		bool HasChildren()
		{
			return mChildrenInUse;
		}

		bool InsertEntity(Entity* entity)
		{
			if( HasChildren() )
			{
				if(mChildNodes[0]->InsertEntity(entity)) return true;
				if(mChildNodes[1]->InsertEntity(entity)) return true;
				if(mChildNodes[2]->InsertEntity(entity)) return true;
				if(mChildNodes[3]->InsertEntity(entity)) return true;
			}

			return PlaceInThisNode(entity);
		}

		void Clear()
		{
			if(HasChildren())
			{
				mChildNodes[0]->Clear();
				mChildNodes[1]->Clear();
				mChildNodes[2]->Clear();
				mChildNodes[3]->Clear();
			}

			mChildrenInUse = false;
			mMyEntities.clear();
		}

		int NumChildNodes()
		{
			int count = 0;

			if(HasChildren())
			{
				count += 4;

				count += mChildNodes[0]->NumChildNodes();
				count += mChildNodes[1]->NumChildNodes();
				count += mChildNodes[2]->NumChildNodes();
				count += mChildNodes[3]->NumChildNodes();
			}

			return count;
		}

		unsigned int CompleteNumEntities()
		{
			unsigned int count = 0;

			if(HasChildren())
			{
				count += mChildNodes[0]->CompleteNumEntities();
				count += mChildNodes[1]->CompleteNumEntities();
				count += mChildNodes[2]->CompleteNumEntities();
				count += mChildNodes[3]->CompleteNumEntities();
			}

			count += mMyEntities.size();

			return count;
		}
	};
}

#endif
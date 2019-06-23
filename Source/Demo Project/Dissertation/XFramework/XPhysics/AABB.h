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

namespace XF
{
	class AABB
	{
	private:
		float mMinX, mMinY, mMinZ;
		float mMaxX, mMaxY, mMaxZ;

	public:
		AABB() : mMinX(0.0f), mMinY(0.0f), mMinZ(0.0f), mMaxX(0.0f), mMaxY(0.0f), mMaxZ(0.0f) {}
		AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) : 
			mMinX(minX), mMinY(minY), mMinZ(minZ), 
			mMaxX(maxX), mMaxY(maxY), mMaxZ(maxZ) {}
		AABB(AABB &aabb)
		{
			mMinX = aabb.mMinX;
			mMinY = aabb.mMinY;
			mMinZ = aabb.mMinZ;
			mMaxX = aabb.mMaxX;
			mMaxY = aabb.mMaxY;
			mMaxZ = aabb.mMaxZ;
		}
		~AABB() {}

		float Width(){return mMaxX - mMinX;}
		float Height(){return mMaxY - mMinY;}
		float Depth(){return mMaxZ - mMinZ;}

		float MinX(){ return mMinX; }
		float MinY(){ return mMinY; }
		float MinZ(){ return mMinZ; }
		float MaxX(){ return mMaxX; }
		float MaxY(){ return mMaxY; }
		float MaxZ(){ return mMaxZ; }

		float CenterX(){ return mMinX + (Width() * 0.5f); }
		float CenterY(){ return mMinY + (Height() * 0.5f); }
		float CenterZ(){ return mMinZ + (Depth() * 0.5f); }

		bool Intersects(const AABB &aabb)
		{
			return (
				mMinX <= aabb.mMaxX &&
				mMaxX >= aabb.mMinX &&
				mMinY <= aabb.mMaxY &&
				mMaxY >= aabb.mMinY &&
				mMinZ <= aabb.mMaxZ &&
				mMaxZ >= aabb.mMinZ );
		}

		bool Contains(const float x, const float y, const float z, const float radius)
		{
			return(
				mMinX < x - radius &&
				mMaxX > x + radius &&
				mMinY < y - radius &&
				mMaxY > y + radius && 
				mMinZ < z - radius &&
 				mMaxZ > z + radius);
		}
	};
}
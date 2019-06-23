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
	class RectangleF
	{
	private:
		float mMinX, mMinY, mMaxX, mMaxY;

	public:
		RectangleF() : mMinX(0.0f), mMinY(0.0f), mMaxX(0.0f), mMaxY(0.0f) {}
		RectangleF(float minX, float minY, float maxX, float maxY) : mMinX(minX), mMinY(minY), mMaxX(maxX), mMaxY(maxY) {}

		bool Contains(const float x, const float y, const float radius)
		{
			return(
				mMinX < x - radius &&
				mMinY < y - radius &&
				mMaxX > x + radius &&
				mMaxY > y + radius);
		}

		bool Intersects(const float x, const float y, const float radius)
		{
			return(
				mMinX < x + radius &&
				mMinY < y + radius &&
				mMaxX > x - radius &&
				mMaxY > y - radius);
		}

		float X(){return mMinX;}
		float Y(){return mMinY;}
		float Width(){return mMaxX - mMinX;}
		float Height(){return mMaxY - mMinY;}
	};
}
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

#ifndef ENTITY_H
#define ENTITY_H

#include "Vector2.h"

namespace XCS
{
	class Entity
	{
	public:
		enum EntityType
		{
			AGENT,
			OBSTACLE,
		};

	private:
		EntityType mType;

	protected:
		Vector2f mPosition, mVelocity;
		float mRadius;
		bool mColliding;

	public:
		Entity(EntityType type) 
			: mPosition(0.0f), mVelocity(0.0f), mColliding(false), mRadius(1.0f), mType(type){}

		Entity(EntityType type, float radius) 
			: mPosition(0.0f), mVelocity(0.0f), mColliding(false), mRadius(radius), mType(type){}

		~Entity(){}

		EntityType Type(){ return mType; }

		Vector2f Position() const {return mPosition;}
		void Position(Vector2f val){mPosition = val;}

		Vector2f Velocity() const { return mVelocity; }
		void Velocity(Vector2f val) { mVelocity = val; }

		void Colliding(bool val){mColliding = val;}
		bool Colliding(){return mColliding;}

		void Radius(float val){mRadius = val;}
		float Radius(){return mRadius;}

		virtual void Update(double dT){}

		virtual void Log(){}
	};
}

#endif
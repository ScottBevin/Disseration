/************************************************************************/
/*    Author: Scott Bevin                                               */
/************************************************************************/


#pragma once

#include <Ogre.h>
#include <limits>
#include "MathHelper.h"
#include "../XFrameworkTypedefsAndDefines.h"

namespace XF
{
	class GeometryHelper
	{
	public:
		static bool DoesLineIntersectCircle(
			const XF::Vector2& O,  // Line origin
			const XF::Vector2& D,  // Line direction
			const XF::Vector2& C,  // Circle center
			float radius)      // Circle radius
		{
			XF::Vector2 d = O - C;
			float a = D.dotProduct(D);
			float b = d.dotProduct(D);
			float c = d.dotProduct(d) - radius * radius;

			float disc = b * b - a * c;
			if (disc < 0.0f) {
				return false;
			}
			return true;
		} 


		static float RayDiskTimeToCollision(
			const XF::Vector2& p,
			const XF::Vector2& v,
			const XF::Vector2& p2,
			float radius,
			bool collision)
		{
			XF::Vector2 ba = p2 - p;
			float sqDiam = radius * radius;
			float time;

			float discr = -MathHelper::Sqr(MathHelper::Determinant(v, ba)) + sqDiam * v.squaredLength();

			if(discr > 0)
			{
				if(collision)
				{
					time = (v.dotProduct(ba) + std::sqrt(discr)) / (float)v.squaredLength();

					if(time < 0)
						time = -MathHelper::Infinity();
				}
				else
				{
					time = (v.dotProduct(ba) - std::sqrt(discr)) / (float)v.squaredLength();

					if(time < 0)
						time = MathHelper::Infinity();
				}
			}
			else
			{
				if(collision)
				{
					time = -MathHelper::Infinity();
				}
				else
				{
					time = MathHelper::Infinity();
				}
			}

			return time;
		}


		// Start = start of ray
		// Dir = direction of ray (normalized)
		// Normal = plane normal (normalized)
		// D = Distance of plane from origin along normal (D in the plane equation)
		// IntPoint = final pointof intersection
		// returns true if an intersection was found, or false if not.
		static bool RayPlaneIntersect(
			const XF::Vector3 &start, 
			const XF::Vector3 &dir, 
			const XF::Vector3 &normal, 
			float d, 
			XF::Vector3 &intPoint)
		{
			float dist = 0;
			float i = normal.dotProduct(dir);    

			if(i == 0) 
				return false;           // ray is parallel to plane, no hit


			float j = -(normal.dotProduct(start) + d);
			dist = j / i;                             // compute distance along ray to intersection point

			if( dist < std::numeric_limits<float>::epsilon()) 
				return false ;   // Hit is behind start of ray, no hit

			intPoint = start + dist * dir;          // compute the point of intersection, using the ray equation


			return true;
		}

	};
}


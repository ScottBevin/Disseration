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

#ifndef XCS_MATH
#define XCS_MATH

#include <cstdlib>
#include <math.h>
#include "Vector2.h"

namespace XCS
{
	class Math
	{
	public:
		static inline float Pi(){return 3.14159265f;}
		static inline float TwoPi(){return 6.28318531f;}
		static inline float PiOver2(){return 1.57079633f;}
		static inline float PiOver4(){return 0.785398163f;}
		static float Infinity(){return 9e9f;}
		static inline float VerySmallFloat(){return 0.00000000000000000000000000000000000000000000000000000000000000000000000000000000001f;}

		// Returns an integer value in the range >=0 and <lt
		static int RandomInt(int max)
		{
			return rand() % max;
		}

		// Returns an integer value in the range >=Min and <max
		static int RandomInt(int min, int max)
		{
			return min + rand() % (max - min);
		}

		// Returns a float value in the range >=0.0 and <1.0
		static float RandomFloat()
		{
			return (float)rand()/((float)RAND_MAX + 1.0f);
		}

		// Returns a float value in the range >=0.0 and <max
		static float RandomFloat(float max)
		{
			return max * (float)rand()/((float)RAND_MAX + 1.0f);
		}

		// Returns a float value in the range >=Min and <max
		static float RandomFloat(float min, float max)
		{
			return min + (max - min) * ( (float)rand()/((float)RAND_MAX + 1.0f));
		}

		static float Sqr(float num){return num * num;}

		static inline float Lerp(float value1, float value2, float amount)
		{
			return(value1 + ((value2 - value1) * amount));
		}

		static inline void Clamp(float &clampVal, float min, float max)
		{
			clampVal = std::min(clampVal, max);
			clampVal = std::max(clampVal, min);
		}
		
		static float Determinant(const XCS::Vector2f& p, const XCS::Vector2f& q)
		{
			return p.mX * q.mY - p.mY * q.mX;
		}

		static float PercentageFromTo(float value, float from, float to)
		{
			float dif = to - from;
			float difToValue = value - from;

			float percent = difToValue / dif;
			Clamp(percent, 0.0f, 1.0f);
			return percent;
		}

		static bool DoesLineIntersectCircle(
			const XCS::Vector2f& O,  // Line origin
			const XCS::Vector2f& D,  // Line direction
			const XCS::Vector2f& C,  // Circle center
			float radius)      // Circle radius
		{
			XCS::Vector2f d = O - C;
			float a = D.DotProduct(D);
			float b = d.DotProduct(D);
			float c = d.DotProduct(d) - radius * radius;

			float disc = b * b - a * c;
			if (disc < 0.0f) {
				return false;
			}
			return true;
		} 

		static bool DoesLineSegmentIntersectCircle(
			const XCS::Vector2f &p1,
			const XCS::Vector2f &p2,
			const XCS::Vector2f &center,
			float radius)
		{
			XCS::Vector2f dp = p2 - p1;
			float a = dp.mX * dp.mX + dp.mY * dp.mY;
			float b = 2 * (dp.mX * (p2.mX - center.mX)) + dp.mY * (p1.mY - center.mY);
			float c = Sqr(center.mX) + Sqr(center.mY) + Sqr(p1.mX) + Sqr(p1.mY);
			c -= 2 * (center.mX * p1.mX + center.mY * p1.mY);
			c -= Sqr(radius);

			float bb4ac = b * b - a * a * c;

			if(abs(a) < Math::VerySmallFloat() || bb4ac < 0)
			{
				return false;
			}

			float sqbb4ac = sqrt(bb4ac);
			float mul1 = (-b + sqbb4ac) / (2 * a);
			float mul2 = (-b - sqbb4ac) / (2 * a);

			return true;
		}
		
		static float RayDiskTimeToCollision(
			const XCS::Vector2f& p,
			const XCS::Vector2f& v,
			const XCS::Vector2f& p2,
			float radius,
			bool collision)
		{
			XCS::Vector2f ba = p2 - p;
			float sqDiam = radius * radius;
			float time;

			float discr = -Sqr(Determinant(v, ba)) + sqDiam * v.SquaredLength();

			if(discr > 0)
			{
				if(collision)
				{
					time = (v.DotProduct(ba) + sqrt(discr)) / (float)v.SquaredLength();

					if(time < 0)
						time = -Infinity();
				}
				else
				{
					time = (v.DotProduct(ba) - sqrt(discr)) / (float)v.SquaredLength();

					if(time < 0)
						time = Infinity();
				}
			}
			else
			{
				if(collision)
				{
					time = -Infinity();
				}
				else
				{
					time = Infinity();
				}
			}

			return time;
		}

		  /* Time to collision of a ray to a line segment.
			 \param p The start position of the ray
			 \param v The velocity vector of the ray
			 \param a The first endpoint of the line segment
			 \param b The second endpoint of the line segment
			 \param collision Specifies whether the time to collision is computed (false), or the time from collision (true).
			 \returns Returns the time to collision of ray p + tv to segment ab, and #RVO_INFTY when the line segment is not hit. If collision is true, the value is negative.
		*/
		static float RayLineTimeToCollision(
			const XCS::Vector2f &p,
			const XCS::Vector2f &v,
			const XCS::Vector2f &a,
			const XCS::Vector2f &b,
			bool collision)
		{
			float d = Determinant(v, b - a);

			if(d == 0)
			{
				if(collision)
					return -Infinity();
				else
					return Infinity();
			}

			float invD = 1.0f / d;
			float t = Determinant(a - p, b - a) * invD;
			float s = Determinant(p - a, v) * -invD;

			if(t < 0 || s < 0 || s > 1)
			{
				if(collision)
					return -Infinity();
				else
					return Infinity();
			}
			else
			{
				return t;
			}
		}

		static bool DoesCircleIntersectRectangle(const XCS::Vector2f &circleCenter, float circleRadius, const XCS::Vector2f &min, const XCS::Vector2f &max)
		{
			return	min.mX < circleCenter.mX + circleRadius &&
				min.mY < circleCenter.mY + circleRadius &&
				max.mX > circleCenter.mX - circleRadius &&
				max.mY > circleCenter.mY - circleRadius;
		}

		static bool DoesCircleIntersectCircle(const XCS::Vector2f &circle1Center, float circle1Radius, const XCS::Vector2f &circle2Center, float circle2Radius)
		{
			return	(circle1Center - circle2Center).SquaredLength() <= Sqr(circle1Radius + circle2Radius);
		}

		static bool LineLineIntersection(
			const Vector2f &p1,
			const Vector2f &p2,
			const Vector2f &p3,
			const Vector2f &p4,
			Vector2f &intersectionPoint)
		{
			float  x1 = p1.mX, x2 = p2.mX, x3 = p3.mX, x4 = p4.mX;
			float y1 = p1.mY, y2 = p2.mY, y3 = p3.mY, y4 = p4.mY;

			float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
			// If d is zero, there is no intersection
			if (d == 0) return false;

			// Get the x and y
			float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
			float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
			float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

			// Check if the x and y coordinates are within both lines
			if ( x < std::min(x1, x2) || x > std::max(x1, x2) ||
				x < std::min(x3, x4) || x > std::max(x3, x4) ) return NULL;
			if ( y < std::min(y1, y2) || y > std::max(y1, y2) ||
				y < std::min(y3, y4) || y > std::max(y3, y4) ) return NULL;

			// Return the point of intersection
			intersectionPoint.mX = x;
			intersectionPoint.mY = y;
			return true;
		}
	};
}

#endif
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

#ifndef XF_MATHHELPER
#define XF_MATHHELPER

#include <math.h>
#include "../XFrameworkTypedefsAndDefines.h"

#define RAD 0.017453292519943295769236907684886//chris
#define DEG 57.295779513082320876798154814105//chris

namespace XF
{
	class MathHelper
	{
	public:
		static inline float Pi(){return 3.14159265f;}
		static inline float TwoPi(){return 6.28318531f;}
		static inline float PiOver2(){return 1.57079633f;}
		static inline float PiOver4(){return 0.785398163f;}
		static float Infinity(){return 9e9f;}
		static inline float RadConversion(){return 0.017453292519943295769236907684886f;}
		static inline float DegConversion(){return 57.295779513082320876798154814105f;}
	
		static inline float Lerp(float value1, float value2, float amount)
		{
			return(value1 + ((value2 - value1) * amount));
		}
		static inline Vector3 VectorLerp(Vector3 value1, Vector3 value2, float amount) //chris
		{
			return(value1 + ((value2 - value1) * amount));
		}

		static inline float Min(float val1, float val2)
		{
			return ((val1 < val2) ? val1 : val2);
		}

		static inline float Max(float val1, float val2)
		{
			return ((val1 > val2) ? val1 : val2);
		}

		static inline void Clamp(float &clampVal, float min, float max)
		{
			clampVal = Min(clampVal, max);
			clampVal = Max(clampVal, min);
		}

		static inline float DegToRad(float value){return value * RadConversion();}
		static inline float RadToDeg(float value){return value * DegConversion();}

        static float Sqr(float num){return num * num;}

        static float Determinant(const XF::Vector2& p, const XF::Vector2& q)
        {
                return p.x * q.y - p.y * q.x;
		}
		//Chris thought i'd add these two functions:
		static bool IsOdd(int number) 
		{
			return (number & 0x1);
		}
		static bool IsEven(int number)
		{
			return (!(number & 0x1));
		}
	};
}

#endif
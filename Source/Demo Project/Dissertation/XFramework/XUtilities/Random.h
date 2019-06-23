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

#include <cstdlib>
#include <time.h>
#include <mmsystem.h>

namespace XF
{
	namespace Random
	{
		// Returns an integer value in the range >=0 and <lt
		inline int NextInt(int max)
		{
			return rand() % max;
		}

		// Returns an integer value in the range >=Min and <max
		inline int NextInt(int min, int max)
		{
			return min + rand() % (max - min);
		}
		
		// Returns a float value in the range >=0.0 and <1.0
		inline float NextFloat()
		{
			return (float)rand()/((float)RAND_MAX + 1.0f);
		}

		// Returns a float value in the range >=0.0 and <max
		inline float NextFloat(float max)
		{
			return max * (float)rand()/((float)RAND_MAX + 1.0f);
		}

		// Returns a float value in the range >=Min and <max
		inline float NextFloat(float min, float max)
		{
			return min + (max - min) * ( (float)rand()/((float)RAND_MAX + 1.0f));
		}

		/*
		Start the random numbers from a known seed to get a
		repeatable series of random numbers
		*/
		inline void Seed(int s)
		{
			srand(s);
		}

		inline void Seed()
		{
			srand((unsigned int)time(0));
		}
	}
}
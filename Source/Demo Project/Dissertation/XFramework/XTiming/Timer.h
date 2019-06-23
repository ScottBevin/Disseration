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

/**
This class is an abstract class that acts as the interface for timers, for a timer to be usable it must implement
the update function.
@author Scott Bevin
@version 1.0
*/

namespace XF
{
	class Timer
	{
	protected:
		/**
		The elapsed time since last frame
		*/
		double m_deltaTime;
			/**
		The time stamp of the last frame
		*/
		double m_lastFrameIndex;
			/**
		The time stamp of the most current call to update()
		*/
		double m_thisFrameIndex;

	public:
				/**
		Constructor simply initializes variables
		*/
		Timer()
		{
			m_deltaTime = m_lastFrameIndex 
				= m_thisFrameIndex = 0;
		}

				/**
		blank virtual destructor to allow polymorphism to work correctly
		*/
		virtual ~Timer(){}

				/**
		update() must be implemented by child classes and will be used to update the variables in an API
		specific way
		*/
		virtual void update() = 0;

				/**
		returns the elapsed time since last frame
		*/
		double getDeltaTime(){return m_deltaTime;}
	};
}
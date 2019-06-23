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

#include "MessageSubjectEnumerations.h"

#include <math.h>

namespace XF
{
	struct Message
	{
		XF::MessageSubject::ENUM Subject;
		int SenderID;
		int ReceiverID;
		float SendTime;
		void* Data;
	};

	//these telegrams will be stored in a priority queue. Therefore the >
	//operator needs to be overloaded so that the PQ can sort the telegrams
	//by time priority. Note how the times must be smaller than
	//SmallestDelay apart before two Telegrams are considered unique.
	const double SmallestDelay = 0.25;


	inline bool operator==(const Message& m1, const Message& m2)
	{
		return ( fabs(m1.SendTime - m2.SendTime) < SmallestDelay) &&
			(m1.SenderID == m2.SenderID)        &&
			(m1.ReceiverID == m2.ReceiverID)    &&
			(m1.Subject == m2.Subject);
	}

	inline bool operator<(const Message& m1, const Message& m2)
	{
		if (m1 == m2)
		{
			return false;
		}

		else
		{
			return  (m1.SendTime < m2.SendTime);
		}
	}
}

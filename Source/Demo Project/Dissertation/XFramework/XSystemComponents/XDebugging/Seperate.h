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
#include <iosfwd>
#include <deque>
#include <list>
#include <vector>
#include "OgreVector3.h"
#include "OgreVector2.h"

class LineBreak
{
public:
	std::stringstream buffer;
	LineBreak(std::string with = "*",int howMany = 35)
	{
		buffer.str("");	
		for(int i = 0; i < 35; ++i)
			buffer << with;

	}
};
class Seperate
{
public:

	
	std::stringstream buffer;
	Seperate(const Ogre::Vector3 value,std::string with = "\t")
	{
		buffer.str("");
		buffer  << with << (value.x) << with  << (value.y) << with  <<(value.z);

	}

	

	Seperate(const Ogre::Vector2 value,std::string with = "\t")
	{ 
		buffer.str("");
		buffer << with<<  (value.x) << with  << (value.y);

	}
	template<class T>
	Seperate(const T value,std::string with = "\t");


	template <class T2>
	Seperate(std::deque<T2>& few,std::string with = "\t")
	{
		std::deque<T2>::iterator it = few.begin();
		for(it; it != few.end(); ++it)
		{
			Seperate temp(*it,with);
			buffer << with << temp.buffer;
		}
	}

	template <class T2>
	Seperate(std::list<T2>& few,std::string with = "\t")
	{
		std::list<T2>::iterator it = few.begin();
		for(it; it != few.end(); ++it)
		{
			Seperate temp(*it,with);
			buffer << with << temp.buffer;
		}
	}

	template <class T2>
	Seperate(std::vector<T2>& few,std::string with = "\t")
	{
		std::vector<T2>::iterator it = few.begin();
		for(it; it != few.end(); ++it)
		{
			Seperate temp(*it,with);
			buffer << with << temp.buffer;
		}
	}


};
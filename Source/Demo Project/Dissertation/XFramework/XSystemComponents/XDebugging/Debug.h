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

#include <ostream>

//all extras for special << 
#include "OgreVector3.h"
#include <iosfwd>
#include <deque>
#include <list>
#include <vector>
#include "XPropertys/IProperty.h"
#include "Seperate.h"

/*
The debug message class can be used with any debug display, the debug display 
has the functionality for output through the virtual function Log.

The debug message class allows for use of the << operator in debug messages like so
DebugMessage(aDisplay) << this number is << num;

It currently provides overloads for ogre vectors and quanternions so << Vector3(0,0,0) will output the vector as {0,0,0}

It has special operations: nextl, linebreak, tab, sep

nextl will go down to the next line
linebreak will go down two lines leaving a blank line
tab will put in 5 spaces
sep will use the displays "GetSeperator" function allowing for special string seperators such as "\t" 

the class Seperate, which is shorthanded to SEP allows for seperation of certain types for example SEP(Vector(1,2,3), "|") will output 1|2|3, the default is "\t"

the class LineBreak, which is shorthanded to lb allows for a quick linebreak of characters lb() will put do the same as linebreak but with 35 "-" filling the line, lb("#",50) would fill the line with 50 "#"

Both Seperate and normal << can read in a list, vector or deque, allowing you to quickly debug large information

aFileDisplay = new DebugToFile("Temp.txt")
the line DebugMessage(aFileDisplay) << "preparing to output list << nextl << SEP(myListOfVectors);

would output the entire list of vectors to a file called temp, with tab spacing between the numbers, allowing for a direct copy and paste to Excell

*/

namespace XF
{
	enum SpecialOperations
	{
		nextl,//next line, equivalent of std::endl
		linebreak, //equivelent of nextl << nextl<<
		tab,//5 spaces
		sep,//display will use GetSeperator which in a file would be "\t"
	};

	class DebugDisplay
	{
	public:
		virtual void Log(std::string message) = 0;
		virtual std::string GetSeperator(){return "     ";}
	};


	class DebugMessage 
	{
		std::stringstream buffer;
		
		DebugDisplay* display;



	public:
		DebugMessage& operator<<(const XF::IProperty& t) 
		{
			//if you are unlucky enough to put << getproperty("something")
			//instead of getproperty<type>("something") , this is the best way of finding it!
			buffer << "ERROR you read in a raw property, you must have forgot <int/float/whatever>";
			if(display)display->Log(buffer.str());
			assert(true);
			return *this;
		} //stream into the buffer

		std::string BufferResult(){return buffer.str();}
		DebugMessage(DebugDisplay& debug): display(&debug) { } //on creation we tell the DebugMessage what to use as a display
		DebugMessage(DebugDisplay* debug): display(debug) {}

		template <typename T>
		DebugMessage& operator<<(const T& t) { buffer << t; return *this; } //stream into the buffer
		

		DebugMessage& operator<<(const LineBreak& value)
		{
		
			if(display)display->Log(buffer.str());	
			buffer.str("");
			buffer << value.buffer.str();		
			if(display)display->Log(buffer.str());
			buffer.str("");

			return *this;
		}
		DebugMessage& operator<<(const Seperate& value)
		{
			
			buffer << value.buffer.str();				
	
			return *this;
		}
		DebugMessage& operator<<(SpecialOperations so)
		{
			switch(so)
			{
		
			case nextl:
				{
					if(display)display->Log(buffer.str());
					buffer.str("");
				}
			case linebreak:
				{
					if(display)display->Log(buffer.str());
					if(display)display->Log(" ");
					buffer.str("");				
				}
				
			case tab:
				{
					buffer << "     ";
					break;
				} 
			case sep:
				{
					if(display)
					buffer << display->GetSeperator();
					break;
				}
		
			default:
				break;
			}
			
			return *this;
			//return DebugMessage(*this); 
		}

		
		template <class T2>
		DebugMessage& operator<<(std::deque<T2>& few)
		{
			std::deque<T2>::iterator it = few.begin();
			for(it; it != few.end(); ++it)
			{
				DebugMessage(display) << tab << (*it);
			}
			return *this; 
		}

		template <class T2>
		DebugMessage& operator<<(std::list<T2>& few)
		{
			std::list<T2>::iterator it = few.begin();
			for(it; it != few.end(); ++it)
			{
				DebugMessage(display) << tab << (*it);
			}
			return *this; 
		}

		template <class T2>
		DebugMessage& operator<<(std::vector<T2>& few)
		{
			std::vector<T2>::iterator it = few.begin();
			for(it; it != few.end(); ++it)
			{
				DebugMessage(display) << tab << (*it);
			}
			return *this; 
		}


		DebugMessage& operator<<(bool &value)
		{
			buffer << (value ? "true" : "false");
			return *this; 
		}

		DebugMessage& operator<<(const Ogre::Vector3 &value)
		{
			buffer << "{ "   << (value.x)  << " ,  "  << (value.y)  << " ,  "   <<(value.z) << " }";
			return *this; 
		}

		DebugMessage& operator<<(const Ogre::Vector2 &value)
		{
			buffer << "{"   << (value.x) <<  " ,  "  <<(value.y) << "}";
			return *this; 
		}

		DebugMessage& operator<<(const Ogre::Quaternion &value)
		{
			buffer << "{X: "  << value.x << ", Y: "  <<value.y << ", Z: "  << value.z  <<  "W: "  <<  value.w  << ", Roll: "  << value.getRoll()  << ", Pitch: "  << value.getPitch() << ", Yaw: "  << value.getYaw() <<"}";
			return *this; 
		}
		
		void Flush(){display->Log(buffer.str());}//if you so choose you can create one instance and flush it every frame or more
		~DebugMessage() { if(display)display->Log(buffer.str()); } //on deletion output to the debug
	};
}
#define SEP Seperate
#define lb LineBreak

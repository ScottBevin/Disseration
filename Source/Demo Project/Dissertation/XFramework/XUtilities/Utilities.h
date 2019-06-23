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

#include <stdio.h> 
#include <stdlib.h> 
#include <windows.h> 

#include <vector>
#include <string>
#include <sstream>

#include <Ogre.h>

#include "../XFrameworkTypedefsAndDefines.h"
#include "Random.h"

namespace XF
{
	namespace Utilities
	{
		inline std::string IntToString(const int i)
		{
			std::stringstream sstr;
			sstr << i;
			return sstr.str();
		}

		template<class T>
		inline std::string ToString(const T value)
		{
			std::stringstream sstr;
			sstr << value;
			return sstr.str();
		}

		inline std::string ToString(const XF::Vector3 &value)
		{
			return "{" + ToString<float>(value.x) + ", " + ToString<float>(value.y) + ", " + ToString<float>(value.z) + "}";
		}

		inline std::string ToString(const XF::Vector2 &value)
		{
			return "{" + ToString<float>(value.x) + ", " + ToString<float>(value.y) + "}";
		}

		inline std::string ToString(const bool &value)//chris
		{
			return (value ? "true" : "false");
		}

		inline std::string CreateRandomString()
		{
			return XF::Utilities::ToString(XF::Random::NextInt(10000, 99999)) + 
				XF::Utilities::ToString(XF::Random::NextInt(10000, 99999)) + 
				XF::Utilities::ToString(XF::Random::NextInt(10000, 99999));
		}

		template <class T>
		inline bool FromString(T& t, 
						 const std::string& s, 
						 std::ios_base& (*f)(std::ios_base&) = std::dec)
		{
		  std::istringstream iss(s);
		  return !(iss >> f >> t).fail();
		}


		inline void TokenizeString(const std::string& str,
					std::vector<std::string>& tokens,
					const std::string& delimiters = " ")
		{
			// Skip delimiters at beginning.
			std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
			// Find first "non-delimiter".
			std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

			while (std::string::npos != pos || std::string::npos != lastPos)
			{
				// Found a token, add it to the vector.
				tokens.push_back(str.substr(lastPos, pos - lastPos));
				// Skip delimiters.  Note the "not_of"
				lastPos = str.find_first_not_of(delimiters, pos);
				// Find next "non-delimiter"
				pos = str.find_first_of(delimiters, lastPos);
			}
		}

		inline XF::Vector3 FromString(std::string value)
		{
			XF::Vector3 outValue;

			// remove { }
			std::string stripped = value.substr(1, value.size() - 2);

			std::vector<std::string> values;
			TokenizeString(stripped, values, ", ");
			
			FromString<float>(outValue.x, values[0], std::dec);
			FromString<float>(outValue.y, values[1], std::dec);
			FromString<float>(outValue.z, values[2], std::dec);

			return outValue;
		}

		inline bool StringContains(const std::string& str, const std::string& value)
		{
			return (str.find(value) != str.npos);
		}

		inline bool StringOnlyContains(const std::string& str, char var)
		{
			for(int i = 0; i < (int)str.size(); ++i)
			{
				if(str[i] != var) return false;
			}
			return true;
		}
		inline std::string ToLower(std::string val)
		{
			std::string lowerVal = val;
			int i = 0;
			while (lowerVal[i])
			{
				char c=lowerVal[i];
				lowerVal[i] = (tolower(c));
				i++;
			}
			return lowerVal;
		}
		inline std::string JoinStrings(std::vector<std::string> &strVec)
		{
			std::string outStr;

			for(int i = 0; i < (int)strVec.size(); ++i)
				outStr += strVec[i] + " ";

			return outStr;
		}

		inline std::vector<std::string> GetAllFilesInDirectory(std::string directory, std::string extention = ".*")
		{
			std::vector<std::string> retVec;

			WIN32_FIND_DATA info;

			char oldDir[MAX_PATH] = {""};
			GetCurrentDirectory(MAX_PATH, oldDir);

			std::string dir = std::string(oldDir) + "/" + directory + "/*" + extention;

			HANDLE fileHandle = FindFirstFile(dir.c_str(), &info);

			if (fileHandle != INVALID_HANDLE_VALUE)
			{
				do
				{
					if (!(strcmp(info.cFileName, ".") == 0 || strcmp(info.cFileName, "..") == 0))
					{
						retVec.push_back(info.cFileName);
					}

				} while (FindNextFile(fileHandle, &info));

				FindClose(fileHandle);
			}

			return retVec;
		}
	}
}
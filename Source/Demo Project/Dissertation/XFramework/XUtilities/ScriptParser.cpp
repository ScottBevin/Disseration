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

#include <fstream>

#include "ScriptParser.h"
#include "Logger.h"
#include "Utilities.h"

namespace XF
{
	ScriptParser::ScriptParser()
	{
	}

	ScriptParser::~ScriptParser()
	{
	}

	bool ScriptParser::ParseFile(std::string fileName, ConfigScriptCallback callbackFunction)
	{
		ILOG << "[XF::SP] Parsing file: " << fileName;

		mCallbackFunction = callbackFunction;
		mLines.clear();
		mVariableMap.clear();
		
		while(mOpenFiles.size() > 0)
			mOpenFiles.pop();

		if(!ReadFile(fileName)) 
		{
			ILOG << "[XF::SP] Reading file: " << fileName << " failed!";
			return false;
		}

		bool success = ParseAllLines();

		if(success)
		{
			ILOG << "[XF::SP] File parse successful";
		}
		else
		{
			ILOG << "[XF::SP] File parse failed";
		}
		
		return success;
	}

	bool ScriptParser::ReadFile(std::string fileName)
	{
		std::ifstream file(fileName.c_str());

		if(!file.is_open())
		{
			ILOG << "[XF::SP] Unable to open file: " << fileName;
			return false;
		}

		mOpenFiles.push(fileName);

		while(!file.eof())
		{
			std::string buffer, buffer2 = "";

			while(!XF::Utilities::StringContains(buffer2, ";") && !file.eof())//chris: it likes to get stuck here when there is spaces at the end of the file
			{
				std::getline(file, buffer);

				if(buffer.substr(0, 2) == "//" || buffer.size() <= 0)
					break;

				if(buffer.substr(0, 1) == "#")
				{
					if(!ParseSystemCommand(buffer.substr(1, buffer.size())))
					{
						ILOG << "[XF::SP] Failed to parse line: " << buffer;
						return false;
					}

					continue;
				}

				buffer2 += buffer + " ";
			}

			if(buffer2.size() > 0 && !Utilities::StringOnlyContains(buffer2,' ')) //chris: to fix spaces at end of script error with lines
			{
				buffer2 = buffer2.substr(0, buffer2.size() - 2);

				mLines.push_back(buffer2);
			}
		}

		file.close();

		mOpenFiles.pop();

		return true;
	}

	bool ScriptParser::ParseAllLines()
	{
		for(unsigned int i = 0; i < mLines.size(); ++i)
		{
			if(!ParseLine(mLines[i]))
			{
				XF::Logger::GetInstance()->Log("[XF::SP] Failed to parse line: " + mLines[i]);
				return false;
			}
		}

		return true;
	}

	bool ScriptParser::ParseLine(std::string line)
	{
		std::vector<std::string> strings;
		if(!SeperateLine(line, strings)) return false;

		std::string key = strings[0];
		strings.erase(strings.begin());

		if(mTypedefMap.find(key) != mTypedefMap.end())
			key = mTypedefMap[key];

		if(mCallbackFunction)
			mCallbackFunction(key, strings);

		return true;
	}

	bool ScriptParser::ParseSystemCommand(std::string line)
	{
		std::vector<std::string> strings;
		if(!SeperateLine(line, strings)) return false;

		if(strings[0] == "typedef") 
			RegisterTypedef(strings[1], strings[2]);
		else if(strings[0] == "include")
		{
			for(unsigned int i = 1; i < strings.size(); ++i)
				if(!ReadFile(ResolveFileName(strings[i]))) 
					return false;
		}

		return true;
	}

	bool ScriptParser::SeperateLine(std::string line, std::vector<std::string> &outStrings)
	{
		std::string buffer;
		std::stringstream ss(line);

		while(ss >> buffer)
			outStrings.push_back(buffer);

		if(outStrings.size() <= 0)
			return false;

		return true;
	}

	void ScriptParser::RegisterTypedef(std::string key, std::string value)
	{
		if(mTypedefMap.find(key) == mTypedefMap.end())
			mTypedefMap.insert(std::make_pair(key, value));
	}

	std::string ScriptParser::ResolveFileName(std::string inputFileName)
	{
		std::string currentPath = mOpenFiles.top();

		//remove fileName from end to get current directory
		currentPath = currentPath.substr(0, currentPath.find_last_of("/") + 1);

		// decompose the input string
		while(inputFileName.size() > 0)
		{
			size_t idx = inputFileName.find("/");

			if(idx == -1)
			{
				currentPath += inputFileName.substr(0, inputFileName.size());
				break;
			}

			std::string s = inputFileName.substr(0, idx);
			inputFileName = inputFileName.substr(idx + 1, inputFileName.size());

			// . means back one level
			if(s.substr(0, 2) == "..")
			{
				for(unsigned int i = 0; i < s.size(); ++i)
				{
					currentPath = currentPath.substr(0, currentPath.find_last_of("/"));
				}

				currentPath += "/";
			}
			else if(s[0] == '.')
			{
				// skip here this allows same directory to be expressed as "./file"
			}
			else
			{
				currentPath += s + "/";
			}
		}

		return currentPath;
	}
}

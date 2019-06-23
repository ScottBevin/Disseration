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

#include <sstream>
#include <string>
#include <map>
#include <vector>

#include <cassert>
#include <fstream>
#include "XPropertys\PropertyContainer.h"
#include <string>

namespace XF
{
	class GameSettings : public PropertyContainer
	{
	private:
		std::string mFileName;
		static GameSettings* smInstance;

		void FileParserCallback(std::string key, std::vector<std::string> &arguments);

		GameSettings(){}
		~GameSettings();
	public:
		static void Initialise(std::string fileName);
		static void ShutDown();

		static GameSettings* Instance();

		static void SaveSettings();
	};

#ifndef XF_GAMESETTINGS
#define XF_GAMESETTINGS XF::GameSettings::Instance()
#endif
}
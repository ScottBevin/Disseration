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

#include <ogre.h>
#include <string>
#include <vector>
#include <map>

#include "XUtilities/ScriptParser.h"
#include "GameEntity.h"


namespace XF
{
	class GameEntityFactory
	{
	private:
		GameEntity* mNewGameEntity;
		std::map<std::string, std::string> mFilepathMap;
		std::map<std::string, GameEntity*> mEntityCache;

		void FileParserCallback(std::string key, std::vector<std::string> &arguments);
		void ConfigFileParserCallback(std::string key, std::vector<std::string> &arguments);

		GameEntityFactory();
		static GameEntityFactory* smInstance;

	public:
		~GameEntityFactory();

		static bool Initialize(std::string configFilePath);

		static GameEntityFactory* GetInstance()
		{
			assert(smInstance);

			return smInstance;
		}

		GameEntity* CreateGameEntity(std::string fileName);
	   //chris to scott can you please create this in a way that disallows modification? i tried a few ways but the only on that compiled crashed in the property mananger,thanks		
		GameEntity* LookUpGameEntity(std::string fileName);
		bool GameEntityFactory::LoadAllGameEntitiesIntoMemory()/*chris */;


		void ClearCache();
		
	};
}
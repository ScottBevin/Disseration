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

#include "GameEntityFactory.h"
#include "..\XUtilities\Utilities.h"
#include "XEntityComponents\EntityComponentFactory.h"
#include "XEntityComponents\EntityComponent.h"
#include "XUtilities\Logger.h"

namespace XF
{
	GameEntityFactory* GameEntityFactory::smInstance = NULL;

	GameEntityFactory::GameEntityFactory()
	{
		mNewGameEntity = NULL;
	}

	GameEntityFactory::~GameEntityFactory()
	{
		ClearCache();
	}
	GameEntity* GameEntityFactory::LookUpGameEntity(std::string fileName)
	{
		fileName = XF::Utilities::ToLower(fileName);
		if(mEntityCache.find(fileName) == mEntityCache.end())
		{
			//creating one will put it in the entity cache
			delete CreateGameEntity(fileName);
		}
		GameEntity* ret = mEntityCache[fileName];
		return ret;
	}
	void GameEntityFactory::ClearCache()
	{
		ILOG << "XF::GameEntityFactory - cleared cache!";

		for(std::map<std::string, GameEntity*>::iterator it = mEntityCache.begin();
			it != mEntityCache.end(); ++it)
		{
			delete it->second;
		}

		mEntityCache.clear();
	}

	bool GameEntityFactory::Initialize(std::string configFilePath)
	{
		ScriptParser fileParser;

		smInstance = new GameEntityFactory();

		if(!fileParser.ParseFile(configFilePath,
			ScriptParser::ConfigScriptCallback::FromMethod<GameEntityFactory, &GameEntityFactory::ConfigFileParserCallback>(smInstance)))
		{
			SSELOG << "XF::GameEntityFactory - Failed to initialize.";
			delete smInstance;
			smInstance = NULL;
		}
		else
		{
			SSELOG << "XF::GameEntityFactoy - succesfully initialized.";
		}

		return (smInstance != NULL);
	}

	bool GameEntityFactory::LoadAllGameEntitiesIntoMemory()//chris
	{
		std::map<std::string, std::string>::iterator it;
		for(it = mFilepathMap.begin(); it != mFilepathMap.end(); ++it)
		{
			CreateGameEntity(it->first);
		}
		return true;
	}
	

	GameEntity* GameEntityFactory::CreateGameEntity(std::string fileName)
	{
		if(mNewGameEntity)
		{
			delete mNewGameEntity;
			mNewGameEntity = NULL;
		}

		fileName = XF::Utilities::ToLower(fileName); // chris

		std::map<std::string, GameEntity*>::iterator it = mEntityCache.find(fileName);

		if(it != mEntityCache.end())
		{
			mNewGameEntity = new GameEntity(*mEntityCache[fileName]);
		}
		else
		{
			mNewGameEntity = new GameEntity();

			std::string filePath = mFilepathMap[fileName];

			XF::ScriptParser fileParser;

			if(!fileParser.ParseFile(filePath, 
				XF::ScriptParser::ConfigScriptCallback::FromMethod<GameEntityFactory, &GameEntityFactory::FileParserCallback>(this)))
			{
				ILOG << "XF::GameEntityFactory - Failed to parse script file: " << filePath;

				delete mNewGameEntity;
				mNewGameEntity = NULL;
				return NULL;
			}

			mEntityCache.insert(std::make_pair(fileName, mNewGameEntity));
			mNewGameEntity = new GameEntity(*mEntityCache[fileName]);
			
			
		}

		mNewGameEntity->AddProperty<std::string>("ScriptFile",fileName); //chris, for error messages
		GameEntity* ret = mNewGameEntity;
		mNewGameEntity = NULL;
		return ret;
	}

	void GameEntityFactory::ConfigFileParserCallback(std::string key, std::vector<std::string> &arguments)
	{
		std::string fileDir = key + " " + XF::Utilities::JoinStrings(arguments);
		fileDir[fileDir.size() - 1] = '/';

		std::vector<std::string> vec = XF::Utilities::GetAllFilesInDirectory(fileDir);

		for(int i = 0; i < (int)vec.size(); ++i)
		{
			std::string key = XF::Utilities::ToLower(vec[i]); //chris, just so that everything is stored in lower case 
			mFilepathMap.insert(std::make_pair(key, fileDir + vec[i]));
	
		}
	}

	void GameEntityFactory::FileParserCallback(std::string key, std::vector<std::string> &arguments)
	{
		if(key == "component")
		{
			XF::EntityComponentFactory factory;
			XF::EntityComponent *newComponent = factory.CreateComponent(arguments[0], mNewGameEntity);

			std::string variables = "";

			for(unsigned int i = 1; i < arguments.size(); ++i) variables += arguments[i] + " ";

			std::vector<std::string> vars;
			XF::Utilities::TokenizeString(variables, vars, ":");

			for(unsigned int i = 0; i < vars.size(); ++i)
			{
				std::vector<std::string> tokens;
				XF::Utilities::TokenizeString(vars[i], tokens);

				PropertySerializer::PropertyTypeId typeID = PropertySerializer::TypeIdFromString(tokens[0]);

				IProperty* newProp = PropertySerializer::CreateProperty(typeID, tokens[1]);

				std::string value = "";

				for(unsigned int i = 2; i < tokens.size(); ++i) value += tokens[i] + " ";
				value = value.substr(0, value.size() - 1);

				newProp->SetFromString(value);
				newComponent->AddProperty(newProp);
			}

			mNewGameEntity->AddComponent(newComponent);
		}
		else if(key == "property")
		{
			PropertySerializer::PropertyTypeId typeID = PropertySerializer::TypeIdFromString(arguments[0]);

			IProperty* newProp = PropertySerializer::CreateProperty(typeID, arguments[1]);

			std::string value = "";

			for(unsigned int i = 2; i < arguments.size(); ++i) value += arguments[i] + " ";
			value = value.substr(0, value.size() - 1);

			newProp->SetFromString(value);

			mNewGameEntity->AddProperty(newProp);
		}
	}
}
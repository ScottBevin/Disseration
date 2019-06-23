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


#include "GameSettings.h"

#include "Utilities.h"
#include "ScriptParser.h"
#include <iosfwd>
#include "XPropertys\PropertySerializer.h"
#include "Logger.h"

namespace XF
{
	GameSettings* GameSettings::smInstance = NULL;

	GameSettings::~GameSettings()
	{
	}

	GameSettings* GameSettings::Instance()
	{
		assert(smInstance);

		return smInstance;
	}

	void GameSettings::Initialise(std::string fileName)
	{
		assert(!smInstance);

		SSELOG << "Loading game settings file: " << fileName;

		smInstance = new GameSettings();
		smInstance->mFileName = fileName;

		XF::ScriptParser fileParser;

		if(!fileParser.ParseFile(fileName, 
			XF::ScriptParser::ConfigScriptCallback::FromMethod<GameSettings, &GameSettings::FileParserCallback>(smInstance)))
		{
			SSELOG << "Failed to load settings file";

			smInstance = NULL;
		}
		else
			SSELOG << "Finished loading settings";
	}

	void GameSettings::FileParserCallback(std::string key, std::vector<std::string> &arguments)
	{
		PropertySerializer::PropertyTypeId typeID = PropertySerializer::TypeIdFromString(key);

		IProperty* newProp = PropertySerializer::CreateProperty(typeID, arguments[0]);

		std::string value = "";

		for(unsigned int i = 1; i < arguments.size(); ++i) value += arguments[i] + " ";
		value = value.substr(0, value.size() - 1);

		newProp->SetFromString(value);

		smInstance->AddProperty(newProp);

		ILOG << "Game setting loaded: (" << key << ") " << arguments[0] << ": " << value;
	}

	void GameSettings::ShutDown()
	{
		assert(smInstance);

		GameSettings::SaveSettings();

		delete smInstance;
		smInstance = NULL;
	}

	void GameSettings::SaveSettings()
	{
		std::fstream file;

		ILOG << "Saving game settings";

		file.open(smInstance->mFileName.c_str(), std::ios_base::out);

		if(file.is_open())
		{
			for(std::map<std::string, IProperty*>::iterator it = smInstance->mProperties.begin();
				it != smInstance->mProperties.end(); ++it)
			{
				std::string type = PropertySerializer::StringFromTypeID((PropertySerializer::PropertyTypeId)it->second->TypeID());
				std::string name = it->second->Name();
				std::string value = it->second->ToString();

				file << type << " " << name << " " << value << ";\n";

				ILOG << "Game setting saved: (" << type << ") " << name << ": " << value;
			}
		}

		ILOG << "Finished saving game settings";
	}
}
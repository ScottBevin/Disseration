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

#include "PropertyContainer.h"
#include "PropertySerializer.h"

namespace XF
{
	PropertyContainer::PropertyContainer()
	{
	}

	PropertyContainer::PropertyContainer(PropertyContainer &copy)
	{
		for(std::map<std::string, IProperty*>::const_iterator it = copy.mProperties.begin();
			it != copy.mProperties.end(); ++it)
		{
			IProperty* newProp = PropertySerializer::CreateProperty((PropertySerializer::PropertyTypeId)it->second->TypeID(), it->first, it->second->ToString());
			AddProperty(newProp);
		}
	}

	PropertyContainer::~PropertyContainer()
	{
		RemoveAllProperties();
	}

	void PropertyContainer::AddProperty(IProperty *prop)
	{
		if(!HasProperty(prop->Name()))
		{
			mProperties[prop->Name()] = prop;
		}
	}

	IProperty *PropertyContainer::GetIProperty(const std::string &name)
	{
		std::map<std::string, IProperty*>::iterator it = mProperties.find(name);

		if(it != mProperties.end())
		{
			return it->second;
		}
		else
		{
			throw "Property does not exist: " + name;
		}
	}


	void PropertyContainer::RemoveProperty(const std::string &name)
	{
		std::map<std::string, IProperty*>::iterator it = mProperties.find(name);

		if(it != mProperties.end())
		{
			delete it->second;
			mProperties.erase(it);
		}
	}

	void PropertyContainer::RemoveAllProperties()
	{
		for(std::map<std::string, IProperty*>::iterator it = mProperties.begin();
			it != mProperties.end(); ++it)
		{
			delete it->second;
		}

		mProperties.clear();
	}
}

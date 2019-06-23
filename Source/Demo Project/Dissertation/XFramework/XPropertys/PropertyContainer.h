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

#include <map>
#include <string>

#include "Property.h"
#include "../XUtilities/Logger.h"

namespace XF
{
	class PropertyContainer
	{
	protected:
		std::map<std::string, IProperty*> mProperties;

	public:
		PropertyContainer();
		PropertyContainer(PropertyContainer &copy);
		virtual ~PropertyContainer();

		bool HasProperty(std::string propName)
		{
			return (mProperties.find(propName) != mProperties.end());
		}
		
		template<class T>
		Prop<T> AddProperty(const std::string &name, const T &value);		
		void AddProperty(IProperty *prop);

		template<class T>
		Prop<T>& Property(const std::string &name);
		
		IProperty *GetIProperty(const std::string &name);

		void RemoveProperty(const std::string &name);
		void RemoveAllProperties();

		std::map<std::string, IProperty*> &Properties(){return mProperties;}
	};

	template<class T>
	Prop<T> PropertyContainer::AddProperty(const std::string &name, const T &value)
	{
		std::map<std::string, IProperty*>::iterator it = mProperties.find(name);

		if(it != mProperties.end())
		{
			Prop<T>* prop = dynamic_cast< Prop<T>* >(it->second);

			if(!prop)
				throw "Property " + name + " already exists with a different type!";

			prop->Set(value);
			return *prop;
		}

		Prop<T> *newProp = new Prop<T>(name);
		newProp->Set(value);
		mProperties[name] = newProp;

		return *newProp;
	}

	
	template<class T>
	Prop<T>& PropertyContainer::Property(const std::string &name)
	{
		std::map<std::string, IProperty*>::iterator it = mProperties.find(name);

		if(it != mProperties.end())
		{
			Prop<T>* prop = dynamic_cast< Prop<T>* >(it->second);

			assert(prop);

			return *prop;
		}
		else
		{
			LOG << "[XF::PropertyContainer]: Creating property[Name: " << name << "]";
			AddProperty<T>(name, T());
			return Property<T>(name);
		}
	}
}


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

#include <string>
#include <Ogre.h>

#include "../XFrameworkTypedefsAndDefines.h"

namespace XF
{
	class IProperty;

	class PropertySerializer
	{
	public:
		enum PropertyTypeId
		{
			TYPE_UNDEFINED,
			TYPE_UNSIGNED_INT,
			TYPE_INT,
			TYPE_FLOAT,
			TYPE_DOUBLE,
			TYPE_BOOL,
			TYPE_STRING,
			TYPE_VECTOR3,
			TYPE_OGRE_QUATERNION,
			TYPE_VOID_PTR,
		};

		PropertySerializer();
		~PropertySerializer();

		static IProperty *CreateProperty(PropertyTypeId typeID, const std::string name, std::string defaultValue = "");

		static PropertyTypeId TypeIdFromString(std::string value);
		static std::string StringFromTypeID(PropertyTypeId typeID);

		template<typename Type>
		static std::string ToString(const Type &value);
		template<typename Type>
		static void FromString(const std::string &inValue, Type &outValue);
		template<typename Type>
		static PropertyTypeId GetTypeId(const Type &value);

		static std::string ToString(const unsigned int &value);
		static void FromString(const std::string &inValue, unsigned int &outValue);
		static PropertyTypeId GetTypeId(const unsigned int &value);

		static std::string ToString(const int &value);
		static void FromString(const std::string &inValue, int &outValue);
		static PropertyTypeId GetTypeId(const int &value);

		static std::string ToString(const float &value);
		static void FromString(const std::string &inValue, float &outValue);
		static PropertyTypeId GetTypeId(const float &value);

		static std::string ToString(const double &value);
		static void FromString(const std::string &inValue, double &outValue);
		static PropertyTypeId GetTypeId(const double &value);

		static std::string ToString(const bool &value);
		static void FromString(const std::string &inValue, bool &outValue);
		static PropertyTypeId GetTypeId(const bool &value);

		static std::string ToString(const std::string &value);
		static void FromString(const std::string &inValue, std::string &outValue);
		static PropertyTypeId GetTypeId(const std::string &value);

		static std::string ToString(const XF::Vector3 &value);
		static void FromString(const std::string &inValue, XF::Vector3 &outValue);
		static PropertyTypeId GetTypeId(const XF::Vector3 &value);

		static std::string ToString(const Ogre::Quaternion &value);
		static void FromString(const std::string &inValue, Ogre::Quaternion &outValue);
		static PropertyTypeId GetTypeId(const Ogre::Quaternion &value);

		static std::string ToString(const void* value);
		static void FromString(const void* inValue, void* outValue);
		static PropertyTypeId GetTypeId(const void* value);
	};
}
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

#include "PropertySerializer.h"
#include "Property.h"
#include "XUtilities/Utilities.h"

#include <ogre.h>
#include <vector>

namespace XF
{
	PropertySerializer::PropertySerializer()
	{
	}

	PropertySerializer::~PropertySerializer()
	{
	}

	IProperty *PropertySerializer::CreateProperty(PropertyTypeId typeID, const std::string name, std::string defaultValue)
	{
		IProperty *retProp = NULL;

		switch(typeID)
		{
		case PropertySerializer::TYPE_UNSIGNED_INT:
			retProp = new Prop<unsigned int>(name); break;
		case PropertySerializer::TYPE_INT:
			retProp = new Prop<int>(name); break;
		case PropertySerializer::TYPE_FLOAT:
			retProp = new Prop<float>(name); break;
		case PropertySerializer::TYPE_DOUBLE:
			retProp = new Prop<double>(name); break;
		case PropertySerializer::TYPE_BOOL:
			retProp = new Prop<bool>(name); break;
		case PropertySerializer::TYPE_STRING:
			retProp = new Prop<std::string>(name); break;
		case PropertySerializer::TYPE_VECTOR3:
			retProp = new Prop<XF::Vector3>(name); break;
		case PropertySerializer::TYPE_OGRE_QUATERNION:
			retProp = new Prop<Ogre::Quaternion>(name); break;
		case PropertySerializer::TYPE_VOID_PTR:
			retProp = new Prop<void*>(name); break;
		default:
			throw "PropertySerializer::CreateProperty - unknown property type";
		}

		if(defaultValue != "")
			retProp->SetFromString(defaultValue);

		return retProp;
	}

	std::string PropertySerializer::StringFromTypeID(PropertyTypeId typeID)
	{
		switch(typeID)
		{
		case PropertySerializer::TYPE_UNSIGNED_INT:
			return "uint";
		case PropertySerializer::TYPE_INT:
			return "int";
		case PropertySerializer::TYPE_FLOAT:
			return "float";
		case PropertySerializer::TYPE_DOUBLE:
			return "double";
		case PropertySerializer::TYPE_BOOL:
			return "bool";
		case PropertySerializer::TYPE_STRING:
			return "string";
		case PropertySerializer::TYPE_VECTOR3:
			return "vector3";
		case PropertySerializer::TYPE_OGRE_QUATERNION:
			return "quaternion";
		case PropertySerializer::TYPE_VOID_PTR:
			return "voidPtr";
		default:
			throw "PropertySerializer::StringFromTypeID - unknown property type";
		}
	}

	PropertySerializer::PropertyTypeId PropertySerializer::TypeIdFromString(std::string value)
	{
		if(value == "uint")
			return PropertySerializer::TYPE_UNSIGNED_INT;

		if(value == "int")
			return PropertySerializer::TYPE_INT;

		if(value == "float")
			return PropertySerializer::TYPE_FLOAT;

		if(value == "double")
			return PropertySerializer::TYPE_DOUBLE;

		if(value == "bool")
			return PropertySerializer::TYPE_BOOL;

		if(value == "string")
			return PropertySerializer::TYPE_STRING;

		if(value == "vector3")
			return PropertySerializer::TYPE_VECTOR3;

		if(value == "quaternion")
			return PropertySerializer::TYPE_OGRE_QUATERNION;

		if(value == "voidPtr")
			return PropertySerializer::TYPE_VOID_PTR;

		throw "PropertySerializer::TypeIdFromString - Unknown property type: " + value;
	}


	template<typename Type>
	std::string PropertySerializer::ToString(const Type &value)
	{
		return XF::Utilities::ToString<Type>(value);
	}

	template<typename Type>
	void PropertySerializer::FromString(const std::string &inValue, Type &outValue)
	{
		XF::Utilities::FromString<Type>(outValue, inValue, std::dec);
	}

	template<typename Type>
	PropertySerializer::PropertyTypeId PropertySerializer::GetTypeId(const Type &value)
	{
		return PropertySerializer::TYPE_UNDEFINED;
	}


	std::string PropertySerializer::ToString(const unsigned int &value)
	{
		return XF::Utilities::ToString<unsigned int>(value);
	}

	void PropertySerializer::FromString(const std::string &inValue, unsigned int &outValue)
	{
		XF::Utilities::FromString<unsigned int>(outValue, inValue, std::dec);
	}

	PropertySerializer::PropertyTypeId PropertySerializer::GetTypeId(const unsigned int &value)
	{
		return PropertySerializer::TYPE_UNSIGNED_INT;
	}


	std::string PropertySerializer::ToString(const int &value)
	{
		return XF::Utilities::ToString<int>(value);
	}

	void PropertySerializer::FromString(const std::string &inValue, int &outValue)
	{
		XF::Utilities::FromString<int>(outValue, inValue, std::dec);
	}

	PropertySerializer::PropertyTypeId PropertySerializer::GetTypeId(const int &value)
	{
		return PropertySerializer::TYPE_INT;
	}


	std::string PropertySerializer::ToString(const float &value)
	{
		return XF::Utilities::ToString<float>(value);
	}

	void PropertySerializer::FromString(const std::string &inValue, float &outValue)
	{
		XF::Utilities::FromString<float>(outValue, inValue, std::dec);
	}

	PropertySerializer::PropertyTypeId PropertySerializer::GetTypeId(const float &value)
	{
		return PropertySerializer::TYPE_FLOAT;
	}

	std::string PropertySerializer::ToString(const double &value)
	{
		return XF::Utilities::ToString<double>(value);
	}

	void PropertySerializer::FromString(const std::string &inValue, double &outValue)
	{
		XF::Utilities::FromString<double>(outValue, inValue, std::dec);
	}

	PropertySerializer::PropertyTypeId PropertySerializer::GetTypeId(const double &value)
	{
		return PropertySerializer::TYPE_DOUBLE;
	}

	std::string PropertySerializer::ToString(const bool &value)
	{
		return XF::Utilities::ToString(value);
	}

	void PropertySerializer::FromString(const std::string &inValue, bool &outValue)
	{
		outValue = (inValue == "true" ? true : false);
	}

	PropertySerializer::PropertyTypeId PropertySerializer::GetTypeId(const bool &value)
	{
		return PropertySerializer::TYPE_BOOL;
	}


	std::string PropertySerializer::ToString(const std::string &value)
	{
		return value;
	}

	void PropertySerializer::FromString(const std::string &inValue, std::string &outValue)
	{
		outValue = inValue;
	}

	PropertySerializer::PropertyTypeId PropertySerializer::GetTypeId(const std::string &value)
	{
		return PropertySerializer::TYPE_STRING;
	}

	std::string PropertySerializer::ToString(const XF::Vector3 &value)
	{
		return XF::Utilities::ToString(value);
	}

	void PropertySerializer::FromString(const std::string &inValue, XF::Vector3 &outValue)
	{
		outValue = XF::Utilities::FromString(inValue);
	}

	PropertySerializer::PropertyTypeId PropertySerializer::GetTypeId(const XF::Vector3 &value)
	{
		return PropertySerializer::TYPE_VECTOR3;
	}
	//--
	std::string PropertySerializer::ToString(const Ogre::Quaternion &value)
	{
		return "{" + XF::Utilities::ToString(value.x) + ", " + XF::Utilities::ToString(value.y) + ", " + XF::Utilities::ToString(value.z) + ", " + XF::Utilities::ToString(value.w) + "}";   
	}

	void PropertySerializer::FromString(const std::string &inValue, Ogre::Quaternion &outValue)
	{
		// remove { }
		std::string stripped = inValue.substr(1, inValue.size() - 2);

		std::vector<std::string> values;
		XF::Utilities::TokenizeString(stripped, values, ", ");
		
		XF::Utilities::FromString<float>(outValue.x, values[0], std::dec);
		XF::Utilities::FromString<float>(outValue.y, values[1], std::dec);
		XF::Utilities::FromString<float>(outValue.z, values[2], std::dec);
		XF::Utilities::FromString<float>(outValue.w, values[3], std::dec);
	}

	PropertySerializer::PropertyTypeId PropertySerializer::GetTypeId(const Ogre::Quaternion &value)
	{
		return PropertySerializer::TYPE_OGRE_QUATERNION;
	}

	std::string PropertySerializer::ToString(const void* value)
	{
		return "";
	}

	void PropertySerializer::FromString(const void* inValue, void* outValue)
	{
		outValue = NULL;
	}

	PropertySerializer::PropertyTypeId PropertySerializer::GetTypeId(const void* value)
	{
		return PropertySerializer::TYPE_VOID_PTR;
	}
}
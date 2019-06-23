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

#include "XDelegates/Event.h"
#include "IProperty.h"
#include "PropertySerializer.h"

namespace XF
{
	template<class T>
	class PropertyData
	{
	public:
		T mValue;
		std::string mName;

		XF::Event2<void, T, T> mOnValueChanged;
	};

	template<class T>
	class Prop: public IProperty
	{
	private:
		PropertyData<T> mData;

	public:
		Prop(){}
		Prop(const Prop& copy) : mData(copy.mData){}
		Prop(const std::string &name)
		{
			mData.mName = name;
		}

		virtual ~Prop(){}

		XF::Event2<void, T, T> &OnValueChanged(){return mData.mOnValueChanged;}

		void Set(const T& value)
		{
			if(mData.mValue != value)
			{
				T oldValue = mData.mValue;
				mData.mValue = value;

				mData.mOnValueChanged.Invoke(oldValue, mData.mValue);
			}
		}

		const T& Get() const {return mData.mValue;}

		std::string Name()
		{
			return mData.mName;
		}

		std::string ToString()
		{
			return PropertySerializer::ToString(mData.mValue);
		}

		void SetFromString(std::string value)
		{
			return PropertySerializer::FromString(value, mData.mValue);
		}

		int TypeID()
		{
			return PropertySerializer::GetTypeId(mData.mValue);
		}

		Prop<T> operator= (const Prop<T> &rhs);
		Prop<T> operator= (const T& rhs);

		Prop<T> operator+= (const Prop<T> &rhs);
		Prop<T> operator+= (const T& rhs);

		Prop<T> operator-= (const Prop<T> &rhs);
		Prop<T> operator-= (const T& rhs);

		bool operator== (const Prop<T> &rhs);
		bool operator== (const T& rhs);

		bool operator!= (const Prop<T> &rhs);
		bool operator!= (const T& rhs);

		bool operator> (const Prop<T> &rhs);
		bool operator> (const T& rhs);

		bool operator< (const Prop<T> &rhs);
		bool operator< (const T& rhs);

		operator T() const {return mData.mValue;}
	};

	template<class T>
	inline Prop<T> Prop<T>::operator =(const Prop<T> &rhs)
	{
		mData = rhs.mData;
		return *this;
	}

	template<class T>
	inline Prop<T> Prop<T>::operator =(const T& rhs)
	{
		Set(rhs);
		return *this;
	}


	template<class T>
	inline Prop<T> Prop<T>::operator +=(const Prop<T> &rhs)
	{
		Set(mData.mValue + rhs.mData.mValue);
		return *this;
	}

	template<class T>
	inline Prop<T> Prop<T>::operator+= (const T& rhs)
	{
		Set(mData.mValue + rhs);
		return *this;
	}


	template<class T>
	inline Prop<T> Prop<T>::operator-= (const Prop<T> &rhs)
	{
		Set(mData.mValue - rhs.mData.mValue);
		return *this;
	}

	template<class T>
	inline Prop<T> Prop<T>::operator-= (const T& rhs)
	{
		Set(mData.mValue - rhs); //chris (slight fail Set(mData.mValue + rhs) =]
		return *this;
	}


	template<class T>
	inline bool Prop<T>::operator== (const Prop<T> &rhs)
	{
		return mData == rhs.mData;
	}

	template<class T>
	inline bool Prop<T>::operator== (const T& rhs)
	{
		return (mData.mValue == rhs);
	}


	template<class T>
	inline bool Prop<T>::operator!= (const Prop<T> &rhs)
	{
		return mData != rhs.mData;	
	}

	template<class T>
	inline bool Prop<T>::operator!= (const T& rhs)
	{
		return (mData.mValue != rhs);
	}


	template<class T>
	inline bool Prop<T>::operator> (const Prop<T> &rhs)
	{
		return (mData.mValue > rhs.mData.mValue);
	}

	template<class T>
	inline bool Prop<T>::operator> (const T& rhs)
	{
		return (mData.mValue > rhs);
	}


	template<class T>
	inline bool Prop<T>::operator< (const Prop<T> &rhs)
	{
		return (mData.mValue < rhs.mData.mValue);
	}

	template<class T>
	inline bool Prop<T>::operator< (const T& rhs)
	{
		return (mData.mValue < rhs);
	}
}
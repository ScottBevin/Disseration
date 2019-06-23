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
#include "Delegate.h"

#include <vector>
#include <algorithm>

#define XF_EVENT_JOIN_MACRO( X, Y) XF_EVENT_DO_JOIN( X, Y )
#define XF_EVENT_DO_JOIN( X, Y ) XF_EVENT_DO_JOIN2(X,Y)
#define XF_EVENT_DO_JOIN2( X, Y ) X##Y

#if XF_EVENT_PARAM_COUNT > 0
#define XF_EVENT_SEPARATOR ,
#else
#define XF_EVENT_SEPARATOR
#endif

#define XF_EVENT_CLASS_NAME XF_EVENT_JOIN_MACRO(Event,XF_EVENT_PARAM_COUNT)
#define XF_EVENT_DELEGATE_CLASS_NAME XF_EVENT_JOIN_MACRO(Delegate,XF_EVENT_PARAM_COUNT)

namespace XF
{
	template <typename R XF_EVENT_SEPARATOR XF_EVENT_TEMPLATE_PARAMS>
	class XF_EVENT_CLASS_NAME
	{
	public:
		typedef XF_EVENT_DELEGATE_CLASS_NAME<R XF_EVENT_SEPARATOR XF_EVENT_TEMPLATE_PARAMS> DelegateType;

	private:
		std::vector< DelegateType > mFunctions;

	public:
		XF_EVENT_CLASS_NAME(){}

		void RegisterDelegate(DelegateType del)
		{
			if(!Exists(del))
				mFunctions.push_back(del);
		}

		template <class T, R (T::*TMethod)(XF_EVENT_TEMPLATE_ARGS)>
		void RegisterMethod(T* obj)
		{
			DelegateType del = FromMethod<T, TMethod>(obj);

			if(!Exists(del))
				mFunctions.push_back(del);
		}

		template <class T, R (T::*TMethod)(XF_EVENT_TEMPLATE_ARGS) const>
		void RegisterConstMethod(T const* obj)
		{
			DelegateType del = FromConstMethod<T, TMethod>(obj);

			if(!Exists(del))
				mFunctions.push_back(del);
		}
	
		template <R (*TMethod)(XF_EVENT_TEMPLATE_ARGS)>
		void RegisterFunction()
		{
			DelegateType del = FromFunction<TMethod>();

			if(!Exists(del))
				mFunctions.push_back(del);
		}


		void UnregisterDelegate(DelegateType del)
		{
			Remove(del);
		}

		template <class T, R (T::*TMethod)(XF_EVENT_TEMPLATE_ARGS)>
		void UnregisterMethod(T* obj)
		{
			DelegateType del = FromMethod<T, TMethod>(obj);

			Remove(del);
		}

		template <class T, R (T::*TMethod)(XF_EVENT_TEMPLATE_ARGS) const>
		void UnregisterConstMethod(T const* obj)
		{
			DelegateType del = FromConstMethod<T, TMethod>(obj);

			Remove(del);
		}
	
		template <R (*TMethod)(XF_EVENT_TEMPLATE_ARGS)>
		void UnregisterFunction()
		{
			DelegateType del = FromFunction<TMethod>();

			Remove(del);
		}


		void Invoke(XF_EVENT_PARAMS)
		{
			for(unsigned int i = 0; i < mFunctions.size(); ++i)
			{
				mFunctions[i](XF_EVENT_ARGS);
			}
		}

		bool Exists(DelegateType &del)
		{
			for(unsigned int i = 0; i < mFunctions.size(); ++i)
			{
				if(mFunctions[i] == del)
					return true;
			}

			return false;
		}

		void Remove(DelegateType &del)
		{
			for(unsigned int i = 0; i < mFunctions.size(); ++i)
			{
				if(mFunctions[i] == del)
				{
					mFunctions[i] = mFunctions[mFunctions.size() - 1];
					mFunctions.pop_back();

					return;
				}
			}
		}

		template <class T, R (T::*TMethod)(XF_EVENT_TEMPLATE_ARGS)>
		static DelegateType FromMethod(T* obj)
		{
			return DelegateType::FromMethod<T, TMethod>(obj);
		}

		template <R (*TMethod)(XF_EVENT_TEMPLATE_ARGS)>
		static DelegateType FromFunction()
		{
			return DelegateType::FromFunction<TMethod>();
		}

		template <class T, R (T::*TMethod)(XF_EVENT_TEMPLATE_ARGS) const>
		static DelegateType FromConstMethod(T const* obj)
		{
			return DelegateType::FromConstMethod<T, TMethod>(obj);
		}
	};
}

#undef XF_EVENT_DELEGATE_CLASS_NAME
#undef XF_EVENT_CLASS_NAME
#undef XF_EVENT_SEPARATOR
#undef XF_EVENT_JOIN_MACRO
#undef XF_EVENT_DO_JOIN
#undef XF_EVENT_DO_JOIN2
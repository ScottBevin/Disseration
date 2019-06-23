/**************************************************************************************************
 *
 *	XCS Crowd Simulation Software
 *	Author: Scott Bevin
 *	Web: www.gameXcore.co.uk
 *	Email: scott@gameXcore.co.uk
 *	All code (c)2008 Scott Bevin, all rights reserved.
 *	
 *	This software is part of the disssertation work entitled "Real time crowd simulations incorporating individual
 *	agent personality models and group interactions" submitted to The University of Bolton by Scott Bevin in partial
 *	fulfilment of the requirements of The University of Bolton for the degree of Bachelor of Science in May 2010.
 *	
 **************************************************************************************************/

#include "Delegate.h"

#include <vector>
#include <algorithm>

#define XCS_EVENT_JOIN_MACRO( X, Y) XCS_EVENT_DO_JOIN( X, Y )
#define XCS_EVENT_DO_JOIN( X, Y ) XCS_EVENT_DO_JOIN2(X,Y)
#define XCS_EVENT_DO_JOIN2( X, Y ) X##Y

#if XCS_EVENT_PARAM_COUNT > 0
#define XCS_EVENT_SEPARATOR ,
#else
#define XCS_EVENT_SEPARATOR
#endif

#define XCS_EVENT_CLASS_NAME XCS_EVENT_JOIN_MACRO(Event,XCS_EVENT_PARAM_COUNT)
#define XCS_EVENT_DELEGATE_CLASS_NAME XCS_EVENT_JOIN_MACRO(Delegate,XCS_EVENT_PARAM_COUNT)

namespace XCS
{
	template <typename R XCS_EVENT_SEPARATOR XCS_EVENT_TEMPLATE_PARAMS>
	class XCS_EVENT_CLASS_NAME
	{
	public:
		typedef XCS_EVENT_DELEGATE_CLASS_NAME<R XCS_EVENT_SEPARATOR XCS_EVENT_TEMPLATE_PARAMS> DelegateType;

	private:
		std::vector< DelegateType > mFunctions;

	public:
		XCS_EVENT_CLASS_NAME(){}

		void RegisterDelegate(DelegateType del)
		{
			if(!Exists(del))
				mFunctions.push_back(del);
		}

		template <class T, R (T::*TMethod)(XCS_EVENT_TEMPLATE_ARGS)>
		void RegisterMethod(T* obj)
		{
			DelegateType del = FromMethod<T, TMethod>(obj);

			if(!Exists(del))
				mFunctions.push_back(del);
		}

		template <class T, R (T::*TMethod)(XCS_EVENT_TEMPLATE_ARGS) const>
		void RegisterConstMethod(T const* obj)
		{
			DelegateType del = FromConstMethod<T, TMethod>(obj);

			if(!Exists(del))
				mFunctions.push_back(del);
		}
	
		template <R (*TMethod)(XCS_EVENT_TEMPLATE_ARGS)>
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

		template <class T, R (T::*TMethod)(XCS_EVENT_TEMPLATE_ARGS)>
		void UnregisterMethod(T* obj)
		{
			DelegateType del = FromMethod<T, TMethod>(obj);

			Remove(del);
		}

		template <class T, R (T::*TMethod)(XCS_EVENT_TEMPLATE_ARGS) const>
		void UnregisterConstMethod(T const* obj)
		{
			DelegateType del = FromConstMethod<T, TMethod>(obj);

			Remove(del);
		}
	
		template <R (*TMethod)(XCS_EVENT_TEMPLATE_ARGS)>
		void UnregisterFunction()
		{
			DelegateType del = FromFunction<TMethod>();

			Remove(del);
		}


		void Invoke(XCS_EVENT_PARAMS)
		{
			for(unsigned int i = 0; i < mFunctions.size(); ++i)
			{
				mFunctions[i](XCS_EVENT_ARGS);
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

		template <class T, R (T::*TMethod)(XCS_EVENT_TEMPLATE_ARGS)>
		static DelegateType FromMethod(T* obj)
		{
			return DelegateType::FromMethod<T, TMethod>(obj);
		}

		template <R (*TMethod)(XCS_EVENT_TEMPLATE_ARGS)>
		static DelegateType FromFunction()
		{
			return DelegateType::FromFunction<TMethod>();
		}

		template <class T, R (T::*TMethod)(XCS_EVENT_TEMPLATE_ARGS) const>
		static DelegateType FromConstMethod(T const* obj)
		{
			return DelegateType::FromConstMethod<T, TMethod>(obj);
		}
	};
}

#undef XCS_EVENT_DELEGATE_CLASS_NAME
#undef XCS_EVENT_CLASS_NAME
#undef XCS_EVENT_SEPARATOR
#undef XCS_EVENT_JOIN_MACRO
#undef XCS_EVENT_DO_JOIN
#undef XCS_EVENT_DO_JOIN2

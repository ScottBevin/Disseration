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
 * ORIGINAL CODE:
 *
 *		(c) Sergey Ryazanov (http://home.onego.ru/~ryazanov)
 *
 *		Template file. May be included many times with different predefined macros.
 **************************************************************************************************/

#if XCS_DELEGATE_PARAM_COUNT > 0
#define XCS_DELEGATE_SEPARATOR ,
#else
#define XCS_DELEGATE_SEPARATOR
#endif

// see BOOST_JOIN for explanation
#define XCS_DELEGATE_JOIN_MACRO( X, Y) XCS_DELEGATE_DO_JOIN( X, Y )
#define XCS_DELEGATE_DO_JOIN( X, Y ) XCS_DELEGATE_DO_JOIN2(X,Y)
#define XCS_DELEGATE_DO_JOIN2( X, Y ) X##Y

namespace XCS
{
#ifdef XCS_DELEGATE_PREFERRED_SYNTAX
#define XCS_DELEGATE_CLASS_NAME Delegate
#define XCS_DELEGATE_INVOKER_CLASS_NAME delegate_invoker
#else
#define XCS_DELEGATE_CLASS_NAME XCS_DELEGATE_JOIN_MACRO(Delegate,XCS_DELEGATE_PARAM_COUNT)
#define XCS_DELEGATE_INVOKER_CLASS_NAME XCS_DELEGATE_JOIN_MACRO(delegate_invoker,XCS_DELEGATE_PARAM_COUNT)
	template <typename R XCS_DELEGATE_SEPARATOR XCS_DELEGATE_TEMPLATE_PARAMS>
	class XCS_DELEGATE_INVOKER_CLASS_NAME;
#endif

	template <typename R XCS_DELEGATE_SEPARATOR XCS_DELEGATE_TEMPLATE_PARAMS>
#ifdef XCS_DELEGATE_PREFERRED_SYNTAX
	class XCS_DELEGATE_CLASS_NAME<R (XCS_DELEGATE_TEMPLATE_ARGS)>
#else
	class XCS_DELEGATE_CLASS_NAME
#endif
	{
	public:
		typedef R ReturnType;
#ifdef XCS_DELEGATE_PREFERRED_SYNTAX
		typedef ReturnType (XCS_DELEGATE_CALLTYPE *signature_type)(XCS_DELEGATE_TEMPLATE_ARGS);
		typedef XCS_DELEGATE_INVOKER_CLASS_NAME<signature_type> InvokerType;
#else
		typedef XCS_DELEGATE_INVOKER_CLASS_NAME<R XCS_DELEGATE_SEPARATOR XCS_DELEGATE_TEMPLATE_ARGS> InvokerType;
#endif

		XCS_DELEGATE_CLASS_NAME()
			: mObjectPtr(0)
			, mStubPtr(0)
		{}

		template <ReturnType (*TMethod)(XCS_DELEGATE_TEMPLATE_ARGS)>
		static XCS_DELEGATE_CLASS_NAME FromFunction()
		{
			return FromStub(0, &FunctionStub<TMethod>);
		}

		template <class T, ReturnType (T::*TMethod)(XCS_DELEGATE_TEMPLATE_ARGS)>
		static XCS_DELEGATE_CLASS_NAME FromMethod(T* mObjectPtr)
		{
			return FromStub(mObjectPtr, &MethodStub<T, TMethod>);
		}
		

		template <class T, ReturnType (T::*TMethod)(XCS_DELEGATE_TEMPLATE_ARGS) const>
		static XCS_DELEGATE_CLASS_NAME FromConstMethod(T const* mObjectPtr)
		{
			return FromStub(const_cast<T*>(mObjectPtr), &ConstMethodStub<T, TMethod>);
		}

		ReturnType operator()(XCS_DELEGATE_PARAMS) const
		{
			return (*mStubPtr)(mObjectPtr XCS_DELEGATE_SEPARATOR XCS_DELEGATE_ARGS);
		}

		operator bool () const
		{
			return mStubPtr != 0;
		}

		bool operator!() const
		{
			return !(operator bool());
		}

		bool operator== (const XCS_DELEGATE_CLASS_NAME<R XCS_DELEGATE_SEPARATOR XCS_DELEGATE_TEMPLATE_ARGS> &rhs) const
		{
			return(mStubPtr == rhs.mStubPtr && mObjectPtr == rhs.mObjectPtr);
		}

		bool operator!= (const XCS_DELEGATE_CLASS_NAME<R XCS_DELEGATE_SEPARATOR XCS_DELEGATE_TEMPLATE_ARGS> &rhs) const
		{
			return(mStubPtr != rhs.mStubPtr && mObjectPtr != rhs.mObjectPtr);
		}

	private:
		
		typedef ReturnType (XCS_DELEGATE_CALLTYPE *StubType)(void* mObjectPtr XCS_DELEGATE_SEPARATOR XCS_DELEGATE_PARAMS);

		void* mObjectPtr;
		StubType mStubPtr;

		static XCS_DELEGATE_CLASS_NAME FromStub(void* mObjectPtr, StubType mStubPtr)
		{
			XCS_DELEGATE_CLASS_NAME d;
			d.mObjectPtr = mObjectPtr;
			d.mStubPtr = mStubPtr;
			return d;
		}

		template <ReturnType (*TMethod)(XCS_DELEGATE_TEMPLATE_ARGS)>
		static ReturnType XCS_DELEGATE_CALLTYPE FunctionStub(void* XCS_DELEGATE_SEPARATOR XCS_DELEGATE_PARAMS)
		{
			return (TMethod)(XCS_DELEGATE_ARGS);
		}

		template <class T, ReturnType (T::*TMethod)(XCS_DELEGATE_TEMPLATE_ARGS)>
		static ReturnType XCS_DELEGATE_CALLTYPE MethodStub(void* mObjectPtr XCS_DELEGATE_SEPARATOR XCS_DELEGATE_PARAMS)
		{
			T* p = static_cast<T*>(mObjectPtr);
			return (p->*TMethod)(XCS_DELEGATE_ARGS);
		}

		template <class T, ReturnType (T::*TMethod)(XCS_DELEGATE_TEMPLATE_ARGS) const>
		static ReturnType XCS_DELEGATE_CALLTYPE ConstMethodStub(void* mObjectPtr XCS_DELEGATE_SEPARATOR XCS_DELEGATE_PARAMS)
		{
			T const* p = static_cast<T*>(mObjectPtr);
			return (p->*TMethod)(XCS_DELEGATE_ARGS);
		}
	};

	template <typename R XCS_DELEGATE_SEPARATOR XCS_DELEGATE_TEMPLATE_PARAMS>
#ifdef XCS_DELEGATE_PREFERRED_SYNTAX
	class XCS_DELEGATE_INVOKER_CLASS_NAME<R (XCS_DELEGATE_TEMPLATE_ARGS)>
#else
	class XCS_DELEGATE_INVOKER_CLASS_NAME
#endif
	{
		XCS_DELEGATE_INVOKER_DATA

	public:
		XCS_DELEGATE_INVOKER_CLASS_NAME(XCS_DELEGATE_PARAMS)
#if XCS_DELEGATE_PARAM_COUNT > 0
			:
#endif
			XCS_DELEGATE_INVOKER_INITIALIZATION_LIST
		{
		}

		template <class TDelegate>
		R operator()(TDelegate d) const
		{
			return d(XCS_DELEGATE_ARGS);
		}
	};
}

#undef XCS_DELEGATE_CLASS_NAME
#undef XCS_DELEGATE_SEPARATOR
#undef XCS_DELEGATE_JOIN_MACRO
#undef XCS_DELEGATE_DO_JOIN
#undef XCS_DELEGATE_DO_JOIN2

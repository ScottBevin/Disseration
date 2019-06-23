/************************************************************************/
/*    Author: Scott Bevin                                               */
/************************************************************************/

/*
ORIGINAL CODE:

	(c) Sergey Ryazanov (http://home.onego.ru/~ryazanov)

	Template file. May be included many times with different predefined macros.
*/

/************************************************************************/
/*    Author: Scott Bevin   
		Modified impossibly fast delegates to both conform with XF namespacing and naming conventions, and also
	    implemented Delegate comparisons, this was described as Sergey as "Impossible" but nothing is impossible
		when you need it=]                                              */
/************************************************************************/

#if XF_DELEGATE_PARAM_COUNT > 0
#define XF_DELEGATE_SEPARATOR ,
#else
#define XF_DELEGATE_SEPARATOR
#endif

// see BOOST_JOIN for explanation
#define XF_DELEGATE_JOIN_MACRO( X, Y) XF_DELEGATE_DO_JOIN( X, Y )
#define XF_DELEGATE_DO_JOIN( X, Y ) XF_DELEGATE_DO_JOIN2(X,Y)
#define XF_DELEGATE_DO_JOIN2( X, Y ) X##Y

namespace XF
{
#ifdef XF_DELEGATE_PREFERRED_SYNTAX
#define XF_DELEGATE_CLASS_NAME Delegate
#define XF_DELEGATE_INVOKER_CLASS_NAME delegate_invoker
#else
#define XF_DELEGATE_CLASS_NAME XF_DELEGATE_JOIN_MACRO(Delegate,XF_DELEGATE_PARAM_COUNT)
#define XF_DELEGATE_INVOKER_CLASS_NAME XF_DELEGATE_JOIN_MACRO(delegate_invoker,XF_DELEGATE_PARAM_COUNT)
	template <typename R XF_DELEGATE_SEPARATOR XF_DELEGATE_TEMPLATE_PARAMS>
	class XF_DELEGATE_INVOKER_CLASS_NAME;
#endif

	template <typename R XF_DELEGATE_SEPARATOR XF_DELEGATE_TEMPLATE_PARAMS>
#ifdef XF_DELEGATE_PREFERRED_SYNTAX
	class XF_DELEGATE_CLASS_NAME<R (XF_DELEGATE_TEMPLATE_ARGS)>
#else
	class XF_DELEGATE_CLASS_NAME
#endif
	{
	public:
		typedef R ReturnType;
#ifdef XF_DELEGATE_PREFERRED_SYNTAX
		typedef ReturnType (XF_DELEGATE_CALLTYPE *signature_type)(XF_DELEGATE_TEMPLATE_ARGS);
		typedef XF_DELEGATE_INVOKER_CLASS_NAME<signature_type> InvokerType;
#else
		typedef XF_DELEGATE_INVOKER_CLASS_NAME<R XF_DELEGATE_SEPARATOR XF_DELEGATE_TEMPLATE_ARGS> InvokerType;
#endif

		XF_DELEGATE_CLASS_NAME()
			: mObjectPtr(0)
			, mStubPtr(0)
		{}

		template <ReturnType (*TMethod)(XF_DELEGATE_TEMPLATE_ARGS)>
		static XF_DELEGATE_CLASS_NAME FromFunction()
		{
			return FromStub(0, &FunctionStub<TMethod>);
		}

		template <class T, ReturnType (T::*TMethod)(XF_DELEGATE_TEMPLATE_ARGS)>
		static XF_DELEGATE_CLASS_NAME FromMethod(T* mObjectPtr)
		{
			return FromStub(mObjectPtr, &MethodStub<T, TMethod>);
		}
		

		template <class T, ReturnType (T::*TMethod)(XF_DELEGATE_TEMPLATE_ARGS) const>
		static XF_DELEGATE_CLASS_NAME FromConstMethod(T const* mObjectPtr)
		{
			return FromStub(const_cast<T*>(mObjectPtr), &ConstMethodStub<T, TMethod>);
		}

		ReturnType operator()(XF_DELEGATE_PARAMS) const
		{
			return (*mStubPtr)(mObjectPtr XF_DELEGATE_SEPARATOR XF_DELEGATE_ARGS);
		}

		operator bool () const
		{
			return mStubPtr != 0;
		}

		bool operator!() const
		{
			return !(operator bool());
		}

		bool operator== (const XF_DELEGATE_CLASS_NAME<R XF_DELEGATE_SEPARATOR XF_DELEGATE_TEMPLATE_ARGS> &rhs) const
		{
			return(mStubPtr == rhs.mStubPtr && mObjectPtr == rhs.mObjectPtr);
		}

		bool operator!= (const XF_DELEGATE_CLASS_NAME<R XF_DELEGATE_SEPARATOR XF_DELEGATE_TEMPLATE_ARGS> &rhs) const
		{
			return(mStubPtr != rhs.mStubPtr && mObjectPtr != rhs.mObjectPtr);
		}

	private:
		
		typedef ReturnType (XF_DELEGATE_CALLTYPE *StubType)(void* mObjectPtr XF_DELEGATE_SEPARATOR XF_DELEGATE_PARAMS);

		void* mObjectPtr;
		StubType mStubPtr;

		static XF_DELEGATE_CLASS_NAME FromStub(void* mObjectPtr, StubType mStubPtr)
		{
			XF_DELEGATE_CLASS_NAME d;
			d.mObjectPtr = mObjectPtr;
			d.mStubPtr = mStubPtr;
			return d;
		}

		template <ReturnType (*TMethod)(XF_DELEGATE_TEMPLATE_ARGS)>
		static ReturnType XF_DELEGATE_CALLTYPE FunctionStub(void* XF_DELEGATE_SEPARATOR XF_DELEGATE_PARAMS)
		{
			return (TMethod)(XF_DELEGATE_ARGS);
		}

		template <class T, ReturnType (T::*TMethod)(XF_DELEGATE_TEMPLATE_ARGS)>
		static ReturnType XF_DELEGATE_CALLTYPE MethodStub(void* mObjectPtr XF_DELEGATE_SEPARATOR XF_DELEGATE_PARAMS)
		{
			T* p = static_cast<T*>(mObjectPtr);
			return (p->*TMethod)(XF_DELEGATE_ARGS);
		}

		template <class T, ReturnType (T::*TMethod)(XF_DELEGATE_TEMPLATE_ARGS) const>
		static ReturnType XF_DELEGATE_CALLTYPE ConstMethodStub(void* mObjectPtr XF_DELEGATE_SEPARATOR XF_DELEGATE_PARAMS)
		{
			T const* p = static_cast<T*>(mObjectPtr);
			return (p->*TMethod)(XF_DELEGATE_ARGS);
		}
	};

	template <typename R XF_DELEGATE_SEPARATOR XF_DELEGATE_TEMPLATE_PARAMS>
#ifdef XF_DELEGATE_PREFERRED_SYNTAX
	class XF_DELEGATE_INVOKER_CLASS_NAME<R (XF_DELEGATE_TEMPLATE_ARGS)>
#else
	class XF_DELEGATE_INVOKER_CLASS_NAME
#endif
	{
		XF_DELEGATE_INVOKER_DATA

	public:
		XF_DELEGATE_INVOKER_CLASS_NAME(XF_DELEGATE_PARAMS)
#if XF_DELEGATE_PARAM_COUNT > 0
			:
#endif
			XF_DELEGATE_INVOKER_INITIALIZATION_LIST
		{
		}

		template <class TDelegate>
		R operator()(TDelegate d) const
		{
			return d(XF_DELEGATE_ARGS);
		}
	};
}

#undef XF_DELEGATE_CLASS_NAME
#undef XF_DELEGATE_SEPARATOR
#undef XF_DELEGATE_JOIN_MACRO
#undef XF_DELEGATE_DO_JOIN
#undef XF_DELEGATE_DO_JOIN2

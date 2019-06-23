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

#ifndef XCS_DELEGATE_INCLUDED
#define XCS_DELEGATE_INCLUDED

namespace XCS
{
#ifdef XCS_DELEGATE_PREFERRED_SYNTAX
	template <typename TSignature> class delegate;
	template <typename TSignature> class delegate_invoker;
#endif
}

#ifdef _MSC_VER
#define XCS_DELEGATE_CALLTYPE __fastcall
#else
#define XCS_DELEGATE_CALLTYPE
#endif

#include "DelegateList.h"

#undef XCS_DELEGATE_CALLTYPE

#endif//XCS_DELEGATE_INCLUDED

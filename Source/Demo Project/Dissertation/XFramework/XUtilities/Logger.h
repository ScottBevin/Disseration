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
#include <Ogre.h>
#include "XPropertys/Property.h"
#include "XSystemComponents/XDebugging/debug.h"

namespace XF
{
	class Logger : public DebugDisplay
	{
	private:
		bool mLogToFile, mLogToCout;
		static Logger *mInstance;
		std::ofstream mFile;

		Logger(){}

		bool OpenFile(std::string file);
		void CloseFile();

		void PrintMessageToFile(std::string message);

	public:
		static Logger *GetInstance();
		~Logger(){}

		XF::Event1<void, std::string> OnLogMessage;

		void EnableLogToFile(std::string file){ mLogToFile = OpenFile(file); }

		void Log(std::string message);

		void ShutDown(){CloseFile();}
	};

	
}

#define XF_LOGGER XF::Logger::GetInstance()

#define COMMENT SLASH(/)
#define SLASH(s) /##s

#if defined _DEBUG
#define DLOG XF::DebugMessage(XF_LOGGER) 
#else
#define DLOG COMMENT
#endif

#if defined XF_FULL_LOGGING
#define LOG XF::DebugMessage(XF_LOGGER) 
#define ILOG XF::DebugMessage(XF_LOGGER)
#define SSELOG XF::DebugMessage(XF_LOGGER)
#elif defined XF_IMPORTANT_MESSAGE_LOGGING
#define LOG COMMENT
#define ILOG XF::DebugMessage(XF_LOGGER)
#define SSELOG XF::DebugMessage(XF_LOGGER)
#else
#define LOG COMMENT 
#define ILOG COMMENT
#define SSELOG XF::DebugMessage(XF_LOGGER)
#endif




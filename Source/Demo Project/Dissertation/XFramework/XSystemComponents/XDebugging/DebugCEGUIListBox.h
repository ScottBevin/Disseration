/************************************************************
Author Christopher John Pepper [2/20/2010]
**************************************************************/
#include <string>

#include "CEGUI/CEGUIWindowManager.h"
#include <CEGUI/CEGUI.h>
#include "debug.h"


#pragma once

namespace XF
{
	class DebugCEGUIListBox : public XF::DebugDisplay
	{
	private:
		bool mInverted;
		bool mEnabled;
		int mKeepThreshold;
			
		
		CEGUI::Listbox* mDebugBox; 
		
	public:
		
		void Log(std::string message);

		

		//static DebugCEGUIListBox& GetQuickInstance(){return *quickInstance;}
		

		void ReInitialize(CEGUI::Listbox* box){mDebugBox = box;};
			
		/*How many items will the debug store in the debug listbox (please note that the app
		will slow down at high numbers)*/
		void SetKeepThreshold(float number){mKeepThreshold=number;};

			DebugCEGUIListBox(CEGUI::Listbox* box, int keepThreshold = 300, bool inverted = false) : 
								mDebugBox(box),mInverted(inverted) , mEnabled(true) , mKeepThreshold(keepThreshold){};

			~DebugCEGUIListBox();
	};
}




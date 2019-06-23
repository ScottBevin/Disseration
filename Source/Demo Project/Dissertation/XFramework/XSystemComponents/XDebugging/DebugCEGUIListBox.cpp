/************************************************************
Author Christopher John Pepper [2/20/2010]
**************************************************************/
#include "DebugCEGUIListBox.h"
#include "OgreVector3.h"
#include "XFramework.h"
namespace XF
{
	void DebugCEGUIListBox::Log( std::string output )
	{

		if(mDebugBox && mEnabled)
		{
			if((int)mDebugBox->getItemCount() > mKeepThreshold)
			{
				if(mInverted)
				{
					mDebugBox->getListboxItemFromIndex(mKeepThreshold)->setAutoDeleted(true);
					mDebugBox->removeItem(mDebugBox->getListboxItemFromIndex(mKeepThreshold));			
				}			
				else
				{
					mDebugBox->removeItem(0);
					mDebugBox->getListboxItemFromIndex(0)->setAutoDeleted(true);
					mDebugBox->removeItem(mDebugBox->getListboxItemFromIndex(0));
				}


			}

			CEGUI::ListboxTextItem* lItem = new CEGUI::ListboxTextItem(output);
			if(mInverted)
				mDebugBox->insertItem(lItem,0);
			else
				mDebugBox->addItem(lItem);


			mDebugBox->getVertScrollbar()->setScrollPosition(mDebugBox->getTotalItemsHeight());

		}


	}

	DebugCEGUIListBox::~DebugCEGUIListBox()
	{

	}



}

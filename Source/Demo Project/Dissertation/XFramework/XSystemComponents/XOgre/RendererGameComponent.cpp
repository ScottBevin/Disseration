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

#include "RendererGameComponent.h"

namespace XF
	{
	RendererGameComponent::RendererGameComponent(XF::Application *ownerApp, XF::OgreRenderer* renderer)
	: XF::SystemComponent(ownerApp), mRenderer(renderer)
	{
	}

	RendererGameComponent::~RendererGameComponent()
	{
	}

	void RendererGameComponent::OnInitialise()
	{
		this->SubscribeToMailingList(XF::MailingList::GRAPHICS_SUBSYSTEM);
	}

	bool RendererGameComponent::OnHandleMessage(const XF::Message &message)
	{
		if(message.Subject == XF::MessageSubject::PLEASE_SEND_RENDERER)
		{
			this->PostAMessage(
				message.SenderID,
				XF::MessageSubject::SENT_RENDERER,
				mRenderer);

			return true;
		}

		return false;
	}

	void RendererGameComponent::OnShutDown()
	{
		this->UnsubscribeFromMailingList(XF::MailingList::GRAPHICS_SUBSYSTEM);
	}
}
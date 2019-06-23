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

#include "QuickAndDirtyOgreDebugRenderer.h"

#include "MathHelper.h"
#include "Utilities.h"
#include "GlobalDefines.h"

namespace XF
{
	int QuickAndDirtyOgreDebugRenderer::smNextId = 0;
	QuickAndDirtyOgreDebugRenderer* QuickAndDirtyOgreDebugRenderer::mSingletonInstance = NULL;

	QuickAndDirtyOgreDebugRenderer* QuickAndDirtyOgreDebugRenderer::GetInstance()
	{
		assert(mSingletonInstance);
		
		return mSingletonInstance;
	}

	void QuickAndDirtyOgreDebugRenderer::InitializeStaticVersion(XF::OgreRenderer *renderer)
	{
		assert(!mSingletonInstance);
		mSingletonInstance = new QuickAndDirtyOgreDebugRenderer(renderer);
	}

	void QuickAndDirtyOgreDebugRenderer::KillStaticVersion()
	{
		delete mSingletonInstance;
		mSingletonInstance = NULL;
	}

	QuickAndDirtyOgreDebugRenderer::QuickAndDirtyOgreDebugRenderer(XF::OgreRenderer *renderer)
	{
		mId = ++smNextId;

		mRenderer = renderer;

		mNode = renderer->GetDefaultSceneManager()->getRootSceneNode()->createChildSceneNode("QuickAndDirtyOgreRendererNode" + XF::Utilities::ToString(mId));
		mManObj = renderer->GetDefaultSceneManager()->createManualObject("QuickAndDirtyOgreRendererManObj" + XF::Utilities::ToString(mId));
		mNode->attachObject(mManObj);

		mManObj->setQueryFlags(DONT_BE_SELECTED); // chris

		mMaterialString = "QuickAndDirtyOgreRendererMaterial" + XF::Utilities::ToString(mId);

		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(mMaterialString, "debugger");
		material->setReceiveShadows(false);
		material->getTechnique(0)->setLightingEnabled(false);

		mY = 0.0f;
	}

	QuickAndDirtyOgreDebugRenderer::~QuickAndDirtyOgreDebugRenderer()
	{
		//mRenderer->GetDefaultSceneManager()->getRootSceneNode()->removeAndDestroyChild("QuickAndDirtyOgreRendererNode" + XF::Utilities::ToString(mId));
	}

	void QuickAndDirtyOgreDebugRenderer::Clear()
	{
		mManObj->clear(); //chris
	}

	void QuickAndDirtyOgreDebugRenderer::Begin()
	{
		mManObj->clear();
		mManObj->begin(mMaterialString, Ogre::RenderOperation::OT_LINE_LIST);
	}

	void QuickAndDirtyOgreDebugRenderer::End()
	{
		mManObj->end();
	}

	void QuickAndDirtyOgreDebugRenderer::DrawLine( const XF::Vector3 &p1, const XF::Vector3 &p2 )
	{
		DrawLine( XF::Vector2(p1.x,p1.z), XF::Vector2(p2.x,p2.z) );
	}
	void QuickAndDirtyOgreDebugRenderer::DrawLine( const XF::Vector2 &p1, const XF::Vector2 &p2 )
	{
		mManObj->position(p1.x, mY, p1.y);
		mManObj->position(p2.x, mY, p2.y);
	}

	void QuickAndDirtyOgreDebugRenderer::DrawLineStrip( std::vector<XF::Vector2> &lines )
	{
		if(lines.size() < 2) return;

		for(int i = 0; i < (int)lines.size() - 1; ++i)
		{
			mManObj->position(lines[i].x, mY, lines[i].y);
			mManObj->position(lines[i + 1].x, mY, lines[i + 1].y);
		}
	}

	void QuickAndDirtyOgreDebugRenderer::DrawCircle( const XF::Vector3 &center, float radius, int smoothFactor /*= 16*/ )
	{

		DrawCircle( XF::Vector2(center.x,center.z),  radius,  smoothFactor );
	}
	void QuickAndDirtyOgreDebugRenderer::DrawCircle( const XF::Vector2 &center, float radius, int smoothFactor /*= 16*/ )
	{
		smoothFactor = XF::MathHelper::Max(smoothFactor, 8);

		float angleChange = XF::MathHelper::TwoPi() / smoothFactor;
		float angle = 0.0f;

		for(int i = 0; i < smoothFactor; ++i)
		{
			mManObj->position(
				center.x + (cos(angle) * radius),
				mY, 
				center.y + (sin(angle) * radius));

			mManObj->position(
				center.x + (cos(angle + angleChange) * radius),
				mY, 
				center.y + (sin(angle + angleChange) * radius));

			angle += angleChange;
		}
	}

	void QuickAndDirtyOgreDebugRenderer::DrawRect( float x1, float x2, float y1, float y2 )
	{
		mManObj->position(x1, mY, y1);
		mManObj->position(x2, mY, y1);

		mManObj->position(x2, mY, y1);
		mManObj->position(x2, mY, y2);

		mManObj->position(x2, mY, y2);
		mManObj->position(x1, mY, y2);

		mManObj->position(x1, mY, y2);
		mManObj->position(x1, mY, y1);
	}

	void QuickAndDirtyOgreDebugRenderer::Color( float r, float g, float b, float a /*= 1.0f*/ )
	{
		mColor = Ogre::Vector4(r, g, b, a);
		mManObj->colour(mColor.x, mColor.y, mColor.z, mColor.w);
	}

	void QuickAndDirtyOgreDebugRenderer::DrawCircleFilled( const XF::Vector2 &center, float radius, int smoothFactor /*= 16*/ )
	{
		smoothFactor = XF::MathHelper::Max(smoothFactor, 8);

		float angleChange = XF::MathHelper::TwoPi() / smoothFactor;
		float angle = 0.0f;

		mManObj->begin(mMaterialString, Ogre::RenderOperation::OT_TRIANGLE_FAN);
		mManObj->colour(mColor.x, mColor.y, mColor.z, mColor.w);

		mManObj->position(center.x, mY, center.y);

		for(int i = 0; i < smoothFactor; ++i)
		{
			mManObj->position(
				center.x + (cos(angle) * radius),
				mY, 
				center.y + (sin(angle) * radius));

			angle += angleChange;
		}

		mManObj->position(
			center.x + (cos(0.0f) * radius),
			mY, 
			center.y + (sin(0.0f) * radius));

		mManObj->end();	
	}

	void QuickAndDirtyOgreDebugRenderer::DrawRectFilled( float x1, float x2, float y1, float y2 )
	{
		mManObj->begin(mMaterialString, Ogre::RenderOperation::OT_TRIANGLE_STRIP);
		mManObj->colour(mColor.x, mColor.y, mColor.z, mColor.w);

		mManObj->position(x1, mY, y1);
		mManObj->position(x2, mY, y1);
		mManObj->position(x1, mY, y2);
		mManObj->position(x1, mY, y1);

		mManObj->end();
	}

	void QuickAndDirtyOgreDebugRenderer::DrawTriangle( const XF::Vector2 &p1, const XF::Vector2 &p2, const XF::Vector2 &p3 )
	{

		mManObj->position(p1.x, mY, p1.y);
		mManObj->position(p2.x, mY, p2.y);

		mManObj->position(p2.x, mY, p2.y);
		mManObj->position(p3.x, mY, p3.y);

		mManObj->position(p3.x, mY, p3.y);
		mManObj->position(p1.x, mY, p1.y);
	}

	void QuickAndDirtyOgreDebugRenderer::DrawTriangleFilled( const XF::Vector2 &p1, const XF::Vector2 &p2, const XF::Vector2 &p3 )
	{
		mManObj->begin(mMaterialString, Ogre::RenderOperation::OT_TRIANGLE_LIST);
		mManObj->colour(mColor.x, mColor.y, mColor.z, mColor.w);

		mManObj->position(p1.x, mY, p1.y);
		mManObj->position(p2.x, mY, p2.y);
		mManObj->position(p3.x, mY, p3.y);

		mManObj->end();
	}
}
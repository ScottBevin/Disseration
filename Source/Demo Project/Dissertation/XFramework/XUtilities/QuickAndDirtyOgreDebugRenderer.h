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

#include <Ogre.h>
#include <string>

#include "XSystemComponents\XOgre\OgreRenderer.h"
#include "../XFrameworkTypedefsAndDefines.h"
#include <vector>

namespace XF
{
	class QuickAndDirtyOgreDebugRenderer
	{
	private:
		int mId;
		static int smNextId;

		static QuickAndDirtyOgreDebugRenderer* mSingletonInstance;

		float mY;
		XF::OgreRenderer *mRenderer;

		Ogre::SceneNode* mNode;
		Ogre::ManualObject* mManObj;
		std::string mMaterialString;

		Ogre::Vector4 mColor;

		void DrawRectFilled(const XF::Vector2 &min, const XF::Vector2 &max){DrawRectFilled(min.x, max.x, min.y, max.y);}
		void DrawRectFilled(float x1, float x2, float y1, float y2);
		void DrawTriangleFilled(const XF::Vector2 &p1, const XF::Vector2 &p2, const XF::Vector2 &p3);
		void DrawCircleFilled(const XF::Vector2 &center, float radius, int smoothFactor = 16);	

	public:
		QuickAndDirtyOgreDebugRenderer(XF::OgreRenderer *renderer);
		~QuickAndDirtyOgreDebugRenderer();

		void Begin();
		void End();

		void DrawLine(const XF::Vector2 &p1, const XF::Vector2 &p2);
		void DrawLine( const XF::Vector3 &p1, const XF::Vector3 &p2 );//chris: for easier quick 3D postion circles
		void DrawLineStrip(std::vector<XF::Vector2> &lines);

		void DrawCircle(const XF::Vector2 &center, float radius, int smoothFactor = 16);
		void DrawCircle( const XF::Vector3 &center, float radius, int smoothFactor = 16 ); //chris: for easier quick 3D postion circles
		
		void DrawRect(const XF::Vector2 &min, const XF::Vector2 &max){DrawRect(min.x, max.x, min.y, max.y);}
		void DrawRect(float x1, float x2, float y1, float y2);

		void DrawTriangle(const XF::Vector2 &p1, const XF::Vector2 &p2, const XF::Vector2 &p3);

		void Color(const XF::Vector3 &color){Color(color.x, color.y, color.z);}
		void Color(const Ogre::Vector4 &color){Color(color.x, color.y, color.z, color.w);}
		void Color(float r, float g, float b, float a = 1.0f);

		float GetYPlane() const { return mY; }
		void SetYPlane(float val) { mY = val; }
		void Clear(); //chris

		static QuickAndDirtyOgreDebugRenderer* GetInstance();
		static void InitializeStaticVersion(XF::OgreRenderer *renderer);
		static void KillStaticVersion();
	};
}

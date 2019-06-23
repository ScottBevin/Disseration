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

#include "QuadTree.h"

namespace XF
{
	QuadTree::QuadTree(float x, float y, float w, float h, unsigned int targetObjects, unsigned int maxDepth)
	{
		mRootNode = new QuadTreeNode(NULL, 0, x, y, w, h);
		mRootNode->MaxDepth(maxDepth);
		mRootNode->TargetObjects(targetObjects);

		mManObjNode = NULL;

		mRenderingTree = false;
	}

	QuadTree::~QuadTree()
	{
		delete mRootNode;
	}

	void QuadTree::AddTreeToOgre()
	{
		if(mManObjNode)
		{
			mOgreManager->getRootSceneNode()->removeChild(mManObjNode);
			delete mManObjNode;
		}

		Ogre::ManualObject *manObj = mOgreManager->createManualObject("QuadTree");
		mManObjNode = mOgreManager->getRootSceneNode()->createChildSceneNode("QuadTree_Node");

		Ogre::MaterialPtr manObjMaterial = Ogre::MaterialManager::getSingleton().create("QuadTreeMaterial", "debugger");
		manObjMaterial->setReceiveShadows(false);
		manObjMaterial->getTechnique(0)->setLightingEnabled(true);
		manObjMaterial->getTechnique(0)->getPass(0)->setDiffuse(1, 1, 1, 1);
		manObjMaterial->getTechnique(0)->getPass(0)->setAmbient(0, 0, 1);
		manObjMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(0, 0, 1);

		manObj->begin("QuadTreeMaterial", Ogre::RenderOperation::OT_LINE_LIST);

		mRootNode->Render(manObj);

		manObj->end();

		mManObjNode->attachObject(manObj);
	}

	void QuadTree::Update(float dT)
	{
		mRootNode->CollideAllBodies();

		if(mRenderingTree && QuadTreeNode::TreeStructureChangedThisFrame())
		{
			AddTreeToOgre();

			QuadTreeNode::ResetTreeStructureChangedBool();
		}
	}
}
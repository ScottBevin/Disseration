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

#ifndef XFRAMEWORK_INCLUDE
#define XFRAMEWORK_INCLUDE

#include "XFrameworkTypedefsAndDefines.h"

// main
#include "Application.h"

//delegates
#include "Event.h"
#include "XDelegates/Delegate.h"

//entity components
#include "XEntityComponents/EntityComponentFactory.h"
#include "XEntityComponents/EntityComponentMaker.h"
#include "XEntityComponents/EntityComponent.h"

//Game entities
#include "XGameEntities/GameEntity.h"
#include "XGameEntities/GameEntityFactory.h"

//input
#include "XInput/Input.h"
#include "XInput/OISInput.h"

//messaging
#include "XMessaging/Messenger.h"

//propertys
#include "XPropertys/PropertyContainer.h"
#include "XPropertys/PropertySerializer.h"

//System components
#include "XSystemComponents/XGameScreens/GameScreenManagerComponent.h"
#include "XSystemComponents/XGameScreens/GameScreen.h"
#include "XSystemComponents/XOgre/OgreRenderer.h"
#include "XSystemComponents/XOgre/OrbitalCamera.h"
#include "XSystemComponents/XOgre/RendererGameComponent.h"
#include "XSystemComponents/XCEGui/CEGUIComponent.h"
#include "XSystemComponents/XDebugging/DebugVariableTracker.h"
#include "XSystemComponents/XDebugging/CEGuiDebuggingTools.h"
#include "XSystemComponents/XDebugging/FPSTracker.h"
#include "XSystemComponents/XDebugging/CEGUIFunctionRunner.h"

//timing
#include "XTiming/Timer.h"
#include "XTiming/WindowsTimer.h"

//utilities
#include "XUtilities/GameSettings.h"
#include "XUtilities/GeometryHelper.h"
#include "XUtilities/Logger.h"
#include "XUtilities/MathHelper.h"
#include "XUtilities/Random.h"
#include "XUtilities/ScriptParser.h"
#include "XUtilities/Utilities.h"
#include "XUtilities/QuickAndDirtyOgreDebugRenderer.h"

//physics
#include "XPhysics/XOctTree/OctTree.h"
#include "XPhysics/XOctTree/OctTreeIterator.h"
#include "XPhysics/XQuadTree/QuadTree.h"
#include "XPhysics/AABB.h"
#include "XPhysics/Body.h"
#include "XPhysics/Rectangle.h"
#include "XPhysics/PhysicsWorld.h"

// Graphs
#include "XGraphs/Graph.h"
#include "XGraphs/XGraphEdges/GraphEdge.h"
#include "XGraphs/XGraphNodes/GraphNode.h"
#include "XGraphs/XGraphNodes/GraphNode3DPosition.h"
#include "XGraphs/XGraphSearches/GraphSearch.h"
#include "XGraphs/XGraphSearches/GraphSearchAStar.h"
#include "XGraphs/XGraphSearches/GraphSearchDFS.h"
#include "GlobalDefines.h"

#endif
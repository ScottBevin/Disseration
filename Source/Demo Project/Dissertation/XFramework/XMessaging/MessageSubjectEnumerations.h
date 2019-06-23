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

#include <string>
/*******
all message subjects are put here 
*********/
#pragma once
#define NO_MESSAGE 999
namespace XF
{
	namespace MessageSubject
	{

		enum ENUM
		{

			//interface		
			UN_CLICKED = 0,
			LOOK_AT,
			CLICKED,
			ATTATCH_CAMERA_TO_RANDOM_ENTITY,
			ATTATCH_CAMERA_TO_ENTITY,
			//end of interface

			//Enity Managment
			REMOVE_ENEMY, //GameEntity*
			//end of Enity Managment

			//collision system
			SENT_ENTITIES_IN_RADUIS,
			SEND_ME_ENITIES_IN_RADIUS,
			//end of collision system
			//Body
			APPLY_FORCE,
			SET_POSITION,
			ROTATE_TO_FACE, //vector3
			//end of body

			//Steering
			STEERING_TURN_OFF_ALL,
			STEERING_TURN_ON_ALL,
			STEERING_SEEK_TO_TARGET_POS,		//XF::vector3
			STEERING_FOLLOW_PATH,				//PathDataPacket
			STEERING_ARRIVE_AT_TARGET_POS,		//XF::vector3
			STEERING_SET_ARRIVE_DECELERATION,	//float
			STEERING_WANDER,
			STEERING_UPDATE_STEER_DATA,			//steering data
			PATH_DATA,
			UPDATE_TARGET_POS,
			UPDATE_TARGET_ENTITY,
			SEND_ME_DISTANCE_TO_GOAL,
			SENT_DISTANCE_TO_GOAL, //float
			TURN_ON_STEERING_BEHAVIOUR,
			TURN_OFF_STEERING_BEHAVIOUR,
			ADD_STEERING_BEHAVIOUR,
			REMOVE_STEERING_BEHAVIOUR,
			//end of steering

			//general path following
			ROAD_MAP_CHANGED,
			ARRIVED_PATH_END, 

			//sound API
			PLAY_SOUND,
			//end of sound API

			//particl API
			SPAWN_EXPLOSION,
			//end of Particle API


		//GUII
			//graphics API
			PLEASE_SEND_RENDERER,
			SENT_RENDERER,	
			//end of API 

			//GUI
			SENT_GUI,
			PLEASE_SEND_GUI,
			//end of GUI


			//AI control
			AI_PAUSE_THINKING,
			AI_RESET_ORDERS_GOALS,
			AI_RESUME_THINKING,
			AI_SET_AGGRESSIVE,
			AI_SET_PASSIVE, 
			AI_SET_DEFENSIVE,
			AI_NEW_PRIMARY_GOAL, //this is used for orders that should be pushed on top of the goal stack and done now (Primary)
			AI_NEW_FOLLOWING_GOAL, //this is used for orders that should be completed after top goal added on to the end
			//end of ai control


			//Game world -> HUD
			TOWER_CLICKED, // GameEntity*
			MULITPLE_TOWER_CLICKED, // std::list<GameEntity*>
			NOTHING_CLICKED, // NULL		
			//end of Game world -> HUD	

			//Currently in main menu
			ADJUST_MONEY, // // int (will add to money)
			ADJUST_LIFE, //int		 (will add to life)
			HOW_MUCH_MONEY_IS_LEFT,
			SENT_HOW_MUCH_MONEY_LEFT,
			MONEY_HAS_CHANGED_TO,
			LIFE_HAS_CHANGED_TO,
			GAME_OVER, //bool, if true the game was won, if false the game was lost
			//end


			// HUD -> Game worlds
			BUILD_TOWER,	// Send string containing filepath of towerFile
			UPGRADE_TOWER, // NULL
			SELL, // NULL
			UPDATE_SELECTED, //NULL
			//end

			//HUD OUT
			TELL_ME_COST_OF, //string (containing filepath of towerFile)
			HOW_MUCH_DO_YOU_COST_TO_UPGRADE,
			WHAT_IS_YOUR_UPGRADE_FILE,
			WHAT_IS_YOUR_SELL_VALUE,
			ARE_YOU_UPGRADABLE,
			YES_I_AM_UPGRADABLE,
			MY_UPGRADE_FILE_IS, //std::string
			//end
			
			// entity managment 
			COST_IS, //string (containing filepath of towerFile)
			MY_UPGRADE_COST_IS,
			MY_SELL_VALUE_IS,
			//end

			//tower commands
			UPGRADE_YOURSELF, //NULL
			BEEN_SOLD, //NULL in case we want a component to make a sparkly effect or something
			TARGET_PRIORITY_CHANGED, //enum TargetPriotiry

			//Entity Creation
			SPAWN_NEXT_WAVE,
			BUILD_SPACE_STATION,
			//end of Entity Creation


			//Game Stuff			
			SPACE_STATION_SELECTED,
			ADD_PROJECTILE_TO_WORLD,
			//end of Game stuff

			//station stuff
			SELL_STATION,
			SELL_ALL_TOWERS,
			UPGRADE_ALL_TOWERS,
			//end of station stuff

			//cosole
			SEND_ME_DISPLAY,
			SENT_CONSOLE_DISPLAY,
			
			SHOW_PAUSE_MENU,

			TRACK_GAME_ENTITY,
			STOP_TRACKING_GAME_ENTITY,
		};

		std::string MessageSubjectToString(XF::MessageSubject::ENUM sub);
		XF::MessageSubject::ENUM StringToMessageSubject(std::string message);
	}
	
}
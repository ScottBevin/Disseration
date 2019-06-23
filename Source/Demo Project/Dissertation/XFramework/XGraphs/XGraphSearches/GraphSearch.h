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

#include "../GraphCommon.h"

namespace XF
{
	template<class TGraphType>
	class GraphSearch
	{
	public:
		typedef typename TGraphType::EdgeType	EdgeType;
		typedef typename TGraphType::NodeType	NodeType;

	protected:
		TGraphType* mGraph;
		int mSourceID, mTargetID;

		virtual void OnInitSearch() = 0;
		virtual bool OnDoStep() = 0;

		bool mFoundPath;

	public:
		GraphSearch(TGraphType *graph) : 
				mGraph(graph), 
				mSourceID(XF_INVALID_NODE_INDEX), 
				mTargetID(XF_INVALID_NODE_INDEX) {}

		virtual ~GraphSearch() {}

		bool FoundPath()
		{
			std::list<int> path = GetPathToTarget();

			return (std::find(path.begin(), path.end(), mTargetID) != path.end() &&
					std::find(path.begin(), path.end(), mSourceID) != path.end());
		}

		void InitSearch(int sourceID, int targetID)
		{
			mSourceID = sourceID;
			mTargetID = targetID;

			OnInitSearch();
		}

		bool DoStep(){return OnDoStep();}

		void PerformSearch(int sourceID, int targetID)
		{
			mFoundPath = false;

			InitSearch(sourceID, targetID);

			while(!FinishedSearching())
			{
				if(DoStep())
				{
					mFoundPath = true;
					break;
				}
			}
		}

		virtual bool FinishedSearching() = 0;

		virtual std::vector<int> GetAllPaths() = 0;
		virtual std::list<int> GetPathToTarget() = 0;
		virtual double GetCostToTarget() = 0;
	};
}

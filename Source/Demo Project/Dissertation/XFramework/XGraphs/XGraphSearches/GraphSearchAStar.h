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

#include "GraphSearch.h"
#include "../../XUtilities/Random.h"
#include "../../XUtilities/PriorityQueue.h"
#include "../../XUtilities/MathHelper.h"
#include "../../XUtilities/Logger.h"
#include <strstream>

namespace XF
{
	class GraphSearchAStar_HeuristicEuclidNoisy
	{
	public:
		template<class TGraphType>
		static double Calculate(const TGraphType* g, int nd1, int nd2)
		{
			return (g->GetNode(nd1).Position() - g->GetNode(nd2).Position()).length() * XF::Random::NextFloat(0.9f, 1.1f);
		}
	};

	class GraphSearchAStar_HeuristicEuclid
	{
	public:
		template<class TGraphType>
		static double Calculate(const TGraphType* g, int nd1, int nd2)
		{
			return (g->GetNode(nd1).Position() - g->GetNode(nd2).Position()).length();
		}
	};

	class GraphSearchAStar_HeuristicManhattanNoisy
	{
	public:
		template<class TGraphType>
		static double Calculate(const TGraphType* g, int nd1, int nd2)
		{
			Vector2 node1Pos = nd1->Position();
			Vector2 node2Pos = nd2->Position();

			return (abs(node1Pos.x - node2Pos.x) + abs(node1Pos.y - node2Pos.y)) * Random::NextFloat(0.9f, 1.1f);
		}
	};

	class GraphSearchAStar_HeuristicManhattan
	{
	public:
		template<class TGraphType>
		static double Calculate(const TGraphType* g, int nd1, int nd2)
		{
			Vector2 node1Pos = nd1->Position();
			Vector2 node2Pos = nd2->Position();

			return abs(node1Pos.x - node2Pos.x) + abs(node1Pos.y - node2Pos.y);
		}
	};

	template<class TGraphType, class THeuristic>
	class GraphSearchAStar : public GraphSearch<TGraphType>
	{
	private:
		std::vector<float>	mGCosts;
		std::vector<float>  mFCosts;

		std::vector<int>	mShortestPaths;

		std::list<int> mClosedSet;
		std::list<int> mOpenSet;

	protected:
		void OnInitSearch()
		{
			mGCosts = std::vector<float>(mGraph->NumNodes(), 0.0);
			mFCosts = std::vector<float>(mGraph->NumNodes(), 0.0);
			mShortestPaths = std::vector<int>(mGraph->NumNodes(), XF_INVALID_NODE_INDEX);

			mClosedSet.clear();

			mOpenSet.clear();
			mOpenSet.push_back(mSourceID);
		}

		bool OnDoStep()
		{
			float lowestFScore = MathHelper::Infinity();
			int currentNode;

			// get lowest FScore node in open set
			for(std::list<int>::const_iterator it = mOpenSet.begin(); it != mOpenSet.end(); ++it)
			{
				int node = *it;

				if(mFCosts[node] < lowestFScore)
				{
					lowestFScore = mFCosts[node];
					currentNode = node;
				}
			}

			if(currentNode == mTargetID)
				return true;

			if(currentNode == XF_INVALID_NODE_INDEX)
				return false;

			mOpenSet.remove(currentNode);
			mClosedSet.push_back(currentNode);

			TGraphType::EdgeIterator edgeIt(mGraph, currentNode);

			for(EdgeType* pE = edgeIt.Begin();
				!edgeIt.End(); pE = edgeIt.Next())
			{
				int connectedNode = pE->To();

				if(connectedNode == XF_INVALID_NODE_INDEX)
					continue;

				if(std::find(mClosedSet.begin(), mClosedSet.end(), connectedNode) != mClosedSet.end())
					continue;

				float gScore = mGCosts[currentNode] + pE->Cost();
				float gScoreBetter = false;

				if(std::find(mOpenSet.begin(), mOpenSet.end(), connectedNode) == mOpenSet.end())
				{					
					mOpenSet.push_back(connectedNode);
					gScoreBetter = true;
				}
				else if(gScore < mGCosts[connectedNode])
				{
					gScoreBetter = true;
				}

				if(gScoreBetter)
				{
					mShortestPaths[connectedNode] = currentNode;
					mGCosts[connectedNode] = gScore;
					mFCosts[connectedNode] = gScore + THeuristic::Calculate(mGraph, mTargetID, connectedNode);
				}
			}

			return false;
		}

	public:
		GraphSearchAStar(TGraphType *graph) : GraphSearch(graph) {}
		~GraphSearchAStar() {}

		bool FinishedSearching()
		{
			return mOpenSet.empty();
		}

		std::vector<int> GetAllPaths()
		{
			return mShortestPaths;
		}

		std::list<int> GetPathToTarget()
		{
			std::list<int> path;

			if(mShortestPaths[mTargetID] != XF_INVALID_NODE_INDEX)
			{
				int nd = mTargetID;

				while(nd != mSourceID && mShortestPaths[nd] != XF_INVALID_NODE_INDEX)
				{
					path.push_front(nd);
					nd = mShortestPaths[nd];
				}

				path.push_front(mSourceID);
			}
			else
				path.push_back(mTargetID);

			return path;
		}

		double GetCostToTarget()
		{
			return mGCosts[mTargetID];
		}
	};
}
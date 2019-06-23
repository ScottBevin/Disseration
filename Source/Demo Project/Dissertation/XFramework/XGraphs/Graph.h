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

#include "GraphCommon.h"

namespace XF
{
	template<class TNodeType, class TEdgeType>
	class Graph
	{
	public:
		typedef TNodeType NodeType;
		typedef TEdgeType EdgeType;

		typedef std::vector<NodeType> NodeVector;
		typedef std::list<EdgeType> EdgeList;
		typedef std::vector<EdgeList> EdgeListVector;

	private:
		NodeVector mNodes;
		EdgeListVector mEdges;

		bool mDigraph;

		int mNextNodeIndex;

	public:
		Graph(bool digraph):
			mNextNodeIndex(0),
			mDigraph(digraph){}

		int NextNodeIndex() const { return mNextNodeIndex; }

		bool IsNodePresent(int nd) const;
		bool IsEdgePresent(int from, int to) const;

		const NodeType& GetNode(int idx) const;
		const EdgeType& GetEdge(int from, int to) const;
		NodeType& GetNode(int idx) ;
		EdgeType& GetEdge(int from, int to) ;

		void AddEdge(EdgeType edge);
		void RemoveEdge(int from, int to);

		int AddNode(NodeType node);

		void CullInvalidEdges();
		void RemoveNode(int node);          

		void SetEdgeCost(int from, int to, double NewCost);
		bool UniqueEdge(int from, int to)const;

		bool Save(const char* FileName)const;
		bool Save(std::ofstream& stream)const;
		bool Load(const char* FileName);
		bool Load(std::ifstream& stream);

		//returns the number of active + inactive nodes present in the graph
		int  NumNodes() const {return mNodes.size();}

		//returns the number of active nodes present in the graph (this method's
		//performance can be improved greatly by caching the value)
		int  NumActiveNodes() const
		{
			int count = 0;

			for (unsigned int n = 0; n < mNodes.size(); ++n)
				if (mNodes[n].Index() != XF_INVALID_NODE_INDEX) 
					++count;

			return count;
		}

		//returns the total number of edges present in the graph
		int  NumEdges()const
		{
			int tot = 0;

			for (EdgeListVector::const_iterator CurEdge = mEdges.Begin();
				CurEdge != mEdges.End();
				++CurEdge)
			{
				tot += CurEdge->size();
			}

			return tot;
		}

		//returns true if the graph is directed
		bool IsDigraph()const{return mDigraph;}

		//returns true if the graph contains no nodes
		bool IsEmpty()const{return mNodes.empty();}

		//clears the graph ready for new node insertions
		void Clear()
		{
			mNextNodeIndex = 0; 
			mNodes.clear(); 
			mEdges.clear();
		}

		void RemoveEdges()
		{
			for (EdgeListVector::iterator it = mEdges.Begin(); it != mEdges.End(); ++it)
			{
				it->Clear();
			}
		}

		class EdgeIterator
		{
		private:                                                                

			typename EdgeList::iterator CurEdge;

			Graph<NodeType, EdgeType>  *mGraph;

			const int mNodeIndex;

		public:

			EdgeIterator(Graph<NodeType, EdgeType>* graph, int node): 
						mGraph(graph),
						mNodeIndex(node)
			{
			  /* we don't need to check for an invalid node index since if the node is
				 invalid there will be no associated edges
			 */

				CurEdge = mGraph->mEdges[mNodeIndex].begin();
			}

			EdgeType*  Begin()
			{        
				CurEdge = mGraph->mEdges[mNodeIndex].begin();

				return &(*CurEdge);
			}

			EdgeType*  Next()
			{
				++CurEdge;

				if (End()) return NULL;

				return &(*CurEdge);
			}

			//return true if we are at the End of the edge list
			bool End()
			{
				return (CurEdge == mGraph->mEdges[mNodeIndex].end());
			}
		};

		friend class EdgeIterator;

		//const class used to iterate through all the edges connected to a specific node. 
		class ConstEdgeIterator
		{
		private:                                                                

			typename EdgeList::const_iterator CurEdge;

			const Graph<NodeType, EdgeType>* mGraph;

			const int mNodeIndex;

		public:

			ConstEdgeIterator(
				const Graph<NodeType, EdgeType>* graph,
				int node): 
					mGraph(graph),
					mNodeIndex(node)
			{
				/* we don't need to check for an invalid node index since if the node is
				invalid there will be no associated edges
				*/

				CurEdge = mGraph->mEdges[mNodeIndex].begin();
			}

			const EdgeType*  Begin()
			{     
				if(mGraph->mEdges[mNodeIndex].size() <= 0)
					return NULL;

				CurEdge = mGraph->mEdges[mNodeIndex].begin();

				return &(*CurEdge);
			}

			const EdgeType*  Next()
			{
				++CurEdge;

				if(End())
				{
					return NULL;
				}
				else
				{
					return &(*CurEdge);
				}

			}

			//return true if we are at the End of the edge list
			bool End()
			{
				return (CurEdge == mGraph->mEdges[mNodeIndex].end());
			}
		};

		friend class ConstEdgeIterator;

		//non const class used to iterate through the nodes in the graph
		class NodeIterator
		{
		private:

			typename NodeVector::iterator         curNode;

			Graph<NodeType, EdgeType>*    mGraph;

			//if a graph node is removed, it is not removed from the 
			//vector of nodes (because that would mean changing all the indices of 
			//all the nodes that have a higher index). This method takes a node
			//iterator as a parameter and assigns the Next valid element to it.
			void GetNextValidNode(typename NodeVector::iterator& it)
			{
				if ( curNode == mGraph->mNodes.end()() || it->Index() != XF_INVALID_NODE_INDEX) 
					return;

				while ( (it->Index() == XF_INVALID_NODE_INDEX) )
				{
					++it;

					if (curNode == mGraph->mNodes.end()()) 
						break;
				}
			}

		public:

			NodeIterator(Graph<NodeType, EdgeType> *graph):
				mGraph(graph)
			{
				curNode = mGraph->mNodes.begin();
			}




			NodeType* Begin()
			{      
				curNode = mGraph->mNodes.begin();

				GetNextValidNode(curNode);

				return &(*curNode);
			}

			NodeType* Next()
			{
				++curNode;

				if (End()) 
					return NULL;

				GetNextValidNode(curNode);

				return &(*curNode);
			}

			bool End()
			{
				return (curNode == mGraph->mNodes.end()());
			}
		};

		friend class NodeIterator;

		//const class used to iterate through the nodes in the graph
		class ConstNodeIterator
		{
		private:

			typename NodeVector::const_iterator	curNode;

			const Graph<NodeType, EdgeType>* mGraph;

			//if a graph node is removed or switched off, it is not removed from the 
			//vector of nodes (because that would mean changing all the indices of 
			//all the nodes that have a higher index. This method takes a node
			//iterator as a parameter and assigns the Next valid element to it.
			void GetNextValidNode(typename NodeVector::const_iterator& it)
			{
				if ( curNode == mGraph->mNodes.end() || it->Index() != XF_INVALID_NODE_INDEX) 
					return;

				while ( (it->Index() == XF_INVALID_NODE_INDEX) )
				{
					++it;

					if (curNode == mGraph->mNodes.end()) 
						break;
				}
			}

		public:

			ConstNodeIterator(const Graph<NodeType, EdgeType> *graph):mGraph(graph)
			{
				curNode = mGraph->mNodes.begin();
			}


			const NodeType* Begin()
			{      
				curNode = mGraph->mNodes.begin();

				GetNextValidNode(curNode);

				return &(*curNode);
			}

			const NodeType* Next()
			{
				++curNode;

				if (End())
				{
					return NULL;
				}
				else
				{
					GetNextValidNode(curNode);

					return &(*curNode);
				}
			}

			bool End()
			{
				return (curNode == mGraph->mNodes.end());
			}
		};

		friend class ConstNodeIterator;
	}; // END OF GRAPH CLASS

	template<class TNodeType, class TEdgeType>
	bool Graph<TNodeType, TEdgeType>::IsNodePresent(int nd) const
	{
		return !((nd >= (int)mNodes.size() || (mNodes[nd].Index() == XF_INVALID_NODE_INDEX)));
	}

	//////////////////////////////////////////////////////////////////////////
	template<class TNodeType, class TEdgeType>
	bool Graph<TNodeType, TEdgeType>::IsEdgePresent(int from, int to) const
	{
		if(IsNodePresent(from) && IsNodePresent(to))
		{
			for(EdgeList::const_iterator it = mEdges[from].Begin();
				it != mEdges[from].End(); ++it)
			{
				if(it->To() == to) 
					return true;
			}

			return false;
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	template<class TNodeType, class TEdgeType>
	const TNodeType& Graph<TNodeType, TEdgeType>::GetNode(int idx) const
	{
		assert( (idx < (int)mNodes.size()) &&
			(idx >= 0)					&&
			"<XF::Graph::GetNode>: Index out of range");

		return mNodes[idx];
	}

	//////////////////////////////////////////////////////////////////////////
	template<class TNodeType, class TEdgeType>
	const TEdgeType& Graph<TNodeType, TEdgeType>::GetEdge(int from, int to) const
	{
		assert( (from < (int)mNodes.size()) &&
				(from >= 0)					&&
				mNodes[from].Index() != XF_INVALID_NODE_INDEX &&
				"<XF::Graph::GetEdge>: 'from' Index invalid");

		assert( (to < (int)mNodes.size()) &&
				(to >= 0)					&&
				mNodes[to].Index() != XF_INVALID_NODE_INDEX &&
				"<XF::Graph::GetEdge>: 'to' Index invalid");

		for(EdgeList::const_iterator it = mEdges[from].Begin();
			it != mEdges[from].End(); ++it)
		{
			if(it->To() == to) 
				return *it;
		}

		assert(0 && "<XF::Graph::GetEdge>: edge does not exist");
	}

	//////////////////////////////////////////////////////////////////////////
	template<class TNodeType, class TEdgeType>
	TNodeType& Graph<TNodeType, TEdgeType>::GetNode(int idx)
	{
		assert( (idx < (int)mNodes.size()) &&
			(idx >= 0)					&&
			"<XF::Graph::GetNode>: Index out of range");

		return mNodes[idx];
	}

	//////////////////////////////////////////////////////////////////////////
	template<class TNodeType, class TEdgeType>
	TEdgeType& Graph<TNodeType, TEdgeType>::GetEdge(int from, int to)
	{
		assert( (from < (int)mNodes.size()) &&
			(from >= 0)					&&
			mNodes[from].Index() != XF_INVALID_NODE_INDEX &&
			"<XF::Graph::GetEdge>: 'from' Index invalid");

		assert( (to < (int)mNodes.size()) &&
			(to >= 0)					&&
			mNodes[to].Index() != XF_INVALID_NODE_INDEX &&
			"<XF::Graph::GetEdge>: 'to' Index invalid");

		for(EdgeList::const_iterator it = mEdges[from].Begin();
			it != mEdges[from].End(); ++it)
		{
			if(it->To() == to) 
				return *it;
		}

		assert(0 && "<XF::Graph::GetEdge>: edge does not exist");
	}

	//////////////////////////////////////////////////////////////////////////
	template<class TNodeType, class TEdgeType>
	void Graph<TNodeType, TEdgeType>::AddEdge(EdgeType edge)
	{
		assert(	(edge.From() < mNextNodeIndex) &&
				(edge.To() < mNextNodeIndex) &&
				"<XF::Graph::AddEdge>: Invalid node Index");

		if(	(mNodes[edge.To()].Index() != XF_INVALID_NODE_INDEX) &&
			(mNodes[edge.From()].Index() != XF_INVALID_NODE_INDEX))
		{
			if(UniqueEdge(edge.From(), edge.To()))
			{
				mEdges[edge.From()].push_back(edge);
			}

			if(!mDigraph)
			{
				if(UniqueEdge(edge.To(), edge.From()))
				{
					EdgeType newEdge = edge;

					newEdge.To(edge.From());
					newEdge.From(edge.To());

					mEdges[edge.To()].push_back(newEdge);
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	template<class TNodeType, class TEdgeType>
	void Graph<TNodeType, TEdgeType>::RemoveEdge(int from, int to)
	{
		assert( (from < (int)mNodes.size()) &&
				(to < (int)mNodes.size()) &&
				"<XF::Graph::RemoveEdge>: Invalid node Index");
		
		EdgeList::iterator CurEdge;

		if(!mDigraph)
		{
			for(CurEdge = mEdges[to].Begin(); CurEdge != mEdges[to].End(); ++CurEdge)
			{
				if(CurEdge->To() == from)
				{
					CurEdge = mEdges = mEdges[to].erase(CurEdge);
					break;
				}
			}
		}

		for(CurEdge = mEdges[from].Begin(); CurEdge != mEdges[from].End(); ++CurEdge)
		{
			if(CurEdge->To() == from)
			{
				CurEdge = mEdges = mEdges[from].erase(CurEdge);
				break;
			}
		}
	}

	//-------------------------- AddNode -------------------------------------
	//
	//  Given a node this method first checks to see if the node has been added
	//  previously but is now innactive. If it is, it is reactivated.
	//
	//  If the node has not been added previously, it is checked to make sure its
	//  index matches the Next node index before being added to the graph
	//------------------------------------------------------------------------
	template<class TNodeType, class TEdgeType>
	int Graph<TNodeType, TEdgeType>::AddNode(NodeType node)
	{
		if(node.Index() < (int)mNodes.size())
		{
			assert(mNodes[node.Index()].Index() == XF_INVALID_NODE_INDEX &&
				"<XF::Graph::AddNode>: Attempting to add a node with a duplicate ID");

			mNodes[node.Index()] = node;
			return mNextNodeIndex;
		}
		else
		{
			assert (node.Index() == mNextNodeIndex && "<XF::Graph::AddNode>:invalid index");

			mNodes.push_back(node);
			mEdges.push_back(EdgeList());

			return ++mNextNodeIndex;
		}
	}

	//----------------------- CullInvalidEdges ------------------------------------
	//
	//  iterates through all the edges in the graph and removes any that point
	//  to an invalidated node
	//-----------------------------------------------------------------------------
	template <class TNodeType, class TEdgeType>
	void Graph<TNodeType, TEdgeType>::CullInvalidEdges()
	{
		for (EdgeListVector::iterator CurEdgeList = mEdges.begin(); CurEdgeList != mEdges.end(); ++CurEdgeList)
		{
			for (EdgeList::iterator CurEdge = (*CurEdgeList).begin(); CurEdge != (*CurEdgeList).end(); ++CurEdge)
			{
				if (mNodes[CurEdge->To()].Index() == XF_INVALID_NODE_INDEX || 
					mNodes[CurEdge->From()].Index() == XF_INVALID_NODE_INDEX)
				{
					CurEdge = (*CurEdgeList).erase(CurEdge);
				}
			}
		}
	}

	//------------------------------- RemoveNode -----------------------------
	//
	//  Removes a node from the graph and removes any links to neighboring
	//  nodes
	//------------------------------------------------------------------------
	template <class TNodeType, class TEdgeType>
	void Graph<TNodeType, TEdgeType>::RemoveNode(int node)                                   
	{
		assert(node < (int)mNodes.size() && "<SparseGraph::RemoveNode>: invalid node index");

		//set this node's index to XF_INVALID_NODE_INDEX
		mNodes[node].Index(XF_INVALID_NODE_INDEX);

		//if the graph is not directed remove all edges leading to this node and then
		//clear the edges leading from the node
		if (!mDigraph)
		{    
			//visit each neighbor and erase any edges leading to this node
			for (EdgeList::iterator CurEdge = mEdges[node].begin(); 
				CurEdge != mEdges[node].end();
				++CurEdge)
			{
				for (EdgeList::iterator curE = mEdges[CurEdge->To()].begin();
					curE != mEdges[CurEdge->To()].end();
					++curE)
				{
					if (curE->To() == node)
					{
						curE = mEdges[CurEdge->To()].erase(curE);

						break;
					}
				}
			}

			//finally, clear this node's edges
			mEdges[node].clear();
		}

		//if a digraph remove the edges the slow way
		else
		{
			CullInvalidEdges();
		}
	}

	//-------------------------- SetEdgeCost ---------------------------------
	//
	//  Sets the cost of a specific edge
	//------------------------------------------------------------------------
	template <class TNodeType, class TEdgeType>
	void Graph<TNodeType, TEdgeType>::SetEdgeCost(int from, int to, double NewCost)
	{
		//make sure the nodes given are valid
		assert( (from < mNodes.size()) && (to < mNodes.size()) &&
			"<SparseGraph::SetEdgeCost>: invalid index");

		//visit each neighbour and erase any edges leading to this node
		for (EdgeList::iterator CurEdge = mEdges[from].Begin(); 
			CurEdge != mEdges[from].End();
			++CurEdge)
		{
			if (CurEdge->To() == to)
			{
				CurEdge->SetCost(NewCost);
				break;
			}
		}
	}

	//-------------------------------- UniqueEdge ----------------------------
	//
	//  returns true if the edge is not present in the graph. Used when adding
	//  edges to prevent duplication
	//------------------------------------------------------------------------
	template <class TNodeType, class TEdgeType>
	bool Graph<TNodeType, TEdgeType>::UniqueEdge(int from, int to)const
	{
		for (EdgeList::const_iterator CurEdge = mEdges[from].begin();
			CurEdge != mEdges[from].end();
			++CurEdge)
		{
			if (CurEdge->To() == to)
			{
				return false;
			}
		}

		return true;
	}

	//-------------------------------- Save ---------------------------------------

	template <class TNodeType, class TEdgeType>
	bool Graph<TNodeType, TEdgeType>::Save(const char* FileName)const
	{
		//open the file and make sure it's valid
		std::ofstream out(FileName);

		if (!out)
		{
			throw std::runtime_error("Cannot open file: " + std::string(FileName));
			return false;
		}

		return Save(out);
	}

	//-------------------------------- Save ---------------------------------------
	template <class TNodeType, class TEdgeType>
	bool Graph<TNodeType, TEdgeType>::Save(std::ofstream& stream)const
	{
		//save the number of nodes
		stream << mNodes.size() << std::endl;

		//iterate through the graph nodes and save them
		NodeVector::const_iterator curNode = mNodes.begin();
		for (curNode; curNode != mNodes.end()(); ++curNode)
		{
			stream << *curNode;
		}

		//save the number of edges
		stream << NumEdges() << std::endl;

		//iterate through the edges and save them
		for (unsigned int nodeIdx = 0; nodeIdx < mNodes.size(); ++nodeIdx)
		{
			for (EdgeList::const_iterator CurEdge = mEdges[nodeIdx].Begin();
				CurEdge != mEdges[nodeIdx].End(); ++CurEdge)
			{
				stream << *CurEdge;
			}  
		}

		return true;
	}

	//------------------------------- Load ----------------------------------------
	//-----------------------------------------------------------------------------
	template <class TNodeType, class TEdgeType>
	bool Graph<TNodeType, TEdgeType>::Load(const char* FileName)
	{
		//open file and make sure it's valid
		std::ifstream in(FileName);

		if (!in)
		{
			throw std::runtime_error("Cannot open file: " + std::string(FileName));
			return false;
		}

		return Load(in);
	}

	//------------------------------- Load ----------------------------------------
	//-----------------------------------------------------------------------------
	template <class TNodeType, class TEdgeType>
	bool Graph<TNodeType, TEdgeType>::Load(std::ifstream& stream)
	{
		Clear();

		//get the number of nodes and read them in
		int NumNodes, NumEdges;

		stream >> NumNodes;

		for (int n=0; n<NumNodes; ++n)
		{
			NodeType NewNode(stream);

			//when editing graphs it's possible to End up with a situation where some
			//of the nodes have been invalidated (their id's set to XF_INVALID_NODE_INDEX). Therefore
			//when a node of index XF_INVALID_NODE_INDEX is encountered, it must still be added.
			if (NewNode.Index() != XF_INVALID_NODE_INDEX)
			{
				AddNode(NewNode);
			}
			else
			{
				mNodes.push_back(NewNode);

				//make sure an edgelist is added for each node
				mEdges.push_back(EdgeList());

				++mNextNodeIndex;
			}
		}

		//now add the edges
		stream >> NumEdges;
		for (int e=0; e< NumEdges; ++e)
		{
			EdgeType NextEdge(stream);

			AddEdge(NextEdge);
		}

		return true;
	}
}
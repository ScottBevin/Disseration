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

#include "CrowdSimDebugRendererMkII.h"

CrowdSimDebugRendererMkII::CrowdSimDebugRendererMkII(XF::OgreRenderer *renderer, XCS::CrowdSimulator *sim)
{
	sim->mOnAddAgentEvent.RegisterMethod<CrowdSimDebugRendererMkII, &CrowdSimDebugRendererMkII::OnAddAgent>(this);
	sim->mOnKillAgentEvent.RegisterMethod<CrowdSimDebugRendererMkII, &CrowdSimDebugRendererMkII::OnKillAgent>(this);

	sim->mOnAddObstacleEvent.RegisterMethod<CrowdSimDebugRendererMkII, &CrowdSimDebugRendererMkII::OnAddObstacle>(this);
	sim->mOnKillObstacleEvent.RegisterMethod<CrowdSimDebugRendererMkII, &CrowdSimDebugRendererMkII::OnKillObstacle>(this);

	mQADRenderer = new XF::QuickAndDirtyOgreDebugRenderer(renderer);
	//mQADRenderer->SetYPlane(0.1f);

	mSim = sim;
}

CrowdSimDebugRendererMkII::~CrowdSimDebugRendererMkII()
{
	delete mQADRenderer;
}

void CrowdSimDebugRendererMkII::UpdateRender(double dT)
{
	mQADRenderer->Begin();

	DrawQuadTree();

	RenderAgents(dT);

	RenderObstacles();


	mQADRenderer->End();
}

void CrowdSimDebugRendererMkII::SetSelectedEntitys( XCS::Entity* agent )
{
	mSelectedAgents.clear();
	mSelectedAgents.push_back(agent);
}

void CrowdSimDebugRendererMkII::SetSelectedEntitys( std::vector<XCS::Entity*> agent )
{
	mSelectedAgents.clear();

	mSelectedAgents = agent;
}

void CrowdSimDebugRendererMkII::OnAddAgent( XCS::Agent* agent )
{
	mAgents.push_back(agent);
}

void CrowdSimDebugRendererMkII::OnKillAgent( XCS::Agent* agent )
{
	mAgents.erase(std::find(mAgents.begin(), mAgents.end(), agent));
}

void CrowdSimDebugRendererMkII::OnAddObstacle( XCS::LineObstacle* obstacle )
{
	mObstacles.push_back(obstacle);
}

void CrowdSimDebugRendererMkII::OnKillObstacle( XCS::LineObstacle* obstacle )
{
	mObstacles.erase(std::find(mObstacles.begin(), mObstacles.end(), obstacle));
}

void CrowdSimDebugRendererMkII::RenderAgents(double dT)
{
	for(unsigned int i = 0; i < mAgents.size(); ++i)
	{
		XCS::Agent* agent = mAgents[i];

		// Main agent circle
		XF::Vector3 color(0.5f);
		
		color.x = agent->Personality().N;
		color.y = agent->Personality().O;
		color.z = agent->Personality().E;

		mQADRenderer->Color(color);	mQADRenderer->DrawCircle(XF::Vector2(agent->Position().mX, agent->Position().mY), agent->Radius()); 
		mQADRenderer->Color(1.0f, 0.0f, 0.0f);	mQADRenderer->DrawLine(
			XF::Vector2(agent->Position().mX, agent->Position().mY), 
			XF::Vector2(agent->Position().mX, agent->Position().mY) + (XF::Vector2(agent->Velocity().mX, agent->Velocity().mY) * 0.5f));
		//mQADRenderer->Color(0.0f, 1.0f, 0.0f);	mQADRenderer->DrawLine(agent->Position(), agent->Position() + (agent->SteeringVelocity() * 0.5f));

		if(IsEntitySelected(agent))
		{
			mQADRenderer->Color(0.0f, 0.0f, 1.0f);	mQADRenderer->DrawCircle(XF::Vector2(agent->Position().mX, agent->Position().mY), agent->Radius() + 0.1f); 
			mQADRenderer->Color(1.0f, 0.0f, 1.0f);	mQADRenderer->DrawCircle(XF::Vector2(agent->Position().mX, agent->Position().mY), agent->Personality().LookDistance);

			mQADRenderer->Color(1.0f, 1.0f, 0.0f);	mQADRenderer->DrawCircle(XF::Vector2(agent->SteeringComponent().Target().mX, agent->SteeringComponent().Target().mY), 0.5f);
			mQADRenderer->Color(1.0f, 6.0f, 0.0f);	mQADRenderer->DrawCircle(XF::Vector2(agent->SteeringComponent().Target().mX, agent->SteeringComponent().Target().mY), 0.4f);
			mQADRenderer->Color(1.0f, 3.0f, 0.0f);	mQADRenderer->DrawCircle(XF::Vector2(agent->SteeringComponent().Target().mX, agent->SteeringComponent().Target().mY), 0.3f);

			mQADRenderer->Color(1.0f, 0.0f, 0.0f);

			for(unsigned int j = 0; j < agent->mVelocitysSampled.size(); ++j)
			{
				mQADRenderer->DrawLine(
					XF::Vector2(agent->mVelocitysSampled[j].first.mX + agent->Position().mX, agent->mVelocitysSampled[j].first.mY + agent->Position().mY),
					XF::Vector2(agent->mVelocitysSampled[j].second.mX + agent->Position().mX, agent->mVelocitysSampled[j].second.mY + agent->Position().mY));
			}

			mQADRenderer->Color(0.0f, 0.0f, 1.0f);

			for(unsigned int j = 0; j < agent->mVelocitysSampled.size(); ++j)
			{
				mQADRenderer->DrawLine(
					XF::Vector2(agent->Position().mX, agent->Position().mY),
					XF::Vector2(agent->mVelocitysSampled[j].second.mX + agent->Position().mX, agent->mVelocitysSampled[j].second.mY + agent->Position().mY));
			}

			std::vector<XCS::Entity*> obstacles = agent->GetThisFramesObstacles();

			for(unsigned int j = 0; j < obstacles.size(); ++j)
			{
				mQADRenderer->Color(0.0f, 0.0f, 0.7f);	
				mQADRenderer->DrawCircle(XF::Vector2(obstacles[j]->Position().mX, obstacles[j]->Position().mY), obstacles[j]->Radius() - 0.1f);
			}

			std::vector<XCS::VOCone> cones = agent->GetVelocityObstacle().Cones();

			for(unsigned int j = 0; j < cones.size(); ++j)
			{
				std::vector<XCS::Vector2f> verts = cones[j].Vertices();
				std::vector<XF::Vector2> xfVerts;

				for(unsigned int k = 0; k < verts.size(); ++k)
				{
					verts[k] += agent->Position();
					xfVerts.push_back(XF::Vector2(verts[k].mX, verts[k].mY));
				}

				mQADRenderer->DrawLineStrip(xfVerts);
			}
		}
	}
}

void CrowdSimDebugRendererMkII::DrawQuadTree()
{
	XCS::QuadTree::NodeIterator it(mSim->GetQuadTree()->RootNode());

	while(!it.End())
	{
		XCS::QuadTreeNode* node = it.Next();

		mQADRenderer->Color(1.0f, 0.0f, 0.0f);
		mQADRenderer->DrawRect(node->Min().mX, node->Max().mX, node->Min().mY, node->Max().mY);

		if(node->IsOvercrowded())
		{
			mQADRenderer->Color(0.0f, 1.0f, 0.0f);
			mQADRenderer->DrawRect(node->Min().mX + 0.2f, node->Max().mX - 0.2f, node->Min().mY + 0.2f, node->Max().mY - 0.2f);
		}
	}
}

void CrowdSimDebugRendererMkII::RenderObstacles()
{
	for(unsigned int i = 0; i < mObstacles.size(); ++i)
	{
		XCS::LineObstacle* obs = mObstacles[i];

		mQADRenderer->Color(0.3f, 0.4f, 1.0f); mQADRenderer->DrawLine(
			XF::Vector2(obs->StartPoint().mX, obs->StartPoint().mY) , 
			XF::Vector2(obs->EndPoint().mX, obs->EndPoint().mY));

		if(IsEntitySelected(obs))
		{
			mQADRenderer->Color(0.4f, 0.5f, 1.0f); 
			mQADRenderer->DrawLine(
				XF::Vector2(obs->Position().mX, obs->Position().mY), 
				XF::Vector2(obs->Position().mX, obs->Position().mY) + XF::Vector2(obs->Normal().mX, obs->Normal().mY));
			mQADRenderer->Color(0.2f, 0.3f, 1.0f); mQADRenderer->DrawCircle(XF::Vector2(obs->Position().mX, obs->Position().mY), obs->Radius());
		}
	}
}
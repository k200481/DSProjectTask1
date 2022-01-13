/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include <sstream>

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	// place node 1 at the center of the screen
	const Vec2 center = Vec2(gfx.ScreenWidth / 2, gfx.ScreenHeight / 2);
	g.AddVertex(Node(1, center));
	// generate all the other nodes in a circle around node 1
	for (size_t i = 1; i < NumVertices; i++)
	{
		// the angle of the node from the center of the screen
		const float angle = 2 * 3.14f * i / (NumVertices - 1);
		// the position of the node on the screen
		const Vec2 pos = center + Vec2{ sin(angle), -cos(angle) } * 200.0f;
		// create a new node
		g.AddVertex(Node(i + 1, pos));
	}

	// read edges from file
	for (size_t i = 0, k = 0; i < doc.GetRowCount() && k < NumVertices; i++)
	{
		// the source vertex
		const size_t src = doc.GetCell<size_t>(SrcColIdx, i);
		// the number of edges from this source
		const size_t numNeighbours = doc.GetCell<size_t>(NeighbourCountColIdx, i);
		
		// check if this vertex already has edges
		// this is needed as there are duplicates in the file
		if (g.GetAdjList_idx(src - 1).empty())
		{
			k++;
			// read a destination vertex and add an edge to it from source
			for (size_t j = 0; j < numNeighbours; j++)
			{
				// the destination vertex
				const size_t neighbour_j = doc.GetCell<size_t>(firstNeighbourColIdx + 3 * j, i);
				g.AddEdge_idx(src - 1, neighbour_j - 1);
			}
		}
	}

	// enabling auto repeat allows us to press and hold a key
	// and have it read as multiple presses
	wnd.kbd.EnableAutorepeat();
}

void Game::Go()
{
	// framework stuff
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	// check if any keyboard event occured
	while (!wnd.kbd.KeyIsEmpty())
	{
		// read the first keyboard event
		auto e = wnd.kbd.ReadKey();
		// if the event was a press
		if (e.IsPress())
		{
			// get the ASCII code of the key that was pressed
			// and act accordingly
			ProcessKey(e.GetCode());

			// make sure the src and dst vertices dont go below 1 or above 16
			src = std::max(size_t(1), std::min(size_t(NumVertices), src));
			dst = std::max(size_t(1), std::min(size_t(NumVertices), dst));

			// use the pathfinding algo currently selected
			if (useBFS)
				path = g.BFS(Node(src), Node(dst));
			else
				path = g.DFS(Node(src), Node(dst));

			// generate a message based on the current state of the variables
			std::ostringstream oss;
			oss << (useBFS ? "BFS" : "DFS")
				<< " from " << src
				<< " to " << dst;
			msg = oss.str();
		}
	}
}

void Game::ProcessKey(unsigned char key)
{
	switch (key)
	{
	case VK_RIGHT:
		// clockwise rotation of src
		src++;
		break;
	case VK_LEFT:
		// counter-clockwise rotation of src
		src--;
		break;
	case VK_UP:
		// clockwise rotation of dst
		dst++;
		break;
	case VK_DOWN:
		// counter-clockwise rotation of dst
		dst--;
		break;
	case VK_SPACE:
		// toggle pathfinding algo
		useBFS = !useBFS;
		break;
	}
}

void Game::ComposeFrame()
{
	const auto& nodes = g.GetVertices();

	// draw all the edges b/w the nodes
	for (size_t i = 0; i < NumVertices; i++)
	{
		// get the adj list for vertex i
		const auto& adj = g.GetAdjList_idx(i);
		// draw each edge in the adj list
		for (auto& edge : adj)
		{
			gfx.DrawLine(
				nodes[edge.src_idx].GetPos(),  // from src node
				nodes[edge.dst_idx].GetPos(),  // to dst node
				Colors::Gray
			);
		}
	}

	// draw the nodes at their positions
	for (auto& node : nodes)
	{
		node.Draw(gfx, Colors::Gray);
	}

	// draw the current highlighted path
	for (int i = 0; i < (int)path.size() - 1; i++)
	{
		// draw lines between the current vtx and the next
		gfx.DrawLine(path[i].GetPos(), path[i + 1].GetPos(), Colors::Cyan);
	}

	// redraw vertices that appear in the path red
	path.front().Draw(gfx, Colors::Green);
	path.back().Draw(gfx, Colors::Red);
	for (int i = 1; i < path.size() - 1; i++)
	{
		path[i].Draw(gfx, Colors::Yellow);
	}

	// draw message test
	font.DrawText(msg, Vei2(20, gfx.ScreenHeight - 40), Colors::Gray, gfx);
}

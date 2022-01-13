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
#include <random>
#include "SpriteEffect.h"
#include <sstream>

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	nodes(16)
{
	// place node 1 at the center of the screen
	const Vec2 center = Vec2(gfx.ScreenWidth / 2, gfx.ScreenHeight / 2);
	nodes[0] = Node(1, center);
	// generate all the other nodes in a circle around node 1
	for (size_t i = 1; i < 16; i++)
	{
		// the angle of the node from the center of the screen
		const float angle = 2 * 3.14f * i / 15.0f;
		// the position of the node on the screen
		const Vec2 pos = center + Vec2{ sin(angle), -cos(angle) } * 200.0f;
		// create a new node
		nodes[i] = Node(i + 1, pos);
	}
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{

}

void Game::ComposeFrame()
{
	// draw the nodes at their positions
	for (auto& node : nodes)
	{
		node.Draw(gfx);
	}
}

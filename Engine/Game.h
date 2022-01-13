/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
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
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "FrameTimer.h"

#include "Node.h"
#include "Graph.h"
#include "RapidCSV.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */

	// function to process key presses
	void ProcessKey(unsigned char key);
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	
	// the graph generated from the file
	Graph<Node> g;
	// the source of the highlighted path
	size_t src = 1;
	// the destination of the highlighted path
	size_t dst = 1;
	// the path to highlight
	DSA<Node> path = DSA<Node>(1, Node(1, Vec2(Graphics::ScreenWidth / 2, Graphics::ScreenHeight / 2)));
	// true when BFS is used, false when DFS is used
	bool useBFS = true;
	// the message to draw on the bottom left of the screen
	std::string msg = "BFS from 1 to 1";
	// font used to draw text
	Font font = Font("Images/Fixedsys16x28.bmp");
	// used to read csv file
	rapidcsv::Document doc = rapidcsv::Document("Final Data Structure.csv", rapidcsv::LabelParams());
	
	// number of vertices in the graph
	static constexpr size_t NumVertices = 16;
	// the index of the column in the file with the sources
	static constexpr size_t SrcColIdx = 3;
	// the index of the column in the file with the number of neighbours
	static constexpr size_t NeighbourCountColIdx = 12;
	// the index of the column in the file with the first neighbour
	static constexpr size_t firstNeighbourColIdx = 14;
	/********************************/
};
#pragma once
#include "Vec2.h"
#include "Font.h"

class Node
{
public:
	Node() = default;
	Node(size_t value, const Vec2& pos = { 0.0f, 0.0f });

	size_t GetAddress() const;

	// not sure if its better to return a const ref or a value
	const Vec2& GetPos() const;

	void SetPos(const Vec2& pos_in);
	// draws a node at it's position on the screen using a gfx ref
	void Draw(Graphics& gfx, Color c) const;

	bool operator==(const Node& rhs) const;

private:
	// the number/address representing a node
	size_t value = 0;
	// position on the screen
	Vec2 pos = { 0.0f, 0.0f };
	// half the dimensions of a character
	// needed for centering the characters as the framework draws the from top-left
	static const Vec2 correctionFactor;
	// used to draw the numbers representing a node
	static const Font font;
};

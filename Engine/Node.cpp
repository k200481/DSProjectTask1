#include "Node.h"
#include <sstream>

const Vec2 Node::correctionFactor = { 8.0f, 14.0f };
const Font Node::font = Font("Images/Fixedsys16x28.bmp");

Node::Node(size_t value, const Vec2& pos)
	:
	value(value),
	pos(pos)
{}

size_t Node::GetAddress() const
{
	return value;
}

const Vec2& Node::GetPos() const
{
	return pos;
}

void Node::SetPos(const Vec2& pos_in)
{
	pos = pos_in;
}

void Node::Draw(Graphics& gfx, Color c) const
{
	std::ostringstream oss;
	oss << value;
	font.DrawText(oss.str(), Vei2(pos - correctionFactor), c, gfx);
}

bool Node::operator==(const Node& rhs) const
{
	return value == rhs.value;
}
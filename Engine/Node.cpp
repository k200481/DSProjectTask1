#include "Node.h"

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

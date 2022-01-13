#pragma once
#include "Vec2.h"

class Node
{
public:
	Node() = default;
	Node(size_t value, const Vec2& pos = { 0.0f, 0.0f });

	size_t GetAddress() const;

	// not sure if its better to return a const ref or a value
	const Vec2& GetPos() const;

	void SetPos(const Vec2& pos_in);

private:
	size_t value;
	Vec2 pos;
};


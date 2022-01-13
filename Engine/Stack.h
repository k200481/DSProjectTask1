#pragma once
#include "DSA.h"

template <typename T>
class ArrayStack
{
public:
	ArrayStack() = default;
	ArrayStack(size_t size)
		:
		arr(size)
	{
		if (size == 0)
			throw std::invalid_argument("Size must be grater than 0");
	}
	
	void push(const T& val)
	{
		if (top_ptr >= arr.size())
		{
			arr.push_back(val);
			top_ptr++;
			return;
		}
		arr[top_ptr++] = val;
	}
	const T& top() const
	{
		if (top_ptr == 0)
		{
			throw std::exception("Stack Underflow");
		}
		return arr[top_ptr - 1];
	}
	void pop()
	{
		if (top_ptr > 0)
			top_ptr--;
	}
	bool empty() const
	{
		return top_ptr == 0;
	}

private:
	DSA<T> arr;
	size_t top_ptr = 0;
};

template <typename T>
class LinkedListStack
{
	struct Node
	{
		T val;
		Node* next = nullptr;
		Node(const T& val)
			:
			val(val)
		{}
		~Node()
		{
			delete next;
		}
	};
public:
	LinkedListStack() = default;
	~LinkedListStack()
	{
		delete top_ptr;
	}
	LinkedListStack(const LinkedListStack& rhs)
	{
		*this = rhs;
	}
	LinkedListStack& operator=(const LinkedListStack& rhs)
	{
		if (this == &rhs)
			return *this;
		delete top_ptr;
		Node* ptr = rhs.top_ptr;
		while (ptr != nullptr)
		{
			push(ptr->val);
			ptr = ptr->next;
		}
	}

	void push(const T& val)
	{
		Node* new_node = new Node(val);
		new_node->next = top_ptr;
		top_ptr = new_node;
	}
	const T& top()
	{
		if (top_ptr == nullptr)
		{
			throw std::exception("Stack Underflow");
		}
		return top_ptr->val;
	}
	void pop()
	{
		if (top_ptr == nullptr)
		{
			throw std::exception("Stack Underflow");
		}
		Node* temp = top_ptr;
		top_ptr = top_ptr->next;
		temp->next = nullptr;
		delete temp;
	}
	bool empty() const
	{
		return top_ptr == nullptr;
	}

private:
	Node* top_ptr = nullptr;
};
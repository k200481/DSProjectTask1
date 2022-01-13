#pragma once
#include "DSA.h"

template <typename T>
class ArrayQueue
{
public:
	ArrayQueue() = default;
	ArrayQueue(size_t size)
		:
		arr(size)
	{}

	void push(const T& val)
	{
		if (tail >= arr.cur_size())
		{
			arr.push_back(val);
			tail++;
			return;
		}
		arr[tail++] = val;
	}
	const T& front() const
	{
		if (tail == 0)
		{
			throw std::exception("Queue Underflow");
		}
		return arr[head];
	}
	void pop()
	{
		if (tail == 0)
		{
			throw std::exception("Queue Underflow");
		}
		
		tail--;
		for (size_t i = 0; i < tail; i++)
		{
			arr[i] = arr[i + 1];
		}
	}

	bool empty() const
	{
		return tail == 0;
	}

private:
	DSA<T> arr;
	size_t head = 0, tail = 0;
};

template <typename T>
class CircularQueue
{
public:
	CircularQueue(size_t size)
		:
		arr(size)
	{}

	void push(const T& val)
	{
		if (tail == 0)
		{
			arr[tail] = val;
		}
		else if (tail < arr.cur_size())
		{
			if (tail == head)
			{
				// full
				return;
			}
			arr[tail] = val;
		}
		else if (tail == arr.cur_size())
		{
			if (head == 0)
			{
				// full
				return;
			}
			tail = 0;
			arr[tail] = val;
		}
		tail++;
	}
	const T& front() const
	{
		if (empty())
		{
			throw std::exception("Queue Underflow");
		}
		return arr[head];
	}
	void pop()
	{
		if (head == 0)
		{
			if (tail == 0)
			{
				throw std::exception("Queue Underflow");
			}
			else if (tail == arr.cur_size())
			{
				head++;
				tail = 0;
				return;
			}
		}
		head = (head + 1) % arr.cur_size();
		if (head == tail)
		{
			head = 0;
			tail = 0;
		}
	}

	bool empty() const
	{
		return tail == 0 && head == 0;
	}

private:
	DSA<T> arr;
	size_t head = 0, tail = 0;
};

template <typename T>
class LinkedListQueue
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
	LinkedListQueue() = default;
	LinkedListQueue(const LinkedListQueue& rhs)
	{
		*this = rhs;
	}
	LinkedListQueue& operator=(const LinkedListQueue& rhs)
	{
		if (this == &rhs)
			return *this;
		delete head;
		
		Node* ptr = rhs.head;
		while (ptr != nullptr)
		{
			push(ptr->val);
			ptr = ptr->next;
		}
	}

	void push(const T& val)
	{
		if (head == nullptr)
		{
			head = new Node(val);
			tail = head;
			return;
		}
		tail->next = new Node(val);
		tail = tail->next;
	}
	const T& front() const
	{
		return head->val;
	}
	void pop()
	{
		if (head == tail)
		{
			delete head;
			head = nullptr;
			tail = nullptr;
			return;
		}
		Node* temp = head;
		head = head->next;
		temp->next = nullptr;
		delete temp;
	}

	bool empty() const
	{
		return head == nullptr;
	}

private:
	Node* head = nullptr;
	Node* tail = nullptr;
};
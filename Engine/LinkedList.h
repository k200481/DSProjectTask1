#pragma once
#include <cassert>

// equivalent to a forward list
template <typename T>
class SinglyLinkedList
{
	struct Node
	{
		T data;
		Node* next = nullptr;

		Node(const T& data)
			:
			data(data)
		{}
		~Node()
		{
			delete next;
		}
	};

public:
	class iterator
	{
		friend class SinglyLinkedList;
	private:
		iterator(Node* ptr)
			:
			ptr(ptr)
		{}
	public:
		iterator() = default;

		T* operator->()
		{
			assert(ptr != nullptr);
			return &ptr->data;
		}
		T& operator*()
		{
			assert(ptr != nullptr);
			return ptr->data;
		}

		iterator& operator++()
		{
			ptr = ptr->next;
			return *this;
		}

		bool operator==(const iterator& rhs)
		{
			return ptr == rhs.ptr;
		}
		bool operator!=(const iterator& rhs)
		{
			return !(*this == rhs);
		}

	private:
		Node* ptr = nullptr;
	};

	class const_iterator
	{
		friend class SinglyLinkedList;
	private:
		const_iterator(Node* ptr)
			:
			ptr(ptr)
		{}
	public:
		const_iterator() = default;

		const T* operator->() const
		{
			assert(ptr != nullptr);
			return &ptr->data;
		}
		const T& operator*() const
		{
			assert(ptr != nullptr);
			return ptr->data;
		}

		const_iterator& operator++()
		{
			ptr = ptr->next;
			return *this;
		}

		bool operator==(const const_iterator& rhs)
		{
			return ptr == rhs.ptr;
		}
		bool operator!=(const const_iterator& rhs)
		{
			return !(*this == rhs);
		}

	private:
		Node* ptr = nullptr;
	};

public:
	// ctor
	SinglyLinkedList() = default;
	// copy ctor
	SinglyLinkedList(const SinglyLinkedList& rhs)
	{
		*this = rhs;
	}
	// copy assignment
	SinglyLinkedList& operator=(const SinglyLinkedList& rhs)
	{
		if (this != &rhs)
		{
			delete first;
			for (auto& elem : rhs)
			{
				push_back(elem);
			}
		}

		return *this;
	}

	// append value
	void push_back(const T& data)
	{
		if (first == nullptr)
		{
			first = new Node(data);
			tail = first;
			return;
		}

		tail->next = new Node(data);
		tail = tail->next;
	}
	// prepend value
	void push_front(const T& data)
	{
		if (first == nullptr)
		{
			first = new Node(data);
			tail = first;
			return;
		}

		Node* ptr = new Node(data);
		ptr->next = first;
		first = ptr;
	}
	// delete last
	void pop_back()
	{
		if (first == tail)
		{
			delete first;
			first = nullptr;
			tail = nullptr;
			return;
		}

		Node* ptr = first;
		while (ptr->next != tail)
		{
			ptr = ptr->next;
		}
		ptr->next = nullptr;
		delete tail;
		tail = ptr;
	}
	// delete first
	void pop_front()
	{
		if (first == tail)
		{
			delete first;
			first = nullptr;
			tail = nullptr;
			return;
		}

		Node* temp = first;
		first = first->next;
		temp->next = nullptr;
		delete temp;
	}
	// first value
	T& front()
	{
		assert(!empty());
		return first->data;
	}
	const T& front() const
	{
		assert(!empty());
		return first->data;
	}
	// last value
	T& back()
	{
		assert(!empty());
		return tail->data;
	}
	const T& back() const
	{
		assert(!empty());
		return tail->data;
	}
	// num of nodes in list
	size_t size() const
	{
		size_t i = 0;
		for (auto& elem : *this)
		{
			i++;
		}
		return i;
	}
	// no value in the list
	bool empty() const
	{
		return first == nullptr;
	}

	iterator find(const T& val)
	{
		for (auto i = begin(); i != end(); ++i)
		{
			if (*i == val)
			{
				return i;
			}
		}
		return end();
	}
	
	// iterator to first element
	iterator begin()
	{
		return iterator(first);
	}
	// const iterator to first element
	const_iterator begin() const
	{
		return const_iterator(first);
	}
	// iterator past last element
	iterator end()
	{
		return iterator();
	}
	// const iterator past last element
	const_iterator end() const
	{
		return const_iterator();
	}

private:
	Node* first = nullptr;
	Node* tail = nullptr;
};
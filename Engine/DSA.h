#pragma once
#include <algorithm>
#include <stdexcept>
#include <cassert>

template <typename T>
class DSA
{
public:
	class iterator
	{
		friend class DSA;
	private:
		iterator(T* ptr)
			:
			ptr(ptr)
		{}
	public:
		iterator() = default;

		iterator& operator++()
		{
			ptr++;
			return *this;
		}

		T* operator->()
		{
			return ptr;
		}
		T& operator*()
		{
			return *ptr;
		}

		bool operator==(const iterator& rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator!=(const iterator& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		T* ptr = nullptr;
	};

	class const_iterator
	{
		friend class DSA;
	private:
		const_iterator(T* ptr)
			:
			ptr(ptr)
		{}
	public:
		const_iterator() = default;

		const_iterator& operator++()
		{
			ptr++;
			return *this;
		}
		const_iterator* operator--()
		{
			ptr--;
			return *this;
		}

		const T* operator->() const
		{
			return ptr;
		}
		const T& operator*() const
		{
			return *ptr;
		}

		bool operator==(const const_iterator& rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator!=(const const_iterator& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		T* ptr = nullptr;
	};

public:
	DSA()
		:
		arr(new T[max_size])
	{}
	DSA(size_t size)
		:
		max_size(size),
		cur_size(size),
		arr(new T[max_size])
	{}
	DSA(size_t size, const T& default_val)
		:
		DSA(size)
	{
		for (size_t i = 0; i < size; i++)
		{
			arr[i] = default_val;
		}
	}
	~DSA()
	{
		delete[] arr;
	}
	DSA(const DSA& rhs)
	{
		*this = rhs;
	}
	DSA& operator=(const DSA& rhs)
	{
		if (this == &rhs)
			return *this;
		delete arr;

		max_size = rhs.max_size;
		cur_size = rhs.cur_size;
		arr = new T[rhs.max_size];
		std::copy(rhs.arr, rhs.arr + rhs.cur_size, arr);
		return *this;
	}

	void push_back(const T& val)
	{
		if (cur_size == max_size)
		{
			resize(cur_size * 2);
		}
		arr[cur_size++] = val;
	}

	const T& operator[](size_t i) const
	{
		if (i < 0 || i >= cur_size)
		{
			throw std::out_of_range("Index out of range");
		}
		return arr[i];
	}
	T& operator[](size_t i)
	{
		if (i < 0 || i >= cur_size)
		{
			throw std::out_of_range("Index out of range");
		}
		return arr[i];
	}

	void resize(size_t new_size)
	{
		if (new_size <= max_size)
		{
			cur_size = new_size;
			return;
		}
		T* temp = arr;
		arr = new T[new_size];
		std::copy(temp, temp + cur_size, arr);
		delete[] temp;
		max_size = new_size;
	}

	size_t size() const
	{
		return cur_size;
	}
	size_t capacity() const
	{
		return max_size;
	}

	const T& front() const
	{
		assert(cur_size > 0);
		return arr[0];
	}
	const T& back() const
	{
		assert(cur_size > 0);
		return arr[cur_size - 1];
	}

	bool Has(const T& val) const
	{
		for (size_t i = 0; i < cur_size; i++)
		{
			if (arr[i] == val)
				return true;
		}
		return false;
	}

	iterator begin()
	{
		return iterator(arr);
	}
	const_iterator begin() const
	{
		return const_iterator(arr);
	}

	iterator end()
	{
		return iterator(arr + cur_size);
	}
	const_iterator end() const
	{
		return const_iterator(arr + cur_size);
	}

private:
	size_t max_size = 1;
	size_t cur_size = 0;
	T* arr = nullptr;
};
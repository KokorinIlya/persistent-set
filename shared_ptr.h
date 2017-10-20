#pragma once

#include <cstdio>
#include <cstddef>
#include <type_traits>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <utility>
#include <algorithm>

namespace cpp_course
{
	template <typename T>
	struct shared_ptr
	{
		shared_ptr() noexcept : ptr(nullptr), cnt(nullptr) {};

		explicit shared_ptr(T* _ptr) : ptr(_ptr), cnt(nullptr)
		{
			if (_ptr != nullptr)
			{
				cnt = new size_t(1);
			}
		}

		shared_ptr(shared_ptr const& other) noexcept : ptr(other.ptr), cnt(other.cnt)
		{
			if (ptr != nullptr) 
			{
				++(*cnt);
			}
		}

		shared_ptr(shared_ptr&& other) noexcept :  ptr(other.ptr), cnt(other.cnt)
		{
			other.ptr = nullptr;
			other.cnt = nullptr;
		}

		~shared_ptr()
		{
			if (cnt != nullptr)
			{
				--(*cnt);
				if (*cnt == 0)
				{
					delete ptr;
					delete cnt;
				}
			}
		}

		shared_ptr& operator=(shared_ptr const& other) noexcept
		{
			shared_ptr tmp(other);
			swap(tmp);
			return *this;
		}

		shared_ptr& operator=(shared_ptr&& other) noexcept
		{
			swap(other);
			return *this;
		}

		T* get() const noexcept
		{
			return ptr;
		}

		operator bool() const noexcept
		{
			return (ptr != nullptr);
		}

		void swap(shared_ptr& other) noexcept
		{
			using std::swap;
			swap(cnt, other.cnt);
			swap(ptr, other.ptr);
		}

		friend void swap(shared_ptr& a, shared_ptr& b) noexcept;

	private:
		T* ptr;
		size_t* cnt;
	};

	template<typename T>
	void swap(shared_ptr<T>& a, shared_ptr<T>& b) noexcept
	{
		a.swap(b);
	}
}
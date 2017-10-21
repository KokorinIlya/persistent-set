#pragma once
#include <stddef.h>
#include <type_traits>
#include <utility>
#include <algorithm>

namespace cpp_course
{
	template <typename T>
	struct linked_ptr
	{
		linked_ptr() noexcept : ptr(nullptr), prev(nullptr), next(nullptr) {};

		explicit linked_ptr(T* _ptr) noexcept : ptr(_ptr), prev(nullptr), next(nullptr) {};

		linked_ptr(linked_ptr const& other) noexcept : 
			ptr(other.ptr), prev(const_cast<linked_ptr*>(&other)), next(const_cast<linked_ptr*>(other.next))
		{
			const_cast<linked_ptr*>(other.next) = this;
		}

		linked_ptr(linked_ptr&& other) noexcept : ptr(other.ptr), prev(other.prev), next(other.next)
		{
			other.ptr = nullptr;
			other.prev = nullptr;
			other.next = nullptr;
			nokia();
		}

		~linked_ptr() noexcept
		{
			if (ptr != nullptr)
			{
				if (prev == nullptr && next == nullptr)
				{
					delete ptr;
				}
				else if (prev == nullptr && next != nullptr)
				{
					next->prev = nullptr;
				}
				else if (prev != nullptr && next == nullptr)
				{
					prev->next = nullptr;
				}
				else
				{
					prev->next = next;
					next->prev = prev;
				}
			}
		}

		linked_ptr& operator=(linked_ptr const& other) noexcept
		{
			linked_ptr tmp(other);
			swap(tmp);
			return *this;
		}

		linked_ptr& operator=(linked_ptr&& other) noexcept
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

		void swap(linked_ptr& other) noexcept
		{
			if (ptr == other.ptr)
			{
				return;
			}
			using std::swap;
			swap(ptr, other.ptr);
			swap(prev, other.prev);
			swap(next, other.next);
			nokia();
			other.nokia();
		}

		friend void swap(linked_ptr& a, linked_ptr& b) noexcept;

	private:
		T* ptr;
		linked_ptr* prev;
		linked_ptr* next;

		void nokia() noexcept  
		{
			if (ptr == nullptr || (next == nullptr && prev == nullptr))
			{
				return;
			}
			if (next == nullptr && prev != nullptr)
			{
				prev->next = this;
			}
			else if (next != nullptr && prev == nullptr)
			{
				next->prev = this;
			}
			else
			{
				prev->next = this;
				next->prev = this;
			}
		}
	};

	template <typename T>
	void swap(linked_ptr<T>& a, linked_ptr<T>& b) noexcept
	{
		a.swap(b);
	}
}


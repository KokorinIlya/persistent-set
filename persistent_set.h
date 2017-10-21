#pragma once
#include <utility>
#include <vector>
#include <algorithm>
#include <memory>
#include <cstdlib>
#include "linked_ptr.h"
#include "shared_ptr.h"

namespace cpp_course
{
	template <typename T, template<typename> typename PtrType = cpp_course::shared_ptr>
	struct persistent_set
	{
		typedef T value_type;

		struct iterator;

		friend struct iterator;
		friend struct persistent_set;

		persistent_set() : root(PtrType<node>(nullptr), PtrType<node>(nullptr)), _size(0) {};

		persistent_set(persistent_set const& other) noexcept :  root(other.root), _size(other._size) {};

		persistent_set(persistent_set&& other) noexcept : root(std::move(other.root)), _size(std::move(other._size)) {};

		~persistent_set() noexcept = default;

		persistent_set& operator=(persistent_set const& other) noexcept
		{
			persistent_set temp(other);
			swap(temp);
			return *this;
		}

		persistent_set& operator=(persistent_set&& other) noexcept
		{
			swap(other);
			return *this;
		}

		iterator find(T const& x) const noexcept;

		std::pair<iterator, bool> insert(T x);

		void erase(iterator const& it);

		iterator begin() const noexcept;
		iterator end() const noexcept;

		void swap(persistent_set& other) noexcept
		{
			root.left.swap(other.root.left);
			std::swap(_size, other._size);
		}

		bool has_element(T const& x) const noexcept
		{
			return (find(x) != end());
		}

		size_t size() const noexcept
		{
			return _size;
		}

		bool empty() const noexcept
		{
			return _size == 0;
		}

	private:
		struct node;

		struct base_node
		{
		public:
			friend struct persistent_set;

			base_node(base_node const& other) noexcept : left(other.left), right(other.right) {};

			base_node(base_node&& other) noexcept: left(std::move(other.left)), right(std::move(other.right)) {};

			base_node(PtrType<node> _left, PtrType<node> _right) noexcept : left(std::move(_left)), right(std::move(_right)) {};

			base_node& operator=(base_node const& other) noexcept
			{
				base_node temp(other);
				swap(temp);
				return *this;
			}

			base_node& operator=(base_node&& other) noexcept
			{
				swap(other);
				return *this;
			}

			void swap(base_node& other) noexcept
			{
				left.swap(other.left);
				right.swap(other.right);
			}

			friend void swap(base_node& a, base_node& b) noexcept;

		private:
			PtrType<node> left;
			PtrType<node> right;
		};

		struct node : base_node
		{
		public:
			friend struct persistent_set;

			explicit node(T _data) : base_node(PtrType<node>(nullptr), PtrType<node>(nullptr)), data(std::move(_data)) {}

			node(node const& other) noexcept(noexcept(T(other.data))) : base_node(other), data(other.data) {};

			node(node&& other) noexcept(noexcept(T(std::move(other.data)))) : 
				base_node(std::move(other)), data(std::move(other.data)) {}

			node(PtrType<node> _left, PtrType<node> _right, T _data) noexcept(noexcept(T(std::move(_data)))) :
				base_node(std::move(_left), std::move(_right)), data(std::move(_data)) {}

			node& operator=(node const& other) noexcept
			{
				node temp(other);
				swap(temp);
				return *this;
			}

			node& operator=(node&& other) noexcept
			{
				swap(other);
				return *this;
			}

			void swap(node& other) noexcept
			{
				using std::swap;
				base_node::swap(other);
				swap(data, other.data);
			}

			friend void swap(node& a, node& b) noexcept;

		private:
			T data;
		};

		base_node root;
		size_t _size;

		void get_path(std::vector<node*>& path, node* _root, T const &x) const noexcept;
		void get_left_path(std::vector<node*>& path, node* _root) const noexcept;
		PtrType<node> erase_impl(node* _root, T const& x);
		node* copy_path(std::vector<node*> const& path);
		base_node const* go_left(base_node const* _root) const noexcept;
		node const* find_impl(node const* _root, T const& x) const noexcept;
	};

	template<typename T, template <typename> typename PtrType>
	void swap(typename persistent_set<T, PtrType>::node& a, typename persistent_set<T, PtrType>::node& b) noexcept
	{
		a.swap(b);
	}

	template<typename T, template <typename> typename PtrType>
	void swap(typename persistent_set<T, PtrType>::base_node& a, typename persistent_set<T, PtrType>::base_node& b) noexcept
	{
		a.swap(b);
	}

	template <typename T, template<typename> typename PtrType>
	struct persistent_set<T, PtrType>::iterator
	{
		friend struct persistent_set<T, PtrType>;

		iterator() noexcept : root(nullptr), ptr(nullptr) {}

		T const& operator*() const
		{
			return (static_cast<node const*>(ptr))->data;
		}

		iterator& operator++();

		iterator operator++(int postfix)
		{
			iterator tmp(*this);
			++(*this);
			return tmp;
		}

		iterator& operator--();

		iterator operator--(int postfix)
		{
			iterator tmp(*this);
			--(*this);
			return tmp;
		}

		bool operator==(iterator const& other) const noexcept
		{
			return (ptr == other.ptr && root == other.root);
		}

		bool operator!=(iterator const& other) const noexcept
		{
			return !(*this == other);
		}
	private:
		iterator(base_node const* a, base_node const* b) noexcept : root(a), ptr(b) {}

		bool next(node const* vert, base_node const* prev, T const& x) noexcept;
		bool previous(node const* vert, base_node const* prev, T const& x) noexcept;
		base_node const* go_left(base_node* _root) const noexcept;
		base_node const* go_right(base_node* _root) const noexcept;

		base_node const* root;
		base_node const* ptr;
	};

	template <typename T, template<typename> typename PtrType>
	void swap(persistent_set<T, PtrType>& a, persistent_set<T, PtrType>& b) noexcept
	{
		a.swap(b);
	}

	template <typename T, template<typename> typename PtrType>
	bool persistent_set<T, PtrType>::iterator::next(node const* vert, base_node const* prev, T const& x) noexcept
	{
		bool ready = false;
		if (x < vert->data)
		{
			ready = next(vert->left.get(), vert, x);
		}
		else if (x > vert->data)
		{
			ready = next(vert->right.get(), vert, x);
		}
		else if (vert->right)
		{
			vert = static_cast<const node*>(go_left(vert->right.get()));
			ptr = vert;
			return true;
		}
		if (!ready && prev->left.get() == vert)
		{
			ptr = prev;
			return true;
		}
		return ready;
	}

	template <typename T, template<typename> typename PtrType>
	bool persistent_set<T, PtrType>::iterator::previous(node const* vert, base_node const* prev, T const& x) noexcept
	{
		bool ready = false;
		if (x < vert->data)
		{
			ready = previous(vert->left.get(), vert, x);
		}
		else if (x > vert->data)
		{
			ready = previous(vert->right.get(), vert, x);
		}
		else if (vert->left)
		{
			ptr = static_cast<const node*>(go_right(vert->left.get()));
			return true;
		}
		if (!ready && prev->right.get() == vert)
		{
			ptr = prev;
			return true;
		}
		return ready;
	}

	template <typename T, template<typename> typename PtrType>
	typename persistent_set<T, PtrType>::iterator& persistent_set<T, PtrType>::iterator::operator++()
	{
		if (root == ptr)
		{
			throw std::runtime_error("No next element");
		}
		next(root->left.get(), root, (static_cast<const node*>(ptr))->data);
		return *this;
	}

	template <typename T, template<typename> typename PtrType>
	typename persistent_set<T, PtrType>::iterator& persistent_set<T, PtrType>::iterator::operator--()
	{
		if (root == ptr)
		{
			if (root->left.get() == nullptr)
			{
				throw std::runtime_error("No previous element");
			}
			ptr = go_right(root->left.get());
		}
		else
		{
			previous(root->left.get(), root, (static_cast<const node*>(ptr))->data);
		}
		return *this;
	}

	template <typename T, template <typename> class PtrType>
	typename persistent_set<T, PtrType>::base_node const* persistent_set<T, PtrType>::iterator::go_left(base_node* _root) const noexcept
	{
		if (_root->left.get() != nullptr)
		{
			return go_left(_root->left.get());
		}
		return _root;
	}

	template <typename T, template <typename> class PtrType>
	typename persistent_set<T, PtrType>::base_node const* persistent_set<T, PtrType>::iterator::go_right(base_node* _root) const noexcept
	{
		if (_root->right.get() != nullptr)
		{
			return go_right(_root->right.get());
		}
		return _root;
	}

	template <typename T, template<typename> typename PtrType>
	typename persistent_set<T, PtrType>::iterator persistent_set<T, PtrType>::begin() const noexcept
	{
		return iterator(&root, go_left(&root));
	}

	template <typename T, template<typename> typename PtrType>
	typename persistent_set<T, PtrType>::iterator persistent_set<T, PtrType>::end() const noexcept
	{
		return iterator(&root, &root);
	}

	template<typename T, template<typename> typename PtrType>
	typename persistent_set<T, PtrType>::iterator persistent_set<T, PtrType>::find(T const& x) const noexcept
	{
		if (root.left.get() == nullptr)
		{
			return iterator(&root, &root);
		}
		node const* res = find_impl(root.left.get(), x);
		if (res != nullptr)
		{
			return iterator(&root, res);
		}
		return iterator(&root, &root);
	}

	template<typename T, template<typename> typename PtrType>
	void persistent_set<T, PtrType>::get_path(std::vector<node*>& path, node* _root, T const& x) const noexcept
	{
		if (_root == nullptr)
		{
			return;
		}
		path.push_back(_root);
		if (x < _root->data)
		{
			get_path(path, _root->left.get(), x);
		}
		if (x > _root->data)
		{
			get_path(path, _root->right.get(), x);
		}
	}

	template <typename T, template <typename> class PtrType>
	void persistent_set<T, PtrType>::get_left_path(std::vector<node*>& path, node* _root) const noexcept
	{
		if (_root == nullptr)
		{
			return;
		}
		path.push_back(_root);
		get_left_path(path, _root->left.get());
	}

	template <typename T, template <typename> class PtrType>
	PtrType<typename persistent_set<T, PtrType>::node> persistent_set<T, PtrType>::erase_impl(node* _root, T const& x)
	{
		if (x < _root->data)
		{
			return PtrType<node>(new node(erase_impl(_root->left.get(), x), _root->right, _root->data));
		}
		if (x > _root->data)
		{
			return PtrType<node>(new node(_root->left, erase_impl(_root->right.get(), x), _root->data));
		}
		if (!_root->left && !_root->right)
		{
			return PtrType<node>(nullptr);
		}
		if (_root->left && !_root->right)
		{
			return _root->left;
		}
		if (!_root->left && _root->right)
		{
			return _root->right;
		}
		std::vector<node*> path;
		get_left_path(path, _root->right.get());
		value_type const& data_for_swap = path.back()->data;
		if (path.size() == 1)
		{
			return PtrType<node>(new node(_root->left, path.back()->right, data_for_swap));
		}
		node* last = new node(path.back()->right, path[path.size() - 2]->right, path[path.size() - 2]->data);
		path.pop_back();
		path.pop_back();
		path.push_back(last);
		std::reverse(path.begin(), path.end());
		last = copy_path(path);
		return PtrType<node>(new node(_root->left, PtrType<node>(last), data_for_swap));
	}

	template <typename T, template <typename> class PtrType>
	typename persistent_set<T, PtrType>::node* persistent_set<T, PtrType>::copy_path(std::vector<node*> const& path)
	{
		node* last = path[0];
		for (size_t i = 1; i < path.size(); i++)
		{
			if (last->data < path[i]->data)
			{
				last = new node(PtrType<node>(last), path[i]->right, path[i]->data);
			}
			else
			{
				last = new node(path[i]->left, PtrType<node>(last), path[i]->data);
			}
		}
		return last;
	}

	template <typename T, template <typename> class PtrType>
	typename persistent_set<T, PtrType>::base_node const* persistent_set<T, PtrType>::go_left(base_node const* _root) const noexcept
	{
		if (_root->left.get() == nullptr)
		{
			return _root;
		}
		return go_left(_root->left.get());
	}

	template <typename T, template <typename> class PtrType>
	typename persistent_set<T, PtrType>::node const* persistent_set<T, PtrType>::find_impl(node const* _root, T const& x) const noexcept
	{
		if (_root == nullptr || _root->data == x)
		{
			return _root;
		}
		if (x < _root->data)
		{
			return find_impl(_root->left.get(), x);
		}
		return find_impl(_root->right.get(), x);
	}

	template<typename T, template<typename> typename PtrType>
	std::pair<typename persistent_set<T, PtrType>::iterator, bool> persistent_set<T, PtrType>::insert(T x)
	{
		iterator it = find(x);
		if (it != end())
		{
			return { it, false };
		}
		++_size;
		std::vector<node*> path;
		get_path(path, root.left.get(), x);

		node* last = new node(std::move(x));
		path.push_back(last);
		std::reverse(path.begin(), path.end());
		last = copy_path(path);
		root.left = PtrType<node>(last);

		it.ptr = last;
		return { it, true };
	}

	template<typename T, template<typename> typename PtrType>
	void persistent_set<T, PtrType>::erase(iterator const& it)
	{
		if (it == end())
		{
			return;
		}
		--_size;
		root.left = erase_impl(root.left.get(), *it);
	}
}

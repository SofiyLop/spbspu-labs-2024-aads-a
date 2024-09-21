#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <cassert>
#include <string>

#include <fstream>
#include <functional>
#include <map>
#include <limits>
#include "tree_node.hpp"
#include "tree_iterator.hpp"
#include "const_tree_iterator.hpp"


namespace lopatina
{
  template < typename Key, typename Value, typename Compare = std::less< Key > >
  class Tree
  {
  public:
    using iterator = TreeIterator<Key, Value, Compare>;
    using const_iterator = ConstTreeIterator<Key, Value, Compare>;

    Tree();
    Tree(std::initializer_list< std::pair< Key, Value > > init_list):
      Tree()
    {
      for (auto it = init_list.begin(); it != init_list.end(); ++it)
      {
        insert(*it);
      }
    }
    template < class InputIterator >
    Tree(InputIterator first, InputIterator last):
      Tree()
	{
		for (auto it = first; it != last; ++it)
		{
			insert(*it);
		}
	}
	Tree(const Tree & other):
		Tree()
	{
		TreeNode< Key, Value > * newroot = other.root_;
		for (auto it = other.cbegin(); it != other.cend(); ++it)
		{
			push((*it).first, (*it).second);
		}
	}
	Tree(Tree && other) noexcept:
		root_(other.root_),
		size_(other.size_),
		comp_(other.comp_)
	{
		other.root_ = nullptr;
		other.size_ = 0;
	}

	~Tree()
	{
		clear();
	}

	Tree & operator=(const Tree & other)
	{
		if (this != std::addressof(other))
		{
			Tree copy{ other };
			swap(copy);
		}
		return *this;
	}
	Tree & operator=(Tree && other) noexcept
	{
		if (this != std::addressof(other))
		{
			clear();
			swap(other);
		}
		return *this;
	}

	Value & operator[](const Key & k)
	{
		TreeNode< Key, Value > * node = search(k);
		if (node)
		{
			return node->data_.second;
		}
		else
		{
			push(k, Value());
			return search(k)->data_.second;
		}
	}

	bool empty() const noexcept;
	size_t size() const noexcept;

	Value & at(const Key & k)
	{
		TreeNode< Key, Value > * node = search(k);
		if (node)
		{
			return node->data_.second;
		}
		throw std::out_of_range("There is no such key");
	}
	const Value & at(const Key & k) const
	{
		TreeNode< Key, Value > * node = search(k);
		if (node)
		{
			return node->data_.second;
		}
		throw std::out_of_range("There is no such key");
	}

	TreeIterator< Key, Value, Compare > find(const Key & k)
	{
		return iterator(search(k));
	}
	ConstTreeIterator< Key, Value, Compare > find(const Key & k) const
	{
		return const_iterator(search(k));
	}

	void swap(Tree & other)
	{
		TreeNode< Key, Value > * subroot = root_;
		size_t subsize = size_;
		Compare subcomp = comp_;
		root_ = other.root_;
		size_ = other.size_;
		comp_ = other.comp_;
		other.root_ = subroot;
		other.size_ = subsize;
		other.comp_ = subcomp;
	}

	//-------------
	void push(const Key key, const Value val)
	{
		TreeNode< Key, Value > * node = root_;
		TreeNode< Key, Value > * parent = nullptr;
		while (node)
		{
			if (node->data_.first == key)
			{
				node->data_.second = val;
				return;
			}
			else if (comp_(key, node->data_.first))
			{
				parent = node;
				node = node->left_;
			}
			else
			{
				parent = node;
				node = node->right_;
			}
		}
		TreeNode< Key, Value > * newnode = new TreeNode< Key, Value >(key, val);
		if (parent)
		{
			if (comp_(key, parent->data_.first))
			{
				parent->left_ = newnode;
				newnode->parent_ = parent;
			}
			else
			{
				parent->right_ = newnode;
				newnode->parent_ = parent;
			}
		}
		else
		{
			root_ = newnode;
		}
		++size_;
	}

	//-------
/*
	size_t getHeight(const TreeNode< Key, Value > * node)
	{
		if (node)
		{
			return node->height_;
		}
		return 0;
	}

	size_t balanceFactor(const TreeNode< Key, Value > * node)
	{
		if (node)
		{
			return height(node->left_) - height(node->right_);
		}
		return 0;
	}

	void updateHeight(TreeNode< Key, Value > * node)
	{
		if (node)
		{
			node->height_ = std::max(getHeight(node->left_), getHeight(node->right_)) + 1;
		}
	}

	void leftRotation(TreeNode< Key, Value > * node)
	{

	}
	void rightRotation(TreeNode< Key, Value > * node)
	{

	}

	//------
*/

	Value get(Key get_key)
	{
		TreeNode< Key, Value > * node = root_;
		while (node)
		{
			Key node_key = node->data_.first;
			if (node_key == get_key)
			{
				return node->data_.second;
			}
			else if (comp_(get_key, node_key))
			{
				node = node->left_;
			}
			else
			{
				node = node->right_;
			}
		}
		throw std::out_of_range("There is no such key");
	}

	TreeNode< Key, Value > * search(const Key get_key) const
	{
		TreeNode< Key, Value > * node = root_;
		while (node)
		{
			Key node_key = node->data_.first;
			if (node_key == get_key)
			{
				return node;
			}
			else if (comp_(get_key, node_key))
			{
				node = node->left_;
			}
			else
			{
				node = node->right_;
			}
		}
		return nullptr;
	}

	TreeIterator< Key, Value, Compare > begin() noexcept
	{
		if (!root_)
		{
			return iterator(root_);
		}
		TreeNode< Key, Value > * node = root_;
		while (node->left_)
		{
			node = node->left_;
		}
		return iterator(node);
	}

	TreeIterator< Key, Value, Compare > end() noexcept
	{
		if (!root_)
		{
			return iterator(root_);
		}
		return iterator(root_->parent_);
	}

	ConstTreeIterator< Key, Value, Compare > begin() const noexcept
	{
		if (!root_)
		{
			return const_iterator(root_);
		}
		TreeNode< Key, Value > * node = root_;
		while (node->left_)
		{
			node = node->left_;
		}
		return const_iterator(node);
	}

	ConstTreeIterator< Key, Value, Compare > end() const noexcept
	{
		if (!root_)
		{
			return const_iterator(root_);
		}
		return const_iterator(root_->parent_);
	}

	ConstTreeIterator< Key, Value, Compare > cbegin() const noexcept
	{
		if (!root_)
		{
			return const_iterator(root_);
		}
		TreeNode< Key, Value > * node = root_;
		while (node->left_)
		{
			node = node->left_;
		}
		return const_iterator(node);
	}

	ConstTreeIterator< Key, Value, Compare > cend() const noexcept
	{
		if (!root_)
		{
			return const_iterator(root_);
		}
		return const_iterator(root_->parent_);
	}

	void clear_one(TreeNode< Key, Value > * todelete)
	{
		if (!todelete)
		{
			return;
		}
		if (!(todelete->left_) && !(todelete->right_))
		{
			if (todelete->parent_)
			{
				if (todelete->parent_->left_ == todelete)
				{
					todelete->parent_->left_ = nullptr;
				}
				else
				{
					todelete->parent_->right_ = nullptr;
				}
			}
			else
			{
				root_ = nullptr;
			}
			delete todelete;
			todelete = nullptr;
			--size_;
			return;
		}
		else if (todelete->left_ && todelete->right_)
		{
			TreeNode< Key, Value > * node = todelete->right_;
			while (node->left_)
			{
				node = node->left_;
			}
			if (node->parent_ == todelete)
			{
				node->parent_ = todelete->parent_;
				node->left_ = todelete->left_;
				todelete->left_->parent_ = node;
				if (root_ == todelete)
				{
					root_ = node;
				}
				else if (todelete->parent_->left_ == todelete)
				{
					todelete->parent_->left_ = node;
				}
				else
				{
					todelete->parent_->right_ = node;
				}
				delete todelete;
				todelete = nullptr;
				--size_;
				return;
			}
			else
			{
				node->parent_->left_ = node->right_;
				if (node->right_)
				{
					node->right_->parent_ = node->parent_;
				}
				node->left_ = todelete->left_;
				if (todelete->left_)
				{
					todelete->left_->parent_ = node;
				}
				node->right_ = todelete->right_;
				if (todelete->right_)
				{
					todelete->right_->parent_ = node;
				}
				if (root_ == todelete)
				{
					root_ = node;
				}
				else if (todelete->parent_->left_ == todelete)
				{
					todelete->parent_->left_ = node;
				}
				else
				{
					todelete->parent_->right_ = node;
				}
				node->parent_ = todelete->parent_;
				delete todelete;
				todelete = nullptr;
				--size_;
				return;
			}
		}
		else if (todelete->left_ || todelete->right_)
		{
			TreeNode< Key, Value > * node = nullptr;
			if (todelete->left_)
			{
				node = todelete->left_;
			}
			else
			{
				node = todelete->right_;
			}
			if (root_ == todelete)
			{
				root_ = node;
			}
			else if (todelete->parent_->left_ == todelete)
			{
				todelete->parent_->left_ = node;
			}
			else
			{
				todelete->parent_->right_ = node;
			}
			node->parent_ = todelete->parent_;
			delete todelete;
			todelete = nullptr;
			--size_;
			return;
		}
	}

	void clear() noexcept
	{
		while (root_)
		{
			clear_one(root_);
		}
	}

	// second group
	// insert single
	std::pair< TreeIterator< Key, Value, Compare >, bool > insert(const std::pair< Key, Value > & val)
	{
		iterator iter = find(val.first);
		if (iter != end())
		{
			return std::pair< iterator, bool >(iter, false);
		}
		push(val.first, val.second);
		return std::pair< iterator, bool >(find(val.first), true);
	}

	template <class InputIterator>
	void insert(InputIterator first, InputIterator last)
	{
		for (auto it = first; it != last; ++it)
		{
			insert(*it);
		}
	}

	TreeIterator< Key, Value, Compare > erase(iterator position)
	{
		if (position != end())
		{
			iterator next_pos = position;
			++next_pos;
			clear_one(position.node_);
			return next_pos;
		}
	}

	size_t erase(const Key & k)
	{
		size_t size_before = size_;
		iterator iter = find(k);
		if (iter == end())
		{
			return 0;
		}
		while (iter != end())
		{
			erase(iter);
			iter = find(k);
		}
		return size_before - size_;
	}

	TreeIterator< Key, Value, Compare > erase(iterator first, iterator last)
	{
		iterator iter = first;
		while (iter != last)
		{
			iter = erase(iter);
		}
		return iter;
	}

	size_t count(const Key & k) const
	{
		if (search(k))
		{
			return 1;
		}
		return 0;
	}

	TreeIterator<Key, Value, Compare> lower_bound(const Key & k)
	{
		for (auto iter = begin(); iter != end(); ++iter)
		{
			if (comp_(iter.node_->data_.first, k) == false)
			{
				return iter;
			}
		}
		return end();
	}
	ConstTreeIterator<Key, Value, Compare> lower_bound(const Key & k) const
	{
		for (auto iter = begin(); iter != end(); ++iter)
		{
			if (comp_(iter.node_->data_.first, k) == false)
			{
				return iter;
			}
		}
		return end();
	}

	TreeIterator<Key, Value, Compare> upper_bound(const Key & k)
	{
		for (auto iter = begin(); iter != end(); ++iter)
		{
			if (comp_(k, iter.node_->data_.first) == true)
			{
				return iter;
			}
		}
		return end();
	}
	ConstTreeIterator<Key, Value, Compare> upper_bound(const Key & k) const
	{
		for (auto iter = begin(); iter != end(); ++iter)
		{
			if (comp_(k, iter.node_->data_.first) == true)
			{
				return iter;
			}
		}
		return end();
	}

	std::pair<ConstTreeIterator<Key, Value, Compare>, ConstTreeIterator<Key, Value, Compare>> equal_range(const Key & k) const
	{
		return std::pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
	}
	std::pair<TreeIterator<Key, Value, Compare>, TreeIterator<Key, Value, Compare>> equal_range(const Key & k)
	{
		return std::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
	}

private:
	TreeNode< Key, Value > * root_;
	Compare comp_;
	size_t size_;
};

template < typename Key, typename Value, typename Compare >
Tree< Key, Value, Compare >::Tree():
	root_(nullptr)
{}

template < typename Key, typename Value, typename Compare >
bool Tree< Key, Value, Compare >::empty() const noexcept
{
	return root_ == nullptr;
}
template < typename Key, typename Value, typename Compare >
size_t Tree< Key, Value, Compare >::size() const noexcept
{
	return size_;
}
}

#endif

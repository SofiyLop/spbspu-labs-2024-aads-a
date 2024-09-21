#ifndef CONST_TREE_ITERATOR_HPP
#define CONST_TREE_ITERATOR_HPP

#include <iterator>
#include <cassert>
#include "tree_node.hpp"

namespace lopatina
{
  template < typename Key, typename Value, typename Compare = std::less< Key > >
  struct ConstTreeIterator: public std::iterator< std::bidirectional_iterator_tag, Value >
  {
    friend class Tree<Key, Value, Compare>;
    friend struct TreeIterator<Key, Value, Compare>;
    public:
    using this_t = ConstTreeIterator< Key, Value, Compare >;
    ConstTreeIterator():
      node_(nullptr)
    {}
    ConstTreeIterator(TreeNode< Key, Value > * node):
      node_(node)
    {}
    ~ConstTreeIterator() = default;

    ConstTreeIterator(const this_t &) = default;
    this_t & operator=(const this_t &) = default;

    this_t & operator++()
    {
      assert(node_ != nullptr);
      if (node_->right_)
      {
        node_ = node_->right_;
        while (node_->left_)
        {
          node_ = node_->left_;
        }
        return *this;
      }
      else
      {
        while ((node_->parent_) && (node_->parent_->right_ == node_))
        {
          node_ = node_->parent_;
        }
        node_ = node_->parent_;
        return *this;
      }
    }
    this_t operator++(int)
    {
      assert(node_ != nullptr);
      this_t result(*this);
      ++(*this);
      return result;
    }

    this_t & operator--()
    {
      assert(node_ != nullptr);
      if (node_->left_)
      {
        node_ = node_->left_;
        while (node_->right_)
        {
          node_ = node_->right_;
        }
        return *this;
      }
      else
      {
        while ((node_->parent_) && (node_->parent_->left_ == node_))
        {
          node_ = node_->parent_;
        }
        node_ = node_->parent_;
        return *this;
      }
    }
    this_t operator--(int)
    {
      assert(node_ != nullptr);
      this_t result(*this);
      --(*this);
      return result;
    }

    const std::pair< Key, Value > & operator*() const
    {
      assert(node_ != nullptr);
      return node_->data_;
    }
    const std::pair< Key, Value > * operator->() const
    {
      assert(node_ != nullptr);
      return std::addressof(node_->data_);
    }

    bool operator==(const this_t & rhs) const
    {
      return node_ == rhs.node_;
    }
    bool operator!=(const this_t & rhs) const
    {
      return !(rhs == *this);
    }

  private:
    TreeNode< Key, Value > * node_;
  };
}

#endif

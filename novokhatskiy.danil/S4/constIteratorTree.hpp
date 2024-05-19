#ifndef CONST_ITERATOR_TREE
#define CONST_ITERATOR_TREE

#include <cassert>
#include <iterator>
#include <functional>
#include <utility>
#include "AVLtreeNode.hpp"
#include "IteratorTree.hpp"

namespace novokhatskiy
{
  template< class Key, class Value, class Compare >
  struct IteratorTree;

  template< class Key, class Value, class Compare >
  class Tree;

  template< class Key, class Value, class Compare  = std::less< Key > >
  struct ConstIteratorTree: public std::iterator< std::bidirectional_iterator_tag, Value >
  {
    using node_t = detail::NodeTree< Key, Value >;
    using constIter = ConstIteratorTree< Key, Value, Compare >;
    using iter = IteratorTree< Key, Value, Compare >;

    ConstIteratorTree():
      node_(nullptr)
    {}

    explicit ConstIteratorTree(node_t* node):
      node_(node)
    {}
    constIter& operator=(const constIter& other) = default;

    bool operator!=(const constIter& other) const
    {
      return !(*this == other);
    }

    bool operator==(const constIter& other) const
    {
      return node_ == other.node_;
    }

    constIter operator++()
    {
      if (node_->right)
      {
        node_ = node_->right;
        while (node_->left)
        {
          node_ = node_->left;
        }
        //goLastLeft();
        return *this;
      }
      else
      {
        while (node_->parent && node_->parent->right == node_)
        {
          node_ = node_->parent;
        }
        node_ = node_->parent;
        return *this;
      }
    }

    constIter operator++(int)
    {
      constIter tmp(*this);
      ++(*this);
      return tmp;
    }

    constIter operator--()
    {
      if (node_->left)
      {
        node_ = node_->left;
        for (; node_->right; node_ = node_->right);
      }
      else
      {
        for (; node_ == node_->parent->left; node_ = node_->parent);
        node_ = node_->parent;
      }
      return *this;
    }

    constIter operator--(int)
    {
      constIter tmp(*this);
      --(*this);
      return tmp;
    }

    const Key& getKey() const
    {
      return node_->value.first;
    }

    const std::pair< Key, Value > operator*() const
    {
      assert(node_ != nullptr);
      return node_->value;
    }

    const std::pair< Key, Value >* operator->() const
    {
      assert(node_ != nullptr);
      return std::addressof(node_->value);
    }

    bool operator==(const iter it) const
    {
      return node_ == it.node_;
    }

    bool operator!=(const iter it) const
    {
      return node_ != it.node_;
    }

  private:
    node_t* node_;
    friend class Tree< Key, Value, Compare >;
    friend struct IteratorTree< Key, Value, Compare >;

    void goLastRight()
    {
      auto tmp = node_->right;
      while (tmp->right)
      {
        tmp = tmp->right;
      }
      node_ = tmp;
    }

    void goLastLeft()
    {
      auto tmp = node_;
      while (tmp->left)
      {
        tmp = tmp->left;
      }
      node_ = tmp;
    }

    node_t* predecessor(node_t* node)
    {
      if (node->left)
      {
        goLastRight(node->left);
      }
      node_t* tmp = node->parent;
      while (tmp && node == tmp->left)
      {
        node = tmp;
        tmp = tmp->parent;
      }
      return tmp;
    }
  };
}

#endif

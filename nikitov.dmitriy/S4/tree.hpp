#ifndef TREE_HPP
#define TREE_HPP

#include "tree_node.hpp"
#include <functional>
#include "const_tree_iterator.hpp"

namespace nikitov
{
  template< class Key, class T, class Compare = std::less< Key > >
  class Tree
  {
  public:
    Tree();
    ~Tree();

    T& operator[](const Key& key);
    T& at(const Key& key);

    ConstTreeIterator< Key, T, Compare > cbegin() const;
    ConstTreeIterator< Key, T, Compare > cend() const;

    bool empty() const;
    size_t size() const;

    void insert(const std::pair< Key, T >& value);
    void clear();
    void swap(Tree< Key, T, Compare >& other);
    ConstTreeIterator< Key, T, Compare > find(const Key& key) const;

  private:
    detail::TreeNode< Key, T, Compare >* root_;
    size_t size_;
    Compare cmp_;

    detail::TreeNode< Key, T, Compare >* search(detail::TreeNode< Key, T, Compare >* node, const Key& key) const;
    detail::TreeNode< Key, T, Compare >* findNode(const Key& value) const;
  };

  template< class Key, class T, class Compare >
  Tree< Key, T, Compare >::Tree() :
    root_(new detail::TreeNode< Key, T, Compare >()),
    size_(0),
    cmp_(Compare())
  {}

  template< class Key, class T, class Compare >
  Tree< Key, T, Compare >::~Tree()
  {
    clear();
    delete root_;
  }

  template< class Key, class T, class Compare >
  T& Tree< Key, T, Compare >::operator[](const Key& key)
  {
    detail::TreeNode< Key, T, Compare >* node = search(root_, key);
    if (node)
    {
      return node->get(key);
    }
    insert({ key, T() });
    node = search(root_, key);
    return node->get();
  }

  template< class Key, class T, class Compare >
  T& Tree< Key, T, Compare >::at(const Key& key)
  {
    detail::TreeNode< Key, T, Compare >* node = search(root_, key);
    if (node)
    {
      return node->get(key);
    }
    else
    {
      throw std::out_of_range("Error: No element by key");
    }
  }

  template< class Key, class T, class Compare >
  ConstTreeIterator< Key, T, Compare > Tree< Key, T, Compare >::cbegin() const
  {
    ConstTreeIterator< Key, T, Compare > iterator(root_);
    iterator.fallLeft();
    return iterator;
  }

  template< class Key, class T, class Compare >
  ConstTreeIterator< Key, T, Compare > Tree< Key, T, Compare >::cend() const
  {
    if (empty())
    {
      return ConstTreeIterator< Key, T, Compare >(root_);
    }
    else
    {
      return ConstTreeIterator< Key, T, Compare >(root_->parent_);
    }
  }

  template< class Key, class T, class Compare >
  bool Tree< Key, T, Compare >::empty() const
  {
    return !size_;
  }

  template< class Key, class T, class Compare >
  void Tree< Key, T, Compare >::clear()
  {
    root_->clear();
    if (root_->parent_)
    {
      root_ = root_->parent_;
    }
    size_ = 0;
  }

  template< class Key, class T, class Compare >
  void Tree< Key, T, Compare >::swap(Tree< Key, T, Compare >& other)
  {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
    std::swap(cmp_, other.cmp_);
  }

  template< class Key, class T, class Compare >
  size_t Tree< Key, T, Compare >::size() const
  {
    return size_;
  }

  template< class Key, class T, class Compare >
  void Tree< Key, T, Compare >::insert(const std::pair< Key, T >& value)
  {
    if (empty())
    {
      root_->middle_ = new detail::TreeNode< Key, T, Compare >(value);
      root_->middle_->parent_ = root_;
      root_ = root_->middle_;
    }
    else
    {
      detail::TreeNode< Key, T, Compare >* newRoot = findNode(value.first)->add(value);
      if (newRoot)
      {
        root_ = newRoot;
      }
    }
    ++size_;
  }

  template< class Key, class T, class Compare >
  ConstTreeIterator< Key, T, Compare > Tree< Key, T, Compare >::find(const Key& key) const
  {
    detail::TreeNode< Key, T, Compare >* node = search(root_, key);
    if (node)
    {
      ConstTreeIterator< Key, T, Compare > iterator(node);
      if (!(iterator.node_->firstValue_.first == key))
      {
        iterator.isFirst_ = false;
      }
      return iterator;
    }
    return cend();
  }

  template< class Key, class T, class Compare >
  detail::TreeNode< Key, T, Compare >* Tree< Key, T, Compare >::search(detail::TreeNode< Key, T, Compare >* node, const Key& key) const
  {
    if (!node)
    {
      return nullptr;
    }
    if (node->find(key))
    {
      return node;
    }
    else if (cmp_(key, node->firstValue_.first))
    {
      return search(node->left_, key);
    }
    else if (node->size_ == 1 || cmp_(node->secondValue_.first, key))
    {
      return search(node->right_, key);
    }
    else
    {
      return search(node->middle_, key);
    }
  }

  template< class Key, class T, class Compare >
  detail::TreeNode< Key, T, Compare >* Tree< Key, T, Compare >::findNode(const Key& value) const
  {
    detail::TreeNode< Key, T, Compare >* node = root_;
    while (node->left_ || node->right_ || node->middle_)
    {
      if (cmp_(value, node->firstValue_.first))
      {
        node = node->left_;
      }
      else if (cmp_(node->secondValue_.first, value))
      {
        node = node->right_;
      }
      else
      {
        node = node->middle_;
      }
    }
    return node;
  }
}
#endif

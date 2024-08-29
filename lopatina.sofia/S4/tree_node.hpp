#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP

namespace lopatina
{
  template < typename Key, typename Value >
  struct TreeNode
  {
    std::pair< Key, Value > data_;
    TreeNode< Key, Value > * left_;
    TreeNode< Key, Value > * right_;
    TreeNode< Key, Value > * parent_;

    TreeNode(const Key & key, const Value & val) :
      left_(nullptr),
      right_(nullptr),
      parent_(nullptr)
    {
      data_ = std::make_pair(key, val);
    }
  };
}

#endif

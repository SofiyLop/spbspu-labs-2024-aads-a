#include <iostream>
#include <cassert>
#include <string>

// tree_node.hpp

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

// сщтые_tree_iterator.hpp
template < typename Key, typename Value >
struct ConstTreeIterator : public std::iterator< std::bidirectional_iterator_tag, Value >
{
public:
  using this_t = ConstTreeIterator< Key, Value >;
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

// tree_iterator.hpp

template < typename Key, typename Value >
struct TreeIterator: public std::iterator< std::bidirectional_iterator_tag, Value >
{
public:
  using this_t = TreeIterator< Key, Value >;
  TreeIterator():
    node_(nullptr)
  {}
  TreeIterator(TreeNode< Key, Value > * node):
    node_(node)
  {}
  ~TreeIterator() = default;

  TreeIterator(const this_t &) = default;
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

  std::pair< Key, Value > & operator*()
  {
    assert(node_ != nullptr);
    return node_->data_;
  }
  std::pair< Key, Value > * operator->()
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

// tree.hpp

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "Wrong parameters\n";
    return 1;
  }
  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "No such file\n";
    return 1;
  }
  // считывание данных из файла в деревья функция input



  std::map< std::string, std::function< void(std::istream &, std::ostream &) > > cmds;
  {
    using namespace std::placeholders;
    cmds["print"] = std::bind(lopatina::areaCmd, figures, _1, _2);
    cmds["complement"] = std::bind(lopatina::maxCmd, figures, _1, _2);
    cmds["intersect"] = std::bind(lopatina::minCmd, figures, _1, _2);
    cmds["union"] = std::bind(lopatina::countCmd, figures, _1, _2);
  }
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;

}

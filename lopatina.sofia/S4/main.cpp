#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <functional>
//
#include <map>
#include <limits>
//
#include "tree_node.hpp"
#include "tree_iterator.hpp"
#include "const_tree_iterator.hpp"
#include "tree.hpp"

void printCmd(const std::map< std::string, std::map< int, std::string > > & trees, std::istream & in, std::ostream & out)
{
  std::string tree_name = "";
  in >> tree_name;
  if (!in || trees.find(tree_name) == trees.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::map< int, std::string > tree = trees.at(tree_name);
  if (tree.empty())
  {
    throw std::logic_error("<EMPTY>");
  }
  out << tree_name;
  for (auto iter = tree.begin(); iter != tree.end(); ++iter)
  {
    out << " " << (*iter).first << " " << (*iter).second;
  }
  out << '\n';
}

void complementCmd(std::map< std::string, std::map< int, std::string > > & trees, std::istream & in)
{
  std::string newdataset = "", dataset1 = "", dataset2 = "";
  in >> newdataset >> dataset1 >> dataset2;
  if (!in || trees.find(dataset1) == trees.end() || trees.find(dataset2) == trees.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::map< int, std::string > complement_tree = trees.at(dataset1);
  std::map< int, std::string > tree2 = trees.at(dataset2);
  for (auto iter = tree2.begin(); iter != tree2.end(); ++iter)
  {
    int key = (*iter).first;
    if (complement_tree.find(key) != complement_tree.end())
    {
      complement_tree.erase(key);
    }
    else
    {
      complement_tree.insert(std::make_pair(key, (*iter).second));
    }
  }
  trees.insert(std::make_pair(newdataset, complement_tree));
}

void intersectCmd(std::map< std::string, std::map< int, std::string > > & trees, std::istream & in)
{
  std::string newdataset = "", dataset1 = "", dataset2 = "";
  in >> newdataset >> dataset1 >> dataset2;
  if (!in || trees.find(dataset1) == trees.end() || trees.find(dataset2) == trees.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::map< int, std::string > tree1 = trees.at(dataset1);
  std::map< int, std::string > tree2 = trees.at(dataset2);
  std::map< int, std::string > intersect_tree(tree1);
  for (auto iter = tree1.begin(); iter != tree1.end(); ++iter)
  {
    int key = (*iter).first;
    if (tree2.find(key) == tree2.end())
    {
      intersect_tree.erase(key);
    }
  }
  trees.insert(std::make_pair(newdataset, intersect_tree));
}

void unionCmd(std::map< std::string, std::map< int, std::string > > & trees, std::istream & in)
{
  std::string newdataset = "", dataset1 = "", dataset2 = "";
  in >> newdataset >> dataset1 >> dataset2;
  if (!in || trees.find(dataset1) == trees.end() || trees.find(dataset2) == trees.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::map< int, std::string > union_tree = trees.at(dataset1);
  std::map< int, std::string > tree2 = trees.at(dataset2);
  for (auto iter = tree2.begin(); iter != tree2.end(); ++iter)
  {
    int key = (*iter).first;
    if (union_tree.find(key) == union_tree.end())
    {
      union_tree.insert(std::make_pair(key, (*iter).second));
    }
  }
  trees.insert(std::make_pair(newdataset, union_tree));
}

void inputTrees(std::istream & in, std::map< std::string, std::map< int, std::string > > & trees)
{
  std::string tree_name = "";
  while (in >> tree_name)
  {
    std::map< int, std::string > tree;
    int key = 0;
    std::string value = "";
    while (in >> key >> value)
    {
      tree.insert(std::make_pair(key, value));
    }
    if (!in && !in.eof())
    {
      in.clear();
    }
    trees.insert(std::make_pair(tree_name, tree));
  }
}

void outputTree(const std::map< std::string, std::map< int, std::string > > & trees)
{
  for (auto it = trees.begin(); it != trees.end(); ++it)
  {
    std::cout << (*it).first << ":\n";
    std::map< int, std::string > tree = (*it).second;
    for (auto it0 = tree.begin(); it0 != tree.end(); ++it0)
    {
      std::cout << (*it0).first << " " << (*it0).second << "; ";
    }
    std::cout << "\n\n";
  }
}

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
  // все map нужно заменить на tree!!!
  std::map< std::string, std::map< int, std::string > > trees;
  inputTrees(input, trees);
  outputTree(trees);

  //система команд
  std::map< std::string, std::function< void(std::map< std::string, std::map< int, std::string > > &, std::istream &, std::ostream &) > > cmds;
  {
    using namespace std::placeholders;
    cmds["print"] = std::bind(printCmd, _1, _2, _3);
    cmds["complement"] = std::bind(complementCmd, _1, _2);
    cmds["intersect"] = std::bind(intersectCmd, _1, _2);
    cmds["union"] = std::bind(unionCmd, _1, _2);
  }
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      if (cmds.find(cmd) != cmds.end())
      {
        cmds.at(cmd)(trees, std::cin, std::cout);
      }
      else
      {
        throw std::logic_error("<INVALID COMMAND>");
      }
    }
    catch (const std::exception & e)
    {
      std::cout << e.what() << '\n';
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}

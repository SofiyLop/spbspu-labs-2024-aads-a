#include <iostream>
#include <cassert>
#include <string>
#include "tree_node.hpp"
#include "tree_iterator.hpp"
#include "const_tree_iterator.hpp"

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

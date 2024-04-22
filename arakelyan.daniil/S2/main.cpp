#include <exception>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include <list/binList.hpp>
#include "queue.hpp"
#include "stack.hpp"
#include "expressionObject.hpp"
#include "inputData.hpp"
#include "transformToPostfix.hpp"
#include "calculateProc.hpp"

int main(int argc, char *argv[])
{
  using namespace arakelyan;
  using namespace detail;
  Queue< Queue< ExpressionObj > > qOfInfQs;

  if (argc == 2)
  {
    std::ifstream inputFile(argv[1]);
    readDataInInfixFormat(inputFile, qOfInfQs);
    if (qOfInfQs.empty())
    {
      std::cerr << "File empty!\n";
      return 1;
    }
  }
  else
  {
    readDataInInfixFormat(std::cin, qOfInfQs);
  }

  Queue< Queue< ExpressionObj > > qOfPostQs;
  try
  {
    while (!qOfInfQs.empty())
    {
      Queue< ExpressionObj > postQ = transformInfixToPostfix(qOfInfQs.front());
      qOfInfQs.pop();
      qOfPostQs.push(postQ);
    }
  }
  catch (const std::logic_error &e)
  {
    std::cout << "Error: " << e.what() << "\n";
    return 1;
  }

  Stack< long long > answQ;
  try
  {
    while (!qOfPostQs.empty())
    {
      calculatePostfixQ(qOfPostQs, answQ);
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  std::cout << answQ.top();
  answQ.pop();
  while (!answQ.empty())
  {
    std::cout << " " << answQ.top();
    answQ.pop();
  }
  std::cout << "\n";
  return 0;
}

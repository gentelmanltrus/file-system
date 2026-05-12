#include <iostream>
#include "include/CommandProcessor.h"

int main()
{

  CommandProcessor cmd;
  std::string input;
  while (true)
  {
    std::cout << ">";
    std::getline(std::cin, input);
    if (input == "end")
      break;

    try
    {
      cmd.processCommand(input);
    }
    catch (std::runtime_error &e)
    {
      std::cout << e.what();
    }
  }

  return 0;
}
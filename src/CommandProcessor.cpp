#include <sstream>
#include "include/CommandProcessor.h"

void CommandProcessor::run()
{
  // must be defined
}

void CommandProcessor::processCommand(const std::string &input)
{
  std::stringstream ss(input);
  std::string commandName;
  ss >> commandName;
  if (commandName == "touch")
  {
    std::string fileName;
    ss >> fileName;
    fs.touch(fileName);
  }
  if (commandName == "ls")
  {
    fs.ls();
  } // more commands, might need other solution
  else
    throw std::runtime_error("Invalid command");

}

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
    try
    {
      fs.touch(fileName);
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
    }
    
  } // more commands, might need other solution
  else
    throw std::runtime_error("Invalid command");

}
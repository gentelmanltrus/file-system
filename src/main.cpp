#include <iostream>
#include "../include/CommandProcessor.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
  try
  {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    CommandProcessor cmd;
    cmd.run();
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << "Unknown error occurred." << std::endl;
  }
  return 0;
}

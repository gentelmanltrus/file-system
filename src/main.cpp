#include <iostream>
#include "../include/CommandProcessor.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#endif
  CommandProcessor cmd;
  cmd.run();

  return 0;
}

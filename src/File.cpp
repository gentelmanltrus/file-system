#include "File.h"

File::File(const std::string &name)
    : FileSystemItem(name) {}

void File::display() const
{
  std::cout << "FILE: " << name << "\n";
}
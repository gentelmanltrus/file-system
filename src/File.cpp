#include "../include/File.h"

File::File(const std::filesystem::path &name)
    : FileSystemItem(name) {}

void File::display() const
{
  std::cout << "FILE: " << name.string() << "\n";
}
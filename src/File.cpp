#include "../include/File.h"

File::File(const std::filesystem::path &name, std::shared_ptr<FileSystemItem> parent)
        : FileSystemItem(name, parent) {};

void File::display() const
{
  std::cout << name.filename().string() << std::endl;
}
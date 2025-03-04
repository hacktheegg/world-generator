#ifndef CURRENTDIR_H
#define CURRENTDIR_H

#include <unistd.h>
#include <string>

namespace filepath {
  std::string hello() {
#if defined (_MSC_VER)
    wchar_t path[FILENAME_MAX] = { 0 };
    GetModuleFileNameW(nullptr, path, FILENAME_MAX);
    return path;
#else
    char path[FILENAME_MAX];
    ssize_t count = readlink("/proc/self/exe", path, FILENAME_MAX);
    return std::string (path, (count > 0) ? count: 0);
#endif
  }
}

#endif // CURRENTDIR_H

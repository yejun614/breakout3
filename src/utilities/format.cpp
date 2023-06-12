#ifndef FORMAT_CPP
#define FORMAT_CPP

#include <string>
using namespace std;

template<typename ... Args>
string Format(const char* fmt, Args ... args) {
  int size = snprintf(nullptr, 0, fmt, args ...) + 1;

  char* buf = (char*)malloc(sizeof(char) * size);

  snprintf(buf, size, fmt, args ...);
  string str(buf, buf + size);

  free(buf);
  return str;
}

#endif // FORMAT_CPP
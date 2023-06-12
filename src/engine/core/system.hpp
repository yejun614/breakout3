#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <functional>
using namespace std;

#include <nlohmann/json.hpp>
using namespace nlohmann;

class System {
public:
  System(unsigned int priority, function<bool (json&)> func)
    : Priority(priority), Func(func)
  {  }

  unsigned int Priority;
  function<bool (json&)> Func;

  bool operator < (const System& another) const {
    return Priority < another.Priority;
  }
};

#endif // SYSTEM_HPP
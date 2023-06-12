#include "logger.hpp"

string GetCurrentDateTime() {
  time_t now = time(0);
  struct tm tstruct = *localtime(&now);
  char buf[80];

  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

  return buf;
}

void Log(string message) {
  cout << GetCurrentDateTime() << " " << message << endl;
}

void Log(string title, string message) {
  Log(title + " : " + message);
}
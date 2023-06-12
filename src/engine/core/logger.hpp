#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>
#include <ctime>
#include <typeinfo>

using namespace std;

string GetCurrentDateTime();

void Log(string message);

void Log(string title, string message);

#endif // LOGGER_HPP
#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <map>
#include <string>
using namespace std;

#include "miniaudio.h"

#include <engine/core/logger.hpp>
#include <utilities/format.cpp>

struct AudioData {
  string path;
  ma_engine* engine;
  ma_sound* sound;
  ma_event* event;
};

static class Audio {
public:
  static ma_result Play(string path, ma_uint64 frameIndex = 0);
  static ma_result Pause(string path);
  static ma_result Stop(string path);
  static ma_result Load(string key, string path);
  static AudioData& GetData(string key);
  static void Delete(string key);
  static void DeleteAll();
  static void Wait(string key);

private:
  static map<string, AudioData> data;
};

#endif // AUDIO_HPP
#include "audio.hpp"

map<string, AudioData> Audio::data;

void Audio::DeleteAll() {
  for (auto& [key, value] : data) {
    Delete(key);
  }
}

ma_result Audio::Play(string path, ma_uint64 frameIndex) {
  AudioData audio;

  if (data.find(path) != data.end() || Load(path, path) == MA_SUCCESS) {
    audio = data[path];
  } else {
    return MA_ERROR;
  }

  ma_sound_seek_to_pcm_frame(audio.sound, frameIndex);    
  ma_sound_start(audio.sound);

  return MA_SUCCESS;
}

ma_result Audio::Pause(string path) {
  ma_sound_stop(data[path].sound);
}

ma_result Audio::Stop(string path) {
  ma_sound* sound = data[path].sound;
  ma_sound_stop(sound);
  ma_sound_seek_to_pcm_frame(sound, 0);
}

ma_result Audio::Load(string key, string path) {
  if (data.find(key) != data.end()) {
    return MA_SUCCESS;
  }

  ma_result result;
  ma_engine* engine = new ma_engine;
  ma_sound* sound = new ma_sound;
  ma_event* stopEvent = new ma_event;

  result = ma_engine_init(NULL, engine);
  if (result != MA_SUCCESS) {
    Log("Audio::Play", "ERROR Failed to initialize audio engine.");
    return result;
  }
  
  result = ma_sound_init_from_file(engine, path.c_str(), 0, NULL, NULL, sound);
  if (result != MA_SUCCESS) {
    Log("Audio::Play", Format("ERROR Failed to initialize sound (%s)", path.c_str()));

    ma_engine_uninit(engine);
    return result;
  }

  ma_event_init(stopEvent);

  data[key] = {
    path,
    engine,
    sound,
    stopEvent
  };

  return MA_SUCCESS;
}

AudioData& Audio::GetData(string key) {
  return data[key];
}

void Audio::Delete(string key) {
  const auto it = data.find(key);
  if (it == data.end()) {
    return;
  }

  const AudioData& audio = it->second;

  ma_event_uninit(audio.event);
  ma_sound_uninit(audio.sound);
  ma_engine_uninit(audio.engine);

  delete audio.event;
  delete audio.sound;
  delete audio.engine;

  data.erase(it);
}

void Audio::Wait(string key) {
  ma_event_wait(data[key].event);
}
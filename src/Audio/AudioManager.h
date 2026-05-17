// ============================================================================
// Dionite — Audio: abstract audio manager (platform impls use OpenAL/AVFoundation)
// ============================================================================
#pragma once
#include <string>
#include <unordered_map>

namespace dionite::audio {

struct AudioBuffer { std::string path; int handle = -1; };

class AudioManager {
public:
    virtual ~AudioManager() = default;
    virtual bool load(const std::string& id, const std::string& path) = 0;
    virtual void playOneShot(const std::string& id, float volume = 1.f, float pitch = 1.f) = 0;
    virtual void playLoop(const std::string& id, float volume = 1.f) = 0;
    virtual void stopLoop(const std::string& id) = 0;
    virtual void setMasterVolume(float v) = 0;
    virtual void setMusicVolume(float v) = 0;
    virtual void setSFXVolume(float v) = 0;
};

// Stub implementation; replaced by platforms/{ios,desktop,android}/Audio*.{mm,cpp}
class AudioManagerStub : public AudioManager {
public:
    bool load(const std::string&, const std::string&) override { return true; }
    void playOneShot(const std::string&, float, float) override {}
    void playLoop(const std::string&, float) override {}
    void stopLoop(const std::string&) override {}
    void setMasterVolume(float v) override { master_ = v; }
    void setMusicVolume(float v) override { music_ = v; }
    void setSFXVolume(float v) override { sfx_ = v; }
private:
    float master_ = 1.f, music_ = 1.f, sfx_ = 1.f;
};

} // namespace dionite::audio

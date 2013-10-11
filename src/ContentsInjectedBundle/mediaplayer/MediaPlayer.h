#ifndef MediaPlayer_h
#define MediaPlayer_h

#include <NixPlatform/MediaPlayer.h>
#include <gst/gst.h>

class MediaPlayer : public Nix::MediaPlayer
{
public:
    MediaPlayer(Nix::MediaPlayerClient*);
    virtual ~MediaPlayer();
    virtual void play();
    virtual void pause();
    virtual float duration() const;
    virtual float currentTime() const;
    virtual void seek(float);
    virtual void setVolume(float);
    virtual void setMuted(bool);
    virtual void supportsType(const char*, const char*);
    virtual bool load(const char* url);

private:
    GstElement* m_playBin;
    bool m_paused;

    bool createPlayBin();
    void setDownloadBuffering();

    static void onGstBusMessage(GstBus*, GstMessage*, MediaPlayer*);
};

#endif

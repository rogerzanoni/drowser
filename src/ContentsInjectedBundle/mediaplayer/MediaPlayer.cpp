/*
 * Copyright (C) 2013 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "MediaPlayer.h"

#include <NixPlatform/MediaStream.h>

#include "BrowserPlatform.h"
#include "DefaultMediaPlayerBackend.h"
#include "MediaStreamPlayerBackend.h"

#include "Logging.h"

Nix::MediaPlayer* BrowserPlatform::createMediaPlayer(Nix::MediaPlayerClient* client)
{
    return new MediaPlayer(client);
}

MediaPlayer::MediaPlayer(Nix::MediaPlayerClient* client)
    : Nix::MediaPlayer(client)
    , m_backend(0)
{
}

MediaPlayer::~MediaPlayer()
{
    delete m_backend;
}

void MediaPlayer::play()
{
    if (!m_backend)
        return;
    if (!m_backend->isPaused())
        return;
    m_backend->play();
}

void MediaPlayer::pause()
{
    if (!m_backend)
        return;
    if (m_backend->isPaused())
        return;
    m_backend->pause();
}

float MediaPlayer::duration() const
{
    if (!m_backend)
        return 0;
    return m_backend->duration();
}

float MediaPlayer::currentTime() const
{
    if (!m_backend)
        return 0;
    return m_backend->currentTime();
}

void MediaPlayer::seek(float time)
{
    if (!m_backend)
        return;

    if (time == currentTime())
        return;

    if (m_backend->isLiveStream())
        return;

    m_backend->seek(time);
}

bool MediaPlayer::seeking() const
{
    if (!m_backend)
        return false;
    return m_backend->seeking();
}

float MediaPlayer::maxTimeSeekable() const
{
    if (!m_backend)
        return 0;
    return m_backend->maxTimeSeekable();
}

void MediaPlayer::setPlaybackRate(float playbackRate)
{
    if (!m_backend)
        return;
    m_backend->setPlaybackRate(playbackRate);
}

void MediaPlayer::setVolume(float volume)
{
    if (!m_backend)
        return;
    m_backend->setVolume(volume);
}

void MediaPlayer::setMuted(bool mute)
{
    if (!m_backend)
        return;
    m_backend->setMuted(mute);
}

bool MediaPlayer::isLiveStream() const
{
    if (!m_backend)
        return false;
    return m_backend->isLiveStream();
}

void MediaPlayer::setSrc(const char* url)
{
    std::string urlString(g_strdup(url)); // FIXME g_strdub
    if (m_backend) {
        DefaultMediaPlayerBackend* defaultBackend = dynamic_cast<DefaultMediaPlayerBackend*>(m_backend);
        if (defaultBackend) {
            defaultBackend->setURL(urlString);
            return;
        }
        delete m_backend;
    }
    m_backend = new DefaultMediaPlayerBackend(m_playerClient, urlString);
}

void MediaPlayer::setSrc(Nix::MediaStream* mediaStream)
{
    if (m_backend) {
        MediaStreamPlayerBackend* streamBackend = dynamic_cast<MediaStreamPlayerBackend*>(m_backend);
        if (streamBackend) {
            streamBackend->setMediaStream(mediaStream);
            return;
        }
        delete m_backend;
    }
    m_backend = new MediaStreamPlayerBackend(m_playerClient, mediaStream);
}

void MediaPlayer::load()
{
    if (!m_backend) {
        LOG(Media, "No 'src' set.");
        return;
    }

    LOG(Media, "Load");
    m_backend->load();
}

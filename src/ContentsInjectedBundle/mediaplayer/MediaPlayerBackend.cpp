#include "MediaPlayerBackend.h"

MediaPlayerBackend::MediaPlayerBackend(Nix::MediaPlayerClient* client)
    : m_playerClient(client)
    , m_readyState(Nix::MediaPlayerClient::HaveNothing)
    , m_networkState(Nix::MediaPlayerClient::Empty)
{
}

void MediaPlayerBackend::setReadyState(Nix::MediaPlayerClient::ReadyState readyState)
{
    if (m_readyState == readyState)
        return;
    m_readyState = readyState;
    m_playerClient->readyStateChanged(m_readyState);
}

void MediaPlayerBackend::setNetworkState(Nix::MediaPlayerClient::NetworkState networkState)
{
    if (m_networkState == networkState)
        return;
    m_networkState = networkState;
    m_playerClient->networkStateChanged(m_networkState);
}


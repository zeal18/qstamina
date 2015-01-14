#include "sounds.h"

#include <QSound>

Sounds::Sounds(Config *config, QObject *parent) : QObject(parent),
    m_config( config )
{
    m_sounds[ "type" ] = m_config->resourcesPath() + "/sounds/type.wav";
    m_sounds[ "error" ] = m_config->resourcesPath() + "/sounds/error.wav";
    m_sounds[ "finish" ] = m_config->resourcesPath() + "/sounds/finish.wav";
}

void Sounds::play( const QString &sound )
{
    if( m_sounds.contains( sound ) && m_config->enableSound() )
        QSound::play( m_sounds[ sound ] );
}


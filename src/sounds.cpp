#include "sounds.h"

#include <QSoundEffect>

Sounds::Sounds(Config *config, QObject *parent) : QObject(parent),
    m_config( config )
{
    m_sounds[ "type" ] = m_config->resourcesPath() + "/sounds/type.wav";
    m_sounds[ "error" ] = m_config->resourcesPath() + "/sounds/error.wav";
    m_sounds[ "finish" ] = m_config->resourcesPath() + "/sounds/finish.wav";
}

void Sounds::play( const QString &sound )
{
    if( m_sounds.contains( sound ) && m_config->enableSound() && m_config->volume() > 0.0 )
    {
        QSoundEffect *soundEffect = new QSoundEffect();

        connect( soundEffect, &QSoundEffect::playingChanged, [soundEffect](){
            if( !soundEffect->isPlaying() )
                soundEffect->deleteLater();
        });

        soundEffect->setSource( QUrl::fromLocalFile( m_sounds[ sound ] ) );
        soundEffect->setVolume( m_config->volume() );
        soundEffect->play();
    }
}


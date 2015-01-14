#ifndef SOUNDS_H
#define SOUNDS_H

#include <QObject>
#include <QMap>

#include "config.h"

class Sounds : public QObject
{
    Q_OBJECT
public:
    explicit Sounds( Config *config, QObject *parent = 0 );
    void play( const QString & );
private:
    Config *m_config;
    QMap<QString, QString> m_sounds;
};

#endif // SOUNDS_H

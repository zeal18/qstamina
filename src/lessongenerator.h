#ifndef LESSONGENERATOR_H
#define LESSONGENERATOR_H

#include <QString>
#include <QList>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QStandardPaths>

#include "config.h"

class LessonGenerator
{
public:
    LessonGenerator(Config *config);
    bool generate();
private:
    struct Lesson {
        QString title;
        QString symbols;
        QStringList words;
    };

    QList<Lesson> m_lessons;

    Config *m_config;

    void generateLessons();
    QStringList generateWords(QString alphabet, int wordsMaxCount = 7, int minSymbols = 3, int maxSymbols = 7);
    bool save(QString saveFilePath);
};

#endif // LESSONGENERATOR_H

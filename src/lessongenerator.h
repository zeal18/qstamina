#ifndef LESSONGENERATOR_H
#define LESSONGENERATOR_H

#include <QString>
#include <QList>
#include <QDomDocument>
#include <QDomElement>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTime>

class LessonGenerator
{
public:
    LessonGenerator();
    bool generate(QString rulesFilePath, QString saveFilePath);
private:
    struct Lesson {
        QString title;
        QString symbols;
        QStringList words;
    };

    QList<Lesson> m_lessons;

    void generateLessons();
    QStringList generateWords(QString alphabet, int wordsMaxCount = 7, int minSymbols = 3, int maxSymbols = 7);
    bool save(QString saveFilePath);
};

#endif // LESSONGENERATOR_H

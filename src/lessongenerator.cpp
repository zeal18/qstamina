#include "lessongenerator.h"

LessonGenerator::LessonGenerator(Config *config)
{
    m_config = config;
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
}

bool LessonGenerator::generate()
{
    QString rulesFilePath = m_config->resourcesPath() + "/generatorRules/" + m_config->currentLayout()->name + ".json";
    QString saveFilePath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/generatedLessons/" + m_config->currentLayout()->name +".lsn";
    qDebug()<<"Generate rules file: "<<rulesFilePath;
    qDebug()<<"Generate save file: "<<saveFilePath;
    QFile rulesFile;
    rulesFile.setFileName(rulesFilePath);
    if( rulesFile.open(QIODevice::ReadOnly) )
    {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(rulesFile.readAll());
        QJsonArray jsonArray = jsonDoc.array();
        for( int i = 0; i < jsonArray.count(); i++ )
        {
            Lesson lesson;
            lesson.title = jsonArray.at(i).toObject().value("title").toString();
            lesson.symbols = jsonArray.at(i).toObject().value("symbols").toString();
            m_lessons.append(lesson);
        }
    }

    generateLessons();
    if( !save(saveFilePath) )
    {
        return false;
    }
    return true;
}

void LessonGenerator::generateLessons()
{
    QList<Lesson> lessons;
    for( int i = 0; i < m_lessons.size(); i++ )
    {
        Lesson lesson = m_lessons.at(i);
        QString symbols = lesson.symbols;
        qDebug()<<symbols;
        QStringList words;
        for( int j = 0; j < (symbols.size() - 1); j++ )
        {
            for( int k = j + 1; k < symbols.size(); k++ )
            {
                QString alphabet = symbols.at(j);
                alphabet.append(symbols.at(k));
                words.append(generateWords( alphabet ));
            }
        }
        lesson.words = words;
        lessons.append(lesson);
    }
    m_lessons = lessons;
}

QStringList LessonGenerator::generateWords(QString alphabet, int wordsMaxCount, int minSymbols, int maxSymbols)
{

    int wordLen = qrand() % ((maxSymbols + 1) - minSymbols) + minSymbols;
    int wordsCount = qrand() % (wordsMaxCount) + 2;

    //qDebug()<<alphabet<<wordsCount<<wordLen;
    QStringList words;
    for( int j = 0; j < wordsCount; j++ )
    {
        QString word;
        for( int i = 0; i < wordLen; i++ )
        {
            int symbolNumber = qrand() % (alphabet.size());
            word.append(alphabet.at(symbolNumber));
        }
        words.append(word);
    }
    return words;
}

bool LessonGenerator::save(QString saveFilePath)
{
    QFile rulesFile;
    rulesFile.setFileName(saveFilePath);
    rulesFile.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);

    QJsonArray jsonArray;
    for( int i = 0; i < m_lessons.size(); i++ )
    {
        QJsonObject jsonObject;
        jsonObject.insert("title",QJsonValue(m_lessons.at(i).title));
        jsonObject.insert("content",QJsonValue(m_lessons.at(i).words.join(" ")));
        jsonArray.append(QJsonValue(jsonObject));
    }
    QJsonDocument jsonDoc;
    jsonDoc.setArray(jsonArray);
//qDebug()<<jsonDoc.toJson();
    QTextStream out(&rulesFile);
    out << jsonDoc.toJson();

    rulesFile.close();
    m_config->lessonsGenerated();
    return true;
}

#include "lessongenerator.h"

LessonGenerator::LessonGenerator()
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
}

bool LessonGenerator::generate(QString rulesFilePath, QString saveFilePath)
{
    qDebug()<<"Generate rules file: "<<rulesFilePath;
    qDebug()<<"Generate save file: "<<saveFilePath;
    QFile rulesFile;
    rulesFile.setFileName(rulesFilePath);
    rulesFile.open(QIODevice::ReadOnly);

    QDomDocument lessons;
    lessons.setContent(&rulesFile);

    QDomElement root = lessons.documentElement();
    if( root.tagName() == "lessons" )
    {
        QDomElement lesson = root.firstChildElement("lesson");
        while( !lesson.isNull() ){
            Lesson lsn;
            lsn.title = lesson.firstChildElement("title").text().trimmed();
            lsn.symbols = lesson.firstChildElement("symbols").text().trimmed();
            m_lessons.append(lsn);
            lesson = lesson.nextSiblingElement("lesson");
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
    rulesFile.open(QFile::WriteOnly | QFile::Text);

    QDomDocument lessons;
    QDomElement root = lessons.createElement("lessons");
    for( int i = 0; i < m_lessons.size(); i++ )
    {
        Lesson lsn = m_lessons.at(i);
        QDomElement lesson = lessons.createElement("lesson");
        QDomElement title = lessons.createElement("title");
        QDomText titleText = lessons.createTextNode(lsn.title);
        title.appendChild(titleText);
        lesson.appendChild(title);
        QDomElement content = lessons.createElement("content");
        QDomText contentText = lessons.createTextNode(lsn.words.join(" "));
        content.appendChild(contentText);
        lesson.appendChild(content);
        root.appendChild(lesson);
    }
    const int IndentSize = 4;
    lessons.appendChild(root);
    QTextStream out(&rulesFile);
    lessons.save(out, IndentSize);
    return true;
}

#include "config.h"

Config::Config() :
    m_currentLayout(NULL)
{
    m_settings = new QSettings(qApp->applicationName(),qApp->organizationName());
    m_lastLayoutFile = m_settings->value("lastLayoutFile",QLocale::system().name()+".ltf").toString();
    m_fontSize = m_settings->value("fontSize",QVariant(12)).toInt();
    m_separateKeyboard = m_settings->value("separateKeyboard",QVariant(false)).toBool();
    m_lastLayout = m_settings->value("lastLayout",QLocale::system().name()).toString();


    loadLayouts();
}

Config::~Config()
{

}
QString Config::lastLayoutFile() const
{
    return m_lastLayoutFile;
}

void Config::setLastLayoutFile(const QString &lastLayoutFile)
{
    m_lastLayoutFile = lastLayoutFile;
    m_settings->setValue("lastLayoutFile",lastLayoutFile);
}
int Config::fontSize() const
{
    return m_fontSize;
}

void Config::setFontSize(int fontSize)
{
    m_fontSize = fontSize;
    m_settings->setValue("fontSize",fontSize);
}
bool Config::separateKeyboard() const
{
    return m_separateKeyboard;
}

void Config::setSeparateKeyboard(bool separateKeyboard)
{
    m_separateKeyboard = separateKeyboard;
    m_settings->setValue("separateKeyboard",separateKeyboard);
}

QString Config::resourcesPath()
{
#ifdef Q_OS_LINUX
    QDir resourcesDir;
    resourcesDir.setCurrent("/usr/share/qstamina");
    if( !resourcesDir.exists() )
        resourcesDir.setCurrent(QApplication::applicationDirPath());
    return resourcesDir.absolutePath();
#endif
#ifdef Q_OS_MACX
    return QApplication::applicationDirPath()+"/../Resources";
#endif
#ifdef Q_OS_WIN
    return QApplication::applicationDirPath();
#endif
    return QString();
}
QString Config::lastLayout() const
{
    return m_lastLayout;
}

void Config::setLastLayout(const QString &lastLayout)
{
    m_lastLayout = lastLayout;
    m_settings->setValue("lastLayout",lastLayout);
}

Layout *Config::currentLayout() const
{
    return m_currentLayout;
}

bool Config::setCurrentLayout(const int layoutIndex)
{
    if( layoutIndex >= 0 && layoutIndex < m_layouts.count() && m_currentLayout->name != m_layouts.at( layoutIndex )->name)
    {
        m_currentLayout = m_layouts.at( layoutIndex );
        loadLessons();
        loadGeneratedLessons();
        return true;
    }
    return false;
}

QList<Layout *> Config::layouts() const
{
    return m_layouts;
}

LessonsList Config::lessons() const
{
    return m_lessons;
}

QList<Lesson *> Config::generatedLessons() const
{
    return m_generatedLessons;
}

void Config::lessonsGenerated()
{
    loadGeneratedLessons();
}


void Config::loadLayouts()
{
    QFile layoutsFile(resourcesPath() + "/layouts.ltf");
    if( layoutsFile.open(QIODevice::ReadOnly) )
    {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(layoutsFile.readAll());
        QJsonArray jsonArray = jsonDoc.array();
        for( int i = 0; i < jsonArray.count(); i++ )
        {
            Layout *layout = new Layout;
            layout->name = jsonArray.at(i).toObject().value("layout").toString();
            layout->title = jsonArray.at(i).toObject().value("title").toString();
            layout->symbols = jsonArray.at(i).toObject().value("symbols").toString();
            m_layouts.append(layout);
            if( layout->name == m_lastLayout )
            {
                m_currentLayout = layout;
            }
        }
        if( m_layouts.count() < 1 )
        {
            qDebug()<<"No layouts loaded!";
            exit(EXIT_FAILURE);
        }
        if( !m_currentLayout )
        {
            m_currentLayout = m_layouts.at(0);
        }
        loadLessons();
        loadGeneratedLessons();
    }
    else
    {
        qDebug()<<"Can't open layouts file!";
        exit(EXIT_FAILURE);
    }
}

void Config::loadLessons()
{
    clearLessons();
    QFile lessonsFile(resourcesPath() + "/baselessons/" + m_currentLayout->name + ".lsn");
    qDebug()<<"loding lessons from file: "<<resourcesPath() + "/baselessons/" + m_currentLayout->name + ".lsn";
    if( lessonsFile.open(QIODevice::ReadOnly) )
    {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(lessonsFile.readAll());
        QJsonArray jsonArray = jsonDoc.array();
        for( int i = 0; i < jsonArray.count(); i++ )
        {
            Lesson *lesson = new Lesson;
            lesson->title = jsonArray.at(i).toObject().value("title").toString();
            lesson->author = jsonArray.at(i).toObject().value("author").toString();
            lesson->version = jsonArray.at(i).toObject().value("version").toString();
            lesson->content = jsonArray.at(i).toObject().value("content").toString();
            lesson->group = jsonArray.at(i).toObject().value("group").toString();

            m_lessons.append(lesson);
        }
        if( m_lessons.count() < 1 )
        {
            qDebug()<<"No lessons loaded!";
        }
    }
    else
    {
        qDebug()<<"Can't open lessons file!";
    }
}

void Config::loadGeneratedLessons()
{
    clearGeneratedLessons();
    QFile lessonsFile(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/generatedLessons/" + m_currentLayout->name + ".lsn");
    qDebug()<<"loding generated lessons from file: "<<QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/generatedLessons/" + m_currentLayout->name + ".lsn";
    if( lessonsFile.open(QIODevice::ReadOnly) )
    {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(lessonsFile.readAll());
        QJsonArray jsonArray = jsonDoc.array();
        for( int i = 0; i < jsonArray.count(); i++ )
        {
            Lesson *lesson = new Lesson;
            lesson->title = jsonArray.at(i).toObject().value("title").toString();
            lesson->author = "lessons generator";
            lesson->version = "0.0.0";
            lesson->content = jsonArray.at(i).toObject().value("content").toString();

            m_generatedLessons.append(lesson);
        }
        if( m_generatedLessons.count() < 1 )
        {
            qDebug()<<"No generated lessons loaded!";
        }
    }
    else
    {
        qDebug()<<"Can't open generated lessons file!";
    }
}

void Config::clearLessons()
{
    m_lessons.clear();
}

void Config::clearGeneratedLessons()
{
    QList<Lesson*> lessons = m_generatedLessons;
    m_generatedLessons.clear();
    for( int i = 0; i < lessons.count(); i++ )
    {
        Lesson *lesson = lessons.at(i);
        delete lesson;
    }
}




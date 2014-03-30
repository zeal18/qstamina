#include "lessonslist.h"

LessonsList::LessonsList()
{
}

void LessonsList::append(Lesson *lesson)
{
    m_lessons.append(lesson);
    if( lesson->group == "" )
        lesson->group = QObject::tr("Other");
    QList<Lesson*> *groupList = m_groups.value(lesson->group,NULL);
    if( !groupList )
    {
        qDebug()<<"Creating new lessons group: "<<lesson->group;
        groupList = new QList<Lesson*>;
        m_groups[lesson->group] = groupList;
    }
    groupList->append(lesson);
}

void LessonsList::append(const QString &title, const QString &author, const QString &version, const QString &content, const QString &group)
{
    Q_UNUSED(group)
    Lesson *lesson = new Lesson;
    lesson->title = title;
    lesson->author = author;
    lesson->version = version;
    lesson->content = content;
    lesson->group = group;
    append(lesson);
}

int LessonsList::count() const
{
    return m_lessons.count();
}

Lesson * const &LessonsList::at(int i) const
{
    return m_lessons.at(i);
}

void LessonsList::clear()
{
    QList<Lesson*> lessons = m_lessons;
    QList<QList<Lesson*>*> groups = m_groups.values();
    m_lessons.clear();
    m_groups.clear();
    for( int i = 0; i < lessons.count(); i++ )
    {
        Lesson *lesson = lessons.at(i);
        delete lesson;
    }
    for( int i = 0; i < groups.count(); i++ )
    {
        QList<Lesson*>* group = groups.at(i);
        group->clear();
        delete group;
    }

}

QStringList LessonsList::groups() const
{
    return m_groups.keys();
}

QList<Lesson *> LessonsList::lessonsByGroup(const QString &group)
{
    QList<Lesson *>* groups = m_groups.value(group, NULL);
    if( !groups )
    {
        return QList<Lesson*>();
    }
    return *groups;
}

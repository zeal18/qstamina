#ifndef LESSONSLIST_H
#define LESSONSLIST_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDebug>

struct Lesson {
    QString title;
    QString author;
    QString version;
    QString content;
    QString group;
};

class LessonsList
{
public:
    LessonsList();
    void append(Lesson *);
    void append(const QString &title, const QString &author, const QString &version, const QString &content, const QString &group);
    int count() const;
    Lesson* const & at(int i) const;
    void clear();
    QStringList groups() const;
    QList<Lesson*> lessonsByGroup(const QString &group);
private:
    QList<Lesson*> m_lessons;
    QMap<QString,QList<Lesson*>*> m_groups;
};

Q_DECLARE_METATYPE(Lesson*)

#endif // LESSONSLIST_H

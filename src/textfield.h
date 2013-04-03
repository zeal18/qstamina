#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include <QWidget>

class TextField : public QWidget
{
    Q_OBJECT
public:
    explicit TextField(QWidget *parent = 0);
    virtual void keyPressed(QString key);
    virtual void setText(QString text);
    virtual QString nextSymbol();
    virtual void reset();
    virtual void start();
    virtual void stop();
    int rightSymbols();
    int countSymbols();
    int wrongSymbols();

protected:
    QString m_text;
    int m_typeRights;
    int m_width;
    int m_height;
    int m_rightSymbols;
    int m_countSymbols;
    int m_wrongSymbols;
signals:
    void noMoreText();
public slots:
    
};

#endif // TEXTFIELD_H

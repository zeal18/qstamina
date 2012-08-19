#include "tstamina.h"
#include "ui_tstamina.h"

TStamina::TStamina(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TStamina)
{
    ui->setupUi(this);
    this->buildMainMenu();

    QFont font;
    font.setPointSize(18);
    ui->frame->setMaximumWidth(600);
    ui->frame->setMinimumWidth(600);
    ui->txtNewText->setMaximumWidth(300);
    ui->txtOldText->setMaximumWidth(300);
    ui->txtNewText->setFont(font);
    ui->txtOldText->setFont(font);

    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));
    time = 0;
    typeErrors = 0;
    typeRights = 0;
    speed = 0;
}

TStamina::~TStamina()
{
    delete ui;
}

void TStamina::keyReleaseEvent(QKeyEvent *event)
{
    qDebug()<<event->key();
    if( event->key() == Qt::Key_Escape )
        this->endLesson();
    if( event->text() != "")
    {
        if( ui->txtNewText->text().startsWith(event->text()) )
        {
            this->typeRights++;
            ui->txtOldText->setText(ui->txtOldText->text()+event->text());
            ui->txtNewText->setText(ui->txtNewText->text().right(ui->txtNewText->text().size()-1));
            this->updateKeyboard();
            if( ui->txtNewText->text().length() == 0 )
            {
                this->endLesson();
            }
        } else {
            this->typeErrors++;
        }

    }

}

void TStamina::buildMainMenu()
{
    mainMenu = new QMenuBar(this);
    this->setMenuBar(mainMenu);
    QMenu *menu = new QMenu("Уроки");
    QAction *action;

    QDir lessonDir;
    lessonDir.setCurrent(QApplication::applicationDirPath()+"/baselessons/ru_RU");
    QStringList lessons = lessonDir.entryList(QDir::Files);
    for( int i = 0; i < lessons.count(); i++ )
    {
        qDebug()<<lessonDir.absolutePath()+"/"+lessons.at(i);
        QSettings lesson(lessonDir.absolutePath()+"/"+lessons.at(i),QSettings::IniFormat);
        lesson.setIniCodec("utf-8");
        qDebug()<<lesson.value("title").toString();
        if( lesson.value("title").toString() != "" && lesson.value("content").toString() != "" )
        {
            action = menu->addAction(lesson.value("title").toString(),this,SLOT(lessonChoosed()));
            action->setData(lessonDir.absolutePath()+"/"+lessons.at(i));
        }
    }

    mainMenu->addMenu(menu);
}

void TStamina::loadLesson(QString lesson)
{
    qDebug()<<"loading lesson from: "<<lesson;
    QSettings ls(lesson,QSettings::IniFormat);
    ls.setIniCodec("utf-8");
    ui->txtOldText->setText("");
    ui->txtNewText->setText(ls.value("content").toString());
    this->timer->start(1000);
    this->updateKeyboard();

}

void TStamina::endLesson()
{
    this->timer->stop();
}

void TStamina::updateKeyboard()
{
    ui->lblLShift->setStyleSheet("background-image: url();color: black;");
    ui->lblRShift->setStyleSheet("background-image: url();color: black;");
    QRegExp regexp("letter_[0-9]*");
    QList<QLabel*> list = ui->frmKeyboard->findChildren<QLabel*>(regexp);
    for( int i = 0; i < list.count(); i++ )
    {
        //qDebug()<<list.at(i)->text();
        if( list.at(i)->text() == ui->txtNewText->text().left(1).toUpper() ){
            list.at(i)->setStyleSheet("background-image: url();color: red;");
            const QChar* letter = ui->txtNewText->text().left(1).unicode();
            if ( letter->isUpper() )
            {
                if( list.at(i)->x() > 270 )
                {
                    ui->lblLShift->setStyleSheet("background-image: url();color: red;");
                } else {
                    ui->lblRShift->setStyleSheet("background-image: url();color: red;");
                }

            }
        } else {
            list.at(i)->setStyleSheet("background-image: url();color: black;");
        }
    }
}

void TStamina::lessonChoosed()
{
    QAction *action = (QAction*)this->sender();
    //qDebug()<<action->data().toString();
    this->loadLesson(action->data().toString());
}

void TStamina::timeout()
{
    this->time++;
    this->speed = this->typeRights / this->time * 60;
    qDebug()<<this->speed;
    QString speed;
    speed.setNum(this->speed);
    QTime time;
    time.setHMS(0,0,0,0);
    time = time.addSecs(this->time);
    ui->lblTimer->setText(time.toString("hh:mm:ss"));
}

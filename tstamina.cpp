/*
#    This file is part of QStamina
#
#    Aleksey Lezhoev <lezhoev@gmail.com>
#    Ideas and lessons: http://stamina.ru
#
#    QStamina is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#     any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "tstamina.h"
#include "ui_tstamina.h"

TStamina::TStamina(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TStamina)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("QStamina"));
    this->generalSettings = new QSettings("QStamina","QStamina");
    QString lastLayoutFile = generalSettings->value("lastLayoutFile").toString();
    if( lastLayoutFile == "" || !QFile::exists(QApplication::applicationDirPath()+"/layouts/"+lastLayoutFile) )
    {
        QDir layoutDir;
        QStringList layoutNameFilters;
        layoutNameFilters << "*.ltf";
        layoutDir.setCurrent(QApplication::applicationDirPath()+"/layouts");
        layoutDir.setNameFilters(layoutNameFilters);
        QStringList layouts = layoutDir.entryList(QDir::Files);
        if( layouts.count() > 0 )
        {
            lastLayoutFile = layouts.at(0);
        } else {
            qDebug()<<"Layouts folder is empty! Install layouts first!";
        }
    }
    //this->buildMainMenu();

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
    typeLastSecond = 0;
    speed = 0;

    this->unionLetters = true;
    this->lessonStarted = false;
    this->lessonLoaded = false;

    mainMenu = new QMenuBar(this);
    this->setMenuBar(mainMenu);
    lessonsMenu = new QMenu("Уроки");
    mainMenu->addMenu(lessonsMenu);
    layoutsMenu = new QMenu("Раскладки");
    mainMenu->addMenu(layoutsMenu);

    QMenu *helpMenu = mainMenu->addMenu("?");
    helpMenu->addAction("О программе",this,SLOT(aboutTriggered()));

    loadLayoutMenu();

    loadLayout(lastLayoutFile);

    this->setFixedSize(this->size());
}

TStamina::~TStamina()
{
    delete ui;
}

void TStamina::keyPressEvent(QKeyEvent *event)
{
    //qDebug()<<event->text();
    if( this->lessonStarted )
    {
        if( event->key() == Qt::Key_Escape )
            //this->endLesson();
            this->on_pushButton_released();
        if( event->text() != "")
        {
            if( ui->txtNewText->text().startsWith(event->text()) )
            {
                this->typeRights++;
                this->typeLastSecond++;
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


}

void TStamina::loadLessonsMenu()
{
    this->lessonsMenu->clear();
    QAction *action;
    QDir lessonDir;
    lessonDir.setCurrent(QApplication::applicationDirPath()+"/baselessons/"+this->currentLayout);
    QStringList lessons = lessonDir.entryList(QDir::Files);
    for( int i = 0; i < lessons.count(); i++ )
    {
        qDebug()<<lessonDir.absolutePath()+"/"+lessons.at(i);
        QSettings lesson(lessonDir.absolutePath()+"/"+lessons.at(i),QSettings::IniFormat);
        lesson.setIniCodec("utf-8");
        qDebug()<<lesson.value("title").toString();
        if( lesson.value("title").toString() != "" && lesson.value("content").toString() != "" )
        {
            action = lessonsMenu->addAction(lesson.value("title").toString(),this,SLOT(lessonChoosed()));
            action->setData(lessonDir.absolutePath()+"/"+lessons.at(i));
        }
    }
}

void TStamina::loadLesson(QString lesson)
{
    qDebug()<<"loading lesson from: "<<lesson;
    if( this->lessonStarted )
        this->endLesson();
    QSettings ls(lesson,QSettings::IniFormat);
    ls.setIniCodec("utf-8");
    ui->txtOldText->setText("");
    ui->txtNewText->setText(ls.value("content").toString());
    ui->lblLesson->setText(ls.value("title").toString());
    this->lessonTitle = ls.value("title").toString();
    this->lessonContent = ls.value("content").toString();
    this->lessonLoaded = true;
}

void TStamina::loadLayout(QString layoutFileName)
{
    qDebug()<<"loading layout from: "<<layoutFileName;
    if( this->lessonStarted )
        this->endLesson();

    QString layoutTitle;
    QString layoutName;
    QString layoutSymbols;
    QFile layoutFile(qApp->applicationDirPath()+"/layouts/"+layoutFileName);
    if(!layoutFile.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", layoutFile.errorString());
    }

    QTextStream in(&layoutFile);
    QString layout = in.readAll();
    //qDebug()<<"Readed layout: "<<layout;
    layoutFile.close();

    QRegExp regexp("<title>(.*)</title>");
    int pos = regexp.indexIn(layout);
    if (pos > -1) {
        layoutTitle = regexp.cap(1);
    }
    regexp.setPattern("<layout>(.*)</layout>");
    pos = regexp.indexIn(layout);
    if (pos > -1) {
        layoutName = regexp.cap(1);
    }
    regexp.setPattern("<symbols>(.*)</symbols>");
    pos = regexp.indexIn(layout);
    if (pos > -1) {
        layoutSymbols = regexp.cap(1);
    }

    this->currentLayout = layoutName;
    this->generalSettings->setValue("lastLayoutFile",layoutFileName);
    ui->lblLayout->setText(layoutTitle);
    this->loadKeyboard(layoutSymbols);
    ui->txtOldText->setText("");
    ui->txtNewText->setText("");
    this->lessonLoaded = false;
    loadLessonsMenu();
}

void TStamina::endLesson()
{
    this->timer->stop();
    ui->txtOldText->setText("");
    ui->txtNewText->setText(this->lessonContent);
    QTime time;
    time.setHMS(0,0,0,0);
    time = time.addSecs(this->time);
    QString errors;
    errors.setNum(this->typeErrors);
    QString rights;
    rights.setNum(this->typeRights);
    QString speed;
    speed.setNum(this->speed);
    qDebug()<<"Ошибок: "<<this->typeErrors;
    qDebug()<<"Всего: "<<this->typeRights;
    qDebug()<<"Затрачено времени: "<<time.toString("hh:mm:ss");
    qDebug()<<"Скорость: "<<speed;

    TResults *resultsDialog = new TResults();
    resultsDialog->setWindowTitle(tr("Результаты"));
    resultsDialog->setErrors(errors);
    resultsDialog->setRights(rights);
    resultsDialog->setTime(time.toString("hh:mm:ss"));
    resultsDialog->setSpeed(speed);
    resultsDialog->drawGraph(this->speedBySecond,this->avgSpeedBySecond);
    this->time = 0;
    this->speed = 0;
    this->typeErrors = 0;
    this->typeRights = 0;
    this->typeLastSecond = 0;
    this->avgSpeedBySecond.clear();
    this->speedBySecond.clear();
    resultsDialog->show();
    resultsDialog->setFixedSize(resultsDialog->size());
}

void TStamina::loadKeyboard(QString layout)
{
    qDebug()<<"Loading keyboard layout: "<<layout;
    int ind = 1;
    for( int i = 0; i < layout.length(); i += 2 )
    {
        QString id;
        id.setNum(ind);

        QLabel *letter = ui->frmKeyboard->findChild<QLabel *>("key_"+id);
        QLabel *letterU = ui->frmKeyboard->findChild<QLabel *>("key_"+id+"U");
        //qDebug()<<letter;
        if( this->unionLetters && layout.at(i).toUpper().unicode() == layout.at(i+1).unicode())
        {
            letter->setText(layout.at(i+1));
            //letter->setGeometry(letter->x()+4,letter->y(),letter->width(),letter->height());
            letterU->setText(" ");
        } else {
            letter->setText(layout.at(i));
            letterU->setText(layout.at(i+1));
        }
        ind++;
    }
}

void TStamina::updateKeyboard()
{
    ui->lblLShift->setStyleSheet("background-image: url();color: black;");
    ui->lblRShift->setStyleSheet("background-image: url();color: black;");
    QRegExp regexp("key_[0-9U]*");
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

void TStamina::layoutChoosed()
{
    QAction *action = (QAction*)this->sender();
    //qDebug()<<action->data().toString();
    this->loadLayout(action->data().toString());
}

void TStamina::timeout()
{
    this->time++;
    this->speed = this->typeRights / this->time * 60;

    this->speedBySecond.append(this->typeLastSecond);
    this->avgSpeedBySecond.append(this->typeRights / this->time);
    this->typeLastSecond = 0;
    //qDebug()<<this->speed;
    QString speed;
    speed.setNum(this->speed);
    QTime time;
    time.setHMS(0,0,0,0);
    time = time.addSecs(this->time);
    ui->lblTimer->setText(time.toString("hh:mm:ss"));
}

void TStamina::loadLayoutMenu()
{
    this->layoutsMenu->clear();
    QAction *action;
    QDir layoutDir;
    QStringList layoutNameFilters;
    layoutNameFilters << "*.ltf";
    layoutDir.setCurrent(QApplication::applicationDirPath()+"/layouts");
    layoutDir.setNameFilters(layoutNameFilters);
    QStringList layoutFilesList = layoutDir.entryList(QDir::Files);
    QString layoutTitle;
    for( int i = 0; i < layoutFilesList.count(); i++ )
    {
        qDebug()<<layoutDir.absolutePath()+"/"+layoutFilesList.at(i);
        QFile layoutFile(layoutDir.absolutePath()+"/"+layoutFilesList.at(i));
        if(!layoutFile.open(QIODevice::ReadOnly)) {
            QMessageBox::information(0, "error", layoutFile.errorString());
        }

        QTextStream in(&layoutFile);
        QString layout = in.readAll();
        //qDebug()<<"Readed layout: "<<layout;
        layoutFile.close();

        QRegExp regexp("<title>(.*)<\/title>");
        int pos = regexp.indexIn(layout);
        if (pos > -1) {
            layoutTitle = regexp.cap(1); // "189"
            qDebug()<<layoutTitle;
            //QString unit = regexp.cap(2);  // "cm"
            // ...
        }
        /*QSettings layout(layoutDir.absolutePath()+"/"+layouts.at(i),QSettings::IniFormat);
        layout.setIniCodec("utf-8");
        qDebug()<<layout.value("title").toString();*/
        action = layoutsMenu->addAction(layoutTitle,this,SLOT(layoutChoosed()));
        action->setData(layoutFilesList.at(i));
    }
}

void TStamina::on_pushButton_released()
{
    if( this->lessonStarted )
    {
        this->lessonStarted = false;
        ui->pushButton->setText("Старт");
        this->endLesson();
    } else {
        if( this->lessonLoaded )
        {
            this->lessonStarted = true;
            ui->pushButton->setText("Стоп");
            this->timer->start(1000);
            this->updateKeyboard();
        }
    }
    ui->pushButton->clearFocus();
}

void TStamina::aboutTriggered()
{
    TAbout *about = new TAbout;
    about->setWindowTitle("О программе");
    about->setModal(true);
    about->setFixedSize(about->size());
    about->show();
}

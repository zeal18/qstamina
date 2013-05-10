/*
*  This file is part of QStamina
*
*  Copyright (C) 2012 Aleksey Lezhoev <lezhoev@gmail.com>
*
*  QStamina is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 2 or (at your option)
*  version 3 of the License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stamina.h"
#include "ui_stamina.h"

Stamina::Stamina(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Stamina)
{
    ui->setupUi(this);
    this->setWindowTitle("QStamina");
    m_settings = new Settings;
    m_settings->setModal(true);
    m_settings->setWindowTitle(tr("Settings"));
    connect(m_settings,SIGNAL(settingsSaved()),this,SLOT(settingsSaved()));
    QString lastLayoutFile = m_settings->lastLayoutFile();
#ifdef Q_OS_LINUX
    this->resourcesDir.setCurrent("/usr/share/qstamina");
    if( !this->resourcesDir.exists() )
        this->resourcesDir.setCurrent(QApplication::applicationDirPath());
#endif
#ifdef Q_OS_MACX
    this->resourcesDir.setCurrent(QApplication::applicationDirPath()+"/../Resources");
#endif
#ifdef Q_OS_WIN
    this->resourcesDir.setCurrent(QApplication::applicationDirPath());
#endif
    if( lastLayoutFile == "" || !QFile::exists(this->resourcesDir.absolutePath()+"/layouts/"+lastLayoutFile) )
    {
        QDir layoutDir;
        QStringList layoutNameFilters;
        layoutNameFilters << "*.ltf";
        layoutDir.setCurrent(this->resourcesDir.absolutePath()+"/layouts");
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

    QVBoxLayout *layout = new QVBoxLayout(ui->frameTextField);
    ui->frameTextField->setLayout(layout);
    InlineField *inlineField = new InlineField(ui->frameTextField);
    m_textfield = dynamic_cast< TextField* >(inlineField);
    layout->addWidget(m_textfield);
    connect(m_textfield,SIGNAL(noMoreText()),this,SLOT(on_pushButton_released()));

    m_textfield->setFontPixelSize(m_settings->fontSize());

    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));
    time = 0;
    typeLastSecond = 0;
    speed = 0;

    this->lessonStarted = false;
    this->lessonLoaded = false;

    mainMenu = new QMenuBar(this);
    this->setMenuBar(mainMenu);

    QMenu *fileMenu = mainMenu->addMenu(tr("File"));
    fileMenu->addAction(tr("Settings"),this,SLOT(settingsTriggered()));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Quit"),qApp,SLOT(quit()));

    lessonsMenu = new QMenu(tr("Lessons"));
    mainMenu->addMenu(lessonsMenu);
    layoutsMenu = new QMenu(tr("Layouts"));
    mainMenu->addMenu(layoutsMenu);

    QMenu *helpMenu = mainMenu->addMenu(tr("?"));
    helpMenu->addAction(tr("About"),this,SLOT(aboutTriggered()));

    if( m_settings->separateKeyboard() )
    {
        m_keyboard = new Keyboard(0);
        m_keyboard->show();
        m_keyboard->setWindowTitle(tr("Keyboard"));
        m_keyboard->setFixedSize(m_keyboard->width(),m_keyboard->height());
    } else {
        m_keyboard = new Keyboard(this);
        ui->centralWidget->layout()->addWidget(m_keyboard);
    }

    loadLayoutMenu();

    loadLayout(lastLayoutFile);

    /*ui->frmKeyboard->setFrameStyle(QFrame::StyledPanel);
    ui->frmKeyboard->setStyleSheet("border: 0px;");*/
}

Stamina::~Stamina()
{
    delete ui;
}

void Stamina::keyPressEvent(QKeyEvent *event)
{
    //qDebug()<<event->text();
    if( this->lessonStarted )
    {
        if( event->key() == Qt::Key_Escape )
            //this->endLesson();
            on_pushButton_released();
        if( event->key() == Qt::Key_Backspace )
        {
            m_textfield->keyPressed("Backspace");
        }
        else if( event->key() == Qt::Key_Space )
        {
            m_textfield->keyPressed("Space");
        }
        else if( event->text() != "")
        {
            if( checkKey(event->text()) )
            {
                m_textfield->keyPressed(event->text());
            }
        }
        m_keyboard->updateKeyboard(m_textfield->nextSymbol());
    }


}
bool Stamina::checkKey(QString key)
{
    return currentLayoutSymbols.contains(key);
}

void Stamina::loadLessonsMenu()
{
    qDebug()<<"Loading lessons menu";
    this->lessonsMenu->clear();
    QAction *action;

    QFile lessonsFile(this->resourcesDir.absolutePath()+"/baselessons/"+this->currentLayout+".lsn");

    if ( lessonsFile.open(QFile::ReadOnly) )
    {
        QDomDocument dom;
        dom.setContent(&lessonsFile);
        QDomElement root = dom.documentElement();
        if( root.tagName() == "lessons" )
        {
            QDomElement lesson = root.firstChildElement("lesson");
            while( !lesson.isNull() ){
                Lesson lsn;
                lsn.title = lesson.firstChildElement("title").text().trimmed();
                lsn.content = lesson.firstChildElement("content").text().trimmed();

                if( lsn.title != "" )
                {
                    m_lessons.append(lsn);

                    qDebug()<<"Lesson: "<<lsn.title<<" added to menu.";
                    action = lessonsMenu->addAction(lsn.title,this,SLOT(lessonChoosed()));
                    action->setData(m_lessons.size() - 1);
                }

                lesson = lesson.nextSiblingElement("lesson");
            }
        } else {
            QMessageBox::critical(0, tr("Error"), tr("Lessons file is in wrong format."));
            //exit(EXIT_FAILURE);
        }
    } else {
        QMessageBox::critical(0, tr("Error"), tr("Can't open lessons file."));
        //exit(EXIT_FAILURE);
    }

}

void Stamina::loadLesson(int lessonIndex)
{
    if( this->lessonStarted )
        this->endLesson();

    Lesson lesson = m_lessons.at(lessonIndex);
    qDebug()<<"loading lesson: "<<lesson.title;
    QString lessonTitle = lesson.title;
    QString lessonContent = lesson.content;

    this->m_textfield->setText(lessonContent);
    ui->lblLesson->setText(lessonTitle);
    this->lessonTitle = lessonTitle;
    this->lessonContent = lessonContent;
    this->lessonLoaded = true;
}

void Stamina::loadLayout(QString layoutFileName)
{
    qDebug()<<"loading layout from: "<<this->resourcesDir.absolutePath()+"/layouts/"+layoutFileName;
    if( this->lessonStarted )
        this->endLesson();

    QString layoutTitle;
    QString layoutName;
    QString layoutSymbols;
    QFile layoutFile(this->resourcesDir.absolutePath()+"/layouts/"+layoutFileName);
    if(!layoutFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, tr("Error"), tr("Error loading layout file: %1").arg(layoutFile.errorString()));
        exit(EXIT_FAILURE);
    }

    QTextStream in(&layoutFile);
    QString layout = in.readAll();
    //
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
    this->m_settings->setLastLayoutFile(layoutFileName);
    ui->lblLayout->setText(layoutTitle);
    this->currentLayoutSymbols = layoutSymbols;
    m_keyboard->loadKeyboard(layoutSymbols);
    this->lessonLoaded = false;
    loadLessonsMenu();
}

void Stamina::endLesson()
{
    this->lessonStarted = false;
    m_textfield->stop();
    ui->pushButton->setText(tr("Start"));
    this->timer->stop();
    QTime time;
    time.setHMS(0,0,0,0);
    time = time.addSecs(this->time);
    QString errors;
    errors.setNum(m_textfield->wrongSymbols());
    QString rights;
    rights.setNum(m_textfield->rightSymbols());
    QString speed;
    speed.setNum(this->speed);
    qDebug()<<"Mistypes: "<<m_textfield->wrongSymbols();
    qDebug()<<"Symbols: "<<m_textfield->rightSymbols();
    qDebug()<<"Time: "<<time.toString("hh:mm:ss");
    qDebug()<<"Speed: "<<speed;

    m_textfield->reset();

    Results *resultsDialog = new Results();
    resultsDialog->setErrors(errors);
    resultsDialog->setRights(rights);
    resultsDialog->setTime(time.toString("hh:mm:ss"));
    resultsDialog->setSpeed(speed);
    resultsDialog->drawGraph(this->speedBySecond,this->avgSpeedBySecond);
    this->time = 0;
    this->speed = 0;
    this->typeLastSecond = 0;
    this->avgSpeedBySecond.clear();
    this->speedBySecond.clear();
    resultsDialog->show();
    resultsDialog->setFixedSize(resultsDialog->size());
}

void Stamina::lessonChoosed()
{
    QAction *action = (QAction*)this->sender();
    //qDebug()<<action->data().toString();
    this->loadLesson(action->data().toInt());
}

void Stamina::layoutChoosed()
{
    QAction *action = (QAction*)this->sender();
    //qDebug()<<action->data().toString();
    this->loadLayout(action->data().toString());
}

void Stamina::timeout()
{
    this->time++;
    this->speed = m_textfield->rightSymbols() / this->time * 60;

    this->speedBySecond.append(m_textfield->rightSymbols() - this->typeLastSecond);
    this->avgSpeedBySecond.append(m_textfield->rightSymbols() / this->time);
    this->typeLastSecond = m_textfield->rightSymbols();
    //qDebug()<<this->speed;
    QString speed;
    speed.setNum(this->speed);
    QTime time;
    time.setHMS(0,0,0,0);
    time = time.addSecs(this->time);
    ui->lblTimer->setText(time.toString("hh:mm:ss"));
}

void Stamina::loadLayoutMenu()
{
    this->layoutsMenu->clear();
    QAction *action;
    QDir layoutDir;
    QStringList layoutNameFilters;
    layoutNameFilters << "*.ltf";
    layoutDir.setCurrent(this->resourcesDir.absolutePath()+"/layouts");
    layoutDir.setNameFilters(layoutNameFilters);
    QStringList layoutFilesList = layoutDir.entryList(QDir::Files);
    QString layoutTitle;
    for( int i = 0; i < layoutFilesList.count(); i++ )
    {
        qDebug()<<layoutDir.absolutePath()+"/"+layoutFilesList.at(i);
        QFile layoutFile(layoutDir.absolutePath()+"/"+layoutFilesList.at(i));
        if(!layoutFile.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(0, tr("Error"), tr("Error loading layouts files: %1").arg(layoutFile.errorString()));
            exit(EXIT_FAILURE);
        }

        QTextStream in(&layoutFile);
        QString layout = in.readAll();
        //
        layoutFile.close();

        QRegExp regexp("<title>(.*)</title>");
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

void Stamina::on_pushButton_released()
{
    if( this->lessonStarted )
    {
        this->endLesson();
    } else {
        if( this->lessonLoaded )
        {
            this->lessonStarted = true;
            m_textfield->start();
            ui->pushButton->setText(tr("Stop"));
            this->timer->start(1000);
            m_keyboard->updateKeyboard(m_textfield->nextSymbol());
        }
    }
    ui->pushButton->clearFocus();
}

void Stamina::aboutTriggered()
{
    About *about = new About;
    about->setModal(true);
    about->setFixedSize(about->size());
    about->show();
}

void Stamina::settingsTriggered()
{
    m_settings->show();
}

void Stamina::settingsSaved()
{
    m_textfield->setFontPixelSize(m_settings->fontSize());
}

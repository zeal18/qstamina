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

#include "stamina.h"
#include "ui_stamina.h"

Stamina::Stamina(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Stamina)
{
    ui->setupUi(this);
    this->setWindowTitle("QStamina");
    this->generalSettings = new QSettings("QStamina","QStamina");
    QString lastLayoutFile = generalSettings->value("lastLayoutFile").toString();
#ifdef Q_OS_LINUX
    this->resourcesDir.setCurrent("/usr/share/qstamina");
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


    InlineField *inlineField = new InlineField(ui->frameTextField);
    m_textfield = dynamic_cast< TextField* >(inlineField);

    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));
    time = 0;
    typeLastSecond = 0;
    speed = 0;

    this->unionLetters = true;
    this->lessonStarted = false;
    this->lessonLoaded = false;

    mainMenu = new QMenuBar(this);
    this->setMenuBar(mainMenu);
    lessonsMenu = new QMenu(tr("Уроки"));
    mainMenu->addMenu(lessonsMenu);
    layoutsMenu = new QMenu(tr("Раскладки"));
    mainMenu->addMenu(layoutsMenu);

    QMenu *helpMenu = mainMenu->addMenu(tr("?"));
    helpMenu->addAction(tr("О программе"),this,SLOT(aboutTriggered()));

    loadLayoutMenu();

    loadLayout(lastLayoutFile);

    ui->frmKeyboard->setFrameStyle(QFrame::StyledPanel);
    ui->frmKeyboard->setStyleSheet("border: 0px; background-image: url("+this->resourcesDir.absolutePath()+"/keyboard.png);");

    this->setFixedSize(this->size());
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
        if( event->text() != "")
        {
            m_textfield->keyPressed(event->text());
            updateKeyboard();
        }
    }


}

void Stamina::loadLessonsMenu()
{
    this->lessonsMenu->clear();
    QAction *action;
    QDir lessonDir;

    if ( lessonDir.setCurrent(this->resourcesDir.absolutePath()+"/baselessons/"+this->currentLayout) )
    {
        QStringList lessons = lessonDir.entryList(QDir::Files);
        for( int i = 0; i < lessons.count(); i++ )
        {


            QString lessonTitle;

            QFile lessonFile(lessonDir.absolutePath()+"/"+lessons.at(i));

            if(!lessonFile.open(QIODevice::ReadOnly)) {
                QMessageBox::information(0, tr("Ошибка"), lessonFile.errorString());
            }

            QTextStream in(&lessonFile);
            QString lesson = in.readAll();
            //
            lessonFile.close();

            QRegExp regexp("<title>(.*)</title>");
            int pos = regexp.indexIn(lesson);
            if (pos > -1) {
                lessonTitle = regexp.cap(1);
            }
            if( lessonTitle != "" )
            {
                qDebug()<<lessonDir.absolutePath()+"/"+lessons.at(i)<<" added to menu as: "<<lessonTitle;
                action = lessonsMenu->addAction(lessonTitle,this,SLOT(lessonChoosed()));
                action->setData(lessonDir.absolutePath()+"/"+lessons.at(i));
            }
        }
    }

}

void Stamina::loadLesson(QString lessonFilePath)
{
    qDebug()<<"loading lesson from: "<<lessonFilePath;
    if( this->lessonStarted )
        this->endLesson();
    /*QSettings ls(lesson,QSettings::IniFormat);
    ls.setIniCodec("utf-8");*/

    QString lessonTitle;
    QString lessonContent;

    QFile lessonFile(lessonFilePath);

    if(!lessonFile.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, tr("Ошибка"), lessonFile.errorString());
    }

    QTextStream in(&lessonFile);
    QString lesson = in.readAll();

    lessonFile.close();

    QRegExp regexp("<title>(.*)</title>");
    int pos = regexp.indexIn(lesson);
    if (pos > -1) {
        lessonTitle = regexp.cap(1);
    }
    regexp.setPattern("<content>(.*)</content>");
    pos = regexp.indexIn(lesson);
    if (pos > -1) {
        lessonContent = regexp.cap(1);
    }

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
        QMessageBox::information(0, tr("Ошибка"), layoutFile.errorString());
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
    this->generalSettings->setValue("lastLayoutFile",layoutFileName);
    ui->lblLayout->setText(layoutTitle);
    this->loadKeyboard(layoutSymbols);
    this->lessonLoaded = false;
    loadLessonsMenu();
}

void Stamina::endLesson()
{
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

    Results *resultsDialog = new Results();
    //resultsDialog->setWindowTitle(tr("Результаты"));
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

void Stamina::loadKeyboard(QString layout)
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
        if ( layout.length() < ( i+2 ) )
        {
            letter->setText(layout.at(i));
            letterU->setText("");
        } else {
            if( this->unionLetters && layout.at(i).toUpper().unicode() == layout.at(i+1).unicode())
            {
                letter->setText(layout.at(i+1));
                //letter->setGeometry(letter->x()+4,letter->y(),letter->width(),letter->height());
                letterU->setText("");
            } else {
                letter->setText(layout.at(i));
                letterU->setText(layout.at(i+1));
            }
        }

        ind++;
    }
}

void Stamina::updateKeyboard()
{
    ui->lblLShift->setStyleSheet("background-image: url();color: black;");
    ui->lblRShift->setStyleSheet("background-image: url();color: black;");
    //ui->fFrame->setStyleSheet("background-image: url();background-color: orange;color: black;");
    QRegExp regexp("key_[0-9U]*");
    QList<QLabel*> list = ui->frmKeyboard->findChildren<QLabel*>(regexp);
    QString letter = this->m_textfield->nextSymbol();
    const QChar* symbol = letter.unicode();
    for( int i = 0; i < list.count(); i++ )
    {
        //qDebug()<<list.at(i)->parentWidget();
        if( list.at(i)->text() == letter.toUpper() ){
            list.at(i)->setStyleSheet("background-image: url();color: red;");

            if ( symbol->isUpper() )
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

void Stamina::lessonChoosed()
{
    QAction *action = (QAction*)this->sender();
    //qDebug()<<action->data().toString();
    this->loadLesson(action->data().toString());
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
            QMessageBox::information(0, tr("Ошибка"), layoutFile.errorString());
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
        this->lessonStarted = false;
        ui->pushButton->setText(tr("Старт"));
        this->endLesson();
    } else {
        if( this->lessonLoaded )
        {
            this->lessonStarted = true;
            ui->pushButton->setText(tr("Стоп"));
            this->timer->start(1000);
            this->updateKeyboard();
        }
    }
    ui->pushButton->clearFocus();
}

void Stamina::aboutTriggered()
{
    About *about = new About;
    //about->setWindowTitle("О программе");
    about->setModal(true);
    about->setFixedSize(about->size());
    about->show();
}

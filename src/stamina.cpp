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


    InlineField *inlineField = new InlineField(ui->frameTextField);
    m_textfield = dynamic_cast< TextField* >(inlineField);
    connect(m_textfield,SIGNAL(noMoreText()),this,SLOT(on_pushButton_released()));

    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));
    time = 0;
    typeLastSecond = 0;
    speed = 0;

    m_lastSelectedSymbol = 0;

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
    ui->frmKeyboard->setStyleSheet("border: 0px;");

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
        updateKeyboard();
    }


}
bool Stamina::checkKey(QString key)
{
    return currentLayoutSymbols.contains(key);
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
                QMessageBox::information(0, tr("Ошибка"), tr("Не удается загрузить файлы уроков: ")+lessonFile.errorString());
                exit(EXIT_FAILURE);
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
        QMessageBox::information(0, tr("Ошибка"), tr("Не удается загрузить файлы уроков: ")+lessonFile.errorString());
        exit(EXIT_FAILURE);
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
        QMessageBox::information(0, tr("Ошибка"), tr("Не удается загрузить раскладок клавиатуры: ")+layoutFile.errorString());
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
    this->generalSettings->setValue("lastLayoutFile",layoutFileName);
    ui->lblLayout->setText(layoutTitle);
    this->currentLayoutSymbols = layoutSymbols;
    this->loadKeyboard(layoutSymbols);
    this->lessonLoaded = false;
    loadLessonsMenu();
}

void Stamina::endLesson()
{
    this->lessonStarted = false;
    m_textfield->stop();
    ui->pushButton->setText(tr("Старт"));
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
    QRegExp regexp("button_([a-z0-9]*)[_]{0,1}([LR]*)_[0-9]");
    QString name;
    QString colorName;
    QColor color;
    QWidget *letterParent;
    QList<QFrame*> list = ui->frmKeyboard->findChildren<QFrame*>(regexp);
    for( int q = 0; q < list.count(); q++ )
    {
        name = list.at(q)->objectName();
        regexp.indexIn(name);
        colorName = "#"+regexp.cap(1);
        color.setNamedColor(colorName);
        color = color.darker(120);
        list.at(q)->setStyleSheet("border: 1px solid "+color.name()+"; border-radius: 3px; background-image: url(); background-color: "+colorName+";");
    }
    QList<QLabel*> commLbls;
    commLbls.append(ui->lblBackspace);
    commLbls.append(ui->lblTab);
    commLbls.append(ui->lblCaps);
    commLbls.append(ui->lblSpace);
    commLbls.append(ui->lblLAlt);
    commLbls.append(ui->lblLControl);
    commLbls.append(ui->lblLShift);
    commLbls.append(ui->lblLSuper);

    commLbls.append(ui->lblEnter);
    commLbls.append(ui->lblRAlt);
    commLbls.append(ui->lblRControl);
    commLbls.append(ui->lblRShift);
    commLbls.append(ui->lblRSuper);

    for( int w = 0; w < commLbls.count(); w++)
    {
        letterParent = commLbls.at(w)->parentWidget();
        name = letterParent->objectName();
        regexp.indexIn(name);
        colorName = "#"+regexp.cap(1);
        color.setNamedColor(colorName);
        color = color.darker(140);
        commLbls.at(w)->setStyleSheet("border: 0px; background-image: url(); background-color: ; color: "+color.name()+";;");
        commLbls.at(w)->setContentsMargins(0,0,0,0);
        commLbls.at(w)->setAlignment(Qt::AlignCenter);
        //commLbls.at(w)->setGeometry((int)(letterParent->width() / 2 - commLbls.at(w)->width() / 2),(int)(letterParent->height() / 2 - commLbls.at(w)->height() / 2),commLbls.at(w)->width(),commLbls.at(w)->height());
    }


    int ind = 1;
    for( int i = 0; i < layout.length(); i += 2 )
    {
        QString id;
        id.setNum(ind);

        QLabel *letter = ui->frmKeyboard->findChild<QLabel *>("key_"+id);
        letterParent = letter->parentWidget();
        QLabel *letterU = ui->frmKeyboard->findChild<QLabel *>("key_"+id+"U");
        //qDebug()<<letter;
        qDebug()<<layout.at(i)<<layout.at(i+1);
        if( layout.length() < ( i+1 ) || ( this->unionLetters && layout.at(i).toUpper().unicode() == layout.at(i+1).unicode() ) )
        {

            if( letter != 0)
            {
                letter->setText(layout.at(i).toUpper());
                letter->setGeometry(0,0,20,20);
                letter->setContentsMargins(0,0,0,0);
                letter->setAlignment(Qt::AlignCenter);

                letter->setGeometry((int)(letterParent->width() / 2 - letter->width() / 2),(int)(letterParent->height() / 2 - letter->height() / 2),letter->width(),letter->height());
                name = letterParent->objectName();
                regexp.indexIn(name);
                colorName = "#"+regexp.cap(1);
                color.setNamedColor(colorName);
                color = color.darker(140);
                letter->setStyleSheet("border: 0px; background-image: url(); background-color: ; color: "+color.name()+";");
            }

            if( letterU != 0 )
            {
                letterU->setText("");
                letterU->setGeometry(0,0,0,0);
            }
        } else {
            letter->setText(layout.at(i));
            letterU->setText(layout.at(i+1));


            name = letterParent->objectName();
            QRegExp regName("button_([a-z0-9]*)[_]{0,1}([LR]*)_[0-9]");
            regName.indexIn(name);
            QString colorName = "#"+regName.cap(1);
            QColor color;
            color.setNamedColor(colorName);
            color = color.darker(140);
            QString styleSheet = "border: 0px; background-image: url(); background-color: ; color: "+color.name()+";";

            letter->setStyleSheet(styleSheet);
            letter->setGeometry(0,0,20,20);
            letter->setContentsMargins(0,0,0,0);
            letter->setAlignment(Qt::AlignCenter);
            letter->setGeometry(1,letterParent->height() - letter->height() - 1,letter->width(),letter->height());

            letterU->setStyleSheet(styleSheet);
            letterU->setGeometry(0,0,20,20);
            letterU->setContentsMargins(0,0,0,0);
            letterU->setAlignment(Qt::AlignCenter);
            letterU->setGeometry(letterParent->width() - letterU->width() - 1,1,letterU->width(),letterU->width());
        }

        ind++;
    }

}

void Stamina::updateKeyboard()
{
    QRegExp regName("button_([a-z0-9]*)[_]{0,1}([LR]*)_[0-9]*");
    QList<QLabel*> commLbls;
    commLbls.append(ui->lblLShift);
    commLbls.append(ui->lblRShift);
    commLbls.append(ui->lblSpace);

    for( int w = 0; w < commLbls.count(); w++)
    {
        QWidget *letterParent = commLbls.at(w)->parentWidget();
        QString name = letterParent->objectName();
        regName.indexIn(name);
        QString colorName = "#"+regName.cap(1);
        QColor color;
        color.setNamedColor(colorName);
        QColor usualTextColor = color.darker(140);
        commLbls.at(w)->setStyleSheet("border: 0px; background-image: url(); background-color: ; color: "+usualTextColor.name()+";;");
        commLbls.at(w)->setContentsMargins(0,0,0,0);
        commLbls.at(w)->setAlignment(Qt::AlignCenter);
        QColor usualButtonBorderColor = color.darker(120);
        letterParent->setStyleSheet("border: 1px solid "+usualButtonBorderColor.name()+"; border-radius: 3px; background-image: url(); background-color: "+color.name()+";");
    }
    QRegExp regexp("key_[0-9]*([U]{0,1})");
    QList<QLabel*> list = ui->frmKeyboard->findChildren<QLabel*>(regexp);
    QString letter = this->m_textfield->nextSymbol();
    const QChar* symbol = letter.unicode();
    QWidget *parentButton;
    if( m_lastSelectedSymbol != 0 )
    {
        parentButton  = m_lastSelectedSymbol->parentWidget();
        QString name = parentButton->objectName();
        regName.indexIn(name);
        QString colorName = "#"+regName.cap(1);
        QColor color;
        color.setNamedColor(colorName);
        QColor usualButtonColor = color;
        QColor usualButtonBorderColor = color.darker(120);
        QColor usualTextColor = color.darker(140);

        QString usualTextStyleSheet = "border: 0px; background-image: url(); background-color: ; color: "+usualTextColor.name()+";";
        QString usualButtonStyleSheet = "border: 1px solid "+usualButtonBorderColor.name()+"; border-radius: 3px; background-image: url(); background-color: "+usualButtonColor.name()+";";
        m_lastSelectedSymbol->setStyleSheet(usualTextStyleSheet);
        parentButton->setStyleSheet(usualButtonStyleSheet);
    }
    if( letter == " " )
    {
        m_lastSelectedSymbol = ui->lblSpace;

        parentButton  = ui->lblSpace->parentWidget();
        QString name = parentButton->objectName();
        QRegExp regName("button_([a-z0-9]*)[_]{0,1}([LR]*)_[0-9]*");
        regName.indexIn(name);
        QString colorName = "#"+regName.cap(1);
        QColor color;
        color.setNamedColor(colorName);
        QColor hilightButtonColor = color.darker(120);
        QColor hilightButtonBorderColor = color.darker(150);
        QColor hilightTextColor = color.darker(250);

        QString hilightTextStyleSheet = "border: 0px; background-image: url(); background-color: ; color: "+hilightTextColor.name()+";";
        QString hilightButtonStyleSheet = "border: 1px solid "+hilightButtonBorderColor.name()+"; border-radius: 3px; background-image: url(); background-color: "+hilightButtonColor.name()+";";

        ui->lblSpace->setStyleSheet(hilightTextStyleSheet);
        parentButton->setStyleSheet(hilightButtonStyleSheet);

    } else {
        for( int i = 0; i < list.count(); i++ )
        {
            parentButton  = list.at(i)->parentWidget();
            QString labelName = list.at(i)->objectName();
            regexp.indexIn(labelName);
            bool upperSymbol = (regexp.cap(1) == "U");
            QString name = parentButton->objectName();
            QRegExp regName("button_([a-z0-9]*)[_]{0,1}([LR]*)_[0-9]*");
            regName.indexIn(name);
            QString colorName = "#"+regName.cap(1);
            bool rightSymbol = (regName.cap(2) == "R");
            QColor color;
            color.setNamedColor(colorName);
            QColor hilightButtonColor = color.darker(120);
            QColor hilightButtonBorderColor = color.darker(150);
            QColor hilightTextColor = color.darker(250);

            QString hilightTextStyleSheet = "border: 0px; background-image: url(); background-color: ; color: "+hilightTextColor.name()+";";
            QString hilightButtonStyleSheet = "border: 1px solid "+hilightButtonBorderColor.name()+"; border-radius: 3px; background-image: url(); background-color: "+hilightButtonColor.name()+";";

            if( list.at(i)->text() == letter.toUpper() ){
                list.at(i)->setStyleSheet(hilightTextStyleSheet);
                m_lastSelectedSymbol = list.at(i);
                parentButton->setStyleSheet(hilightButtonStyleSheet);
                if ( symbol->isUpper() || upperSymbol )
                {
                    QLabel *shift;
                    if( rightSymbol )
                    {
                        shift = ui->lblLShift;

                    } else {
                        shift = ui->lblRShift;
                    }
                    name = shift->parentWidget()->objectName();
                    regName.indexIn(name);
                    colorName = "#"+regName.cap(1);
                    color.setNamedColor(colorName);
                    hilightButtonColor = color.darker(120);
                    hilightButtonBorderColor = color.darker(150);
                    hilightTextColor = color.darker(250);

                    hilightTextStyleSheet = "border: 0px; background-image: url(); background-color: ; color: "+hilightTextColor.name()+";";
                    hilightButtonStyleSheet = "border: 1px solid "+hilightButtonBorderColor.name()+"; border-radius: 3px; background-image: url(); background-color: "+hilightButtonColor.name()+";";

                    shift->setStyleSheet(hilightTextStyleSheet);
                    shift->parentWidget()->setStyleSheet(hilightButtonStyleSheet);

                }
                break;
            }
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
            QMessageBox::information(0, tr("Ошибка"), tr("Не удается загрузить раскладок клавиатуры: ")+layoutFile.errorString());
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

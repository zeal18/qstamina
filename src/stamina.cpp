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
    ui(new Ui::Stamina),
    m_settings(NULL)
{
    ui->setupUi(this);

    m_config = new Config;

    QDir storage;
    storage.mkpath(QStandardPaths::writableLocation(QStandardPaths::DataLocation)+"/generatedLessons");

    this->setWindowTitle("QStamina");

    QVBoxLayout *layout = new QVBoxLayout(ui->frameTextField);
    ui->frameTextField->setLayout(layout);
    InlineField *inlineField = new InlineField(ui->frameTextField);
    m_textfield = dynamic_cast< TextField* >(inlineField);
    layout->addWidget(m_textfield);
    connect(m_textfield,SIGNAL(noMoreText()),this,SLOT(on_pushButton_released()));

    m_textfield->setFontPixelSize(m_config->fontSize());

    m_timer = new QTimer();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeout()));
    m_time = 0;
    m_typeLastSecond = 0;
    m_speed = 0;

    m_lessonStarted = false;
    m_lessonLoaded = false;

    m_mainMenu = new QMenuBar(this);
    this->setMenuBar(m_mainMenu);

#ifdef Q_OS_MACX
    QMenu *fileMenu = m_mainMenu->addMenu("File");
    fileMenu->addAction("Settings",this,SLOT(settingsTriggered()));
    fileMenu->addSeparator();
    fileMenu->addAction("Quit",qApp,SLOT(quit()));
#elif
    QMenu *fileMenu = m_mainMenu->addMenu(tr("File"));
    fileMenu->addAction(tr("Settings"),this,SLOT(settingsTriggered()));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Quit"),qApp,SLOT(quit()));
#endif
    m_lessonsMenu = new QMenu(tr("Lessons"));
    m_mainMenu->addMenu(m_lessonsMenu);
    m_layoutsMenu = new QMenu(tr("Layouts"));
    m_mainMenu->addMenu(m_layoutsMenu);
    m_generatorMenu = new QMenu(tr("Generator"));
    //m_mainMenu->addMenu(m_generatorMenu);

#ifdef Q_OS_MACX
    QMenu *helpMenu = m_mainMenu->addMenu(tr("?"));
    helpMenu->addAction("About",this,SLOT(aboutTriggered()));
#elif
    QMenu *helpMenu = m_mainMenu->addMenu(tr("?"));
    helpMenu->addAction(tr("About"),this,SLOT(aboutTriggered()));
#endif

    if( m_config->separateKeyboard() )
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
    loadCurrentLayout();
}

Stamina::~Stamina()
{
    delete ui;
}

void Stamina::keyPressEvent(QKeyEvent *event)
{
    //qDebug()<<event->text();
    if( m_lessonStarted )
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
            m_textfield->keyPressed(event->text());
        }
        m_keyboard->updateKeyboard(m_textfield->nextSymbol());
    }


}

void Stamina::loadLessonsMenu()
{
    qDebug()<<"Loading lessons menu";
    m_lessonsMenu->clear();
    QAction *action;

    QStringList groups = m_config->lessons().groups();

    for( int i = 0; i < groups.count(); i++ )
    {
        QList<Lesson*> lessons = m_config->lessons().lessonsByGroup(groups.at(i));
        QMenu *groupMenu = new QMenu(groups.at(i),m_lessonsMenu);
        for( int q = 0; q < lessons.count(); q++ )
        {
            action = groupMenu->addAction(lessons.at(q)->title,this,SLOT(lessonChoosed()));
            QVariant actionData;
            actionData.setValue(lessons.at(q));
            action->setData(actionData);
        }
        if( lessons.count() > 0 )
        {
            m_lessonsMenu->addMenu(groupMenu);
        }
    }
    m_generatorMenu->deleteLater();
    m_generatorMenu = new QMenu(tr("Generator"));
    loadGeneratedLessons();
    //qDebug()<<m_generatorMenu->actions().count();
    m_lessonsMenu->addSeparator();
    m_lessonsMenu->addMenu(m_generatorMenu);
}

void Stamina::loadLesson(Lesson *lesson)
{
    if( m_lessonStarted )
        this->endLesson();

    qDebug()<<"loading lesson: "<<lesson->title;

    m_textfield->setText(lesson->content);
    ui->lblLesson->setText(lesson->title);
    m_lessonLoaded = true;
}

void Stamina::loadCurrentLayout()
{
    if( m_lessonStarted )
        this->endLesson();

    ui->lblLesson->setText("   ");
    m_textfield->setText("");
    ui->lblLayout->setText(m_config->currentLayout()->title);
    m_keyboard->loadKeyboard(m_config->currentLayout()->symbols);
    m_lessonLoaded = false;
    loadLessonsMenu();
}

void Stamina::endLesson()
{
    m_lessonStarted = false;
    m_textfield->stop();
    ui->pushButton->setText(tr("Start"));
    m_timer->stop();
    QTime time;
    time.setHMS(0,0,0,0);
    time = time.addSecs(m_time);
    QString errors;
    errors.setNum(m_textfield->wrongSymbols());
    QString rights;
    rights.setNum(m_textfield->rightSymbols());
    QString speed;
    speed.setNum(m_speed);
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
    resultsDialog->drawGraph(m_speedBySecond,m_avgSpeedBySecond);
    m_time = 0;
    m_speed = 0;
    m_typeLastSecond = 0;
    m_avgSpeedBySecond.clear();
    m_speedBySecond.clear();
    resultsDialog->show();
    resultsDialog->setFixedSize(resultsDialog->size());
}

void Stamina::lessonChoosed()
{
    QAction *action = (QAction*)sender();
    //qDebug()<<action->data().toString();
    loadLesson( action->data().value<Lesson*>() );
}

void Stamina::generatedlessonChoosed()
{
    QAction *action = (QAction*)sender();
    //qDebug()<<action->data().toString();
    loadLesson( action->data().value<Lesson*>() );
}

void Stamina::layoutChoosed()
{
    QAction *action = (QAction*)sender();
    if( m_config->setCurrentLayout(action->data().toInt()) )
    {
        loadCurrentLayout();
    }
}

void Stamina::timeout()
{
    m_time++;
    m_speed = m_textfield->rightSymbols() / m_time * 60;

    m_speedBySecond.append(m_textfield->rightSymbols() - m_typeLastSecond);
    m_avgSpeedBySecond.append(m_textfield->rightSymbols() / m_time);
    m_typeLastSecond = m_textfield->rightSymbols();
    //qDebug()<<this->speed;
    QString speed;
    speed.setNum(m_speed);
    QTime time;
    time.setHMS(0,0,0,0);
    time = time.addSecs(m_time);
    ui->lblTimer->setText(time.toString("hh:mm:ss"));
}

void Stamina::loadLayoutMenu()
{
    m_layoutsMenu->clear();
    QAction *action;

    for( int i = 0; i < m_config->layouts().count(); i++ )
    {
        action = m_layoutsMenu->addAction(m_config->layouts().at(i)->title,this,SLOT(layoutChoosed()));
        action->setData(i);
    }
}

void Stamina::loadGeneratorMenu()
{
    m_generatorMenu->clear();
    m_generatorMenu->addAction(tr("Generate"),this,SLOT(generatorTriggered()));
    m_generatorMenu->addSeparator();
}

void Stamina::loadGeneratedLessons()
{
    loadGeneratorMenu();
    qDebug()<<"Loading generated lessons menu.";
    QAction *action;

    for( int i = 0; i < m_config->generatedLessons().count(); i++ )
    {
        qDebug()<<"Lesson: "<<m_config->generatedLessons().at(i)->title<<" added to menu.";
        action = m_generatorMenu->addAction(m_config->generatedLessons().at(i)->title,this,SLOT(generatedlessonChoosed()));
        QVariant actionData;
        actionData.setValue(m_config->generatedLessons().at(i));
        action->setData(actionData);
    }
}

void Stamina::on_pushButton_released()
{
    if( m_lessonStarted )
    {
        this->endLesson();
    } else {
        if( m_lessonLoaded )
        {
            m_lessonStarted = true;
            m_textfield->start();
            ui->pushButton->setText(tr("Stop"));
            m_timer->start(1000);
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
    if( !m_settings )
    {
        m_settings = new SettingsForm(m_config);
        m_settings->setModal(true);
        m_settings->setWindowTitle(tr("Settings"));
        connect(m_settings,SIGNAL(settingsSaved()),this,SLOT(settingsSaved()));
    }
    m_settings->show();
}

void Stamina::settingsSaved()
{
    m_textfield->setFontPixelSize(m_config->fontSize());
}

void Stamina::generatorTriggered()
{
    LessonGenerator lessonGenerator(m_config);
    if( lessonGenerator.generate() )
    {
        loadGeneratedLessons();
    }
}

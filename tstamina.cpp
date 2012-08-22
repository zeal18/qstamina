#include "tstamina.h"
#include "ui_tstamina.h"

TStamina::TStamina(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TStamina)
{
    ui->setupUi(this);
    this->generalSettings = new QSettings("QStamina","QStamina");
    QString lastLayoutFile = generalSettings->value("lastLayoutFile").toString();
    if( lastLayoutFile == "" || !QFile::exists(QApplication::applicationDirPath()+"/layouts/"+lastLayoutFile) )
    {
        QDir layoutDir;
        QStringList layoutNameFilters;
        layoutNameFilters << "*.ini";
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
    speed = 0;

    this->unionLetters = true;


    mainMenu = new QMenuBar(this);
    this->setMenuBar(mainMenu);
    lessonsMenu = new QMenu("Уроки");
    mainMenu->addMenu(lessonsMenu);
    layoutsMenu = new QMenu("Раскладки");
    mainMenu->addMenu(layoutsMenu);
    loadLayoutMenu();

    loadLayout(lastLayoutFile);
}

TStamina::~TStamina()
{
    delete ui;
}

void TStamina::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<event->text();
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
    QSettings ls(lesson,QSettings::IniFormat);
    ls.setIniCodec("utf-8");
    ui->txtOldText->setText("");
    ui->txtNewText->setText(ls.value("content").toString());
    this->timer->start(1000);
    this->updateKeyboard();

}

void TStamina::loadLayout(QString layoutFile)
{
    qDebug()<<"loading layout from: "<<layoutFile;
    QSettings ly(QApplication::applicationDirPath()+"/layouts/"+layoutFile,QSettings::IniFormat);
    ly.setIniCodec("utf-8");
    this->currentLayout = ly.value("layout").toString();
    this->generalSettings->setValue("lastLayoutFile",layoutFile);
    this->loadKeyboard(ly.value("content").toString());
    loadLessonsMenu();
}

void TStamina::endLesson()
{
    this->timer->stop();
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
    layoutNameFilters << "*.ini";
    layoutDir.setCurrent(QApplication::applicationDirPath()+"/layouts");
    layoutDir.setNameFilters(layoutNameFilters);
    QStringList layouts = layoutDir.entryList(QDir::Files);
    for( int i = 0; i < layouts.count(); i++ )
    {
        qDebug()<<layoutDir.absolutePath()+"/"+layouts.at(i);
        QSettings layout(layoutDir.absolutePath()+"/"+layouts.at(i),QSettings::IniFormat);
        layout.setIniCodec("utf-8");
        qDebug()<<layout.value("title").toString();
        action = layoutsMenu->addAction(layout.value("title").toString(),this,SLOT(layoutChoosed()));
        action->setData(layouts.at(i));
    }
}

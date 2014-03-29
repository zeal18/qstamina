#include "keyboard.h"
#include "ui_keyboard.h"

Keyboard::Keyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Keyboard)
{
    ui->setupUi(this);
    m_unionLetters = true;
    m_lastSelectedSymbol = 0;

    this->setStyleSheet("border: 0px");
}

Keyboard::~Keyboard()
{
    delete ui;
}

void Keyboard::loadKeyboard(QString layout)
{
    qDebug()<<"Loading keyboard layout: "<<layout;
    QRegExp regexp("button_([a-z0-9]*)[_]{0,1}([LR]*)_[0-9]");
    QString name;
    QString colorName;
    QColor color;
    QWidget *letterParent;
    QList<QFrame*> list = ui->keyboard->findChildren<QFrame*>(regexp);
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

        QLabel *letter = ui->keyboard->findChild<QLabel *>("key_"+id);
        letterParent = letter->parentWidget();
        QLabel *letterU = ui->keyboard->findChild<QLabel *>("key_"+id+"U");
        //qDebug()<<letter;
        qDebug()<<layout.at(i)<<layout.at(i+1);
        if( layout.length() < ( i+1 ) || ( m_unionLetters && layout.at(i).toUpper().unicode() == layout.at(i+1).unicode() ) )
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

void Keyboard::updateKeyboard(QString nextSymbol)
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
    QList<QLabel*> list = ui->keyboard->findChildren<QLabel*>(regexp);
    QString letter = nextSymbol;
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

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

#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include "stamina.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator myTranslator;
    myTranslator.load("qstamina_" + QLocale::system().name(),QApplication::applicationDirPath()+"/Languages");
    a.installTranslator(&myTranslator);
#ifdef Q_OS_LINUX
    myTranslator.load("qstamina_" + QLocale::system().name(),"/usr/share/qstamina/translations");
    a.installTranslator(&myTranslator);
#endif
#ifdef Q_OS_MACX
    myTranslator.load("qstamina_" + QLocale::system().name(),QApplication::applicationDirPath()+"/../Languages");
    a.installTranslator(&myTranslator);

    QDir pluginsDir(QApplication::applicationDirPath()+"/../Plugins");
    a.setLibraryPaths(QStringList(pluginsDir.absolutePath()));
#endif
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    Stamina w;

    w.show();
    
    return a.exec();
}

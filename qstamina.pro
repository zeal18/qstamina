#-------------------------------------------------
#
# Project created by QtCreator 2012-08-19T17:37:27
#
#-------------------------------------------------
#
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
#
QT       += core gui

TARGET = qstamina
TEMPLATE = app


SOURCES += main.cpp\
        tstamina.cpp \
    tresults.cpp \
    tabout.cpp

HEADERS  += tstamina.h \
    tresults.h \
    tabout.h

FORMS    += tstamina.ui \
    tresults.ui \
    tabout.ui

RESOURCES += \
    res.qrc

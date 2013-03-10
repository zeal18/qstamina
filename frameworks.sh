#!/bin/sh
mkdir qstamina.app/Contents/Frameworks
cp -R /Library/Frameworks/QtCore.framework qstamina.app/Contents/Frameworks/
cp -R /Library/Frameworks/QtGui.framework qstamina.app/Contents/Frameworks/
install_name_tool -id @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore qstamina.app/Contents/Frameworks/QtCore.framework/Versions/4/QtCore

install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui qstamina.app/Contents/Frameworks/QtGui.framework/Versions/4/QtGui

install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore qstamina.app/Contents/MacOS/qstamina

install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui qstamina.app/Contents/MacOS/qstamina

install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore qstamina.app/Contents/Frameworks/QtGui.framework/Versions/4/QtGui

/*
    Copyright 2007-2008 Fela Winkelmolen <fela.kde@gmail.com> 
  
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.
   
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
   
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include <KAboutData>

#include <QApplication>
#include <KLocalizedString>
#include <QCommandLineParser>

#include "mainwindow.h"

int main (int argc, char *argv[])
{
    KAboutData aboutData("kbreakout", i18n("KBreakOut"), "1.1.0");
    aboutData.setShortDescription(i18n("A breakout like game for KDE"));
    aboutData.setLicense(KAboutLicense::GPL);
    aboutData.setCopyrightStatement(i18n("(c) 2007-2008 Fela Winkelmolen"));
    aboutData.addAuthor(i18n("Fela Winkelmolen"), 
                        i18n("original author and maintainer"),
                        "fela.kde@gmail.com");
    aboutData.addAuthor(i18n("Eugene Trounev"), 
                        i18n("artwork"),
                        "eugene.trounev@gmail.com");
    aboutData.addAuthor(i18n("Sean Wilson"),
                        i18n("artwork"),
                        "suseux@gmail.com");
    aboutData.addCredit(i18n("Lorenzo Bonomi"),
                        i18n("testing"),
                        "lorenzo.bonomi@hotmail.it");
    aboutData.addCredit(i18n("Brian Croom"),
                        i18n("port to KGameRenderer"),
                        "brian.s.croom@gmail.com");
    aboutData.addCredit(i18n("Viranch Mehta"),
                        i18n("port to QtQuick"),
                        "viranch.mehta@gmail.com");
    
    QApplication app(argc, argv);
    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
    
    MainWindow* window = new MainWindow();
    Q_UNUSED(window);
    
    return app.exec();
}


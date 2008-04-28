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

#include <QString>

#include <KApplication>
#include <KAboutData>
#include <KMessageBox>
#include <KCmdLineArgs>
#include <KLocale>

#include "mainwindow.h"

int main (int argc, char *argv[])
{
    KAboutData aboutData("kbreakout", 0, ki18n("KBreakOut"), "0.9.5");
    aboutData.setShortDescription(ki18n("A breakout like game for KDE"));
    aboutData.setLicense(KAboutData::License_GPL);
    aboutData.setCopyrightStatement(ki18n("(c) 2007-2008 Fela Winkelmolen"));
    aboutData.addAuthor(ki18n("Fela Winkelmolen"), 
                        ki18n("original author and maintainer"),
                        "fela.kde@gmail.com");
    aboutData.addAuthor(ki18n("Eugene Trounev"), 
                        ki18n("artwork"),
                        "eugene.trounev@gmail.com");
    
    KCmdLineArgs::init( argc, argv, &aboutData );
    KApplication app;
    KGlobal::locale()->insertCatalog("libkdegames");
    
    MainWindow* window = new MainWindow();
    window->show();
    
    return app.exec();
}

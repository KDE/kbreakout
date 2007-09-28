#include <QString>

#include <KApplication>
#include <KAboutData>
#include <KMessageBox>
#include <KCmdLineArgs>
#include <KLocale>

#include "mainwindow.h"

int main (int argc, char *argv[])
{
    KAboutData aboutData("kbreakout", 0, ki18n("KBreakout"), "0.1.0");
    aboutData.setShortDescription(ki18n("A breakout like game for KDE"));
    aboutData.setLicense(KAboutData::License_GPL);
    aboutData.setCopyrightStatement(ki18n("(c) 2007 Fela Winkelmolen"));
    aboutData.addAuthor(ki18n("Fela Winkelmolen"), 
                        ki18n("original author and maintainer"),
                        "fela.kde@gmail.com");
    
    KCmdLineArgs::init( argc, argv, &aboutData );
    KApplication app;
    
    MainWindow* window = new MainWindow();
    window->show();
    
    return app.exec();
}

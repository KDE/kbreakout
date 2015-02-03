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
#include <Kdelibs4ConfigMigrator>
#include <KDBusService>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Kdelibs4ConfigMigrator migrate(QStringLiteral("kbreakout"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("kbreakoutrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("kbreakoutui.rc"));
    migrate.migrate();

    KAboutData aboutData(QLatin1Literal("kbreakout"), i18n("KBreakOut"), QLatin1Literal("1.2.0"));
    aboutData.setShortDescription(i18n("A breakout like game for KDE"));
    aboutData.setLicense(KAboutLicense::GPL);
    aboutData.setCopyrightStatement(i18n("(c) 2007-2008 Fela Winkelmolen"));
    aboutData.addAuthor(i18n("Fela Winkelmolen"),
                        i18n("original author and maintainer"),
                        QLatin1Literal("fela.kde@gmail.com"));
    aboutData.addAuthor(i18n("Eugene Trounev"),
                        i18n("artwork"),
                        QLatin1Literal("eugene.trounev@gmail.com"));
    aboutData.addAuthor(i18n("Sean Wilson"),
                        i18n("artwork"),
                        QLatin1Literal("suseux@gmail.com"));
    aboutData.addCredit(i18n("Lorenzo Bonomi"),
                        i18n("testing"),
                        QLatin1Literal("lorenzo.bonomi@hotmail.it"));
    aboutData.addCredit(i18n("Brian Croom"),
                        i18n("port to KGameRenderer"),
                        QLatin1Literal("brian.s.croom@gmail.com"));
    aboutData.addCredit(i18n("Viranch Mehta"),
                        i18n("port to QtQuick"),
                        QLatin1Literal("viranch.mehta@gmail.com"));

    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    KDBusService service;
    MainWindow *window = new MainWindow();
    Q_UNUSED(window);

    return app.exec();
}


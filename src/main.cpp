/*
    SPDX-FileCopyrightText: 2007-2008 Fela Winkelmolen <fela.kde@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

// own
#include "mainwindow.h"
#include "kbreakout_version.h"
// KF
#include <KAboutData>
#include <KCrash>
#include <KLocalizedString>
#include <Kdelibs4ConfigMigrator>
#include <KDBusService>
// Qt
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    // Fixes blurry icons with fractional scaling
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);

    Kdelibs4ConfigMigrator migrate(QStringLiteral("kbreakout"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("kbreakoutrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("kbreakoutui.rc"));
    migrate.migrate();
    KLocalizedString::setApplicationDomain("kbreakout");

    KAboutData aboutData(QStringLiteral("kbreakout"), i18n("KBreakOut"), QStringLiteral(KBREAKOUT_VERSION_STRING));
    aboutData.setShortDescription(i18n("A Breakout like game by KDE"));
    aboutData.setLicense(KAboutLicense::GPL);
    aboutData.setCopyrightStatement(i18n("(c) 2007-2008 Fela Winkelmolen"));
    aboutData.addAuthor(i18n("Fela Winkelmolen"),
                        i18n("original author and maintainer"),
                        QStringLiteral("fela.kde@gmail.com"));
    aboutData.addAuthor(i18n("Eugene Trounev"),
                        i18n("artwork"),
                        QStringLiteral("eugene.trounev@gmail.com"));
    aboutData.addAuthor(i18n("Sean Wilson"),
                        i18n("artwork"),
                        QStringLiteral("suseux@gmail.com"));
    aboutData.addCredit(i18n("Lorenzo Bonomi"),
                        i18n("testing"),
                        QStringLiteral("lorenzo.bonomi@hotmail.it"));
    aboutData.addCredit(i18n("Brian Croom"),
                        i18n("port to KGameRenderer"),
                        QStringLiteral("brian.s.croom@gmail.com"));
    aboutData.addCredit(i18n("Viranch Mehta"),
                        i18n("port to QtQuick"),
                        QStringLiteral("viranch.mehta@gmail.com"));
    aboutData.setHomepage(QStringLiteral("https://games.kde.org/kbreakout"));

    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    KCrash::initialize();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("kbreakout")));

    KDBusService service;
    MainWindow *window = new MainWindow();
    Q_UNUSED(window);

    return app.exec();
}


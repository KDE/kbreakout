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
#include <KDBusService>
#define HAVE_KICONTHEME __has_include(<KIconTheme>)
#if HAVE_KICONTHEME
#include <KIconTheme>
#endif

#define HAVE_STYLE_MANAGER __has_include(<KStyleManager>)
#if HAVE_STYLE_MANAGER
#include <KStyleManager>
#endif
// Qt
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
#if HAVE_KICONTHEME
    KIconTheme::initTheme();
#endif
    QApplication app(argc, argv);
#if HAVE_STYLE_MANAGER
    KStyleManager::initStyle();
#else // !HAVE_STYLE_MANAGER
#if defined(Q_OS_MACOS) || defined(Q_OS_WIN)
    QApplication::setStyle(QStringLiteral("breeze"));
#endif // defined(Q_OS_MACOS) || defined(Q_OS_WIN)
#endif // HAVE_STYLE_MANAGER
    KLocalizedString::setApplicationDomain(QByteArrayLiteral("kbreakout"));

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
    aboutData.setHomepage(QStringLiteral("https://apps.kde.org/kbreakout"));

    KAboutData::setApplicationData(aboutData);
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("kbreakout")));

    KCrash::initialize();

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    KDBusService service;

    MainWindow *window = new MainWindow();
    Q_UNUSED(window);

    return app.exec();
}


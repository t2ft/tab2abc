/****************************************************************************
** Copyright (C) 2015 by Thomas Thanner, Bobingen, Germany
** Contact: thomas@t2ft.de
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License along
**  with this program; if not, write to the Free Software Foundation, Inc.,
**  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
**
*****************************************************************************
**
** Date         Author  Description
**---------------------------------------------------------------------------
** 2015-11-17   ttha    Initial version
**
*****************************************************************************/

#include "apptranslator.h"

#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QDebug>
#include <QLibraryInfo>


AppTranslator::AppTranslator(const QString &translationFileBaseName, QObject *parent)
    : QObject(parent)
    , m_translationFileBaseName(translationFileBaseName.isEmpty() ? qApp->applicationName() + "_" : translationFileBaseName)
{
    QSettings cfg;
    QString sysLanguage = QLocale::system().name();
    qDebug() << "preferred system language:" << sysLanguage;
    m_uiLanguage = QLocale(cfg.value("Language", sysLanguage ).toString()).language();
    qDebug() << "language from config:" << QLocale(m_uiLanguage).name();
}

void AppTranslator::parseCommandLine(const QStringList &args, const QString &parameterName, const QString &parameterSeparator, Qt::CaseSensitivity caseSensitivity)
{
    foreach(QString a, args)
    {
        if (a.contains(parameterName+parameterSeparator, caseSensitivity))
        {
            QLocale loc = QLocale(a.split("=").last());
            m_uiLanguage = loc.language();
            qDebug() << "language from command line: " << a;
        }
    }
}


void AppTranslator::translate(const QString &defaultTranslationFilePath, QLocale::Language appLanguage, QLocale::Language fallBackLanguage)
{
    QSettings cfg;
    qDebug() << "-> using language: " << QLocale(m_uiLanguage).name();
    // install translations for application strings
#ifdef Q_OS_MACX
    // on MacOSX the application resides in a "bundle" which stores resource files in a separate, distinct location
    QString translationsPath = settings.value("TranslationsPath", QApplication::applicationDirPath() + "/../Resources/lang").toString();
#else
    QString translationsPath = cfg.value("TranslationsPath",
                                         defaultTranslationFilePath.isEmpty() ?
                                             QGuiApplication::applicationDirPath() + "/lang"
                                           : defaultTranslationFilePath).toString();
#endif
    // all strings inside application are already in german, no translation required in that case
    if (m_uiLanguage != appLanguage)
    {
        QString languageFileName = m_translationFileBaseName + QLocale(m_uiLanguage).name();
        qDebug() << "load translation" << languageFileName << "from" << translationsPath ;
        if (!m_appTranslator.load(languageFileName, translationsPath))
        {
            m_uiLanguage = fallBackLanguage;
            languageFileName = m_translationFileBaseName + QLocale(m_uiLanguage).name();
            qDebug() << "FAILED -> try to load translation" << languageFileName << "from" << translationsPath << "next";
            if (!m_appTranslator.load(languageFileName, translationsPath))
                qDebug() << "FAILED -> giving up.";
        }
        qApp->installTranslator(&m_appTranslator);
    }
    // install translations for Qt internal strings, use the same language as for the user defined strings above
    m_qtTranslator.load("qt_" + QLocale(m_uiLanguage).name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(&m_qtTranslator);
}

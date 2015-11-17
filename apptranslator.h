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

#ifndef APPTRANSLATOR_H
#define APPTRANSLATOR_H

#include <QObject>
#include <QLocale>
#include <QTranslator>

class AppTranslator : public QObject
{
    Q_OBJECT
public:
    explicit AppTranslator(const QString &translationFileBaseName=QString(), QObject *parent = 0);

    void parseCommandLine(const QStringList &args,
                          const QString &parameterName = "lang",
                          const QString &parameterSeparator = "=",
                          Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive);

    void translate(const QString &defaultTranslationFilePath = QString(),
                   QLocale::Language appLanguage = QLocale::German,
                   QLocale::Language fallBackLanguage = QLocale::English);

protected:
    QString             m_translationFileBaseName;
    QLocale::Language   m_uiLanguage;
    QTranslator         m_appTranslator;
    QTranslator         m_qtTranslator;
};

#endif // APPTRANSLATOR_H

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
** 2015-11-08   ttha    Initial version
**
*****************************************************************************/

#ifndef CONVERT_H
#define CONVERT_H

#include <QObject>

class QTextStream;
class AbcFile;

class Convert : public QObject
{
    Q_OBJECT
public:
    typedef enum {
        M2_4,
        M3_4,
        M4_4,
        M6_8
    } Metrum;

    explicit Convert(QObject *parent = 0);

    void exec(const QString &inFileName,
              const QString &outFileName,
              Metrum metrum = M4_4,
              const QString &title = QString(),
              const QString &subtitle = QString(),
              const QString &composer = QString(),
              const QString &tempo = QString(),
              const QString &key = QString());

    QString metrumString(Metrum metrum);

signals:
    void debug(const QString &text);
    void info(const QString &text);
    void extrainfo(const QString &text);
    void warning(const QString &text);
    void error(const QString &text);
    void success(const QString &text);

private:
    QString     m_notes;
    int         m_ticks;

    bool convertTabLine(QTextStream *fIn, int lineNumber, QString &tuning, bool *result = NULL);
    QString finishNote(const QString &lastNote, int duration) const;
};

#endif // CONVERT_H

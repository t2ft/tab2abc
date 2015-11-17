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

#ifndef BASSSTRINGLINE_H
#define BASSSTRINGLINE_H

#include <QObject>

class BassStringLine : public QObject
{
    Q_OBJECT
public:
    explicit BassStringLine(char defaulTuning, int lineNumber, int stringNumber, QObject *parent = 0);

    static bool isBassStringLine(const QString &line);
    bool convert(const QString &line);

    int barCount() const;
    int tickCount(int bar) const;
    QChar tuning() const;
    QString note(int bar, int tick) const;
    QString notes(int bar) const;
    QString notes() const;
    const QString &rawLine() const;

signals:
    void debug(const QString &text);
    void info(const QString &text);
    void extrainfo(const QString &text);
    void warning(const QString &text);
    void error(const QString &text);
    void success(const QString &text);

private:
    QString                 m_line;
    char                    m_tuning;
    int                     m_lineNumber;
    int                     m_stringNumber;
    int                     m_tuningBase;
    QList<QStringList>      m_notes;

    QString calcNote(const QString &fretString) const;
    int note2index(char note);
};

#endif // BASSSTRINGLINE_H

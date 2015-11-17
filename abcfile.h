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

#ifndef ABCFILE_H
#define ABCFILE_H

#include <QObject>

class QFile;
class QTextStream;

class AbcFile : public QObject
{
    Q_OBJECT
public:
    explicit AbcFile(QObject *parent = 0);

    ~AbcFile();

    bool open(const QString &fname);

signals:
    void debug(const QString &text);
    void info(const QString &text);
    void warning(const QString &text);
    void error(const QString &text);

private:
    QFile           *m_file;
    QTextStream     *m_ts;
};

#endif // ABCFILE_H

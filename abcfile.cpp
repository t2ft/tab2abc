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

#include "abcfile.h"

#include <QFile>
#include <QDir>
#include <QTextStream>

AbcFile::AbcFile(QObject *parent)
    : QObject(parent)
    , m_file(NULL)
    , m_ts(NULL)
{

}

AbcFile::~AbcFile()
{
    if (m_ts) {
        delete m_ts;
    }
    if (m_file) {
        m_file->close();
        delete m_file;
    }
}



bool AbcFile::open(const QString &fname)
{
    if (m_file) {
        delete m_file;
        m_file = NULL;
    }
    m_file = new QFile(fname);
    return m_file->open(QFile::ReadWrite);
}


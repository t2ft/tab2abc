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
** 2015-11-17  ttha      Initial version
**
*****************************************************************************/

#include "autoupdatelineedit.h"
#include <QTimerEvent>
#include <QDebug>

AutoUpdateLineEdit::AutoUpdateLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    initialize();
}


AutoUpdateLineEdit::AutoUpdateLineEdit(const QString &contents, QWidget *parent)
    : QLineEdit(contents, parent)
{
    initialize();
}

void AutoUpdateLineEdit::initialize()
{
    m_updateTimeout = 1000;
    m_timerId = 0;
    connect(this, SIGNAL(textEdited(QString)), SLOT(on_textEdited(QString)));
    connect(this, SIGNAL(editingFinished()), SLOT(on_editingFinished()));
}

void AutoUpdateLineEdit::setUpdateTimeout(int timeout)
{
    m_updateTimeout = timeout;
}

void AutoUpdateLineEdit::timerEvent(QTimerEvent *ev)
{
    if (ev->timerId() == m_timerId) {
        killTimer(m_timerId);
        m_timerId = 0;
        qDebug() << objectName() << ": update due to timeout";
        emit updateText(text());
    }
}

void AutoUpdateLineEdit::on_textEdited(const QString &text)
{
    Q_UNUSED(text)

    if (m_timerId)
        killTimer(m_timerId);
    m_timerId = startTimer(m_updateTimeout);
}

void AutoUpdateLineEdit::on_editingFinished()
{
    if (m_timerId) {
        killTimer(m_timerId);
        m_timerId = 0;
    }
    qDebug() << objectName() << ": update due editing finished";
    emit updateText(text());
}

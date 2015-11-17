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

#ifndef AUTOUPDATELINEEDIT_H
#define AUTOUPDATELINEEDIT_H

#include <QLineEdit>

class AutoUpdateLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    AutoUpdateLineEdit(QWidget *parent = 0);
    AutoUpdateLineEdit(const QString &contents, QWidget *parent = 0);

    void setUpdateTimeout(int timeout);
    int updateTimeout() const { return m_updateTimeout; }

signals:
    void updateText(const QString &text);

protected slots:
    void on_textEdited(const QString &text);
    void on_editingFinished();

protected:
    int     m_updateTimeout;
    int     m_timerId;

    void timerEvent(QTimerEvent *ev);

private:
    void initialize();
};

#endif // AUTOUPDATELINEEDIT_H

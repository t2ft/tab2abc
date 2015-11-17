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

#ifndef TAB2ABC_H
#define TAB2ABC_H


#include "convert.h"
#include <QWidget>


namespace Ui {
class Tab2Abc;
}

class ElidedLabel;

class Tab2Abc : public QWidget
{
    Q_OBJECT

public:

    explicit Tab2Abc(QWidget *parent = 0);
    ~Tab2Abc();

public slots:
    void debug(const QString &text);
    void info(const QString &text);
    void extrainfo(const QString &text);
    void warning(const QString &text);
    void error(const QString &text);
    void success(const QString &text);


protected:
    void closeEvent(QCloseEvent *ev);

private slots:
    void on_loadInFileName_clicked();
    void on_loadOutFileName_clicked();
    void on_run_clicked();
    void on_m2_4_toggled(bool checked);
    void on_m3_4_toggled(bool checked);
    void on_m4_4_toggled(bool checked);
    void on_m6_8_toggled(bool checked);

private:
    typedef enum {
        LogDebug,
        LogInfo,
        LogExtraInfo,
        LogWarning,
        LogError,
        LogSuccess
    } LogType;

    Ui::Tab2Abc         *ui;
    Convert::Metrum     m_metrum;
    QString             m_inFileName;
    QString             m_outFileName;

    void setMetrum(Convert::Metrum metrum);
    void setFileName(const QString &fname, ElidedLabel *ctrl);
    void log(const QString &msg, LogType logType);
};

#endif // TAB2ABC_H

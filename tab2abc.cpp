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

#include "tab2abc.h"
#include "ui_tab2abc.h"
#include "elidedlabel.h"
#include <QSettings>
#include <QFileDialog>
#include <QDir>
#include <QCloseEvent>
#include <QScopedPointer>
#include <QDebug>
#include <QScrollBar>
#include <QFileInfo>

#define cfgInFileName       "InFileName"
#define cfgOutFileName      "OutFileName"
#define cfgMetrum           "Metrum"
#define cfgWindowGeometry   "window/Geometry"

Tab2Abc::Tab2Abc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab2Abc)
{
    ui->setupUi(this);

    // reload settings
    QSettings settings;
    m_metrum = static_cast<Convert::Metrum>(settings.value(cfgMetrum, Convert::M4_4).toInt());
    m_inFileName = QDir::fromNativeSeparators(settings.value(cfgInFileName).toString());
    m_outFileName = QDir::fromNativeSeparators(settings.value(cfgOutFileName).toString());

    restoreGeometry(settings.value(cfgWindowGeometry).toByteArray());

    setFileName(m_inFileName, ui->inFileName);
    setFileName(m_outFileName, ui->outFileName);
    ui->m2_4->setChecked(m_metrum == Convert::M2_4);
    ui->m3_4->setChecked(m_metrum == Convert::M3_4);
    ui->m4_4->setChecked(m_metrum == Convert::M4_4);
    ui->m6_8->setChecked(m_metrum == Convert::M6_8);
}

Tab2Abc::~Tab2Abc()
{
    delete ui;
}

void Tab2Abc::closeEvent(QCloseEvent *ev)
{
    QSettings settings;
    settings.setValue(cfgWindowGeometry, saveGeometry());
    ev->accept();
}


void Tab2Abc::on_loadInFileName_clicked()
{
    QString fname = QFileDialog::getOpenFileName(this,
                                                 qApp->applicationName() + tr(" - Eingabedatei"),
                                                 QDir::toNativeSeparators(m_inFileName),
                                                 tr("Text Dateien (*.txt);;alle Dateien (*.*)"),
                                                 0,
                                                 QFileDialog::ReadOnly);
    if (!fname.isEmpty()) {
        m_inFileName = QDir::fromNativeSeparators(fname);
        setFileName(m_inFileName, ui->inFileName);
        QSettings settings;
        settings.setValue(cfgInFileName, m_inFileName);
    }
}

void Tab2Abc::on_loadOutFileName_clicked()
{
    QFileInfo fi(m_inFileName);
    QString defaultOutFileName = fi.canonicalPath() + '/' + fi.completeBaseName() + ".abc";
    QString fname = QFileDialog::getSaveFileName(this,
                                                 qApp->applicationName() + tr(" - Ausgabedatei"),
                                                 QDir::toNativeSeparators(defaultOutFileName),
                                                 tr("ABC Dateien (*.abc);;alle Dateien (*.*)"));
    if (!fname.isEmpty()) {
        m_outFileName = QDir::fromNativeSeparators(fname);
        setFileName(m_outFileName, ui->outFileName);
        QSettings settings;
        settings.setValue(cfgOutFileName, m_outFileName);
    }
}


void Tab2Abc::setFileName(const QString &fname, ElidedLabel *ctrl)
{
    QString nativeFileName = QDir::toNativeSeparators(fname);
    ctrl->setText(nativeFileName);
    ctrl->setToolTip(nativeFileName);
}


void Tab2Abc::on_m2_4_toggled(bool checked)
{
    if (checked) {
        setMetrum(Convert::M2_4);
    }
}


void Tab2Abc::on_m3_4_toggled(bool checked)
{
    if (checked) {
        setMetrum(Convert::M3_4);
    }
}


void Tab2Abc::on_m4_4_toggled(bool checked)
{
    if (checked) {
        setMetrum(Convert::M4_4);
    }
}

void Tab2Abc::on_m6_8_toggled(bool checked)
{
    if (checked) {
        setMetrum(Convert::M6_8);
    }
}

void Tab2Abc::setMetrum(Convert::Metrum metrum)
{
    if (m_metrum != metrum) {
        m_metrum = metrum;
        QSettings settings;
        settings.setValue(cfgMetrum, m_metrum);
    }
}

void Tab2Abc::debug(const QString &text)
{
    log(text, LogDebug);
}

void Tab2Abc::info(const QString &text)
{
    log(text, LogInfo);
}

void Tab2Abc::extrainfo(const QString &text)
{
    log(text, LogExtraInfo);
}

void Tab2Abc::warning(const QString &text)
{
    log(text, LogWarning);
}

void Tab2Abc::error(const QString &text)
{
    log(text, LogError);
}

void Tab2Abc::success(const QString &text)
{
    log(text, LogSuccess);
}

void Tab2Abc::log(const QString &msg, LogType type)
{
    static const char *FIXED_PITCH_FONTS = "Courier,Courier New,Lucida Console,Fixedsys,FreeMono,Monospace,Typewriter";
    static const char *VARIABLE_PITCH_FONTS = "Arial, Helvetica, Verdana";
    QString typeString = "???";
    QString textColor = "#ff0000";
    QString backColor = "#ffffff";
    bool fixedpitch = false;
    switch (type) {
    case LogInfo:
        typeString = "III";
        textColor  = "#000000";
        break;
    case LogExtraInfo:
        typeString = "&nbsp;&nbsp;&nbsp;";
        textColor  = "#a0a0a0";
        fixedpitch = true;
        break;
    case LogWarning :
        typeString = "WWW";
        textColor  = "#b08000";
        break;
    case LogError:
        typeString = "EEE";
        textColor  = "#a00000";
        break;
    case LogDebug:
        typeString = "DDD";
        textColor  = "#9090a0";
        break;
    case LogSuccess:
        typeString = "&nbsp;&nbsp;&nbsp;";
        textColor  = "#00a000";
        break;
    }
    QString htmlString = QString("<p>"   \
                                 "<span style=\"font-family:'%5'; font-size:small; color:#808080;background-color:%4\">"  \
                                     "%1:"   \
                                 "</span>"   \
                                 "&nbsp;"    \
                                 "<span style=\"font-family:'%6'; color:%3;background-color:%4\">" \
                                     "%2"    \
                                 "</span>"    \
                             "</p>").arg(typeString).arg(msg).arg(textColor).arg(backColor)
                                    .arg(FIXED_PITCH_FONTS).arg(fixedpitch ? FIXED_PITCH_FONTS : VARIABLE_PITCH_FONTS);
    ui->log->appendHtml(htmlString);
    ui->log->ensureCursorVisible();
    QScrollBar *hScroll = ui->log->horizontalScrollBar();
    hScroll->setValue(hScroll->minimum());
}

void Tab2Abc::on_run_clicked()
{
    qDebug() << "RUN";
    ui->log->clear();
    if (m_inFileName.isEmpty()) {
        on_loadInFileName_clicked();
    }
    if (!m_inFileName.isEmpty()) {
        if (m_outFileName.isEmpty()) {
            on_loadOutFileName_clicked();
        }
        if (!m_outFileName.isEmpty()) {
            QScopedPointer<Convert> cnv(new Convert(m_inFileName, m_outFileName, m_metrum, this));
            connect(cnv.data(), SIGNAL(debug(QString)), SLOT(debug(QString)));
            connect(cnv.data(), SIGNAL(info(QString)), SLOT(info(QString)));
            connect(cnv.data(), SIGNAL(extrainfo(QString)), SLOT(extrainfo(QString)));
            connect(cnv.data(), SIGNAL(warning(QString)), SLOT(warning(QString)));
            connect(cnv.data(), SIGNAL(error(QString)), SLOT(error(QString)));
            connect(cnv.data(), SIGNAL(success(QString)), SLOT(success(QString)));
            cnv->exec();
        }
    }
}


/****************************************************************************
** Copyright (C) 2014 by Thomas Thanner, Bobingen, Germany
** All Righs Reserved
**
** Contact: thomas@t2ft.de
**
*****************************************************************************
**
** Date         Author  Description
**---------------------------------------------------------------------------
**
**
*****************************************************************************/

#include "tab2abc.h"
#include "ui_tab2abc.h"
#include <QSettings>
#include <QFileDialog>
#include <QDir>
#include <QCloseEvent>
#include <QDebug>

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
    m_metrum = static_cast<Metrum>(settings.value(cfgMetrum, M4_4).toInt());
    m_inFileName = QDir::fromNativeSeparators(settings.value(cfgInFileName).toString());
    m_outFileName = QDir::fromNativeSeparators(settings.value(cfgOutFileName).toString());

    restoreGeometry(settings.value(cfgWindowGeometry).toByteArray());

    ui->inFileName->setText(QDir::toNativeSeparators(m_inFileName));
    ui->outFileName->setText(QDir::toNativeSeparators(m_outFileName));
    ui->m2_4->setChecked(m_metrum == M2_4);
    ui->m3_4->setChecked(m_metrum == M3_4);
    ui->m4_4->setChecked(m_metrum == M4_4);
    ui->m6_8->setChecked(m_metrum == M6_8);
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
        ui->inFileName->setText(QDir::toNativeSeparators(m_inFileName));
        QSettings settings;
        settings.setValue(cfgInFileName, m_inFileName);
    }
}

void Tab2Abc::on_loadOutFileName_clicked()
{
    QString fname = QFileDialog::getSaveFileName(this,
                                                 qApp->applicationName() + tr(" - Ausgabedatei"),
                                                 QDir::toNativeSeparators(m_inFileName),
                                                 tr("ABC Dateien (*.abc);;alle Dateien (*.*)"));
    if (!fname.isEmpty()) {
        m_outFileName = QDir::fromNativeSeparators(fname);
        ui->outFileName->setText(QDir::toNativeSeparators(m_outFileName));
        QSettings settings;
        settings.setValue(cfgOutFileName, m_outFileName);
    }
}

void Tab2Abc::on_m2_4_toggled(bool checked)
{
    if (checked) {
        setMetrum(M2_4);
    }
}

void Tab2Abc::on_m3_4_toggled(bool checked)
{
    if (checked) {
        setMetrum(M3_4);
    }
}

void Tab2Abc::on_m4_4_toggled(bool checked)
{
    if (checked) {
        setMetrum(M4_4);
    }
}

void Tab2Abc::on_m6_8_toggled(bool checked)
{
    if (checked) {
        setMetrum(M6_8);
    }
}

void Tab2Abc::setMetrum(Metrum metrum)
{
    if (m_metrum != metrum) {
        m_metrum = metrum;
        QSettings settings;
        settings.setValue(cfgMetrum, m_metrum);
    }
}

void Tab2Abc::on_run_clicked()
{
    qDebug() << "RUN";
}

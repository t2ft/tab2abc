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
    Ui::Tab2Abc *ui;
    Convert::Metrum     m_metrum;
    QString             m_inFileName;
    QString             m_outFileName;

    void setMetrum(Convert::Metrum metrum);
    void setFileName(const QString &fname, ElidedLabel *ctrl);
};

#endif // TAB2ABC_H

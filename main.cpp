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
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set app properties
    a.setApplicationName("tab2abc");
    a.setApplicationVersion("1.0.0");
    a.setOrganizationName("T2FT");
    a.setOrganizationDomain("t2ft.de");

    Tab2Abc w;
    w.show();

    return a.exec();
}

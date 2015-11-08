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

#ifndef CONVERT_H
#define CONVERT_H

#include <QObject>

class Convert : public QObject
{
    Q_OBJECT
public:
    typedef enum {
        M2_4,
        M3_4,
        M4_4,
        M6_8
    } Metrum;

    explicit Convert(const QString &inFileName, const QString &outFileName, Metrum metrum = M4_4, QObject *parent = 0);

signals:

public slots:
};

#endif // CONVERT_H

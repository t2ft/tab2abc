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

#include "convert.h"

Convert::Convert(const QString &inFileName, const QString &outFileName, Metrum metrum, QObject *parent)
    : QObject(parent)
    , m_inFileName(inFileName)
    , m_outFileName(outFileName)
    , m_metrum(metrum)
{

}


void Convert::exec()
{
    emit info(tr("Beginne Konvertierung von \"%1\"").arg(m_inFileName));


    emit info(tr("Konvertierung beendet."));
}

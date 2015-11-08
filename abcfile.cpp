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


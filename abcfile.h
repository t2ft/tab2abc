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

#ifndef ABCFILE_H
#define ABCFILE_H

#include <QObject>

class QFile;
class QTextStream;

class AbcFile : public QObject
{
    Q_OBJECT
public:
    explicit AbcFile(QObject *parent = 0);

    ~AbcFile();

    bool open(const QString &fname);

signals:
    void debug(const QString &text);
    void info(const QString &text);
    void warning(const QString &text);
    void error(const QString &text);

private:
    QFile           *m_file;
    QTextStream     *m_ts;
};

#endif // ABCFILE_H

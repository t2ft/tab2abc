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

class QTextStream;
class AbcFile;

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

    void exec(void);

signals:
    void debug(const QString &text);
    void info(const QString &text);
    void extrainfo(const QString &text);
    void warning(const QString &text);
    void error(const QString &text);
    void success(const QString &text);

private:
    QString     m_inFileName;
    QString     m_outFileName;
    Metrum      m_metrum;
    QString     m_notes;

    bool convertTabLine(QTextStream *fIn, int lineNumber, QString &tuning, bool *result = NULL);
    QString finishNote(const QString &lastNote, int duration) const;
};

#endif // CONVERT_H

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

#ifndef BASSSTRINGLINE_H
#define BASSSTRINGLINE_H

#include <QObject>

class BassStringLine : public QObject
{
    Q_OBJECT
public:
    explicit BassStringLine(char defaulTuning, int lineNumber, int stringNumber, QObject *parent = 0);

    static bool isBassStringLine(const QString &line);
    bool convert(const QString &line);

    int barCount() const;
    int tickCount(int bar) const;
    QChar tuning() const;
    QString note(int bar, int tick) const;
    QString notes(int bar) const;
    QString notes() const;
    const QString &rawLine() const;

signals:
    void debug(const QString &text);
    void info(const QString &text);
    void extrainfo(const QString &text);
    void warning(const QString &text);
    void error(const QString &text);
    void success(const QString &text);

private:
    QString                 m_line;
    char                    m_tuning;
    int                     m_lineNumber;
    int                     m_stringNumber;
    int                     m_tuningBase;
    QList<QStringList>      m_notes;

    QString calcNote(const QString &fretString) const;
    int note2index(char note);
};

#endif // BASSSTRINGLINE_H

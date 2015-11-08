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
#include "bassstringline.h"
#include "abcfile.h"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QScopedPointer>

Convert::Convert(const QString &inFileName, const QString &outFileName, Metrum metrum, QObject *parent)
    : QObject(parent)
    , m_inFileName(inFileName)
    , m_outFileName(outFileName)
    , m_metrum(metrum)
{

}


void Convert::exec()
{
    emit info(tr("Beginne Konvertierung von \"%1\" nach \"%2\"")
                    .arg(QDir::toNativeSeparators(m_inFileName))
                    .arg(QDir::toNativeSeparators(m_outFileName))
              );

    QFile fIn(m_inFileName);
    if (fIn.open(QFile::ReadOnly | QFile::Text)) {
        emit info(tr("Eingabedatei erfolgreich zum Lesen geöffnet."));
        QTextStream ts(&fIn);
        int n = 0;
        QString tuning = "GDAE";
        m_notes.clear();
        while (convertTabLine(&ts, n++, tuning));
        fIn.close();
    } else {
        emit error(tr("Kann Eingabedatei nicht zum Lesen öffnen (%1: %2)").arg(fIn.error()).arg(fIn.errorString()));
    }

    emit info(tr("Konvertierung beendet."));
}

bool Convert::convertTabLine(QTextStream *fIn, int lineNumber, QString &tuning)
{
    // read text lines, until a bass tab notation is found
    // then collect all strings and convert them
    while (!fIn->atEnd()) {
        bool inTabLine = false;
        QList<BassStringLine*>  bassStringLines;
        QString line = fIn->readLine().trimmed();
        if (BassStringLine::isBassStringLine(line)) {
            // start of bass tab system found
            BassStringLine *bsl = new BassStringLine(line, tuning.at(bassStringLines.count()).toLatin1(), lineNumber, bassStringLines.count());
            bassStringLines.append(bsl);
            connect(bsl, SIGNAL(debug(QString)), SIGNAL(debug(QString)));
            connect(bsl, SIGNAL(info(QString)), SIGNAL(info(QString)));
            connect(bsl, SIGNAL(warning(QString)), SIGNAL(warning(QString)));
            connect(bsl, SIGNAL(error(QString)), SIGNAL(error(QString)));
            inTabLine = true;
        } else {
            if (inTabLine) {
                // last line found -> start conversion
                QString msg = tr("Tab #%1 gelesen, %2 Saiten: ").arg(lineNumber).arg(bassStringLines.count());
                for (int i=bassStringLines.count(); i>0; i--) {
                    msg += bassStringLines.at(i-1)->tuning();
                    if (i>1)
                        msg += '-';
                }
                emit info(msg);
                // sanity check: Number of bars has to be identical on each string
                int bars = bassStringLines.first()->barCount();
                foreach (BassStringLine *bsl, bassStringLines) {
                    if (bsl->barCount() != bars) {
                        emit error(tr("Anzahl Takte der %1-Saite unterscheidet sich von der ersten Saite")
                                   .arg(bsl->tuning()));
                        qDeleteAll(bassStringLines);
                        return false;
                    }
                }
                // convert each bar
                for (int b=0; b<bars; b++) {
                    // sanity check: Number of ticks has to be identical on each string
                    int ticks = bassStringLines.first()->tickCount(b);
                    foreach (BassStringLine *bsl, bassStringLines) {
                        if (bsl->tickCount(b) != ticks) {
                            emit error(tr("Anzahl Ticks der %1-Saite in Takt %2 unterscheidet sich von der ersten Saite")
                                       .arg(bsl->tuning())
                                       .arg(b));
                            qDeleteAll(bassStringLines);
                            return false;
                        }
                        // convert each tick
                        int duration = 0;
                        QString lastNote;
                        for (int t=0; t<ticks; t++) {
                            QString note;
                            // search on each string for a note at this tick
                            foreach (BassStringLine *bsl, bassStringLines) {
                                note += bsl->note(b, t);
                            }
                            if (note.isEmpty()) {
                                // no note found -> extend last duration of note or pause
                                    duration++;
                            } else {
                                // new note found -> finish last note or pause
                                if (lastNote.isEmpty()) {
                                    // pause
                                    if (duration > 0)
                                        m_notes += QString("z%1").arg(duration);
                                } else {
                                    m_notes += QString("%1%2").arg(lastNote).arg(duration);
                                }
                                lastNote = note;
                                duration = 1;
                            }
                        }
                        // finish bar
                        m_notes += '|';
                    }
                }
                // update tuning defaults
                tuning.clear();
                foreach (BassStringLine *bsl, bassStringLines) {
                    tuning += bsl->tuning();
                }
                // done, clean up
                qDeleteAll(bassStringLines);
                return true;
            }
        }
    }
    return false;
}

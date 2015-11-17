/****************************************************************************
** Copyright (C) 2015 by Thomas Thanner, Bobingen, Germany
** Contact: thomas@t2ft.de
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License along
**  with this program; if not, write to the Free Software Foundation, Inc.,
**  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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
    , m_ticks(0)
{

}


void Convert::exec()
{
    bool result = true;
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
        while (convertTabLine(&ts, n++, tuning, &result));
        fIn.close();
        emit info(tr("Dateiende erreicht."));
        if (result) {
            emit success(tr("Konvertierung erfolgreich beendet."));
            // write result to output file
            AbcFile abc(this);
            if (abc.create(m_notes, metrumString(m_metrum), m_ticks)) {
                result = abc.save(m_outFileName);
            } else {
                result = false;
            }
        } else {
            emit warning(tr("Konvertierung mit Fehlern beendet."));
        }
    } else {
        emit error(tr("Kann Eingabedatei nicht zum Lesen öffnen (%1: %2)").arg(fIn.error()).arg(fIn.errorString()));
        result = false;
    }

}

bool Convert::convertTabLine(QTextStream *fIn, int lineNumber, QString &tuning, bool *result)
{
    // read text lines, until a bass tab notation is found
    // then collect all strings and convert them
    if (result)
        *result = true;
    bool inTabLine = false;
    QList<BassStringLine*>  bassStringLines;
    while (!fIn->atEnd()) {
        QString line = fIn->readLine().trimmed();
        if (BassStringLine::isBassStringLine(line)) {
            // start of bass tab system found
            BassStringLine *bsl = new BassStringLine(tuning.at(bassStringLines.count()).toLatin1(), lineNumber, bassStringLines.count());
            bassStringLines.append(bsl);
            connect(bsl, SIGNAL(debug(QString)), SIGNAL(debug(QString)));
            connect(bsl, SIGNAL(info(QString)), SIGNAL(info(QString)));
            connect(bsl, SIGNAL(extrainfo(QString)), SIGNAL(extrainfo(QString)));
            connect(bsl, SIGNAL(success(QString)), SIGNAL(success(QString)));
            connect(bsl, SIGNAL(warning(QString)), SIGNAL(warning(QString)));
            connect(bsl, SIGNAL(error(QString)), SIGNAL(error(QString)));
            inTabLine = true;
            bsl->convert(line);
        } else {
            if (inTabLine) {
                if (result)
                    *result = false;
                // last line found -> start conversion
                QString msg = tr("Tab #%1 eingelesen, %2 Saiten: ").arg(lineNumber+1).arg(bassStringLines.count());
                for (int i=bassStringLines.count(); i>0; i--) {
                    msg += bassStringLines.at(i-1)->tuning();
                    if (i>1)
                        msg += '-';
                }
                emit info(msg);
                foreach (BassStringLine *bsl, bassStringLines) {
                    emit extrainfo(bsl->rawLine());
                }
                // sanity check: Number of bars has to be identical on each string
                int bars = bassStringLines.first()->barCount();
                foreach (BassStringLine *bsl, bassStringLines) {
                    if (bsl->barCount() != bars) {
                        emit error(tr("    Anzahl Takte der %1-Saite (%2) unterscheidet sich von denen der ersten Saite (%3)")
                                   .arg(bsl->tuning())
                                   .arg(bsl->barCount())
                                   .arg(bars));
                        emit extrainfo(tr("%1-Saite: ").arg(bassStringLines.first()->tuning()) + bassStringLines.first()->notes());
                        emit extrainfo(tr("%1-Saite: ").arg(bsl->tuning()) + bsl->notes());
                        qDeleteAll(bassStringLines);
                        return false;
                    }
                }
                // convert each bar
                QString notes;
                for (int b=0; b<bars; b++) {
                    // sanity check: Number of ticks has to be identical on each string and each tab line
                    if (m_ticks==0) {
                        m_ticks = bassStringLines.first()->tickCount(b);
                    }
                    foreach (BassStringLine *bsl, bassStringLines) {
                        if (bsl->tickCount(b) != m_ticks) {
                            emit error(tr("    Anzahl Ticks der %1-Saite (%3) in Takt %2 unterscheidet sich von denen der allerersten %5-Saite (%4)")
                                       .arg(bsl->tuning())
                                       .arg(b+1)
                                       .arg(bsl->tickCount(b))
                                       .arg(m_ticks)
                                       .arg(bassStringLines.first()->tuning()));
                            emit extrainfo(tr("%1-Saite: ").arg(bassStringLines.first()->tuning()) + bassStringLines.first()->notes(b));
                            emit extrainfo(tr("%1-Saite: ").arg(bsl->tuning()) + bsl->notes(b));
                            qDeleteAll(bassStringLines);
                            return false;
                        }
                    }
                    // convert each tick
                    int duration = 0;
                    QString lastNote;
                    for (int t=0; t<m_ticks; t++) {
                        QString note;
                        // search on each string for a note at this tick
                        foreach (BassStringLine *bsl, bassStringLines) {
                            note += bsl->note(b, t);
                        }
                        if (note.isEmpty()) {
                            // no note found -> extend last duration of note or pause
                                duration++;
                        } else {
                            notes += finishNote(lastNote, duration);
                            lastNote = note;
                            duration = 1;
                        }
                    }
                    // finish last note in bar
                    notes += finishNote(lastNote, duration);
                    // finish bar
                    notes += " | ";
                }
                // update tuning defaults
                tuning.clear();
                foreach (BassStringLine *bsl, bassStringLines) {
                    tuning += bsl->tuning();
                }
                emit extrainfo(notes);
                emit success(tr("Tab #%1 erfolgreich konvertiert.").arg(lineNumber+1));
                m_notes += notes;
                // done, clean up
                qDeleteAll(bassStringLines);
                if (result)
                    *result = true;
                return true;
            }
        }
    }
    return false;
}

QString Convert::finishNote(const QString &lastNote, int duration) const
{
    QString note;
    // new note found -> finish last note or add a pause
    if (lastNote.isEmpty()) {
        // pause
        if (duration > 0)
            note = QString("z%1 ").arg(duration);
    } else {
        note = QString("%1%2 ").arg(lastNote).arg(duration);
    }
    return note;
}

QString Convert::metrumString(Metrum metrum)
{
    QString ret;
    switch (metrum) {
    case M2_4: ret = "2/4"; break;
    case M3_4: ret = "3/4"; break;
    case M6_8: ret = "6/8"; break;
    default: ret = "4/4";
    }
    return ret;
}

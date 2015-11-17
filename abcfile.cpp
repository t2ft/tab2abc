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
** 2015-11-17  ttha      Initial version
**
*****************************************************************************/

#include "abcfile.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

AbcFile::AbcFile(QObject *parent) : QObject(parent)
{

}

bool AbcFile::create(const QString &notes,
                     const QString &metrum,
                     int ticks,
                     const QString &title,
                     const QString &subTitle,
                     const QString &composer,
                     const QString &tempo,
                     const QString &key)
{
    return create(notes,
                  metrum,
                  ticks,
                  title,
                  QStringList(subTitle),
                  QStringList(composer),
                  tempo,
                  key);
}

bool AbcFile::create(const QString &notes,
                     const QString &metrum,
                     int ticks,
                     const QString &title,
                     const QStringList &subTitle,
                     const QStringList &composer,
                     const QString &tempo,
                     const QString &key)
{
    m_content.clear();
    // add some formating stuff
    m_content.append("%%measurenb 5");
    m_content.append("%%titlefont Times 16");
    m_content.append("%%textfont Helvetica 10");
    // create header
    m_content.append("X:1");
    m_content.append(QString("L:1/%1").arg(ticks));
    if (!title.isEmpty()) {
        m_content.append("T:" +title);
        foreach (QString sub, subTitle) {
            m_content.append("T:" +sub);
        }
    }
    foreach (QString c, composer) {
        m_content.append("C:" + c);
    }
    m_content.append(("M:" + metrum));
    if (!tempo.isEmpty())
        m_content.append("Q:" + tempo);
    m_content.append("K:" + (key.isEmpty() ? "C bass" : key));
    m_content.append("U: h = !thumb!");
    m_content.append("U: p = !snap!");
);
    // create body
    QStringList noteList = notes.split(' ', QString::SkipEmptyParts);
    int bars = 0;
    QString line;
    int slur = 0;
    foreach (QString note, noteList) {
        if ((note.contains('|'))) {
            line += " | ";
            // new bar
            bars++;
            if (bars > 8) {
                bars = 0;
                m_content.append(line);
                line.clear();
            }
        } else {
            if (note.contains('h', Qt::CaseSensitive)) {
                // hammered note, try to symbolize it by a "thumb" symbol
                line += "h";
                note.remove(QChar('h'), Qt::CaseSensitive);
            }
            if (note.contains('.')) {
                // just ignore dots at the moment
                note.remove(('.'));
            }
            if (note.contains('p', Qt::CaseSensitive)) {
                // pulled note, try to symbolize it by a "snap" symbol
                line += "p";
                note.remove(QChar('p'), Qt::CaseSensitive);
            }
            if (note.contains('/')) {
                // slide up, symbolize by a dotted slur
                line += ".(";
                note.remove('/');
                slur = 2;
            }
            if (note.contains('\\')) {
                // slide down, symbolize by a dotted slur
                line += ".(";
                note.remove('\\');
                slur = 2;
            }
            if (note.contains('~')) {
                // bended note
                line += "~";
                note.remove('~');
            }
            if (note.contains('x')) {
                // ghosted note
                line += "!invisible!";
                note.remove(QChar('x'), Qt::CaseSensitive);
            }
            // split notes if required, to avoid "too much dotted note" errors in abcm2ps
            int durationIndex = note.indexOf(QRegExp("[0-9]"));
            int duration = note.mid(durationIndex).toInt();
            QString baseNote = note.left(durationIndex);
            note.clear();
            while ((ticks>2) && (duration % (ticks/2))) {
                while ((ticks>4) && (duration % (ticks/4))) {
                    while ((ticks>8) && (duration % (ticks/8))) {
                        note = baseNote+"1" + (note.isEmpty() ? "" : "-") + note;
                        duration--;
                    }
                    if (duration % (ticks/4)) {
                        note = baseNote+"2" + (note.isEmpty() ? "" : "-") + note;
                        duration -= 2;
                    } else break;
                }
                if (duration % (ticks/2)) {
                    note = baseNote+"4" + (note.isEmpty() ? "" : "-") + note;
                    duration -= 4;
                } else break;
            }
            if (duration > 0) {
                note = QString("%1%2").arg(baseNote).arg(duration) + (note.isEmpty() ? "" : "-") + note;
            }
            line += note + " ";
            if (slur > 0) {
                slur--;
                if (slur==0) {
                    line += ")";
                }
            }
        }
    }
    return true;
}

bool AbcFile::save(const QString &filePath)
{
    bool result = false;
    QFile fOut(filePath);
    if (fOut.open(QFile::WriteOnly | QFile::Text)) {
        emit info(tr("Ausgabedatei erfolgreich zum Schreiben geöffnet."));
        QTextStream ts(&fOut);
        foreach (QString line, m_content) {
            ts << line << endl;
        }
        fOut.close();
        emit success(tr("Ausgabedatei erfolgreich geschrieben."));
        result = true;
    } else {
        emit error(tr("Kann Ausgabedatei nicht zum Schreiben öffnen (%1: %2)")
                   .arg(fOut.error()).arg(fOut.errorString()));
    }
    return result;
}

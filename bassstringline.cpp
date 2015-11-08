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

#include "bassstringline.h"

#include <QRegExp>


BassStringLine::BassStringLine(const QString &line, char defaultTuning, int lineNumber, int stringNumber, QObject *parent)
    : QObject(parent)
    , m_line(line)
    , m_tuning(defaultTuning)
    , m_lineNumber(lineNumber)
    , m_stringNumber(stringNumber)
    , m_bars(0)
{
    typedef enum {
        Start,
        Bar,
        NewNote,
        InNote
    } STATE;
    STATE state = Start;
    int ticks;
    QStringList notes;
    QString note, extra;
    QByteArray l = line.toLatin1().toUpper();
    foreach(char c, l) {
        bool endNote = false;
        bool endBar = false;
        switch(c) {
        case '|':   // bar
            if ((state==Bar) || (state==NewNote)) {
                state = NewNote;
                extra.clear();
            } else {
                endNote = true;
                endBar = true;
            }
            break;
        case '-':   // end of reqular note
            if (state==NewNote) {
                note = '-';
                state = InNote;
                extra.clear();
            } else {
                endNote = true;
            }
            break;
        case 'P':   // end of pulled note
            endNote = true;
            extra = 'p';
            break;
        case 'H':   // end of hammered note
            endNote = true;
            extra = 'h';
            break;
        case 'X':   // end of ghosted note
            endNote = true;
            extra = 'x';
            break;
        case '^':   // end of bended note
            endNote = true;
            extra = '^';
            break;
        case '/':   // end of note, slide up in pitch
            endNote = true;
            extra = '/';
            break;
        case '\\':  // end of note, slide down in pitch
            endNote = true;
            extra = '\\';
            break;
        case '0':   // part of fret number
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (state==NewNote) {
                note = c;
                state = InNote;
            } else if (state==InNote) {
                note += c;
            }
            break;
        case 'A':   // tuning of this string
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
            if (state==Start) {
                m_tuning = c;
                state = Bar;
            }
            break;
        default:
            emit warning(tr("Unbekanntes Zeichen in Bass Tab %1, Takt %2, Schlag %3, %4-Saite")
                       .arg(m_lineNumber)
                       .arg(m_bars+1)
                       .arg(ticks+1)
                       .arg(m_tuning)
                       );
        }
        if (endNote) {
            ticks++;
            notes.append(calcNote(note)+extra);
        }
        if (endBar) {
            m_notes.append(notes);
            notes.clear();
            m_ticks.append(ticks);
            ticks = 0;
            m_bars++;
        }
    }
}

bool BassStringLine::isBassStringLine(const QString &line)
{
    return line.contains(QRegExp("^([ABCDEFG]\\||\\|"));
}

int BassStringLine::barCount() const
{
    return m_bars;
}

int BassStringLine::tickCount(int bar) const
{
    return m_ticks.value(bar, 0);
}

QChar BassStringLine::tuning() const
{
    return m_tuning;
}

QString BassStringLine::note(int bar, int tick) const
{
    return m_notes.value(bar, QStringList()).value(tick, 0);
}

#define NUM_NOTES 8*11
static const char *note_names[NUM_NOTES] =  {
    "C,,,", "^C,,,","D,,,", "^D,,,","E,,,", "F,,,", "^F,,,","G,,,", "^G,,,","A,,,", "B,,,",
    "C,,",  "^C,,", "D,,",  "^D,,", "E,,",  "F,,",  "^F,,", "G,,",  "^G,,", "A,,",  "B,,",
    "C,",   "^C,",  "D,",   "^D,",  "E,",   "F,",   "^F,",  "G,",   "^G,",  "A,",   "B,",
    "C",    "^C",   "D",    "^D",   "E",    "F",    "^F",   "G",    "^G",   "A",    "B",
    "c",    "^c",   "d",    "^d",   "e",    "f",    "^f",   "g",    "^g",   "a",    "b",
    "c'",   "^c'",  "d'",   "^d'",  "e'",   "f'",   "^f'",  "g'",   "^g'",  "a'",   "b'",
    "c''",  "^c''", "d''",  "^d''", "e''",  "f''",  "^f''", "g''",  "^g''", "a''",  "b''"
    "c'''", "^c'''","d'''", "^d'''","e'''", "f'''", "^f'''","g'''", "^g'''","a'''", "b'''"
};

QString BassStringLine::calcNote(const QString &fretString) const
{
    int base = m_stringNumber < 3 ? 2*11 : 11;
    while (!QString(note_names[base]).contains(m_tuning, Qt::CaseInsensitive) && base < NUM_NOTES)
        base++;
    int noteIndex = qBound(0, base+fretString.toInt(), NUM_NOTES-1);
    return QString(note_names[noteIndex]);
}

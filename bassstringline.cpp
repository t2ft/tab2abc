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


BassStringLine::BassStringLine(char defaulTuning, int lineNumber, int stringNumber, QObject *parent)
    : QObject(parent)
    , m_tuning(defaulTuning)
    , m_lineNumber(lineNumber)
    , m_stringNumber(stringNumber)
    , m_tuningBase(note2index(defaulTuning))
{
}

bool BassStringLine::convert(const QString &line)
{
    bool ret = true;
    typedef enum {
        Start,
        Bar,
        NewNote,
        InNote
    } STATE;
    STATE state = Start;
    int ticks = 0;
    QStringList notes;
    QString note, extra;
    QByteArray l = line.toLatin1().toUpper();
    foreach(char c, l) {
        bool endNote = false;
        bool endBar = false;
        switch(c) {
        case '|':   // bar
            if ((state==Start) || (state==Bar)) {
                state = NewNote;
                extra.clear();
            } else {
                endNote = (state==InNote);
                endBar = true;
            }
            break;
        case '~':
        case '-':   // end of reqular note
            if ((state==Bar) || (state==NewNote)) {
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
            if ((state==NewNote) || (state==Bar)) {
                note = c;
                state = InNote;
                extra.clear();
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
                m_tuningBase = note2index(c);
                state = Bar;
            }
            break;
        default:
            emit warning(tr("Unbekanntes Zeichen in Bass Tab %1, Takt %2, Schlag %3, %4-Saite")
                       .arg(m_lineNumber)
                       .arg(barCount()+1)
                       .arg(ticks+1)
                       .arg(m_tuning)
                       );
            ret = false;
        }
        if (endNote) {
            ticks++;
            notes.append(calcNote(note)+extra);
            extra.clear();
            state = NewNote;
        }
        if (endBar) {
            m_notes.append(notes);
            notes.clear();
            extra.clear();
            ticks = 0;
            state = Bar;
        }
    }
    return ret;
}

bool BassStringLine::isBassStringLine(const QString &line)
{
    return line.contains(QRegExp("^([ABCDEFG]\\||\\|)"));
}

int BassStringLine::barCount() const
{
    return m_notes.count();
}

int BassStringLine::tickCount(int bar) const
{
    return m_notes.value(bar).count();
}

QChar BassStringLine::tuning() const
{
    return m_tuning;
}

QString BassStringLine::note(int bar, int tick) const
{
    QString ret = m_notes.value(bar, QStringList()).value(tick, 0);
    if (ret=="-")
        ret.clear();
    return ret;
}

#define NUM_NOTES 8*12
static const char *note_names[NUM_NOTES] =  {
    "C,,,", "^C,,,","D,,,", "^D,,,","E,,,", "F,,,", "^F,,,","G,,,", "^G,,,","A,,,","^A,,,","B,,,",
    "C,,",  "^C,,", "D,,",  "^D,,", "E,,",  "F,,",  "^F,,", "G,,",  "^G,,", "A,,", "^A,,", "B,,",
    "C,",   "^C,",  "D,",   "^D,",  "E,",   "F,",   "^F,",  "G,",   "^G,",  "A,",  "^A,",  "B,",
    "C",    "^C",   "D",    "^D",   "E",    "F",    "^F",   "G",    "^G",   "A",   "^A",   "B",
    "c",    "^c",   "d",    "^d",   "e",    "f",    "^f",   "g",    "^g",   "a",   "^a",   "b",
    "c'",   "^c'",  "d'",   "^d'",  "e'",   "f'",   "^f'",  "g'",   "^g'",  "a'",  "^a'",  "b'",
    "c''",  "^c''", "d''",  "^d''", "e''",  "f''",  "^f''", "g''",  "^g''", "a''", "^a''", "b''"
    "c'''", "^c'''","d'''", "^d'''","e'''", "f'''", "^f'''","g'''", "^g'''","a'''","^a'''","b'''"
};

int BassStringLine::note2index(char note)
{
    int index = m_stringNumber < 2 ? 2*12 : 12;
    while (!QString(note_names[index]).contains(note, Qt::CaseInsensitive) && index < NUM_NOTES)
        index++;
    return index;
}

QString BassStringLine::calcNote(const QString &fretString) const
{
    QString ret;
    if (fretString!="-") {
        int fret = fretString.toInt();
        int noteIndex = qBound(0, m_tuningBase+fret, NUM_NOTES-1);
        ret += QString(note_names[noteIndex]);
    } else {
        ret = "-";
    }
    return ret;
}

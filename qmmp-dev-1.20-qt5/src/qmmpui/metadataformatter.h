/***************************************************************************
 *   Copyright (C) 2015 by Ilya Kotov                                      *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef METADATAFORMATTER_H
#define METADATAFORMATTER_H

#include <QString>
#include <QMap>
#include <QList>
#include <qmmpui/playlisttrack.h>
#include <qmmp/qmmp.h>

/*! @brief The MetaDataFormatter formats metadata using templates.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class MetaDataFormatter
{
public:
    /*!
     * Constructor.
     * @param pattern Metadata template.
     * Syntax:
     * %p - artist,
     * %a - album,
     * %aa - album artist,
     * %t - title,
     * %n - track number,
     * %NN - 2-digit track number,
     * %g - genre,
     * %c - comment,
     * %C - composer,
     * %D - disc number,
     * %f - file name,
     * %F - full path,
     * %y - year,
     * %l - duration,
     * %I - track index,
     * %if(A,B,C) or %if(A&B&C,D,E) - condition.
     */
    MetaDataFormatter(const QString &pattern = QString());
    /*!
     * Setups metadata template.
     * @param pattern Metadata template string.
     */
    void setPattern(const QString &pattern);
    /*!
     * Returns metadata template.
     */
    const QString pattern() const;

    /*!
     * Converts metadata of item \b item to one string using template.
     */
    QString format(const PlayListTrack *item) const;
    /*!
     * Converts metadata to one string using template.
     * @param metaData Metadata array.
     * @param length Length in seconds.
     * @param track Index of track.
     */
    QString format(const QMap<Qmmp::MetaData, QString> &metaData, qint64 length = 0, int track = 0) const;
    /*!
     * Returns formatted length (example: 05:02:03).
     * \param length Length in seconds.
     * \param hideZero Setting for zero values output.
     * If \b hideZero is \b true, then the function outputs empty string for zero length,
     * otherwise outputs "0:00".
     */
    static QString formatLength(qint64 length, bool hideZero = true);

private:
    struct Node;
    struct Param;

    struct Node
    {
        enum {
            PRINT_TEXT = 0,
            IF_KEYWORD,
            OR_OPERATOR,
            AND_OPERATOR
        } command;

        QList<Param> params;
    };

    struct Param
    {
        enum {
            FIELD = 0,
            TEXT,
            NODES
        } type;

        //extra fields
        enum
        {
            TWO_DIGIT_TRACK = Qmmp::URL + 1,
            DURATION,
            FILE_NAME,
            TRACK_INDEX
        };

        int field;
        QString text;
        QList<Node> children;
    };

    bool parseField(QList<Node> *nodes, QString::const_iterator *i, QString::const_iterator end);
    bool parseIf(QList<Node> *nodes, QString::const_iterator *i, QString::const_iterator end);
    void parseText(QList<Node> *nodes, QString::const_iterator *i, QString::const_iterator end);
    void parseEscape(QList<Node> *nodes, QString::const_iterator *i, QString::const_iterator end);

    QString evalute(const QList<Node> *nodes, const QMap<Qmmp::MetaData, QString> *metaData, qint64 length, int track) const;
    QString printParam(Param *p, const QMap<Qmmp::MetaData, QString> *metaData, qint64 length, int track) const;
    QString printField(int field, const QMap<Qmmp::MetaData, QString> *metaData, qint64 length, int track) const;

    QString dumpNode(Node node) const;

    QList<MetaDataFormatter::Node> compile(const QString &expr);
    QString m_pattern;
    QList<MetaDataFormatter::Node> m_nodes;
    QMap<QString, int> m_fieldNames;
};

#endif // METADATAFORMATTER2_H

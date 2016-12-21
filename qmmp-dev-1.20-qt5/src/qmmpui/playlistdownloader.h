/***************************************************************************
 *   Copyright (C) 2012 by Ilya Kotov                                      *
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

#ifndef PLAYLISTDOWNLOADER_H
#define PLAYLISTDOWNLOADER_H

#include <QObject>
#include <QUrl>

class QNetworkAccessManager;
class QNetworkReply;

/*! @brief The PlayListDownloader class downloads playlist from remote URL and extracts tracks
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class PlayListDownloader : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit PlayListDownloader(QObject *parent = 0);

signals:
    /*!
     * Emitted when downloading is finished without errors.
     * @param urls A list of extracted URLs or argument of the \b PlayListDownloader::start()
     * function if remote URL doesn't contain playlist.
     */
    void done(const QStringList &urls);
    /*!
     * Emitted when downloading is finished with error.
     * @param message Error message.
     */
    void error(const QString &message);

public slots:
    /*!
     * Starts playlist downloading
     * @param url URL of remote playlist
     */
    void start(const QUrl &url);

private slots:
    void readResponse(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_manager;
    QUrl m_redirect_url, m_url;
    QNetworkReply *m_getReply;
    QByteArray m_ua;
};

#endif // PLAYLISTDOWNLOADER_H

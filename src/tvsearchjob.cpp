/*
 * Copyright (C) 2014 Vishesh Handa <me@vhanda.in>
 * Copyright (C) 2014 David Faure <faure@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "tvsearchjob.h"
#include "jobparams_p.h"
#include "configuration.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace TmdbQt;

class TmdbQt::TvSearchJobPrivate
{
public:
    TvSearchJobPrivate(const JobParams &params)
        : m_params(params) {}
    QNetworkReply *m_reply;
    QString m_errorMessage;
    TvShowDbList m_result;
    const JobParams &m_params;
};

TvSearchJob::TvSearchJob(const JobParams &params, const QString &name, int searchYear, const QString &language)
    : d(new TvSearchJobPrivate(params))
{
    QUrl url = params.baseUrl;
    url.setPath(url.path() + QStringLiteral("search/tv"));
    url.addQueryItem(QStringLiteral("query"), name);
    if (searchYear > 0)
        url.addQueryItem(QStringLiteral("first_air_date_year"), QString::number(searchYear));
    if (!language.isEmpty())
        url.addQueryItem(QStringLiteral("language"), language);

    QNetworkRequest request(url);
    d->m_reply = params.qnam.get(request);
    connect(d->m_reply, SIGNAL(finished()), this, SLOT(requestFinished()));
}

TvSearchJob::~TvSearchJob()
{
    delete d->m_reply;
    delete d;
}

bool TvSearchJob::hasError() const
{
    return !errorMessage().isEmpty();
}

QString TvSearchJob::errorMessage() const
{
    return d->m_errorMessage;
}

TvShowDbList TvSearchJob::result() const
{
    return d->m_result;
}

void TvSearchJob::requestFinished()
{
    const QByteArray data = d->m_reply->readAll();
    //qDebug() << data;
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        d->m_errorMessage = QStringLiteral("Invalid json received");
    }
    QJsonObject root = doc.object();
    QJsonArray results = root.value(QStringLiteral("results")).toArray();
    d->m_result.load(results, d->m_params.configuration);

    d->m_reply->deleteLater();
    d->m_reply = 0;

    emit result(this);
    deleteLater();
}

/*
 * Copyright (C) 2014  Vishesh Handa <me@vhanda.in>
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

#ifndef TMDBQT_TVDB_H
#define TMDBQT_TVDB_H

#include "tmdbqt_export.h"
#include <QString>
#include <QDate>
#include <QSharedDataPointer>

namespace TmdbQt
{
class TvDbList;
class TvDbPrivate;
class Configuration;

class TMDBQT_EXPORT TvDb
{
public:
    TvDb(const Configuration &config);
    TvDb(const TvDb &other);
    ~TvDb();
    TvDb &operator =(const TvDb &other);

    int id() const;
    QString name() const;
    QString originalName() const;

    QDate firstAiredDate() const;

    QString posterPath() const;
    QString backdropPath() const;

    QUrl posterUrl(const QString &size) const;
    QUrl backdropUrl(const QString &size) const;
private:
    friend class TvDbList;
    void load(const QJsonObject &json);

    QSharedDataPointer<TvDbPrivate> d;
};
}

#endif // TMDBQT_TVDB_H

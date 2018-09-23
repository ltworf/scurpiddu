/*
This file is part of Scurpiddu.

Scurpiddu is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Scurpiddu is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Scurpiddu. If not, see <http://www.gnu.org/licenses/>.

Copyright (C) 2018  Salvo "LtWorf" Tomaselli <tiposchi@tiscali.it>
*/

#include <QVariant>

#include "searchfilter.h"

SearchFilter::SearchFilter(unsigned int limit, QString query) {
    this->limit = limit;
    this->query = query;
}

void SearchFilter::prepare(QSqlQuery *query) {
    QString searched = "%" + this->query + "%";
    query->prepare ("SELECT * "
                    "FROM tracks  "
                    "WHERE "
                        "path LIKE :val OR "
                        "album LIKE :val OR "
                        "album_artist LIKE :val OR "
                        "artist LIKE :val OR "
                        "genre LIKE :val OR "
                        "title LIKE :val "
                    "LIMIT :limit;");

    query->bindValue(":limit", this->limit);
    query->bindValue(":val", searched);
}

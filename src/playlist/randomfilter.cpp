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


#include "randomfilter.h"

RandomFilter::RandomFilter(unsigned int limit) {
    this->limit = limit;
}

void RandomFilter::prepare(QSqlQuery *query) {
    query->prepare ("SELECT * FROM tracks ORDER BY RANDOM() LIMIT :limit;");
    query->bindValue(":limit", this->limit);
}

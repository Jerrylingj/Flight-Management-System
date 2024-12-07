#ifndef FLIGHTAPI_H
#define FLIGHTAPI_H

#include <QObject>
#include "database/databasemanager.h"

QJsonObject getAllFlights(DatabaseManager* m_db);

QJsonObject getFlightByID(int flightID,DatabaseManager* m_db);

#endif // FLIGHTAPI_H

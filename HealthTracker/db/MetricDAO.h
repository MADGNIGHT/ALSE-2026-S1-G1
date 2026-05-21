#ifndef METRICDAO_H
#define METRICDAO_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>

class MetricDAO
{
public:
    MetricDAO(QSqlDatabase database);

    bool insertWeight(int userId,
                      double weightKg,
                      const QString& measuredAt);

    bool insertBloodPressure(int userId,
                             int systolic,
                             int diastolic,
                             const QString& measuredAt);

    bool insertGlucose(int userId,
                       double glucoseMgDl,
                       const QString& measuredAt);

    QSqlQuery getWeightLogs(int userId);

    QSqlQuery getBloodPressureLogs(int userId);

    QSqlQuery getGlucoseLogs(int userId);

    bool deleteWeightLog(int logId);

    bool deleteBloodPressureLog(int logId);

    bool deleteGlucoseLog(int logId);

private:
    QSqlDatabase database;
};

#endif

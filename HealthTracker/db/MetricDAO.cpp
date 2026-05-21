#include "MetricDAO.h"

#include <QSqlError>
#include <QDebug>

MetricDAO::MetricDAO(QSqlDatabase database)
{
    this->database = database;
}

bool MetricDAO::insertWeight(int userId,
                             double weightKg,
                             const QString& measuredAt)
{
    QSqlQuery query(database);

    query.prepare(R"(
        INSERT INTO weight_logs (user_id, weight_kg, measured_at)
        VALUES (:user_id, :weight_kg, :measured_at);
    )");

    query.bindValue(":user_id", userId);
    query.bindValue(":weight_kg", weightKg);
    query.bindValue(":measured_at", measuredAt);

    if (!query.exec()) {
        qDebug() << "Error insertando peso:" << query.lastError().text();
        return false;
    }

    qDebug() << "Peso insertado correctamente.";
    return true;
}

bool MetricDAO::insertBloodPressure(int userId,
                                    int systolic,
                                    int diastolic,
                                    const QString& measuredAt)
{
    QSqlQuery query(database);

    query.prepare(R"(
        INSERT INTO blood_pressure_logs (user_id, systolic, diastolic, measured_at)
        VALUES (:user_id, :systolic, :diastolic, :measured_at);
    )");

    query.bindValue(":user_id", userId);
    query.bindValue(":systolic", systolic);
    query.bindValue(":diastolic", diastolic);
    query.bindValue(":measured_at", measuredAt);

    if (!query.exec()) {
        qDebug() << "Error insertando presion arterial:" << query.lastError().text();
        return false;
    }

    qDebug() << "Presion arterial insertada correctamente.";
    return true;
}

bool MetricDAO::insertGlucose(int userId,
                              double glucoseMgDl,
                              const QString& measuredAt)
{
    QSqlQuery query(database);

    query.prepare(R"(
        INSERT INTO glucose_logs (user_id, glucose_mg_dl, measured_at)
        VALUES (:user_id, :glucose_mg_dl, :measured_at);
    )");

    query.bindValue(":user_id", userId);
    query.bindValue(":glucose_mg_dl", glucoseMgDl);
    query.bindValue(":measured_at", measuredAt);

    if (!query.exec()) {
        qDebug() << "Error insertando glucosa:" << query.lastError().text();
        return false;
    }

    qDebug() << "Glucosa insertada correctamente.";
    return true;
}

QSqlQuery MetricDAO::getWeightLogs(int userId)
{
    QSqlQuery query(database);

    query.prepare(R"(
        SELECT id, user_id, weight_kg, measured_at
        FROM weight_logs
        WHERE user_id = :user_id
        ORDER BY measured_at ASC;
    )");

    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        qDebug() << "Error consultando registros de peso:" << query.lastError().text();
    }

    return query;
}

QSqlQuery MetricDAO::getBloodPressureLogs(int userId)
{
    QSqlQuery query(database);

    query.prepare(R"(
        SELECT id, user_id, systolic, diastolic, measured_at
        FROM blood_pressure_logs
        WHERE user_id = :user_id
        ORDER BY measured_at ASC;
    )");

    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        qDebug() << "Error consultando registros de presion:" << query.lastError().text();
    }

    return query;
}

QSqlQuery MetricDAO::getGlucoseLogs(int userId)
{
    QSqlQuery query(database);

    query.prepare(R"(
        SELECT id, user_id, glucose_mg_dl, measured_at
        FROM glucose_logs
        WHERE user_id = :user_id
        ORDER BY measured_at ASC;
    )");

    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        qDebug() << "Error consultando registros de glucosa:" << query.lastError().text();
    }

    return query;
}

bool MetricDAO::deleteWeightLog(int logId)
{
    QSqlQuery query(database);

    query.prepare(R"(
        DELETE FROM weight_logs
        WHERE id = :id;
    )");

    query.bindValue(":id", logId);

    if (!query.exec()) {
        qDebug() << "Error eliminando registro de peso:" << query.lastError().text();
        return false;
    }

    qDebug() << "Registro de peso eliminado correctamente.";
    return true;
}

bool MetricDAO::deleteBloodPressureLog(int logId)
{
    QSqlQuery query(database);

    query.prepare(R"(
        DELETE FROM blood_pressure_logs
        WHERE id = :id;
    )");

    query.bindValue(":id", logId);

    if (!query.exec()) {
        qDebug() << "Error eliminando registro de presion:" << query.lastError().text();
        return false;
    }

    qDebug() << "Registro de presion eliminado correctamente.";
    return true;
}

bool MetricDAO::deleteGlucoseLog(int logId)
{
    QSqlQuery query(database);

    query.prepare(R"(
        DELETE FROM glucose_logs
        WHERE id = :id;
    )");

    query.bindValue(":id", logId);

    if (!query.exec()) {
        qDebug() << "Error eliminando registro de glucosa:" << query.lastError().text();
        return false;
    }

    qDebug() << "Registro de glucosa eliminado correctamente.";
    return true;
}
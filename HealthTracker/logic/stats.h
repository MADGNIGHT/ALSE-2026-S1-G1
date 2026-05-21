#ifndef STATS_H
#define STATS_H

#include <QVector>
#include <QString>

struct TrendData
{
    double slope;
    double intercept;
    double r_squared;
};

class Stats
{
public:
    static double ave(const QVector<double> &data);
    static double min(const QVector<double> &data);
    static double max(const QVector<double> &data);
    static TrendData calculateTrend(const QVector<double> &data);
};

#endif

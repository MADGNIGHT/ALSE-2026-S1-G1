#include "stats.h"

#include <numeric>
#include <algorithm>
#include <cmath>

double Stats::ave(const QVector<double> &data)
{
    if (data.isEmpty()) {
        return 0.0;
    }
    const double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / data.size();
}

double Stats::min(const QVector<double> &data)
{
    if (data.isEmpty()) {
        return 0.0;
    }
    return *std::min_element(data.begin(), data.end());
}

double Stats::max(const QVector<double> &data)
{
    if (data.isEmpty()) {
        return 0.0;
    }
    return *std::max_element(data.begin(), data.end());
}

TrendData Stats::calculateTrend(const QVector<double> &data)
{
    TrendData trend{};
    const int n = data.size();
    if (n < 2) {
        trend.slope = 0.0;
        trend.intercept = data.isEmpty() ? 0.0 : data[0];
        trend.r_squared = 1.0;
        return trend;
    }

    double sum_x = 0.0;
    double sum_y = 0.0;
    double sum_xx = 0.0;
    double sum_xy = 0.0;

    for (int i = 0; i < n; ++i) {
        const double x = i + 1;
        const double y = data[i];
        sum_x += x;
        sum_y += y;
        sum_xx += x * x;
        sum_xy += x * y;
    }

    const double denominator = n * sum_xx - sum_x * sum_x;
    if (denominator == 0.0) {
        trend.slope = 0.0;
        trend.intercept = sum_y / n;
        trend.r_squared = 1.0;
        return trend;
    }

    trend.slope = (n * sum_xy - sum_x * sum_y) / denominator;
    trend.intercept = (sum_y - trend.slope * sum_x) / n;

    double ss_total = 0.0;
    double ss_residual = 0.0;
    const double mean = ave(data);

    for (int i = 0; i < n; ++i) {
        const double y_predicted = trend.intercept + trend.slope * (i + 1);
        ss_total += std::pow(data[i] - mean, 2);
        ss_residual += std::pow(data[i] - y_predicted, 2);
    }

    trend.r_squared = ss_total > 0.0 ? (1.0 - ss_residual / ss_total) : 1.0;
    return trend;
}

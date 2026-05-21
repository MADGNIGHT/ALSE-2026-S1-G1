#include "ChartsWidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QtMath>
#include <algorithm>
#include <numeric>
#include <cmath>

ChartsWidget::ChartsWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(240);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("background-color: white;");
}

void ChartsWidget::setData(const QVector<double>& data,
                           const QString& title,
                           const QString& yAxisLabel)
{
    values     = data;
    dates.clear();
    chartTitle = title;
    yLabel     = yAxisLabel;
    update();
}

void ChartsWidget::setDataWithDates(const QVector<double>& data,
                                    const QVector<QDateTime>& timestamps,
                                    const QString& title,
                                    const QString& yAxisLabel)
{
    values     = data;
    dates      = timestamps;
    chartTitle = title;
    yLabel     = yAxisLabel;
    update();
}

void ChartsWidget::clearData()
{
    values.clear();
    dates.clear();
    chartTitle.clear();
    yLabel.clear();
    update();
}

QSize ChartsWidget::minimumSizeHint() const
{
    return QSize(360, 240);
}

// ── helpers ────────────────────────────────────────────────────────────────

void ChartsWidget::drawNoData(QPainter& painter, const QRect& plotRect)
{
    painter.setPen(QColor(150, 150, 150));
    QFont f = painter.font();
    f.setItalic(true);
    painter.setFont(f);
    painter.drawText(plotRect, Qt::AlignCenter, tr("Sin datos para graficar"));
}

void ChartsWidget::drawGrid(QPainter& painter, const QRect& plotRect,
                            double minVal, double range, int steps)
{
    QPen gridPen(QColor(220, 220, 220), 1, Qt::DashLine);
    painter.setPen(gridPen);

    QFont smallFont = painter.font();
    smallFont.setPointSize(8);
    painter.setFont(smallFont);

    for (int i = 0; i <= steps; ++i) {
        const double t = static_cast<double>(i) / steps;
        const int y = plotRect.bottom() - static_cast<int>(t * plotRect.height());
        painter.drawLine(plotRect.left(), y, plotRect.right(), y);

        const double labelValue = minVal + t * range;
        const QString lbl = QString::number(labelValue, 'f', 1);
        painter.setPen(QColor(80, 80, 80));
        painter.drawText(QRect(0, y - 8, plotRect.left() - 4, 16),
                         Qt::AlignRight | Qt::AlignVCenter, lbl);
        painter.setPen(gridPen);
    }
}

void ChartsWidget::drawSeries(QPainter& painter, const QRect& plotRect,
                               double minVal, double range)
{
    const int n = values.size();

    auto toPoint = [&](int index, double value) -> QPointF {
        const double x = plotRect.left()
                         + (n == 1 ? plotRect.width() / 2.0
                                   : static_cast<double>(index) * plotRect.width() / (n - 1));
        const double y = plotRect.bottom()
                         - ((value - minVal) / range) * plotRect.height();
        return {x, y};
    };

    // Line
    painter.setPen(QPen(QColor(46, 117, 182), 2));
    for (int i = 1; i < n; ++i) {
        painter.drawLine(toPoint(i - 1, values[i - 1]), toPoint(i, values[i]));
    }

    // Points + value labels
    for (int i = 0; i < n; ++i) {
        const QPointF p = toPoint(i, values[i]);

        painter.setBrush(QColor(46, 117, 182));
        painter.setPen(Qt::white);
        painter.drawEllipse(p, 4, 4);

        painter.setPen(QColor(40, 40, 40));
        QFont vf = painter.font();
        vf.setPointSize(7);
        painter.setFont(vf);
        painter.drawText(QPointF(p.x() - 14, p.y() - 7),
                         QString::number(values[i], 'f', 1));
    }
}

void ChartsWidget::drawXLabels(QPainter& painter, const QRect& plotRect)
{
    const int n = values.size();
    if (n == 0) return;

    QFont xf = painter.font();
    xf.setPointSize(7);
    painter.setFont(xf);
    painter.setPen(QColor(80, 80, 80));

    for (int i = 0; i < n; ++i) {
        const double x = plotRect.left()
                         + (n == 1 ? plotRect.width() / 2.0
                                   : static_cast<double>(i) * plotRect.width() / (n - 1));
        QString lbl;
        if (i < dates.size()) {
            lbl = dates[i].toString("dd/MM");
        } else {
            lbl = QString::number(i + 1);
        }
        painter.drawText(QRectF(x - 18, plotRect.bottom() + 4, 36, 16),
                         Qt::AlignCenter, lbl);
    }
}

void ChartsWidget::drawTrendLine(QPainter& painter, const QRect& plotRect,
                                  double minVal, double range)
{
    const int n = values.size();
    if (n < 3) return;

    // Simple linear regression
    double sumX = 0, sumY = 0, sumXX = 0, sumXY = 0;
    for (int i = 0; i < n; ++i) {
        sumX  += i;
        sumY  += values[i];
        sumXX += i * i;
        sumXY += i * values[i];
    }
    const double denom = n * sumXX - sumX * sumX;
    if (std::abs(denom) < 1e-10) return;

    const double slope = (n * sumXY - sumX * sumY) / denom;
    const double intercept = (sumY - slope * sumX) / n;

    auto trendY = [&](int i) {
        return intercept + slope * i;
    };
    auto toPoint = [&](int index, double value) -> QPointF {
        const double x = plotRect.left()
                         + (n == 1 ? plotRect.width() / 2.0
                                   : static_cast<double>(index) * plotRect.width() / (n - 1));
        const double y = plotRect.bottom()
                         - ((value - minVal) / range) * plotRect.height();
        return {x, y};
    };

    painter.setPen(QPen(QColor(200, 80, 80), 1, Qt::DashLine));
    painter.drawLine(toPoint(0, trendY(0)), toPoint(n - 1, trendY(n - 1)));
}

// ── paintEvent ─────────────────────────────────────────────────────────────

void ChartsWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), Qt::white);

    // Margins
    const int left   = 55;
    const int top    = 36;
    const int right  = 20;
    const int bottom = 36;

    QRect plotRect(left, top, width() - left - right, height() - top - bottom);

    // Border
    painter.setPen(QPen(QColor(180, 180, 180), 1));
    painter.drawRect(plotRect);

    // Title
    QFont titleFont = painter.font();
    titleFont.setBold(true);
    titleFont.setPointSize(9);
    painter.setFont(titleFont);
    painter.setPen(QColor(31, 78, 121));
    painter.drawText(QRect(left, 6, width() - left - right, 24),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     chartTitle.isEmpty() ? tr("Gráfica") : chartTitle);

    // Y-axis label
    if (!yLabel.isEmpty()) {
        QFont yf = painter.font();
        yf.setPointSize(8);
        yf.setBold(false);
        painter.setFont(yf);
        painter.setPen(QColor(100, 100, 100));

        painter.save();
        painter.translate(12, top + plotRect.height() / 2);
        painter.rotate(-90);
        painter.drawText(QRect(-60, -10, 120, 20), Qt::AlignCenter, yLabel);
        painter.restore();
    }

    if (values.isEmpty()) {
        drawNoData(painter, plotRect);
        return;
    }

    const double minVal = *std::min_element(values.constBegin(), values.constEnd());
    const double maxVal = *std::max_element(values.constBegin(), values.constEnd());
    const double range  = qMax(0.0001, maxVal - minVal);

    // Add 10% padding top/bottom
    const double paddedMin = minVal - range * 0.1;
    const double paddedRange = range * 1.2;

    drawGrid(painter, plotRect, paddedMin, paddedRange, 5);
    drawTrendLine(painter, plotRect, paddedMin, paddedRange);
    drawSeries(painter, plotRect, paddedMin, paddedRange);
    drawXLabels(painter, plotRect);
}

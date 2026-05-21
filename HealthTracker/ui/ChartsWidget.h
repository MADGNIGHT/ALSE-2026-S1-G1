#ifndef CHARTSWIDGET_H
#define CHARTSWIDGET_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QDateTime>

/**
 * @brief Widget que dibuja una gráfica de líneas con puntos y etiquetas de fecha.
 *
 * Recibe un vector de valores numéricos y, opcionalmente, un vector de fechas
 * para mostrar en el eje X. Si no se proveen fechas, muestra índices (1, 2, 3…).
 */
class ChartsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChartsWidget(QWidget *parent = nullptr);

    /**
     * @brief Carga datos simples (sin fechas en eje X).
     * @param data     Vector de valores numéricos ordenados cronológicamente.
     * @param title    Título de la gráfica.
     * @param yAxisLabel Etiqueta del eje Y (unidad de medida).
     */
    void setData(const QVector<double>& data,
                 const QString& title = QString(),
                 const QString& yAxisLabel = QString());

    /**
     * @brief Carga datos con fechas reales para el eje X.
     * @param data      Vector de valores numéricos.
     * @param timestamps Vector de fechas correspondientes a cada valor.
     * @param title     Título de la gráfica.
     * @param yAxisLabel Etiqueta del eje Y.
     */
    void setDataWithDates(const QVector<double>& data,
                          const QVector<QDateTime>& timestamps,
                          const QString& title = QString(),
                          const QString& yAxisLabel = QString());

    /** @brief Limpia todos los datos y redibuja. */
    void clearData();

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize minimumSizeHint() const override;

private:
    QVector<double>   values;
    QVector<QDateTime> dates;
    QString           chartTitle;
    QString           yLabel;

    void drawNoData(QPainter& painter, const QRect& plotRect);
    void drawGrid(QPainter& painter, const QRect& plotRect,
                  double minVal, double range, int steps);
    void drawSeries(QPainter& painter, const QRect& plotRect,
                    double minVal, double range);
    void drawXLabels(QPainter& painter, const QRect& plotRect);
    void drawTrendLine(QPainter& painter, const QRect& plotRect,
                       double minVal, double range);
};

#endif // CHARTSWIDGET_H

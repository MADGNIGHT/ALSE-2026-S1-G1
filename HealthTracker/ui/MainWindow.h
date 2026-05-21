#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QLabel>
#include <QTabWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSqlQuery>

#include "../db/UserDAO.h"
#include "../db/MetricDAO.h"
#include "../logic/stats.h"
#include "../logic/cuentas.h"
#include "ChartsWidget.h"
#include "MetricInputDialog.h"

/**
 * @brief Ventana principal de HealthTracker.
 *
 * Gestiona tres pestañas: Registros, Estadísticas y Perfil.
 * Conecta la capa de base de datos (MetricDAO, UserDAO) con la interfaz gráfica.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    using MetricType = MetricInputDialog::MetricType;

    explicit MainWindow(int userId, QWidget *parent = nullptr);

private slots:
    void openMetricDialog();
    void refreshHistory();
    void deleteSelectedEntry();
    void exportCurrentMetricCsv();
    void updateStatsView();
    void saveProfile();
    void calculateBodyMetrics();
    void onHistoryMetricChanged();
    void onStatsMetricChanged();

private:
    int     currentUserId;
    QString currentUserName;

    UserDAO   userDao;
    MetricDAO metricDao;

    QWidget    *central;
    QTabWidget *tabs;

    // ── History tab ──────────────────────────────────────────────────────
    QComboBox    *comboHistoryMetric;
    QTableWidget *tableHistory;
    QPushButton  *btnDeleteSelected;
    QPushButton  *btnExportCsv;
    QPushButton  *btnRefreshHistory;

    // ── Stats tab ────────────────────────────────────────────────────────
    QComboBox    *comboStatsMetric;
    QLabel       *lblAverage;
    QLabel       *lblMin;
    QLabel       *lblMax;
    QLabel       *lblTrend;
    QLabel       *lblTrendDirection;
    ChartsWidget *chartWidget;

    // ── Profile tab ──────────────────────────────────────────────────────
    QSpinBox       *spinAge;
    QDoubleSpinBox *spinHeight;
    QComboBox      *comboSex;
    QComboBox      *comboActivity;
    QComboBox      *comboGoal;
    QPushButton    *btnSaveProfile;
    QPushButton    *btnCalculate;
    QLabel         *lblBmr;
    QLabel         *lblTde;
    QLabel         *lblCalories;
    QLabel         *lblBmi;
    QLabel         *lblBmiInterpretation;

    void buildUi();

    /** @brief Inserta una medición en la base de datos y actualiza la UI. */
    void insertMeasurement(const MetricInputDialog &dialog);

    MetricType historyMetricType() const;
    MetricType statsMetricType()   const;

    QString         metricLabel(MetricType metric) const;

    // BUG FIX: ahora retorna también las fechas para pasarlas al chart
    void loadMetricValuesAndDates(MetricType metric,
                                  QVector<double>   &outValues,
                                  QVector<QDateTime> &outDates);

    QSqlQuery loadMetricQuery(MetricType metric);

    void populateHistoryTable(MetricType metric);
    void updateStatsForMetric(MetricType metric);
    void loadProfileIntoUi();
    bool readLatestWeight(double &weightKg);
    double selectedHeightCm() const;

    static QString csvEscape(const QString &value);
    static QString metricTypeName(MetricType metric);
    static QString normalizeText(const QString &value);
};

#endif // MAINWINDOW_H

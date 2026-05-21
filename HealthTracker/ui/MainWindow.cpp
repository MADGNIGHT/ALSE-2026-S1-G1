#include "MainWindow.h"

#include <QDateTime>
#include <QFileDialog>
#include <QFile>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>
#include <QTextStream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QLocale>
#include <QAbstractItemView>
#include <QToolBar>
#include <QAction>
#include <QStatusBar>

MainWindow::MainWindow(int userId, QWidget *parent)
    : QMainWindow(parent),
      currentUserId(userId),
      userDao(QSqlDatabase::database("healthtracker_connection")),
      metricDao(QSqlDatabase::database("healthtracker_connection"))
{
    QString name;
    if (userDao.getUserName(currentUserId, name)) {
        currentUserName = name;
    } else {
        currentUserName = QStringLiteral("Usuario %1").arg(currentUserId);
    }

    buildUi();
    loadProfileIntoUi();
    populateHistoryTable(historyMetricType());
    updateStatsForMetric(statsMetricType());

    setWindowTitle(QString("HealthTracker — %1").arg(currentUserName));
    resize(1100, 780);

    statusBar()->showMessage(QString("Bienvenido, %1").arg(currentUserName), 4000);
}

void MainWindow::buildUi()
{
    // ── Toolbar ────────────────────────────────────────────────────────────
    QToolBar *toolbar = addToolBar("Acciones");
    toolbar->setMovable(false);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QAction *actionAddMetric = toolbar->addAction("➕  Nueva medición");
    actionAddMetric->setToolTip("Agregar nueva medición (peso, presión o glucosa)");
    connect(actionAddMetric, &QAction::triggered, this, &MainWindow::openMetricDialog);

    central = new QWidget(this);
    setCentralWidget(central);
    tabs = new QTabWidget(central);

    // ── Tab: Registros ─────────────────────────────────────────────────────
    QWidget *tabHistory = new QWidget();
    QVBoxLayout *historyLayout = new QVBoxLayout(tabHistory);

    comboHistoryMetric = new QComboBox();
    comboHistoryMetric->addItem("Peso",             static_cast<int>(MetricType::Weight));
    comboHistoryMetric->addItem("Presión arterial", static_cast<int>(MetricType::BloodPressure));
    comboHistoryMetric->addItem("Glucosa",          static_cast<int>(MetricType::Glucose));

    tableHistory = new QTableWidget();
    tableHistory->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableHistory->setSelectionMode(QAbstractItemView::SingleSelection);
    tableHistory->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableHistory->horizontalHeader()->setStretchLastSection(true);
    tableHistory->verticalHeader()->setVisible(false);
    tableHistory->setAlternatingRowColors(true);

    btnDeleteSelected  = new QPushButton("🗑  Eliminar seleccionado");
    btnExportCsv       = new QPushButton("📄  Exportar CSV");
    btnRefreshHistory  = new QPushButton("🔄  Actualizar");

    // BUG FIX: botón adicional en la pestaña para que sea más visible
    QPushButton *btnAddInHistory = new QPushButton("➕  Nueva medición");
    btnAddInHistory->setStyleSheet(
        "QPushButton { background-color: #2E75B6; color: white; "
        "padding: 8px 16px; border-radius: 4px; font-size: 13px; }"
        "QPushButton:hover { background-color: #1F4E79; }");
    connect(btnAddInHistory, &QPushButton::clicked, this, &MainWindow::openMetricDialog);

    QHBoxLayout *historyButtons = new QHBoxLayout();
    historyButtons->addWidget(btnAddInHistory);
    historyButtons->addStretch();
    historyButtons->addWidget(btnRefreshHistory);
    historyButtons->addWidget(btnDeleteSelected);
    historyButtons->addWidget(btnExportCsv);

    QHBoxLayout *comboRow = new QHBoxLayout();
    comboRow->addWidget(new QLabel("Mostrar:"));
    comboRow->addWidget(comboHistoryMetric);
    comboRow->addStretch();

    historyLayout->addLayout(comboRow);
    historyLayout->addWidget(tableHistory);
    historyLayout->addLayout(historyButtons);

    connect(comboHistoryMetric, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onHistoryMetricChanged);
    connect(btnRefreshHistory,  &QPushButton::clicked, this, &MainWindow::refreshHistory);
    connect(btnDeleteSelected,  &QPushButton::clicked, this, &MainWindow::deleteSelectedEntry);
    connect(btnExportCsv,       &QPushButton::clicked, this, &MainWindow::exportCurrentMetricCsv);

    // ── Tab: Estadísticas ──────────────────────────────────────────────────
    QWidget *tabStats = new QWidget();
    QVBoxLayout *statsLayout = new QVBoxLayout(tabStats);

    comboStatsMetric = new QComboBox();
    comboStatsMetric->addItem("Peso",             static_cast<int>(MetricType::Weight));
    comboStatsMetric->addItem("Presión arterial", static_cast<int>(MetricType::BloodPressure));
    comboStatsMetric->addItem("Glucosa",          static_cast<int>(MetricType::Glucose));

    lblAverage        = new QLabel("—");
    lblMin            = new QLabel("—");
    lblMax            = new QLabel("—");
    lblTrend          = new QLabel("—");
    lblTrendDirection = new QLabel("—");

    chartWidget = new ChartsWidget();

    QGroupBox  *statsGroup = new QGroupBox("Resumen estadístico");
    QFormLayout *statsForm = new QFormLayout(statsGroup);
    statsForm->addRow("Promedio:",       lblAverage);
    statsForm->addRow("Mínimo:",         lblMin);
    statsForm->addRow("Máximo:",         lblMax);
    statsForm->addRow("Tendencia:",      lblTrend);
    statsForm->addRow("Interpretación:", lblTrendDirection);

    QHBoxLayout *statsComboRow = new QHBoxLayout();
    statsComboRow->addWidget(new QLabel("Métrica:"));
    statsComboRow->addWidget(comboStatsMetric);
    statsComboRow->addStretch();

    statsLayout->addLayout(statsComboRow);
    statsLayout->addWidget(statsGroup);
    statsLayout->addWidget(chartWidget, 1);

    connect(comboStatsMetric, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onStatsMetricChanged);

    // ── Tab: Perfil ────────────────────────────────────────────────────────
    QWidget *tabProfile = new QWidget();
    QVBoxLayout *profileLayout = new QVBoxLayout(tabProfile);

    spinAge = new QSpinBox();
    spinAge->setRange(1, 120);
    spinAge->setValue(30);

    spinHeight = new QDoubleSpinBox();
    spinHeight->setRange(50.0, 250.0);
    spinHeight->setDecimals(1);
    spinHeight->setSuffix(" cm");
    spinHeight->setValue(170.0);

    comboSex = new QComboBox();
    comboSex->addItem("Masculino");
    comboSex->addItem("Femenino");

    comboActivity = new QComboBox();
    comboActivity->addItem("Bajo");
    comboActivity->addItem("Moderado");
    comboActivity->addItem("Alto");

    comboGoal = new QComboBox();
    comboGoal->addItem("Perder peso");
    comboGoal->addItem("Mantener peso");
    comboGoal->addItem("Ganar peso");
    comboGoal->setCurrentIndex(1);

    btnSaveProfile = new QPushButton("Guardar perfil");
    btnCalculate   = new QPushButton("Calcular BMR / calorías");

    lblBmr              = new QLabel("—");
    lblTde              = new QLabel("—");
    lblCalories         = new QLabel("—");
    lblBmi              = new QLabel("—");
    lblBmiInterpretation = new QLabel("—");

    QGroupBox  *profileFormGroup = new QGroupBox("Datos del perfil");
    QFormLayout *profileForm     = new QFormLayout(profileFormGroup);
    profileForm->addRow("Edad:",      spinAge);
    profileForm->addRow("Estatura:",  spinHeight);
    profileForm->addRow("Sexo:",      comboSex);
    profileForm->addRow("Actividad:", comboActivity);
    profileForm->addRow("Objetivo:",  comboGoal);

    QHBoxLayout *profileBtns = new QHBoxLayout();
    profileBtns->addWidget(btnSaveProfile);
    profileBtns->addWidget(btnCalculate);

    QGroupBox  *resultsGroup = new QGroupBox("Resultados");
    QFormLayout *resultsForm = new QFormLayout(resultsGroup);
    resultsForm->addRow("BMR (kcal/día):",        lblBmr);
    resultsForm->addRow("TDEE (kcal/día):",       lblTde);
    resultsForm->addRow("Objetivo calórico:",     lblCalories);
    resultsForm->addRow("IMC:",                   lblBmi);
    resultsForm->addRow("Interpretación IMC:",    lblBmiInterpretation);

    profileLayout->addWidget(profileFormGroup);
    profileLayout->addLayout(profileBtns);
    profileLayout->addWidget(resultsGroup);
    profileLayout->addStretch();

    connect(btnSaveProfile, &QPushButton::clicked, this, &MainWindow::saveProfile);
    connect(btnCalculate,   &QPushButton::clicked, this, &MainWindow::calculateBodyMetrics);

    // ── Ensamblar tabs ─────────────────────────────────────────────────────
    tabs->addTab(tabHistory, "📋  Registros");
    tabs->addTab(tabStats,   "📊  Estadísticas");
    tabs->addTab(tabProfile, "👤  Perfil");

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->addWidget(tabs);
}

// ── Slots ──────────────────────────────────────────────────────────────────

void MainWindow::openMetricDialog()
{
    MetricInputDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        insertMeasurement(dialog);
    }
}

void MainWindow::insertMeasurement(const MetricInputDialog &dialog)
{
    const QString measuredAt = dialog.measuredAt().toString(Qt::ISODate);
    bool ok = false;

    switch (dialog.metricType()) {
    case MetricType::Weight:
        ok = metricDao.insertWeight(currentUserId, dialog.weightKg(), measuredAt);
        break;
    case MetricType::BloodPressure:
        ok = metricDao.insertBloodPressure(currentUserId,
                                           dialog.systolic(),
                                           dialog.diastolic(),
                                           measuredAt);
        break;
    case MetricType::Glucose:
        ok = metricDao.insertGlucose(currentUserId, dialog.glucoseMgDl(), measuredAt);
        break;
    }

    if (!ok) {
        QMessageBox::critical(this, "HealthTracker",
                              "No se pudo guardar la medición.\n"
                              "Revisa la conexión con la base de datos.");
        return;
    }

    // Refrescar ambas pestañas
    populateHistoryTable(historyMetricType());
    updateStatsForMetric(statsMetricType());

    statusBar()->showMessage("Medición guardada correctamente.", 3000);
}

MainWindow::MetricType MainWindow::historyMetricType() const
{
    return static_cast<MetricType>(comboHistoryMetric->currentData().toInt());
}

MainWindow::MetricType MainWindow::statsMetricType() const
{
    return static_cast<MetricType>(comboStatsMetric->currentData().toInt());
}

QString MainWindow::metricLabel(MetricType metric) const
{
    switch (metric) {
    case MetricType::Weight:       return "Peso (kg)";
    case MetricType::BloodPressure: return "Presión sistólica (mmHg)";
    case MetricType::Glucose:      return "Glucosa (mg/dL)";
    }
    return QString();
}

QString MainWindow::metricTypeName(MetricType metric)
{
    switch (metric) {
    case MetricType::Weight:       return "peso";
    case MetricType::BloodPressure: return "presion";
    case MetricType::Glucose:      return "glucosa";
    }
    return QString();
}

QString MainWindow::normalizeText(const QString& value)
{
    return value.trimmed().toLower();
}

// BUG FIX: función unificada que carga valores Y fechas de una vez
void MainWindow::loadMetricValuesAndDates(MetricType metric,
                                          QVector<double>    &outValues,
                                          QVector<QDateTime> &outDates)
{
    outValues.clear();
    outDates.clear();

    QSqlQuery query = loadMetricQuery(metric);

    while (query.next()) {
        const QString ts = query.value("measured_at").toString();
        outDates.append(QDateTime::fromString(ts, Qt::ISODate));

        if (metric == MetricType::Weight) {
            outValues.append(query.value("weight_kg").toDouble());
        } else if (metric == MetricType::BloodPressure) {
            outValues.append(query.value("systolic").toDouble());
        } else {
            outValues.append(query.value("glucose_mg_dl").toDouble());
        }
    }
}

QSqlQuery MainWindow::loadMetricQuery(MetricType metric)
{
    switch (metric) {
    case MetricType::Weight:       return metricDao.getWeightLogs(currentUserId);
    case MetricType::BloodPressure: return metricDao.getBloodPressureLogs(currentUserId);
    case MetricType::Glucose:      return metricDao.getGlucoseLogs(currentUserId);
    }
    return QSqlQuery(QSqlDatabase::database("healthtracker_connection"));
}

void MainWindow::populateHistoryTable(MetricType metric)
{
    QSqlQuery query = loadMetricQuery(metric);

    tableHistory->clear();
    tableHistory->setRowCount(0);

    if (metric == MetricType::Weight) {
        tableHistory->setColumnCount(3);
        tableHistory->setHorizontalHeaderLabels({"ID", "Peso (kg)", "Fecha"});
    } else if (metric == MetricType::BloodPressure) {
        tableHistory->setColumnCount(4);
        tableHistory->setHorizontalHeaderLabels({"ID", "Sistólica", "Diastólica", "Fecha"});
    } else {
        tableHistory->setColumnCount(3);
        tableHistory->setHorizontalHeaderLabels({"ID", "Glucosa (mg/dL)", "Fecha"});
    }

    int row = 0;
    while (query.next()) {
        tableHistory->insertRow(row);
        tableHistory->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));

        if (metric == MetricType::Weight) {
            tableHistory->setItem(row, 1,
                new QTableWidgetItem(QString::number(query.value("weight_kg").toDouble(), 'f', 2)));
            tableHistory->setItem(row, 2,
                new QTableWidgetItem(query.value("measured_at").toString()));
        } else if (metric == MetricType::BloodPressure) {
            tableHistory->setItem(row, 1,
                new QTableWidgetItem(query.value("systolic").toString()));
            tableHistory->setItem(row, 2,
                new QTableWidgetItem(query.value("diastolic").toString()));
            tableHistory->setItem(row, 3,
                new QTableWidgetItem(query.value("measured_at").toString()));
        } else {
            tableHistory->setItem(row, 1,
                new QTableWidgetItem(QString::number(query.value("glucose_mg_dl").toDouble(), 'f', 1)));
            tableHistory->setItem(row, 2,
                new QTableWidgetItem(query.value("measured_at").toString()));
        }
        ++row;
    }

    tableHistory->resizeColumnsToContents();
}

void MainWindow::refreshHistory()
{
    populateHistoryTable(historyMetricType());
}

void MainWindow::onHistoryMetricChanged()
{
    refreshHistory();
}

void MainWindow::onStatsMetricChanged()
{
    updateStatsView();
}

void MainWindow::deleteSelectedEntry()
{
    const QList<QTableWidgetItem*> selected = tableHistory->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::information(this, "HealthTracker", "Selecciona un registro primero.");
        return;
    }

    const int row   = selected.first()->row();
    const int logId = tableHistory->item(row, 0)->text().toInt();
    const MetricType metric = historyMetricType();

    bool ok = false;
    if (metric == MetricType::Weight) {
        ok = metricDao.deleteWeightLog(logId);
    } else if (metric == MetricType::BloodPressure) {
        ok = metricDao.deleteBloodPressureLog(logId);
    } else {
        ok = metricDao.deleteGlucoseLog(logId);
    }

    if (!ok) {
        QMessageBox::critical(this, "HealthTracker", "No se pudo eliminar el registro.");
        return;
    }

    populateHistoryTable(historyMetricType());
    updateStatsForMetric(statsMetricType());
    statusBar()->showMessage("Registro eliminado.", 2000);
}

QString MainWindow::csvEscape(const QString& value)
{
    QString escaped = value;
    escaped.replace("\"", "\"\"");
    if (escaped.contains(',') || escaped.contains('"') ||
        escaped.contains('\n') || escaped.contains('\r')) {
        escaped = "\"" + escaped + "\"";
    }
    return escaped;
}

void MainWindow::exportCurrentMetricCsv()
{
    const MetricType metric = historyMetricType();
    const QString filePath = QFileDialog::getSaveFileName(
        this,
        "Guardar CSV",
        QString("healthtracker_%1.csv").arg(metricTypeName(metric)),
        "CSV Files (*.csv)"
    );
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "HealthTracker", "No se pudo abrir el archivo.");
        return;
    }

    QTextStream out(&file);
    out.setLocale(QLocale::c());
    QSqlQuery query = loadMetricQuery(metric);

    if (metric == MetricType::Weight) {
        out << "id,weight_kg,measured_at\n";
        while (query.next()) {
            out << query.value("id").toInt() << ","
                << query.value("weight_kg").toDouble() << ","
                << csvEscape(query.value("measured_at").toString()) << "\n";
        }
    } else if (metric == MetricType::BloodPressure) {
        out << "id,systolic,diastolic,measured_at\n";
        while (query.next()) {
            out << query.value("id").toInt()     << ","
                << query.value("systolic").toInt()  << ","
                << query.value("diastolic").toInt() << ","
                << csvEscape(query.value("measured_at").toString()) << "\n";
        }
    } else {
        out << "id,glucose_mg_dl,measured_at\n";
        while (query.next()) {
            out << query.value("id").toInt()               << ","
                << query.value("glucose_mg_dl").toDouble() << ","
                << csvEscape(query.value("measured_at").toString()) << "\n";
        }
    }

    file.close();
    statusBar()->showMessage("CSV exportado correctamente.", 3000);
}

void MainWindow::updateStatsView()
{
    updateStatsForMetric(statsMetricType());
}

void MainWindow::updateStatsForMetric(MetricType metric)
{
    QVector<double>    values;
    QVector<QDateTime> dates;
    loadMetricValuesAndDates(metric, values, dates);   // BUG FIX: carga también fechas

    const QString title  = QString("Tendencia de %1").arg(comboStatsMetric->currentText());
    const QString yLabel = metricLabel(metric);

    if (values.isEmpty()) {
        lblAverage->setText("Sin datos");
        lblMin->setText("Sin datos");
        lblMax->setText("Sin datos");
        lblTrend->setText("Sin datos");
        lblTrendDirection->setText("Sin datos");
        chartWidget->setData(values, title, yLabel);
        return;
    }

    const double average  = Stats::ave(values);
    const double minValue = Stats::min(values);
    const double maxValue = Stats::max(values);
    const TrendData trend = Stats::calculateTrend(values);

    lblAverage->setText(QString::number(average,  'f', 2));
    lblMin->setText    (QString::number(minValue, 'f', 2));
    lblMax->setText    (QString::number(maxValue, 'f', 2));
    lblTrend->setText  (QString("Pendiente %1 | R² %2")
                        .arg(trend.slope,     0, 'f', 4)
                        .arg(trend.r_squared, 0, 'f', 3));

    QString interpretation;
    if      (trend.slope >  0.05) interpretation = "📈  Tendencia al alza";
    else if (trend.slope < -0.05) interpretation = "📉  Tendencia a la baja";
    else                          interpretation = "➡️  Estable";
    lblTrendDirection->setText(interpretation);

    // BUG FIX: pasa las fechas reales al chart
    chartWidget->setDataWithDates(values, dates, title, yLabel);
}

void MainWindow::loadProfileIntoUi()
{
    int    age = 30;
    double heightCm = 170.0;
    QString sex, activityLevel, goal;

    if (!userDao.loadUserProfile(currentUserId, age, heightCm, sex, activityLevel, goal))
        return;

    spinAge->setValue(age);
    spinHeight->setValue(heightCm);

    comboSex->setCurrentIndex(normalizeText(sex).startsWith("f") ? 1 : 0);

    const QString act = normalizeText(activityLevel);
    if      (act.startsWith("m")) comboActivity->setCurrentIndex(1);
    else if (act.startsWith("a")) comboActivity->setCurrentIndex(2);
    else                          comboActivity->setCurrentIndex(0);

    const QString obj = normalizeText(goal);
    if      (obj.contains("per")) comboGoal->setCurrentIndex(0);
    else if (obj.contains("gan")) comboGoal->setCurrentIndex(2);
    else                          comboGoal->setCurrentIndex(1);
}

void MainWindow::saveProfile()
{
    const bool ok = userDao.saveUserProfile(
        currentUserId,
        spinAge->value(),
        spinHeight->value(),
        comboSex->currentText(),
        comboActivity->currentText(),
        comboGoal->currentText()
    );

    if (ok) {
        statusBar()->showMessage("Perfil guardado correctamente.", 3000);
    } else {
        QMessageBox::critical(this, "HealthTracker", "No se pudo guardar el perfil.");
    }
}

bool MainWindow::readLatestWeight(double &weightKg)
{
    QSqlQuery query(QSqlDatabase::database("healthtracker_connection"));
    query.prepare(R"(
        SELECT weight_kg
        FROM weight_logs
        WHERE user_id = :user_id
        ORDER BY measured_at DESC, id DESC
        LIMIT 1;
    )");
    query.bindValue(":user_id", currentUserId);

    if (!query.exec() || !query.next()) return false;
    weightKg = query.value("weight_kg").toDouble();
    return true;
}

double MainWindow::selectedHeightCm() const
{
    return spinHeight->value();
}

void MainWindow::calculateBodyMetrics()
{
    const int    age      = spinAge->value();
    const double heightCm = selectedHeightCm();
    double weightKg = 70.0;
    readLatestWeight(weightKg);

    const Sexo sex = (comboSex->currentIndex() == 0) ? Sexo::Masculino : Sexo::Femenino;

    nivel_actividad activity = nivel_actividad::Bajo;
    if      (comboActivity->currentIndex() == 1) activity = nivel_actividad::Moderado;
    else if (comboActivity->currentIndex() == 2) activity = nivel_actividad::Alto;

    objetivo goal = objetivo::Mantener_peso;
    if      (comboGoal->currentIndex() == 0) goal = objetivo::Perder_peso;
    else if (comboGoal->currentIndex() == 2) goal = objetivo::Ganar_peso;

    const double bmr      = Cuentas::calcularBMR(weightKg, heightCm, age, sex);
    const double tde      = Cuentas::calcularTDE(bmr, activity);
    const double calories = Cuentas::calcularMeta(tde, goal);
    const double bmi      = Cuentas::calcularBMI(weightKg, heightCm);

    lblBmr->setText             (QString::number(bmr,      'f', 2) + " kcal/día");
    lblTde->setText             (QString::number(tde,      'f', 2) + " kcal/día");
    lblCalories->setText        (QString::number(calories, 'f', 2) + " kcal/día");
    lblBmi->setText             (QString::number(bmi,      'f', 2));
    lblBmiInterpretation->setText(Cuentas::interpretarBMI(bmi));
}

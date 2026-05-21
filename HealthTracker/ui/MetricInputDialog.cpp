#include "MetricInputDialog.h"

#include <QComboBox>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

MetricInputDialog::MetricInputDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Nueva medición");
    setModal(true);
    resize(380, 280);

    comboMetric = new QComboBox(this);
    comboMetric->addItem("Peso", static_cast<int>(MetricType::Weight));
    comboMetric->addItem("Presión arterial", static_cast<int>(MetricType::BloodPressure));
    comboMetric->addItem("Glucosa", static_cast<int>(MetricType::Glucose));

    dtMeasuredAt = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    dtMeasuredAt->setCalendarPopup(true);
    dtMeasuredAt->setDisplayFormat("yyyy-MM-dd HH:mm");

    // BUG FIX: valor inicial en 1.0 para que no bloquee la validación > 0
    spinWeight = new QDoubleSpinBox(this);
    spinWeight->setRange(1.0, 500.0);
    spinWeight->setValue(70.0);
    spinWeight->setDecimals(2);
    spinWeight->setSuffix(" kg");

    spinSystolic = new QSpinBox(this);
    spinSystolic->setRange(1, 300);
    spinSystolic->setValue(120);
    spinSystolic->setSuffix(" mmHg");

    spinDiastolic = new QSpinBox(this);
    spinDiastolic->setRange(1, 200);
    spinDiastolic->setValue(80);
    spinDiastolic->setSuffix(" mmHg");

    spinGlucose = new QDoubleSpinBox(this);
    spinGlucose->setRange(1.0, 500.0);
    spinGlucose->setValue(90.0);
    spinGlucose->setDecimals(1);
    spinGlucose->setSuffix(" mg/dL");

    lblHint = new QLabel(this);
    lblHint->setWordWrap(true);
    lblHint->setStyleSheet("color: #555555; font-style: italic;");

    auto *form = new QFormLayout();
    form->addRow("Tipo:", comboMetric);
    form->addRow("Fecha y hora:", dtMeasuredAt);
    form->addRow("Peso:", spinWeight);
    form->addRow("Sistólica:", spinSystolic);
    form->addRow("Diastólica:", spinDiastolic);
    form->addRow("Glucosa:", spinGlucose);
    form->addRow("", lblHint);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttons->button(QDialogButtonBox::Ok)->setText("Guardar");
    buttons->button(QDialogButtonBox::Cancel)->setText("Cancelar");
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(buttons);

    connect(comboMetric, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MetricInputDialog::updateFieldState);
    updateFieldState();
}

MetricInputDialog::MetricType MetricInputDialog::metricType() const
{
    return static_cast<MetricType>(comboMetric->currentData().toInt());
}

QDateTime MetricInputDialog::measuredAt() const
{
    return dtMeasuredAt->dateTime();
}

double MetricInputDialog::weightKg() const
{
    return spinWeight->value();
}

int MetricInputDialog::systolic() const
{
    return spinSystolic->value();
}

int MetricInputDialog::diastolic() const
{
    return spinDiastolic->value();
}

double MetricInputDialog::glucoseMgDl() const
{
    return spinGlucose->value();
}

void MetricInputDialog::updateFieldState()
{
    const MetricType type = metricType();

    spinWeight->setVisible(type == MetricType::Weight);
    spinSystolic->setVisible(type == MetricType::BloodPressure);
    spinDiastolic->setVisible(type == MetricType::BloodPressure);
    spinGlucose->setVisible(type == MetricType::Glucose);

    if (type == MetricType::Weight) {
        lblHint->setText("Ingresa el peso corporal en kilogramos.");
    } else if (type == MetricType::BloodPressure) {
        lblHint->setText("Ingresa los valores sistólico y diastólico en mmHg.");
    } else {
        lblHint->setText("Ingresa la glucosa en sangre en mg/dL.");
    }

    adjustSize();
}

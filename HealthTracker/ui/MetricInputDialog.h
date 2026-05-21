#ifndef METRICINPUTDIALOG_H
#define METRICINPUTDIALOG_H

#include <QDialog>
#include <QDateTime>

class QComboBox;
class QDateTimeEdit;
class QDoubleSpinBox;
class QSpinBox;
class QLabel;

class MetricInputDialog : public QDialog
{
    Q_OBJECT

public:
    enum class MetricType
    {
        Weight = 0,
        BloodPressure = 1,
        Glucose = 2
    };

    explicit MetricInputDialog(QWidget *parent = nullptr);

    MetricType metricType() const;
    QDateTime measuredAt() const;
    double weightKg() const;
    int systolic() const;
    int diastolic() const;
    double glucoseMgDl() const;

private slots:
    void updateFieldState();

private:
    QComboBox *comboMetric;
    QDateTimeEdit *dtMeasuredAt;

    QDoubleSpinBox *spinWeight;
    QSpinBox *spinSystolic;
    QSpinBox *spinDiastolic;
    QDoubleSpinBox *spinGlucose;

    QLabel *lblHint;
};

#endif // METRICINPUTDIALOG_H

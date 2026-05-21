#include "cuentas.h"

double Cuentas::calcularBMR(double peso, double altura, int edad, Sexo sexo)
{
    if (sexo == Sexo::Masculino) {
        return 10.0 * peso + 6.25 * altura - 5.0 * edad + 5.0;
    }
    return 10.0 * peso + 6.25 * altura - 5.0 * edad - 161.0;
}

double Cuentas::calcularTDE(double bmr, nivel_actividad actividad)
{
    switch (actividad) {
    case nivel_actividad::Bajo:
        return bmr * 1.2;
    case nivel_actividad::Moderado:
        return bmr * 1.55;
    case nivel_actividad::Alto:
        return bmr * 1.9;
    }
    return bmr;
}

double Cuentas::calcularMeta(double tde, objetivo obj)
{
    switch (obj) {
    case objetivo::Perder_peso:
        return tde - 500.0;
    case objetivo::Mantener_peso:
        return tde;
    case objetivo::Ganar_peso:
        return tde + 500.0;
    }
    return tde;
}

double Cuentas::calcularBMI(double peso, double altura)
{
    if (altura <= 0.0) {
        return 0.0;
    }
    return peso / ((altura / 100.0) * (altura / 100.0));
}

QString Cuentas::interpretarBMI(double bmi)
{
    if (bmi <= 0.0) {
        return "Sin datos";
    }
    if (bmi < 18.5) {
        return "Bajo peso";
    }
    if (bmi < 25.0) {
        return "Normal";
    }
    if (bmi < 30.0) {
        return "Sobrepeso";
    }
    return "Obesidad";
}

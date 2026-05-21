#ifndef CUENTAS_H
#define CUENTAS_H

#include <QString>

enum class Sexo
{
    Masculino,
    Femenino
};

enum class nivel_actividad
{
    Bajo,
    Moderado,
    Alto
};

enum class objetivo
{
    Perder_peso,
    Mantener_peso,
    Ganar_peso
};

class Cuentas
{
public:
    static double calcularBMR(double peso, double altura, int edad, Sexo sexo);
    static double calcularTDE(double bmr, nivel_actividad actividad);
    static double calcularMeta(double tde, objetivo obj);
    static double calcularBMI(double peso, double altura);
    static QString interpretarBMI(double bmi);
};

#endif

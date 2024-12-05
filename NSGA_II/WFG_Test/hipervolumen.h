#ifndef HIPERVOLUMEN_H
#define HIPERVOLUMEN_H


#include <vector> 
#include <functional> 
#include "estructuras.h" 

using estructuras::Individuo; 

double calcularHipervolumen(
    const std::vector<Individuo> &frente, 
    const std::vector<double> &referencia
    ); 


#endif
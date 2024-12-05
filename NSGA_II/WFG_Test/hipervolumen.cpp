#include "hipervolumen.h"
#include <algorithm>
#include <stdexcept>

double calcularHipervolumen(
    const std::vector<Individuo> & frente, 
    const std::vector<double> & referencia
)
{
    if (referencia.size() < 2) {
        throw std::invalid_argument("El vector de referencia debe tener al menos 2 elementos.");
    }
    std::vector<Individuo> frente_ordenado = frente; 

    std::sort(frente_ordenado.begin(), frente_ordenado.end(), []
        (const Individuo & a, const Individuo & b){
            if(a.evals[0] == b.evals[0]){
                return a.evals[0] < b.evals[0]; 
            }
            else{
                return a.evals[1] < b.evals[1]; 
            }
        }); 

    double hipervolumen = 0.0; 
    double ultimos_objetivos[2] = {0.0, 0.0}; 

    for(const Individuo & ind: frente_ordenado){
        double volumen = (referencia[0] - ind.evals[0]) * (referencia[1] - ind.evals[1]); 
        if(ind.evals[0] > ultimos_objetivos[0] && ind.evals[1] > ultimos_objetivos[1]){
            hipervolumen += volumen; 
        }

        ultimos_objetivos[0] = ind.evals[0]; 
        ultimos_objetivos[1] = ind.evals[1]; 
    }

    return hipervolumen; 
}
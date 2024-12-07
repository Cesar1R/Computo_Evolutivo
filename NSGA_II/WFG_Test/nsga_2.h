#ifndef NSGA_II 
#define NSGA_II 

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <random>
#include <set>
#include <fstream>
#include <limits>
#include <functional>
#include "estructuras.h" 

using namespace std;
using estructuras::Individuo; 
using estructuras::Funcion; 

#define MAX_DOUBLE  std::numeric_limits<double>::max()
#define MIN_DOUBLE  std::numeric_limits<double>::lowest()
#define INFINITY_DOUBLE std::numeric_limits<double>::infinity()

namespace funciones_opti{

    // Declaración de funciones
    int random_id(int n); 
    double randomperc(); 
    int signo(); 
    bool dominado(const Individuo& a, const Individuo& b, const bool& min);
    void imprimirPoblacion(const vector<Individuo>& poblacion);
    void calcular_FrentesyDistancias(vector<Individuo>& poblacion, const bool& min);  
    vector<Individuo> NSGAII(
        Funcion funcion_objetivo, 
        int n, //Tamaño de la poblacion
        int generaciones, 
        double probabilidad_mutacion, 
        double probabilidad_cruce, 
        bool min, 
        int cant_var, 
        vector<Individuo> &pob_init
        );

}
#endif  // NSGA_II

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

using namespace std;

#define MAX_DOUBLE  std::numeric_limits<double>::max()
#define MIN_DOUBLE  std::numeric_limits<double>::lowest()
#define INFINITY_DOUBLE std::numeric_limits<double>::infinity()

namespace funciones_opti{

    // Estructura para representar un Individuo
    typedef struct Individuo_struct {
        vector< double >  var;  // Variables 
        vector< double >  evals; // Evaluaciones 

        int n;  // Cantidad de variables
        int frente_dominancia; 
        int id_original;    
        double distancia_c; 

        // Constructores
        Individuo_struct(const vector<double>& vec);
        Individuo_struct();
        Individuo_struct(const vector<double>& vars, const vector<double>& eval_vec);
        Individuo_struct(const int n_); 
    } Individuo; 

    // Estructura para la función objetivo
    typedef struct Fun_struct {
        vector<double> lower_bounds; 
        vector<double> upper_bounds; 
        int M; //Cantidad objetivos 
        int k; 
        std::function<std::vector<double>(const std::vector<double>&, int, int)> func; 



        Fun_struct(
            const int M_, 
            const int k_, 
            std::function<std::vector<double>(const std::vector<double>&, int, int)> fun_parametro
            ); 

        vector<double> f(const vector<double>  z);
    } Funcion;

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
        int cant_var);
    double ran_db (double lower_bound, double upper_bound); 

}
#endif  // NSGA_II

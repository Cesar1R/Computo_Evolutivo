#ifndef ESTRUCTURAS_NSGA_II 
#define ESTRUCTURAS_NSGA_II 

#include <vector>
#include <functional>
#include <random>
using namespace std;

namespace estructuras {
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

    double ran_db (double lower_bound, double upper_bound); 

}

#endif
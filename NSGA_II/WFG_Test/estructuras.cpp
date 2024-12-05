#include "estructuras.h" 


namespace estructuras {

    Individuo::Individuo_struct(const vector<double> & vec):
        var(vec), n(vec.size()), frente_dominancia(0), id_original(0), distancia_c(0) {} 

    Individuo::Individuo_struct():
        n(0), frente_dominancia(0), id_original(0), distancia_c(0) {}

    Individuo::Individuo_struct(
        const vector<double> & vec, 
        const vector<double> & evaluaciones
        ):
        var(vec), evals(evaluaciones), 
        n(vec.size()), frente_dominancia(0), id_original(0), distancia_c(0) {}

    Individuo::Individuo_struct(const int n_): 
        n(n_), frente_dominancia(0), id_original(0), distancia_c(0)
    {
        for(int j = 0; j < n; j++){
            double lower_bound = 0.0, 
                upper_bound = 2.0 * (j  + 1); 
            var.push_back( ran_db(lower_bound, upper_bound) );   
        }
    }

    Funcion::Fun_struct(
        const int M_, 
        const int k_,
        std::function<std::vector<double>(const std::vector<double>&, int, int)> fun_parametro
        ):

        M(M_), k(k_),  func(fun_parametro) {}

    vector<double> Funcion::f(const vector<double>  z){
        return func(z, this-> k , this -> M);      
    }

    double ran_db (double lower_bound, double upper_bound){
        static random_device rd; 
        static mt19937 gen(rd()); 
        
        uniform_real_distribution<> dis(lower_bound, upper_bound); 
        return dis(gen); 
    }
}

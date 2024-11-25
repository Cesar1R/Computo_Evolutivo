/*
Algoritmo: Scan 2D

Sin entrada, este algoritmo tiene funciones que pueden crear puntos aleatorios en un rango y puntos no dominados, en el primer cuadrante, que se encuentran en una recta con pendiente -1. 

Retorna los puntos no dominados con enfoque en minimizacion 

Complejidad O(n log n)
*/

#include "tarea2.h"


using namespace std; 


typedef  struct punto2D {
    float x, y; 

    punto2D(float x1, float y1): x(x1), y(y1) {} 

    bool operator<(const punto2D&other) const{
        if(x != other.x) {return x < other.x;}
        else {return y < other.y;}  
    }

    bool dominado(const punto2D&other)const{
        return other.x <= x && other.y <= y && (other.x < x || other.y < y) ; 
    }

} Punto;

void gen_puntos(vector<Punto> &puntos, int cant_puntos){
    srand(time(0));
    float x, y, y_min,
        k = 5.0,  
        X_max = 100.0,
        Y_max = 100.0;

    for (int i  = 0; i < cant_puntos; i++){
        x = 0.1 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (X_max - 0.1)));
        y_min = -x + k; 
        y_min = max(y_min, 0.1f); 
        y = y_min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Y_max - y_min)));
        puntos.emplace_back(x, y); 
    }
}

void generarPuntosNoDominados2D(vector<Punto>& puntos, int cantidad) {

    std::srand(std::time(0));

    for (int i = 0; i < cantidad; ++i) {
        float x = rand() % 200000 + 1; 
        float y = -x + 200000;     

        if (x > 0 && y > 0) {
            puntos.push_back({x, y});
        } else {
            --i;  
        }
    }
}


int main(){


    vi arr = {10000, 20000, 30000, 40000, 50000}; //{10, 100, 1000, 1000000, 10000000}; 
    vector<double> resultados;  
    

    for(const int & cant_puntos: arr){
        vector<float> res_tmp; 
        for(int i = 0;  i < CANT_REPS; i++){ 

            vector<Punto> puntos, no_dominados; 

            generarPuntosNoDominados2D(puntos, cant_puntos); 


            //------------------Incio de algoritmo:Scan 2D-----------------
            auto start = chrono::high_resolution_clock::now();
            sort(puntos.begin(), puntos.end()); 
            float best_y = numeric_limits<float>::max();   

            for(const Punto & solucion: puntos){
                if(solucion.y < best_y){no_dominados.pb(solucion);} 
                best_y = min(best_y, solucion.y); 
            }

            auto end = chrono::high_resolution_clock::now();
            //---------------Finalizacion de algoritmo----------------
            

            chrono::duration<double> duration = end - start;

            res_tmp.pb(duration.count()); 

        }

        double promedio = [res_tmp](){
            return  static_cast<double>(accumulate(res_tmp.begin(), res_tmp.end(), 0.0)) / res_tmp.size();
        }(); 
        
        resultados.pb(promedio); 

    }

    imprimirResultados(arr, resultados); 

    return 0;
}
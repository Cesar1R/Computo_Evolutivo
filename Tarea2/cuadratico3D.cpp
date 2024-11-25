/*
Algoritmo: Naive/Directo

Se utilizan dos ciclos iterados para buscar un punto que domine al de la iteracion actual, si es detectado, se rompe el ciclo anidado. 

Sin entrada, este algoritmo tiene funciones que pueden crear puntos aleatorios en un rango y puntos no dominados, en el primer cuadrante, que se encuentran en un plano, que contiene a una con pendiente -1, en el primer y segundo eje

Retorna los puntos no dominados con enfoque en minimizacion 


Complejidad O(n^2)
*/


#include "tarea2.h"

using namespace std; 


typedef  struct punto3D {
    float x, y, z; 

    punto3D(float x1, float y1, float z1): x(x1), y(y1), z(z1) {} 

    bool operator<(const punto3D&other) const{
        if(x != other.x) {return x < other.x;}
        else if (y != other.y) {return y < other.y;}  
        else return z < other.z; 
    }

    bool dominado(const punto3D&other)const{
        return other.x <= x && other.y <= y  && other.z <=z 
                && (other.x < x || other.y < y || other.z < z); 
    }

} Punto;

void gen_puntos(vector<Punto> &puntos, int cant_puntos) {
    srand(time(0));
    float x, y, z, y_min, z_min, k = 5.0, X_max = 100.0, Y_max = 100.0, Z_max = 100.0;

    for (int i = 0; i < cant_puntos; i++) {
        x = 0.1 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (X_max - 0.1)));
        y_min = -x + k;
        y_min = max(y_min, 0.1f); 
        y = y_min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Y_max - y_min)));
        z_min = 0.1f;  
        z = z_min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Z_max - z_min)));

        puntos.emplace_back(x, y, z);
    }
}


void generarPuntosNoDominados3D(vector<Punto>& puntos, int cantidad) {
    std::srand(std::time(0));

   
    for (int i = 0; i < cantidad; ++i) {
        float x = rand() % 200000 + 1;  
        float z = -x + 200000;       

        
        float y = rand() % 200000 + 1; 

        
        if (x > 0 && y > 0 && z > 0) {
            puntos.push_back({x, y, z});
        } else {
            --i;  
        }
    }
}


int main(){


    vi arr   = {10000, 20000, 30000, 40000, 50000};
    vector<double> resultados;  
    

    for (const int & cant_puntos: arr){
        vector<float> res_tmp; 
        for(int i = 0; i < CANT_REPS; i++){

            vector<Punto> puntos, no_dominados; 

            generarPuntosNoDominados3D(puntos, cant_puntos); 


            //---------Inicio algoritmo: Naive3D---------------
            auto start = chrono::high_resolution_clock::now();

            for(const Punto& solucion: puntos){
                bool dominado = false;  
                for(const Punto & punto: puntos){
                    if(dominado){break;} 
                    dominado = solucion.dominado(punto); 
                }

                if(!dominado){no_dominados.pb(solucion);}
            }


            auto end = chrono::high_resolution_clock::now();

            //-------------Fin agoritmo---------------------

            chrono::duration<double> duration = end - start;

            res_tmp.pb(duration.count()); 

        }

        double promedio = [res_tmp](){
            return  static_cast<double>(accumulate(res_tmp.begin(), res_tmp.end(), 0.0)) / res_tmp.size();
        }(); 
        

        resultados.pb(promedio); 
    }

    imprimirResultados(arr,resultados); 


    return 0;
}
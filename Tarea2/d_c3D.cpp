/*
Algoritmo: Divide y venceras

Utiliza dos funciones principales
    -divide_conquer: que se enfoca en generar las particiones del vector para ordenar en orden logaritmico. 
    -merge: une las dos particiones separando aquellos puntos que sean dominados 

Sin entrada, este algoritmo tiene funciones que pueden crear puntos aleatorios en un rango y puntos no dominados, en el primer cuadrante, que se encuentran en una recta con pendiente -1. 

Retorna los puntos no dominados con enfoque en minimizacion 

Complejidad O(n log^2 n)
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



vector<Punto> merge(const vector<Punto> &left,  vector<Punto> & right){
    vector<Punto> noDom = left; 

    float x_min = FLT_MAX;  


    float best_y = FLT_MAX; 

    sort(right.begin(), right.end(), [](const Punto &a, const Punto&b){
        return (a.y != b.y)? a.y < b.y: a.z <= b.z; 
    });

    for(const Punto & p: right){
        if(p.y < best_y){
            noDom.pb(p);
        } 
        best_y = min(best_y, p.y); 
    }

    return noDom; 
}

vector<Punto> divide_conquer(const vector<Punto> & puntos){

    if(puntos.size() == 1){
        return puntos; 
    }

    size_t mid = puntos.size()/2; 

    vector<Punto> left (puntos.begin(), puntos.begin()+mid), 
                   right (puntos.begin() + mid, puntos.end()), 
                   noDomLeft = divide_conquer(left), 
                   noDomRight = divide_conquer(right);

    return merge(noDomLeft, noDomRight); 
}


int main(){

    ios_base::sync_with_stdio(0); 
    cout.tie(0); 

    vi reps = {10000, 20000, 30000, 40000, 50000};
    vector<double> resultados; 

    for(const int & cant_puntos: reps){ 
        vector<float> res_tmp; 
        for(int i = 0; i < 1; i++){

            vector<Punto> puntos; 

            generarPuntosNoDominados3D(puntos, cant_puntos); 

            //-------------Inicio del algoritmo-------------------------
                auto start = chrono::high_resolution_clock::now();
                sort(puntos.begin(), puntos.end()); 

                vector<Punto> no_dom = divide_conquer(puntos); 

                auto end = chrono::high_resolution_clock::now();
            //--------------Fin algoritmo-------------------------------
                chrono::duration<double> duration = end - start;

                res_tmp.pb(duration.count()); 
        }

        double promedio = [res_tmp](){
            return  static_cast<double>(accumulate(res_tmp.begin(), res_tmp.end(), 0.0)) / res_tmp.size();
        }(); 
        
        resultados.pb(promedio); 
    } 

    imprimirResultados(reps,resultados); 

    return 0; 
}
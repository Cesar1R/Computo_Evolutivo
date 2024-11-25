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

typedef struct punto2D {
    float x, y;

    punto2D(float x1, float y1) : x(x1), y(y1) {}

    bool operator<(const punto2D& other) const {
        if (x != other.x) { return x < other.x; }
        else { return y <= other.y; }
    }

    bool dominado(const punto2D& other) const {
        return other.x <= x && other.y <= y && (other.x < x || other.y < y);
    }

} Punto;

void gen_puntos(vector<Punto>& puntos, int cant_puntos) {

    float x, y, y_min,
        k = 5.0,  
        X_max = 100.0,
        Y_max = 100.0;

    for (int i = 0; i < cant_puntos; i++) {
        x = 0.1 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (X_max - 0.1)));
        y_min = -x + k;
        y_min = max(y_min, 0.1f);
        y = y_min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Y_max - y_min)));
        puntos.emplace_back(x, y);
    }
}

vector<Punto> merge(const vector<Punto> & left, vector<Punto> & right) {

    vector<Punto> noDom = left;

    float y_min = FLT_MAX; 

    for(size_t i = 0; i < left.size(); i++){
        y_min = min(y_min, left[i].y); 
    }

    size_t j = 0;

    sort(right.begin(), right.end(), [](const Punto & a, const Punto &b){
        return a.y > b.y; 
    }); 

    while(j < right.size()){
        if(right[j].y > y_min){
            j++; 
        }
        else{
            noDom.pb(right[j]);
            j++;  
        }
    }

    return noDom;

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

vector<Punto> divide_conquer(vector<Punto>& puntos) {
    if (puntos.size() == 1) {
        return puntos; 
    }

    size_t mid = puntos.size() / 2;
    vector<Punto> LeftPart(puntos.begin(), puntos.begin() + mid),
                  RightPart(puntos.begin() + mid, puntos.end()),
                  noDomLeft = divide_conquer(LeftPart),
                  noDomRight = divide_conquer(RightPart); 


    return merge(noDomLeft, noDomRight);
}

int main() {
    ios_base::sync_with_stdio(0); 
    cout.tie(0); 
    srand(time(0));
    //int reps[] = {10, 100, 1000, 1000000, 10000000}; 
    vi reps = {10000, 20000, 30000, 40000, 50000};
    vector<double> resultados;

    for (const int& cant_puntos : reps) {
        vector<float> res_tmp;
        for (int i = 0; i < CANT_REPS; i++) {
            vector<Punto> puntos;
            generarPuntosNoDominados2D(puntos, cant_puntos);

            //-----------------Inicio algoritmo-------------------------
            auto start = chrono::high_resolution_clock::now();

            sort(puntos.begin(), puntos.end());

            vector<Punto> no_dom = divide_conquer(puntos);

            auto end = chrono::high_resolution_clock::now();
            //-----------------Fin algoritmo-----------------------------
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

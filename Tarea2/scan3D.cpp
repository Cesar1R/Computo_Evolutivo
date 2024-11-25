/*
Para este algoritmo se utilizo la estructura SegmentTree para guardar el menor valor encontrado de la tecer entrada, en el rango [0, p.y], donde p.y representa la segunda entrada
Despues de ordenar los puntos por la primer entrada, se fueron recorriendo en orden creciente, en cada iteracion se utiliza el segment tree para determinar si es dominado o no, 
en caso de no ser dominado, se actualizaba el menor valor encontrado hasta p.y. 

Complejidad O(n log n)
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

class SegmentTree{
public: 
    SegmentTree(const vi & arr){
        n = arr.size(); 
        tree.resize(4 * n); 
        build(arr, 1, 0, n-1); 
    }

    int query(int left, int right){
        return queryUtil(1, 0, n-1, left, right); 
    }

    void update(int index, int value){
        updateUtil(1, 0, n-1, index, value); 
    }

private: 
    int n; 
    vi tree; 

    void build(const vi &arr, int node, int start, int end){
        if(start == end){
            tree[node] = arr[start]; 
        } 
        else{
            int mid = (start + end) / 2; 
            build(arr, 2 * node, start, mid); 
            build(arr, 2 * node + 1, mid + 1, end); 
            tree[node] = min(tree[2  * node], tree[2 * node + 1]); 
        }
    }

    int queryUtil(int node, int start, int end, int left, int right){
        if(left > end || right < start){
            return -1;
        }
        if(left <= start && end <= right){
            return tree[node]; 
        }

        int mid = (start + end)/ 2;
        int leftMin = queryUtil(2 * node, start, mid, left, right); 
        int rightMin = queryUtil(2 * node + 1, mid+1, end, left, right); 
        return min(leftMin, rightMin);  
    }

    void updateUtil(int node, int start, int end, int index, int value){
        if(start == end){
            tree[node] = value; 
        }
        else{
            int mid = (start + end) /2; 
            if(index <= mid){
                updateUtil(2 * node, start, mid, index, value);
            }
            else{
                updateUtil(2 * node +1, mid, end, index, value); 
            }
            tree[node] = min(tree[2 * node], tree[2 * node + 1]); 
        }
    }


};

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


void compresion_coordenadas(vector<Punto>& puntos, vector<map<float, int>*>& mapas) {
    set<float> x_cords, y_cords, z_cords; 

    map<float, int>* x_map = mapas[0], 
                   *y_map = mapas[1], 
                   *z_map = mapas[2]; 

    for (const Punto& p : puntos) {
        x_cords.insert(p.x); 
        y_cords.insert(p.y); 
        z_cords.insert(p.z); 
    }

    int new_cord = 0; 
    for (const float& cord : x_cords) {
        (*x_map)[cord] = new_cord++; 
    }

    new_cord = 0; 
    for (const float& cord : y_cords) {
        (*y_map)[cord] = new_cord++; 
    }

    new_cord = 0; 
    for (const float& cord : z_cords) {
        (*z_map)[cord] = new_cord++; 
    }

    for (Punto& p : puntos) {
        p.x = (*x_map)[p.x]; 
        p.y = (*y_map)[p.y]; 
        p.z = (*z_map)[p.z];
    }
}


int main(){

    vi arr = {10000, 20000, 30000, 40000, 50000};
    vector<double> resultados;  
    
    for (const int & cant_puntos: arr){
        vector<float> res_tmp; 
        cout << cant_puntos << endl; 

        for(int i = 0; i < 1; i++){ 
            vector<Punto> puntos, no_dominados; 


            generarPuntosNoDominados3D(puntos, cant_puntos); 

            sort(puntos.begin(), puntos.end()); 

            map<float, int> x_map, y_map, z_map; 
            vector<map<float, int>*> mapas = {&x_map, &y_map, &z_map}; 

            compresion_coordenadas(puntos, mapas); 

            //Cantidad de valores distintos encontrados para la segunda entrada
            int cantidad_valores_y = y_map.size(),
                f2, f3, curr_f3_val; 
            vector<int> valores_y (cantidad_valores_y+1, INT_MAX); 
            SegmentTree ST (valores_y);  

            //---------Inicio algoritmo: Scan3D---------------
            auto start = chrono::high_resolution_clock::now();
            for(const Punto & p : puntos){
                f2 = p.y; f3 = p.z; 
                curr_f3_val = ST.query(0, f2); 
                if(curr_f3_val > f3){
                    ST.update(f2, f3); 
                    no_dominados.pb(p); 
                }
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
    
    imprimirResultados(arr, resultados); 


    return 0; 
}
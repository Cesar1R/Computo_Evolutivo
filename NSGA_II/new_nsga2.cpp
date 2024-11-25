#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <ctime>
#include <algorithm>
#include <fstream> 
#include <set> 
#include <stdexcept> 

using namespace std;

#define MAX_DOUBLE  std::numeric_limits<double>::max()
#define MIN_DOUBLE  std::numeric_limits<double>::lowest()
#define INFINITY_DOUBLE std::numeric_limits<double>::infinity()
#define CALCULO_LIMS(minArg, maxArg, valor)  {minArg = std::min(minArg, valor); maxArg = std::max(maxArg, valor);}
#define pb push_back 


typedef pair<double, double> pdd; 


// Definición de la estructura Punto
struct Punto {
    double x1, x2, 
           f1, f2; 

    Punto(double x_1, double x_2, double ob1, double ob2): 
    x1(x_1), x2(x_2), f1(ob1), f2(ob2) {}

    Punto() : x1(0.0), x2(0.0), f1(0.0), f2(0.0) {}  

    Punto(const Punto & p)
    : x1(p.x1), x2(p.x2), f1(p.f1), f2(p.f2) {} 

    Punto& operator= (const Punto& otro){
        if(this != &otro){
            this -> x1 = otro.x1;
            this -> x2 = otro.x2; 
            this -> f1 = otro.f1; 
            this -> f2 = otro.f2;  
        }

        return *this; 
    }
    
}; 

// Función de ejemplo para obtener f1 y f2
struct Funcion {
    double x_min, x_MAX, y_min, y_MAX;

    Funcion(double xMin, double xMAX, double yMin, double yMAX)
        : x_min(xMin), x_MAX(xMAX), y_min(yMin), y_MAX(yMAX) {}

    // Método de evaluación de la función (f1, f2)
    pdd f(double x1, double x2) const {
        double f1 = 1.1 - x1;
        double f2 = 60 - (1 + x2 / x1);
        return make_pair(f1, f2);
    }
};

vector<pair<int, double>> get_crowding_distances (vector<Punto> & poblacion);

void crossover(vector<Punto> &hijos, vector<Punto> &padres, double pcross, const Funcion &fun); 

void mutate(Punto &p, const double &pmut, const Funcion &fun);

int gend_id_aleatorio(int n);

vector<Punto> get_matin (vector<Punto> & poblacion, const bool & minimizar);

vector<vector<Punto>> clasificacion_por_frentes(vector<Punto> &poblacion, const bool & minimizar); 

vector<Punto> seleccion_por_frentes (vector<Punto> & poblacion, const bool & minimizar, const int & pob_size);

double randomperc(); 

bool dominado (const Punto & a, const Punto & b, const bool & minimizar);

vector<Punto> get_noDom(vector<Punto> &puntos);


// Función para generar valores aleatorios entre 0 y 1
double randomperc() {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

void crossover(vector<Punto> &hijos, vector<Punto> &padres, double pcross, const Funcion &fun) {
    int pop_size = padres.size();

    for (int i = 0; i < pop_size / 2; ++i) {

        if(2*i  >= pop_size || 2 * i +1 >= pop_size){
            continue; 
        }

        Punto &padre1 = padres[2 * i];
        Punto &padre2 = padres[2 * i + 1];
        
        Punto &child1 = hijos[2 * i];
        Punto &child2 = hijos[2 * i + 1];

        if (randomperc() < pcross) {
            if (randomperc() < 0.5) {
                child1.x1 = padre1.x1;
                child1.x2 = padre2.x2;
                child2.x1 = padre2.x1;
                child2.x2 = padre1.x2;
            } else {
                child1 = padre1;
                child2 = padre2;
            }

            pdd evals_child1 = fun.f(child1.x1, child1.x2);
            pdd evals_child2 = fun.f(child2.x1, child2.x2);
            child1.f1 = evals_child1.first;
            child1.f2 = evals_child1.second;
            child2.f1 = evals_child2.first;
            child2.f2 = evals_child2.second;
        } else {
            child1 = padre1;
            child2 = padre2;
        }
    }
}

bool dominado(const Punto &a, const Punto &b, const bool &minimizar) {
    if (minimizar) {
        return (a.f1 <= b.f1 && a.f2 <= b.f2 && (a.f1 < b.f1 || a.f2 < b.f2));
    } else {
        return (a.f1 >= b.f1 && a.f2 >= b.f2 && (a.f1 > b.f1 || a.f2 > b.f2));
    }
}


void mutate(Punto &p, const double &pmut, const Funcion &fun) {
    double rand = randomperc(); 
    if (rand <= pmut) {
        double delta = (0.5 - rand) * (fun.x_MAX - fun.x_min) * 0.1;
        p.x1 += delta;
        p.x1 = min(max(p.x1, fun.x_min), fun.x_MAX); // Respetar los límites de x1

        delta = (0.5 - rand) * (fun.y_MAX - fun.y_min) * 0.1;
        p.x2 += delta;
        p.x2 = min(max(p.x2, fun.y_min), fun.y_MAX); // Respetar los límites de x2
    }

    pdd evals = fun.f(p.x1, p.x2);
    p.f1 = evals.first;
    p.f2 = evals.second;
}


//Funcion que retorna el frente no dominado por medio del algoritmo scan 2D 
vector<Punto> get_noDom(vector<Punto> &puntos){
    sort(puntos.begin(), puntos.end(), [](const Punto & a, const Punto & b){
        if(a.f1 != b.f1) {return a.f1 < b.f1;} 
        else {return a.f2 < b.f2;} 
    });

    double best_y = MAX_DOUBLE; 

    vector<Punto> noDom(puntos.size()); 
    for(const Punto & solucion: puntos){
        if(solucion.f2 < best_y){noDom.pb(solucion);}
        best_y = min(best_y, solucion.f2); 
    }

    return noDom; 

}

// Función para realizar un torneo binario aleatorio
int gend_id_aleatorio(int n) {
    static random_device rd; 
    static mt19937 gen(rd()); 

    uniform_int_distribution<> distrib(0, n - 1); 
    return distrib(gen); 
}

vector<pair<int, double>> get_crowding_distances (vector<Punto> & poblacion){

    const int n = poblacion.size(); 
    
    //Mapa que para cada indice original, como clave, tendra la distancia como valor
    map<int, double> map_distancias;

    //Vectores que almacenaran la relacion entre el indice original y la distancia de crowding
    vector<pair<int, double>> distances_f1, 
                              distances_f2;
        distances_f1.reserve(n);
        distances_f2.reserve(n); 

    double max_f1 = MIN_DOUBLE, min_f1 = MAX_DOUBLE, 
           max_f2 = MIN_DOUBLE, min_f2 = MAX_DOUBLE; 

    size_t i; 
    //Calculo de los valores minimo y maximo para cada funcion objetivo 
    for(i = 0; i < poblacion.size(); i++){
        CALCULO_LIMS(min_f1, max_f1, poblacion[i].f1)
        CALCULO_LIMS(min_f2, max_f2, poblacion[i].f2)
        distances_f1[i] = {i, poblacion[i].f1}; 
        distances_f2[i] = {i, poblacion[i].f2};
        map_distancias[i] = 0.0;  
    }

    double const_f1 = max(max_f1, max_f1 - min_f1),
           const_f2 = max(max_f2, max_f2 - min_f2); 
    
    //Ordenamiento descendente en cada funcion objetivo 
    sort(distances_f1.begin(), distances_f1.end(), [](const pair<int, double> & a, const pair<int, double> & b){
        return a.second <= b.second; 
    });

    sort(distances_f2.begin(), distances_f2.end(), [](const pair<int, double> & a, const pair<int, double> & b){
        return a.second <= b.second; 
    });

    //Asignacion de infinito a los extremos
    distances_f1[0].second = INFINITY_DOUBLE; distances_f1[n-1].second = INFINITY_DOUBLE; 
    distances_f2[0].second = INFINITY_DOUBLE; distances_f2[n-1].second = INFINITY_DOUBLE;

 
    for(i = 1; i < n-1; i++){
        map_distancias[distances_f1[i].first] += (distances_f1[i+1].second - distances_f1[i-1].second)/const_f1; 
        map_distancias[distances_f2[i].first] += (distances_f2[i+1].second - distances_f2[i-1].second)/const_f2; 
    } 

    vector<pair<int, double>> vec_distances(n);  

    for(const pair<int, double> & p_map: map_distancias){
        vec_distances.pb(p_map); 
    }

    sort(vec_distances.begin(), vec_distances.end(), [](const pair<int, double> & a, const pair<int, double> & b){
        return a.second > b.second; 
    });
         
    return vec_distances; 
}

// Función para obtener el vector de selección de padres basado en el torneo binario
vector<Punto> get_matin (vector<Punto> & poblacion, const bool & minimizar){

    vector<vector<Punto>> frentes = clasificacion_por_frentes(poblacion, minimizar); 

    //Vector que guarda la poblacion ordenada por frentes
    int n = poblacion.size(); 
    vector<Punto> pob (n); 

    //Mapa que para cada clave: indice del vector de poblacion, 
    //      guarda el el valor: frente al que pertenece 
    map<int, int> frente_map;  
    int k = 0, m; 
    size_t i, j; 
    for(i = 0; i < frentes.size(); i++){ // i:  frente actual 
        m = frentes[i].size(); 

        for(j = 0; j < m; j++){ // j: elemento del frente actual en el indice j
            pob.pb(frentes[i][j]); 
            frente_map[j] = i; //Asignamos el frente i al indice j 

        }
    } 


    vector<Punto> mating;
        mating.reserve(n);  

    for(i = 0; i < n; i++){

        int id1 = gend_id_aleatorio(n), 
            id2 = gend_id_aleatorio(n), 
            winner; 

        //Si los elementos se encuentran en diferentes frentes, elegimos el del menor frente
        if(frente_map[id1] != frente_map[id2]){
            winner = (frente_map[id1] > frente_map[id2]) ? id2 : id1; 
        }
        else{ //En caso de que pertenezcan al mismo frente:
            //Obtenemos las distancias crowding de todos los elementos de la poblacion, el arreglo esta ordenado de mayor a menor
            vector<pair<int, double>> distancias = get_crowding_distances(pob); 

            //Elegimos aquel que tenga menor indice dentro del arreglo de distancias
            for(i = 0; i < distancias.size(); i++){
                if(distancias[i].first == id1){
                    winner = id1; 
                    break;
                }
                if(distancias[i].first == id2){
                    winner = id2; 
                    break; 
                }
            }
        }
        mating.pb(pob[winner]); 
    } 

    return mating; 
}


vector<vector<Punto>> clasificacion_por_frentes (vector<Punto> & poblacion, const bool & minimizar){
    int n = poblacion.size(); 

    bool dominancia_matrix [n][n] ={{false}}; 
    //dominancia_matrix[i][j] = el elemento i es dominado por el elemento j 
    size_t i, j; 
    for(i = 0; i < poblacion.size(); i++){
        for(j =0; j < poblacion.size(); j++){
            dominancia_matrix[i][j] = dominado(poblacion[j], poblacion[i], minimizar); 
        }
    }

    int valores_asingados = 0, k, frente_val = 0; 
    vector<vector<Punto>> frentes; 

    set<int> id_contados; 

    while (valores_asingados < n){
        vector<Punto> frente;
            frente.reserve(n); 

        
        for(i = 0; i < poblacion.size(); i++){
            //Para evitar un doble conteo, utilizamos el set para guardar aquellso elementos
            //que ya han sido agregados a su respectivo
            if(id_contados.find(i) != id_contados.end()){
                continue;  
            }

            //Contabilizamos los elementos que dominan al elemento j
            k = 0;
            for(j = 0; j < poblacion.size(); j++){
                k += dominancia_matrix[i][j]; 
            }

            if(k == 0){
                id_contados.insert(i); 
                valores_asingados++; 
                frente.pb(poblacion[i]); 
                for(j = 0; j < poblacion.size(); j++){
                    dominancia_matrix[j][i] = false;  
                } 
            }
        }
        if(!frente.empty()){
            frentes.pb(frente);
        }
        frente_val++; 
    }

    return frentes; 

}


vector<Punto> seleccion_por_frentes (vector<Punto> & poblacion, const bool & minimizar, const int & pob_size){

    vector<vector<Punto>> frentes = clasificacion_por_frentes(poblacion, minimizar); 

    int n = pob_size; 

    vector<Punto> nueva_poblacion;
        nueva_poblacion.reserve(n);  


    int k = 0, i; 
    while(nueva_poblacion.size() < n){
        //Si el frente actual cabe completo en la nueva poblacion, lo asignamos 
        if(frentes[k].size() < n - nueva_poblacion.size()){
            nueva_poblacion.insert(nueva_poblacion.end(), frentes[k].begin(), frentes[k].end()); 
        }
        else{ // En caso de que se requieran menos inividuos, utilizamos los que tengan mayor distancia de crowding

            vector<pair<int, double>> distancias_vector = get_crowding_distances(frentes[k]); 
            //Se incluyen los elementos del frente actual hasta que se complete la cantidad de la nueva poblacion
            
            size_t j, 
                    cant_restante = n - nueva_poblacion.size();
    
            for(j = 0; j < min(cant_restante, frentes[k].size()); j++){
                nueva_poblacion.pb(frentes[k][distancias_vector[j].first]);  
            }

        }
        k++; 
    }



    return nueva_poblacion; 
    
}

int main() {
    srand(time(nullptr));
    
    // Parámetros del algoritmo
    int pob_size = 100;  // Tamaño de la población
    double pcross = 0.9;  // Probabilidad de cruce
    double pmut = 0.5;   // Probabilidad de mutación
    int generaciones = 100;  // Número de generaciones
    bool minimizar = true; 


    double min_x1 = 0.1, max_x1 = 1, 
           min_x2 = 0, max_x2 = 5; 

    // Crear función
    Funcion fun(min_x1, max_x1, min_x2, max_x2);   // Rango de x1, x2

    // Generar población inicial
    vector<Punto> poblacion;
    poblacion.reserve(pob_size+1);
    for (int i = 0; i < pob_size; i++) {
        double x1 = fun.x_min + (fun.x_MAX - fun.x_min) * randomperc();
        double x2 = fun.y_min + (fun.y_MAX - fun.y_min) * randomperc();


        pdd f = fun.f(x1, x2);
        poblacion.push_back(Punto(x1, x2, f.first, f.second));
    }

    // Ciclo de generaciones
    for (int gen = 0; gen < generaciones; gen++) {
        try{
            cout << "Gen: " << gen << endl;
            // Obtener el vector de mating basado en el torneo binario
            cout << "->Mating" << endl;
            vector<Punto> mating_vector = get_matin(poblacion, minimizar); 

            cout << "->Crossover" << endl; 
            // Realizar cruce con el vector de mating
            vector<Punto> nueva_poblacion = poblacion;
            crossover(nueva_poblacion, mating_vector, pcross, fun);

            cout << "->Mutacion" << endl; 
            // Aplicar mutación
            for (auto &p : nueva_poblacion) {
                mutate(p, pmut, fun);  // Mutación con el rango adecuado
            }

            cout << "->Combinacion de poblacion" << endl; 
            // Combinar padres e hijos  
            
            if(poblacion.size() > pob_size){poblacion.resize(pob_size);}
            if(nueva_poblacion.size() > pob_size){nueva_poblacion.resize(pob_size);}

            vector<Punto> poblacion_combinada = poblacion;
            
            poblacion_combinada.insert(poblacion_combinada.end(), nueva_poblacion.begin(), nueva_poblacion.end());

            if(poblacion_combinada.size() > 2 * pob_size){poblacion_combinada.resize(2 * pob_size); }

            cout << "->Seleccion por frentes" << endl;
            // Seleccionar la nueva población por frentes de Pareto
            poblacion = seleccion_por_frentes(poblacion_combinada, minimizar, pob_size);

            if (gen % 10 == 0) {
                cout  << (gen * 100.0 / generaciones) << "%" << endl;
            }
        } catch (const exception & e){
            cerr << "Error: " << e.what() << endl; 
            return 1; 
        }
    }


    vector<vector<Punto>> frentes = clasificacion_por_frentes(poblacion, minimizar); 

    vector<Punto> soluciones_noDominadas = frentes[0];  

    cout << "Resultado final:" << endl;
    int k = 0;
    //for (const auto &p : soluciones_noDominadas) {
      //  cout << " ( " << p.f1 << "," << p.f2 << ")"<< endl; 
    //}
    
    ofstream outFile("resultados.txt"); 

    if(outFile.is_open()){
        outFile << "[ "; 
        for(const Punto & p: soluciones_noDominadas){
            outFile << " (" << p.f1 << "," << p.f2 << "), "; 
        }

        outFile << " ]"; 
    }
    else{
        cout << "No se pudo abrir el archivo correctamente" << endl; 
    }

    return 0;
}

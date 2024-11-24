#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <ctime>
#include <algorithm>
#include <fstream> 
#include <set> 

using namespace std;

#define MAX_DOUBLE   std::numeric_limits<double>::max()
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
        return make_pair(1.1 - x1, 60 - (1 + x2 / x1));
    }
};



map<int, double> get_crowding_distances(vector<Punto>& poblacion); 

void crossover(vector<Punto> &hijos, vector<Punto> &padres, double pcross, const Funcion &fun); 

void mutate(Punto &p, const double &pmut, const double &min_val_f1, const double &max_val_f1, const double &min_val_f2, const double &max_val_f2, const Funcion &fun);

int gend_id_aleatorio(int n);

vector<Punto> crown_sorting(const vector<Punto> & puntos);

vector<Punto> get_mating(vector<Punto>& poblacion);

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

bool dominado (const Punto & a, const Punto & b, const bool & minimizar){
    if(minimizar){
        return (b.f1 <= a.f1 && b.f2 <= a.f2 && (b.f1 < a.f1 || b.f2 < b.f2)); 
    }
    else{
        return (a.f1 <= b.f1 && a.f2 <= b.f2 && (a.f1 < b.f1 || a.f2 < b.f2)); 
    }
}

void mutate(Punto &p, const double &pmut, const double &min_val_f1, const double &max_val_f1, const double &min_val_f2, const double &max_val_f2, const Funcion &fun) {
    if (randomperc() <= pmut) {
        double delta = (randomperc() - 0.5) * (max_val_f1 - min_val_f1) * 0.1;
        p.x1 += delta;
        p.x1 = min(max(p.x1, min_val_f1), max_val_f1);

        delta = (randomperc() - 0.5) * (max_val_f2 - min_val_f2) * 0.1;
        p.x2 += delta;
        p.x2 = min(max(p.x2, min_val_f2), max_val_f2);
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

    vector<Punto> noDom; 
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


vector<Punto> crown_sorting(const vector<Punto> & puntos){
    int n = puntos.size(), i; 

    //Vector auxiliar que guardara el indice y la distancia de crowding
    vector<pair<int, double>> distancia_crowding(n);

    for(i = 0; i < n; i++){
        distancia_crowding[i] = {i, 0.0}; 
    } 
    
    //Asignamos a infinito los crown values en los extremos
    distancia_crowding[0].second = INFINITY_DOUBLE; 
    distancia_crowding[n-1].second = INFINITY_DOUBLE; 

    double f1_max = MIN_DOUBLE, f1_min = MAX_DOUBLE,
           f2_max = MIN_DOUBLE, f2_min = MAX_DOUBLE; 

    for(i = 0; i < n; i++){
        CALCULO_LIMS(f1_min, f1_max, puntos[i].f1); 
        CALCULO_LIMS(f2_min, f2_max, puntos[i].f2); 
    }

    //Ordenamiento de los puntos por f1 descendente 
    vector<Punto> puntos_ord = puntos; 
    sort(puntos_ord.begin(), puntos_ord.end(), [](const Punto &a, const Punto &b){
        return a.f1 > b.f1; 
    });

    // Constantes que utilizaremos durante el calculo de la distancai de crowding
    double const_f1 = f1_max - f1_min, 
           const_f2 = f2_max - f2_min; 

    for(i = 1; i < n-1; i++){     //Calculo de f1  + Calculo de f2
        //En este caso, tenemos que M=2
        distancia_crowding[i].second = (puntos[i+1].f1 - puntos[i-1].f1)/const_f1 
                                     + (puntos[i+1].f2 - puntos[i-1].f2)/const_f2; 
    }

    sort(distancia_crowding.begin(), distancia_crowding.end(), 
        [](const pair<int,double> & a, const pair<int, double> & b){
            return a.second > b.second; 
    });

    //Vector que contendra los puntos ordenados por la distancia de crowding
    vector<Punto> sorted_points; 
    for(const pair<int, double> & pt: distancia_crowding){
        sorted_points.push_back(puntos[pt.first]); 
    }
    
    return sorted_points; 
}

map<int, double>  get_crowding_distances(vector<Punto> & puntos){
    int n = puntos.size(), i;  

    vector<pair<int, double>> distancia_crowding (n); 
    for(i = 0; i < n; i++){
        distancia_crowding[i] = {i, 0.0};   
    }

    distancia_crowding[0].second = INFINITY_DOUBLE; 
    distancia_crowding[n-1].second = INFINITY_DOUBLE; 

    double f1_max = MIN_DOUBLE, f1_min = MAX_DOUBLE, 
           f2_max = MIN_DOUBLE, f2_min = MAX_DOUBLE; 

    for(i = 1; i < n - 1; i++){
        CALCULO_LIMS(f1_min, f1_max, puntos[i].f1); 
        CALCULO_LIMS(f2_min, f2_max, puntos[i].f2); 
    }

    vector<Punto> sorted_puntos = puntos; 

    sort(sorted_puntos.begin(), sorted_puntos.end(), [](const Punto & a, const Punto &b){
        return a.f1 > b.f1;
    });

    double const_f1 = f1_max - f1_min, 
           const_f2 = f2_max - f2_min; 
    for(i = 1; i < n-1; i++){
        distancia_crowding[i].second = (sorted_puntos[i+1].f1 - sorted_puntos[i-1].f1)/ const_f1 
                                + (sorted_puntos[i + 1].f2 - sorted_puntos[i - 1].f2) /const_f2; 
    }

    map<int, double> mapa_distancias; 

    for(const pair<int, double> & dist : distancia_crowding){
        mapa_distancias[dist.first] = dist.second; 
    }

    return mapa_distancias; 
}

// Función para obtener el vector de selección de padres basado en el torneo binario
vector<Punto> get_mating(vector<Punto>& poblacion) {
    map<int, double> map_distancias = get_crowding_distances(poblacion); 
    int n = poblacion.size(), i, id1, id2;
    vector<Punto> mating_vector(n);
    
    for(i = 0; i < n; i++) {
        id1 = gend_id_aleatorio(n);  
        id2 = gend_id_aleatorio(n); 

        if(map_distancias[id1] > map_distancias[id2]) {
            mating_vector[i] = poblacion[id1];
        } else {
            mating_vector[i] = poblacion[id2]; 
        }
    }

    return mating_vector;  
}


vector<vector<Punto>> clasificacion_por_frentes (vector<Punto> & poblacion, const bool & minimizar){
    int n = poblacion.size(); 

    cout << "--------------Clasificacion por frentes ----------" << endl; 
    cout << "n:" << n << endl; 
    bool dominancia_matrix [n][n] ={{false}}; 
    //dominancia_matrix[i][j] = el elemento i domina al elemento j 
    size_t i, j, l; 
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
        
        for(i = 0; i < poblacion.size(); i++){
            if(id_contados.find(i) != id_contados.end()){
                continue;  
            }

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
        cout << "Frente: " << frente_val << " size: " << frente.size() << endl;  
        cout << "Vals asignados: " << valores_asingados << endl;
        if(!frente.empty()){
            frentes.pb(frente);
        }
        frente_val++; 
    }

    return frentes; 

}


vector<Punto> seleccion_por_frentes (vector<Punto> & poblacion, const bool & minimizar, const int & pob_size){
    cout << "----------Seleccion por frentes ---------------" << endl; 
    cout << "Clasificacion por frentes" << endl; 
    vector<vector<Punto>> frentes = clasificacion_por_frentes(poblacion, minimizar); 

    int n = pob_size; 

    vector<Punto> nueva_poblacion; 


    cout << "Asignacion de los frentes" << endl; 
    int k = 0, i; 
    while(nueva_poblacion.size() < n){
        cout << "nueva_poblacion.size: " << nueva_poblacion.size() << endl; 
        cout << "Frente: " << k << " Size frentes[k]" << frentes[k].size() << endl;
        if(frentes[k].size() < n - nueva_poblacion.size()){
            nueva_poblacion.insert(nueva_poblacion.end(), frentes[k].begin(), frentes[k].end()); 
        }
        else{
            cout << "Entra else" << endl; 
            vector<pair<int, double>> distancia_crowding (n); 
            for(i = 0; i < n; i++){
                distancia_crowding[i] = {i, 0.0};   
            }

            distancia_crowding[0].second = INFINITY_DOUBLE; 
            distancia_crowding[n-1].second = INFINITY_DOUBLE; 

            double f1_max = MIN_DOUBLE, f1_min = MAX_DOUBLE, 
                f2_max = MIN_DOUBLE, f2_min = MAX_DOUBLE; 

            for(i = 1; i < n - 1; i++){
                CALCULO_LIMS(f1_min, f1_max, poblacion[i].f1); 
                CALCULO_LIMS(f2_min, f2_max, poblacion[i].f2); 
            }

            vector<Punto> sorted_puntos = poblacion; 

            sort(sorted_puntos.begin(), sorted_puntos.end(), [](const Punto & a, const Punto &b){
                return a.f1 > b.f1;
            });

            double const_f1 = f1_max - f1_min, 
                    const_f2 = f2_max - f2_min; 
            for(i = 1; i < n-1; i++){
                distancia_crowding[i].second = (sorted_puntos[i+1].f1 - sorted_puntos[i-1].f1)/ const_f1 
                                        + (sorted_puntos[i + 1].f2 - sorted_puntos[i - 1].f2) /const_f2; 
            }

            sort(distancia_crowding.begin(), distancia_crowding.end(), [](const pair<int, double> &a, const pair<int, double> &b){
                return a.second > b.second; 
            });

            cout << "Inicia proceso de asignacion" << endl; 
            size_t cant_restante = n - nueva_poblacion.size(); 

            cout << "Se agregaran: " << min(cant_restante, frentes[k].size()) << endl; 
            for(i = 0; i < min(cant_restante, frentes[k].size()); i++){
                nueva_poblacion.pb(frentes[k][i]); 
            }
            cout << "Sale else" << endl; 
        }

        k++; 
    }

    cout << "Termina while" << endl; 

    return nueva_poblacion; 
    
}

int main() {
    srand(time(nullptr));
    
    // Parámetros del algoritmo
    int pob_size = 100;  // Tamaño de la población
    double pcross = 0.9;  // Probabilidad de cruce
    double pmut = 0.5;   // Probabilidad de mutación
    int generaciones = 10;  // Número de generaciones
    bool minimizar = true; 


    // Crear función
    Funcion fun(0.1, 1, 0, 5);   // Rango de x1, x2

    // Generar población inicial
    vector<Punto> poblacion;
    for (int i = 0; i < pob_size; i++) {
        double x1 = fun.x_min + (fun.x_MAX - fun.x_min) * randomperc();
        double x2 = fun.y_min + (fun.y_MAX - fun.y_min) * randomperc();
        pdd f = fun.f(x1, x2);
        poblacion.push_back(Punto(x1, x2, f.first, f.second));
    }

    cout << "inicio del ciclo" << endl; 
    // Ciclo de generaciones
    for (int gen = 0; gen < generaciones; gen++) {

        cout << "Gen: " << gen << endl;
        // Obtener el vector de mating basado en el torneo binario
        vector<Punto> mating_vector = get_mating(poblacion);

        cout << "Nueva poblacion" << endl; 
        // Realizar cruce con el vector de mating
        vector<Punto> nueva_poblacion = poblacion;
        crossover(nueva_poblacion, mating_vector, pcross, fun);

        cout << "Mutacion" << endl; 
        // Aplicar mutación
        for (auto &p : nueva_poblacion) {
            mutate(p, pmut, fun.x_min, fun.x_MAX, fun.y_min, fun.y_MAX, fun);  // Mutación con el rango adecuado
        }

        cout << "Combinacion de las poblaciones" << endl; 
        // Combinar padres e hijos  
        vector<Punto> poblacion_combinada = poblacion;
        poblacion_combinada.insert(poblacion_combinada.end(), nueva_poblacion.begin(), nueva_poblacion.end());

        cout << "Seleccion por frentes" << endl; 
        // Seleccionar la nueva población por frentes de Pareto
        poblacion = seleccion_por_frentes(poblacion_combinada, minimizar, pob_size);


        // Opcional: Mostrar el progreso de la evolución
        if (gen % 10 == 0) {
            cout << "Generacion " << gen << " - Poblacion: " << poblacion.size() << " individuos." << endl;
  
        }
    }


    vector<vector<Punto>> frentes = clasificacion_por_frentes(poblacion, minimizar); 

    vector<Punto> soluciones_noDominadas = frentes[0];  

    cout << "Resultado final (última generacion):" << endl;
    int k = 0;
    for (const auto &p : soluciones_noDominadas) {
        cout << " ( " << p.f1 << "," << p.f2 << ")"<< endl; 
    }

    
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

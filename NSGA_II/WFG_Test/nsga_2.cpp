#include "nsga_2.h" 


namespace funciones_opti{ 
        
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

    int random_id(int n) {
        static random_device rd;
        static mt19937 gen(rd());

        uniform_int_distribution<> distrib(0, n - 1);
        return distrib(gen);
    }

    double randomperc() {
        static random_device rd;
        static mt19937 gen(rd());
        uniform_real_distribution<> dis(0.0, 1.0);
        return dis(gen);
    }

    int signo() {
        static random_device rd;
        static mt19937 gen(rd());
        uniform_int_distribution<> distrib(0, 1);
        return distrib(gen);
    }

    // Función que determina si 'a' domina a 'b'
    bool dominado(const Individuo &a, const Individuo &b, const bool &min) {
        int n = a.evals.size();  

        bool flag1 = true, flag2 = false;

        //Minimizacion
        if (min) {  
            for (int i = 0; i < n; i++) {
                flag1 = flag1 && (a.evals[i] <= b.evals[i]);   
                flag2 = flag2 || (a.evals[i] < b.evals[i]);   
            }
            return flag1 && flag2;
        }

        //Maximización
        for (int i = 0; i < n; i++) {
            flag1 = flag1 && (a.evals[i] >= b.evals[i]);   
            flag2 = flag2 || (a.evals[i] > b.evals[i]);   
        }

        return flag1 && flag2;
    }


    void calcular_FrentesyDistancias(vector<Individuo> & poblacion, const bool & min){
        try{
            size_t n = poblacion.size();

            // Matriz que almacena si el elemento i domina al elemento j
            vector<vector<bool>> dominancia (n, vector<bool> (n, false));

            size_t i, j;

            // Llenado de la matriz de dominancia
            for(i = 0; i < n; i++){
                poblacion[i].id_original = i;
                for(j = 0; j < n; j++){
                    dominancia[i][j] = dominado(poblacion[i], poblacion[j], min);
                }
            }

            set<int>  id_utlizados;
            vector<Individuo> curr_frente;
            vector<vector<Individuo>> frentes;
            int k = 0, cont;

            while(id_utlizados.size() < n ){
                curr_frente.clear();
                for(i = 0; i < n; i++){
                    if(id_utlizados.find(i) != id_utlizados.end()){
                        continue; 
                    }

                    cont = 0;
                    for(j = 0; j < n; j++){
                        cont += dominancia[j][i];
                    }

                    if(cont == 0){
                        id_utlizados.insert(i);
                        poblacion[i].frente_dominancia = k;
                        curr_frente.push_back(poblacion[i]);

                        for(j = 0; j < n; j++){
                            dominancia[i][j] = false;
                        }
                    }
                }

                if(!curr_frente.empty()){
                    frentes.push_back(curr_frente);
                    k++;
                }
            }

            // Cálculo de distancias de crowding
            for(vector<Individuo> & frente: frentes){
                vector<pair<int, double>> dist_f1(frente.size()),
                                        dist_f2(frente.size());

                double f1_min = MAX_DOUBLE, f1_max = MIN_DOUBLE, f2_min = MAX_DOUBLE, f2_max = MIN_DOUBLE;

                // Asignación de los valores de f1 y f2 a vectores auxiliares
                for(i = 0; i < frente.size(); i++){
                    double f1 = frente[i].evals[0]; 

                    dist_f1[i] = {i, f1};
                    f1_min = std::min(f1, f1_min);
                    f1_max = std::max(f1_max, f1);

                    double f2 = frente[i].evals[1]; 

                    dist_f2[i] = {i, f2};
                    f2_min = std::min(f2, f2_min);
                    f2_max = std::max(f2_max, f2);
                }

                // Ordenamiento independiente de cada uno de los vectores auxiliares
                sort(dist_f1.begin(), dist_f1.end(), [](const pair<int, double> & a, const pair<int, double> & b){
                    return a.second < b.second;
                });
                sort(dist_f2.begin(), dist_f2.end(), [](const pair<int, double> & a, const pair<int, double> & b){
                    return a.second < b.second;
                });

                vector<double> distancias (frente.size(), 0.0);

                double const_f1 = max(f1_max - f1_min, 0.0000001),
                    const_f2 = max(f2_max - f2_min, 0.0000001);


                // Almacenamiento de la distancia acumulada utilizando el índice original
                for(i = 1; i < frente.size() - 1; i++){
                    distancias[dist_f1[i].first] += (dist_f1[i+1].second - dist_f1[i-1].second) / const_f1;
                    distancias[dist_f2[i].first] += (dist_f2[i+1].second - dist_f2[i-1].second) / const_f2;
                }

                dist_f1[0].second = INFINITY_DOUBLE;  dist_f1[frente.size()-1].second = INFINITY_DOUBLE; 
                dist_f2[0].second = INFINITY_DOUBLE; dist_f2[frente.size()-1].second = INFINITY_DOUBLE; 


                // Asignación de la distancia final de crowding al individuo
                for(i = 0; i < frente.size(); i++){
                    frente[i].distancia_c = distancias[i];
                    poblacion[frente[i].id_original].distancia_c = distancias[i];
                }
            }
        }catch(...){
            cerr<<"Error en la función de cálculo de frentes y distancias."<<endl;
        }
    }

    vector<Individuo> NSGAII(
        Funcion funcion_objetivo, 
        int n, //Tamaño de la poblacion
        int generaciones, 
        double probabilidad_mutacion, 
        double probabilidad_cruce, 
        bool min, 
        int cant_var 
    )
    {   
        set<int> nums = {10, 20, 50, 80, 100}; 

        vector<Individuo> poblacion_inicial; 
                            poblacion_inicial.reserve(n);
        try{

            //Inicializacion de la poblacion inicial
            for(int i = 0; i < n; i++){
                poblacion_inicial.push_back(Individuo(cant_var));  

                poblacion_inicial[i].evals = funcion_objetivo.f(poblacion_inicial[i].var); 
            }

            calcular_FrentesyDistancias(poblacion_inicial, min); 

            //Ciclo de evolucion 

            for(int gen = 0; gen < generaciones; gen++){
                vector < Individuo > nueva_poblacion = poblacion_inicial;     
                cout << "Gen: " << gen << "<----------------" << endl;
                

                size_t k; 
                //Seleccion
                //cout << "---------------Torneo binario" << endl; 
                vector<Individuo> mating(poblacion_inicial.size());
                for(k = 0; k < poblacion_inicial.size(); k++){
                    int id1 = random_id(n), 
                        id2 = random_id(n), 
                        winner  = id1; 

                    if(dominado( poblacion_inicial[id1], poblacion_inicial[id2], min) || 
                        dominado( poblacion_inicial[id2], poblacion_inicial[id1], min ) ){
    
                        winner = ( dominado( poblacion_inicial[id1], poblacion_inicial[id2], min ) ) ? id1 : id2; 
                    }
                    else{
                        winner = (poblacion_inicial[id1].distancia_c > poblacion_inicial[id2].distancia_c) ? id1 : id2; 
                    }
                    
                    mating[k] = poblacion_inicial[winner]; 
                }

                //Crossover:

                for(k = 0; k < poblacion_inicial.size() / 2; k++){
                    Individuo &padre1 = mating[2 * k], 
                            &padre2 = mating[2 * k + 1]; 
                    
                    Individuo &hijo1 = nueva_poblacion[2 * k], 
                            &hijo2 = nueva_poblacion[2 * k + 1]; 

                    if(randomperc() > probabilidad_cruce){
                        if(randomperc() < 0.5){
                            
                            for(int j = 0; j < cant_var/2; j++){
                                //Asignacion de cromosomas pares 
                                hijo1.var[ 2*j ] = padre1.var[ 2*j ]; 
                                hijo2.var[ 2*j ] = padre2.var[ 2*j ]; 

                                //Asinacion de cromosomas impares 
                                hijo1.var[ 2 * j + 1 ] = padre2.var[ 2 * j + 1 ]; 
                                hijo2.var[ 2 * j + 1 ] = padre1.var[ 2 * j + 1 ];

                                hijo1.evals = funcion_objetivo.f(hijo1.var); 
                                hijo2.evals = funcion_objetivo.f(hijo2.var); 
                            }
                        }
                        else{
                            hijo1 = padre1;
                            hijo2 = padre2; 
                        }

                    }
                    else{
                        hijo1 = padre1; 
                        hijo2 = padre2; 
                    }
                }

                //Mutacion: 

                double delta, 
                    escala = 1.0/1000; 
                for(Individuo & ind : nueva_poblacion){
                    if(randomperc() < probabilidad_mutacion){
                        /*
                        Aplicamos una variacion dentro del rango, ajustado a cada variable
                        Cada variable va de [0, 2 * i]
                        */
                        for(int j = 0; j < cant_var; j++){
                            delta = (2.0 * j) * 10.0 * randomperc() * escala; 

                            ind.var[j] += signo() ?  delta: (-1.0) * delta; 

                            ind.var[j] = std::max(0.0, std::min(2.0 *  j, ind.var[j] + delta)); 
                        }

                        ind.evals = funcion_objetivo.f(ind.var); 
                    }
                }

                //Concatenacion: 

                vector< Individuo > pob_concatenadas = poblacion_inicial; 
                pob_concatenadas.insert(pob_concatenadas.end(), nueva_poblacion.begin(), nueva_poblacion.end()); 

                calcular_FrentesyDistancias(pob_concatenadas, min); 

                sort(pob_concatenadas.begin(), pob_concatenadas.end(), 
                    [](const Individuo & a, const Individuo & b){
                        if(a.frente_dominancia != b.frente_dominancia){
                            return a.frente_dominancia < b.frente_dominancia; 
                        }
                        else{ 
                            return a.distancia_c > b.distancia_c; 
                        }
                    });

                pob_concatenadas.resize(n); 

                poblacion_inicial = pob_concatenadas; 

            }

        
    
            
        } catch (const exception & e){
            cerr << "Error: " << e.what() << endl; 
        } catch (...) {
            cerr << "Error desconocido. " << endl; 
        }

        return poblacion_inicial;   
        
    }
}
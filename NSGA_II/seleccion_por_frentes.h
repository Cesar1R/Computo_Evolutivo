#include<iostream> 


//Funcion que determina si a es dominado por b
bool dominado (const Punto & a, const Punto & b, const bool & minimizar){
    if(minimizar){
        return (b.f1 <= a.f1 && b.f2 <= a.f2 && (b.f1 < a.f1 || b.f2 < b.f2)); 
    }
    else{
        return (a.f1 <= b.f1 && a.f2 <= b.f2 && (a.f1 < b.f1 || a.f2 < b.f2)); 
    }
}
vector<vector<Punto>> clasificacion_por_frentes (vector<Punto> & poblacion, const bool & minimizar){
    int n = poblacion.size(); 

    cout << "--------------INICIO::Clasificacion por frentes ----------" << endl; 
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

    cout << "--------------FIN::Clasificacion por frentes ----------" << endl; 
    return frentes; 

}


vector<Punto> seleccion_por_frentes (vector<Punto> & poblacion, const bool & minimizar, const int & pob_size){

    vector<vector<Punto>> frentes = clasificacion_por_frentes(poblacion, minimizar); 

    int n = pob_size; 

    vector<Punto> nueva_poblacion; 


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

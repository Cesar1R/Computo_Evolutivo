//Funcion que aplica el torneo binario en funcion de los frentes y las distancias cowding

// Función para obtener el vector de selección de padres basado en el torneo binario
vector<Punto> get_matin (vector<Punto> & poblacion, const bool & minimizar){
    cout << "--------------Inicio::get_matin----------------------" << endl;

    vector<vector<Punto>> frentes = clasificacion_por_frentes(poblacion, minimizar); 

    //Vector que guarda la poblacion ordenada por frentes
    vector<Punto> pob; 

    cout << "Asingaciones mapa y poblacion" << endl; 
    //Mapa que para cada clave: indice del vector de poblacion, 
    //      guarda el el valor: frente al que pertenece 
    map<int, int> frente_map;  
    int k = 0, n = poblacion.size(), m; 
    size_t i, j; 
    for(i = 0; i < frentes.size(); i++){ // i:  frente actual 
        m = frentes[i].size(); 
        cout << "i: " << i << endl; 
        cout << "j: "; 
        for(j = 0; j < m; j++){ // j: elemento del frente actual en el indice j
            pob.pb(frentes[i][j]); 
            frente_map[j] = i; //Asignamos el frente i al indice j 
            cout << j << " ";  
        }
        cout << endl; 
    } 


    vector<Punto> mating; 

    cout << "Seleccion de mating" << endl; 
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
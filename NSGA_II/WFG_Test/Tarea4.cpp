//////////////////////////Cabecera//////////////////////////

#include "ExampleProblems.h" 
#include "nsga_2.h" 
#include <iostream> 
#include <vector> 
#include <cstdlib>
#include <ctime>
#include <random> 

//////////////////////////funcines auxiliares//////////////////////////


void print_frente(string path_file, const std::vector< funciones_opti::Individuo > & poblacion){
    ofstream outFile(path_file); 

    if(outFile.is_open()){
        outFile << "["; 
        for(const funciones_opti::Individuo & p: poblacion){
            outFile << "(" << p.evals[0] << "," << p.evals[1] << "), ";  
        }

        outFile << "]";  
    }
    
}   


//////////////////////////codigo principal//////////////////////////

int main(){

    using funciones_opti::Individuo; 
    using WFG::Toolkit::Examples::Problems::WFG1; 
    using WFG::Toolkit::Examples::Problems::WFG2; 
    using WFG::Toolkit::Examples::Problems::WFG7;
    using funciones_opti::Funcion; 


    //Parametros 
    int k = 4,  
        M = 2, //Cantidad de objetivos
        cant_var = 24; // Cantidad de variables 
    
    int pob_size = 100, generaciones = 100;
    bool min = true; 
    double probabilidad_mutacion = 0.5, 
            probabilidad_cruce  = 0.9; 

    string path = "resultados"; 

    vector< Individuo >  frente_WFG1 = funciones_opti::NSGAII(
                Funcion (M, k, WFG1), 
                pob_size, 
                generaciones, 
                probabilidad_mutacion, 
                probabilidad_cruce, 
                min,
                cant_var);
                
    print_frente(path + "WFG1.txt", frente_WFG1); 

    vector< Individuo >  frente_WFG2 = funciones_opti::NSGAII(
                Funcion (M, k, WFG2), 
                pob_size, 
                generaciones, 
                probabilidad_mutacion, 
                probabilidad_cruce, 
                min,
                cant_var);    
    print_frente(path + "WFG2.txt", frente_WFG2); 


    vector< Individuo >  frente_WFG7 = funciones_opti::NSGAII(
                Funcion (M, k, WFG7), 
                pob_size, 
                generaciones, 
                probabilidad_mutacion, 
                probabilidad_cruce, 
                min,
                cant_var); 

    print_frente(path + "WFG3.txt", frente_WFG7); 





    return 0; 
}
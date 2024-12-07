//////////////////////////Cabecera//////////////////////////

#include "ExampleProblems.h" 
#include "nsga_2.h" 
#include "hipervolumen.h"
#include <iostream> 
#include <vector> 
#include <cstdlib>
#include <ctime>
#include <random> 

//////////////////////////funcines auxiliares//////////////////////////


void print_frente(string path_file, const std::vector< Individuo > & poblacion){
    ofstream outFile(path_file); 

    if(outFile.is_open()){
        outFile << "["; 
        for(const Individuo & p: poblacion){
            outFile << "(" << p.evals[0] << "," << p.evals[1] << "), ";  
        }

        outFile << "]";  
    }
    
}   


//////////////////////////codigo principal//////////////////////////

int main(){


    using WFG::Toolkit::Examples::Problems::WFG3; 
    using WFG::Toolkit::Examples::Problems::WFG2; 
    using WFG::Toolkit::Examples::Problems::WFG7;


    //Parametros 
    int k = 4,  
        M = 2, //Cantidad de objetivos
        cant_var = 24; // Cantidad de variables 
    
    int pob_size = 100, generaciones = 100;
    bool min = true; 
    double probabilidad_mutacion = 0.5, 
            probabilidad_cruce  = 0.9; 

    string path = "resultados"; 

    vector<double> referencia = {2.2, 4.4}; 

    vector<Individuo> poblacion_inicial; 

    vector< Individuo >  frente_WFG3 = funciones_opti::NSGAII(
                Funcion (M, k, WFG3), 
                pob_size, 
                generaciones, 
                probabilidad_mutacion, 
                probabilidad_cruce, 
                min,
                cant_var, 
                poblacion_inicial);
    print_frente(path + "WFG3_pobIn.txt", poblacion_inicial); 
    print_frente(path + "WFG3.txt", frente_WFG3); 
    std::cout << "HV WFG3: " << calcularHipervolumen(frente_WFG3, referencia) << " "; 

    vector< Individuo >  frente_WFG2 = funciones_opti::NSGAII(
                Funcion (M, k, WFG2), 
                pob_size, 
                generaciones, 
                probabilidad_mutacion, 
                probabilidad_cruce, 
                min,
                cant_var, 
                poblacion_inicial);

    print_frente(path + "WFG2_pobIn.txt", poblacion_inicial);    
    print_frente(path + "WFG2.txt", frente_WFG2); 
    std::cout << "HV WFG2: " <<  calcularHipervolumen(frente_WFG2, referencia) << " ";



    vector< Individuo >  frente_WFG7 = funciones_opti::NSGAII(
                Funcion (M, k, WFG7), 
                pob_size, 
                generaciones, 
                probabilidad_mutacion, 
                probabilidad_cruce, 
                min,
                cant_var, 
                poblacion_inicial); 
    print_frente(path + "WFG7_pobIn.txt", poblacion_inicial); 
    print_frente(path + "WFG7.txt", frente_WFG7);
     
    std::cout << "HV WFG7: " << calcularHipervolumen(frente_WFG7, referencia) << " ";


    







    return 0; 
}
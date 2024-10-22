#include<iostream>  
#include<vector> 
#include<cmath> 
#include<fstream>
#include<chrono>  

#define MAX 1000000
#define CANT_P 1024
#define EPS 1e-9


using namespace std; 

using ld = double; 

struct punto {
    ld x; //real
    ld y; //imaginaria

    punto(ld x_val, ld y_val): x(x_val), y(y_val) {} 

    punto operator+(const punto& other) const{
        return punto(x + other.x, y + other.y); 
    }

    punto operator*(const punto& other) const{
        return punto(x * other.x - y * other.y, x * other.y + y * other.x); 
    }
}; 

inline ld  norma(punto p1){
    return sqrt(p1.x*p1.x + p1.y*p1.y); 
}

inline bool eval(ld x, ld y){

    punto c(x, y), 
          z(0, 0),
          z_k = c; 

    for(int i = 1; i <= MAX; i++){
        
        if(norma(z_k) > 2){
            return false;  
        }

        z_k = z_k*z_k + c; 
    }

    return true; 
} 

int main(int argc, char* argv[]){

    auto start = chrono::high_resolution_clock::now(); 

    ios_base::sync_with_stdio(0); 
    cout.tie(0); 
    vector<punto> conjuntos_Mal;  
    ofstream file("/home/est_licenciatura_cesar.rivera/Evolutivo/Segunda_Parte/Tarea1/imagen_v3.pgm"); 

    if(!file){
        cerr << "Error al abrir el archivo." << endl; 
        return 1; 
    }

    file << "P2\n"; 
    file << CANT_P << " " << CANT_P << "\n"; 
    file << 1 << "\n"; 

    ld paso_x = 2.0 /CANT_P, 
       paso_y = 2.0 /CANT_P, 
       x, y; 

    for(int k = 0; k < CANT_P; k++){
        x = -1.5 + paso_x * k; 
        for(int i=0; i < CANT_P; i++){
             y = -1.0 + paso_y * i; 
            if(eval(x, y)){
		// conjuntos_Mal.emplace_back(x, y);
                file << "1 ";   
            }
            else{
                file << "0 "; 
            }
        }
        file << "\n";
    }   


    auto end = chrono::high_resolution_clock::now(); 

    chrono::duration<double> duration = end - start; 

    int horas = duration.count()/3600, 
	minutos = (duration.count() - horas*3600)/60, 
       segundos = (duration.count() - horas*3600 - minutos * 60); 	
   cout << "Tiempo de ejecucion" << endl; 
    cout << horas << ":" << minutos << ":" << segundos << endl;

    return 0; 
}

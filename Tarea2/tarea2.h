#ifndef TAREA2_H
#define TAREA2_H

#include<iostream>
#include<vector> 
#include<algorithm> 
#include<chrono> 
#include<ctime> 
#include<cstdlib>
#include<fstream> 
#include<cfloat> 
#include <numeric>
#include<set> 
#include <map> 
#include<climits> 

#define pb push_back  
using vi = std::vector<int>;

#define CANT_REPS 1


void imprimirResultados(const std::vector<int>& arr, const std::vector<double>& vector) {
    size_t n = std::min(arr.size(), vector.size());  
    
    for (size_t i = 0; i < n; ++i) {
        std::cout << "El resultado para " << arr[i] << " puntos: " << vector[i] << "\n";
    }

    std::cout << "["; 
    for (size_t i = 0; i < n; i++){
       std::cout << vector[i] << ',';  
    }

    std::cout << "]," << std:: endl;
}


struct Punto2D {
    float x, y;
    Punto2D(float _x, float _y) : x(_x), y(_y) {}
};

struct Punto3D {
    float x, y, z;
    Punto3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

bool esDominado2D(const Punto2D& p1, const Punto2D& p2) {
    // Verifica si p1 es dominado por p2 (p2 tiene coordenadas mayores o iguales)
    return p2.x >= p1.x && p2.y >= p1.y;
}

bool esDominado3D(const Punto3D& p1, const Punto3D& p2) {
    // Verifica si p1 es dominado por p2 (p2 tiene coordenadas mayores o iguales)
    return p2.x >= p1.x && p2.y >= p1.y && p2.z >= p1.z;
}

bool esNoDominado2D(const std::vector<Punto2D>& puntos, const Punto2D& nuevo_punto) {
    for (const auto& p : puntos) {
        if (esDominado2D(nuevo_punto, p)) return false;
    }
    return true;
}

bool esNoDominado3D(const std::vector<Punto3D>& puntos, const Punto3D& nuevo_punto) {
    for (const auto& p : puntos) {
        if (esDominado3D(nuevo_punto, p)) return false;
    }
    return true;
}

void gen_puntos_no_dominados_2D(std::vector<Punto2D>& puntos, int cant_puntos) {
    srand(time(0));
    float X_max = 100.0, Y_max = 100.0;

    for (int i = 0; i < cant_puntos; i++) {
        float x = 0.1 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (X_max - 0.1)));
        float y = 0.1 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Y_max - 0.1)));
        
        Punto2D nuevo_punto(x, y);
        if (esNoDominado2D(puntos, nuevo_punto)) {
            puntos.push_back(nuevo_punto);
        } else {
            i--; // Si el punto es dominado, generamos otro
        }
    }
}

void gen_puntos_no_dominados_3D(std::vector<Punto3D>& puntos, int cant_puntos) {
    srand(time(0));
    float X_max = 100.0, Y_max = 100.0, Z_max = 100.0;

    for (int i = 0; i < cant_puntos; i++) {
        float x = 0.1 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (X_max - 0.1)));
        float y = 0.1 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Y_max - 0.1)));
        float z = 0.1 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Z_max - 0.1)));
        
        Punto3D nuevo_punto(x, y, z);
        if (esNoDominado3D(puntos, nuevo_punto)) {
            puntos.push_back(nuevo_punto);
        } else {
            i--; // Si el punto es dominado, generamos otro
        }
    }
}


#endif  // TAREA2_H

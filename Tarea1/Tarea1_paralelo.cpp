#include <iostream>
#include <mpi.h>
#include <queue> 
#include <stack>
#include <fstream> 
#include <cmath> 
#include <vector> 
#include <string>
#include <chrono> 



#define MAX_PTS 1024
#define MAX_IT 1000000



using ld = double; 
using namespace std; 
#define  M_LD  MPI_DOUBLE


class punto{
    public: 
    ld x; 
    ld y; 

    punto(ld tx, ld ty): x(tx), y(ty) {} 

    punto operator*(const punto &other)const {
        return punto(x*other.x - y*other.y, x*other.y + y*other.x); 
    }

    punto operator+(const punto &other)const{
        return punto(x + other.x, y + other.y); 
    }

    ld norma() const {
        return sqrt(x * x + y * y); 
    }

    punto(const punto &other): x(other.x), y(other.y) {}

    punto &operator=(const punto &other){
        if(this != &other){
            x = other.x; 
            y = other.y; 
        }

        return *this; 
    }
};

struct coordenadas_id{ 
    int x; 
    int y; 

    coordenadas_id(int tx, int ty): x(tx), y(ty) {}
}; 

class Tarea{
    public: 
    punto p; 
    coordenadas_id cord;
    int tag, source;  

    Tarea(punto pt, coordenadas_id coord): p(pt), cord(coord) {} 

    Tarea(ld pt_x, ld pt_y, int c_x, int c_y): p(pt_x, pt_y), cord(c_x, c_y) {}

    bool eval(){
        punto z_k (0,0); 

        for(int i = 0; i < MAX_IT; i++){
            z_k = z_k*z_k + p; 
            if(z_k.norma() > 2){
                return false; 
            }
        }
    
        return true; 
    }
    


    void init(MPI_Datatype & mpi_task_type){
        int block_lengths[4] = {1, 1, 1, 1}; // 2 ld + 2 ints
        MPI_Aint offsets[4];
        MPI_Datatype types[4] = {M_LD, M_LD, MPI_INT, MPI_INT}; 

        offsets[0] = offsetof(Tarea, p.x); 
        offsets[1] = offsetof(Tarea, p.y); 
        offsets[2] = offsetof(Tarea, cord.x); 
        offsets[3] = offsetof(Tarea, cord.y); 

        MPI_Type_create_struct(4, block_lengths, offsets, types, &mpi_task_type); 
        MPI_Type_commit(&mpi_task_type); 
    }

    void send (int slave, int tag, MPI_Datatype MPI_TASK){
        MPI_Send(this, 1, MPI_TASK, slave, tag, MPI_COMM_WORLD);
    }

    
    
    void get (int from, MPI_Datatype MPI_TASK){
        MPI_Status status; 
        if(from == -1){
            MPI_Recv(this, 1, MPI_TASK, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }else {
            MPI_Recv(this, 1, MPI_TASK, from, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }
        this->source = status.MPI_SOURCE; 
        this->tag = status.MPI_TAG; 
    }

}; 








void Maestro (int size){
    MPI_Datatype MPI_TASK;
    MPI_Status status; 
    Tarea t(0, 0, 0, 0); 
    t.init(MPI_TASK);   

    vector<vector<bool>> mat_resultados (MAX_PTS+1, vector<bool> (MAX_PTS+1));
    
    cout << "-------Comienza el proceso de asignacion de Tareas--------" << endl; 
    int id_slave;
    Tarea curr_task(0, 0, 0, 0), curr_result(0, 0, 0, 0); 

    //Genera tareas
    cout << "----Comienza generacion de tareas---" << endl;  
        vector<Tarea> tareas; 
        ld delta_x = 2.0 / MAX_PTS, 
        delta_y = 2.0 / MAX_PTS, 
        x, y; 

        for(int i = 0; i < MAX_PTS; i++){
            x = -1.5 + i *delta_x; 
            for(int j = 0; j < MAX_PTS; j++){
                y = -1.0 + j *delta_y;  
                
                tareas.emplace_back(x, y, i, j);  
            }
        }


        cout << "--Asignaciones iniciales--" << endl;
        // Asignacion iniciales, una tarea a cada uno de los esclavos
        for(int i = 1; i < size; i++){
            curr_task = tareas.back(); 
            tareas.pop_back(); 
            curr_task.send(i, 3, MPI_TASK); 
        }
    

        int datos_rec = 0, // no se ha recibido ningun dato
            curr_slave, 
            datos_env = 11; // ya se han asignado una tarea a cada slave 

        cout << "--Asignaciones hasta recibir la cantidad de puntos deseada--"<< endl;
        // Ponemos la condicion de paro que se hayan enviado todas las tareas 
        //y se hayan recibito todas las que se enviaron. 
        while (datos_rec < MAX_PTS * MAX_PTS) {  

            // Verificar si algún slave se desocupó y recibir evaluaciones
            curr_result.get(-1, MPI_TASK);

            // Verificar si el punto pertenece o no dependiendo de la etiqueta que regresa
            // Anotar evaluaciones
            switch (curr_result.tag) { 
                case 1: // Pertenece
                    mat_resultados[curr_result.cord.x][curr_result.cord.y] = 0;
                    break;
                case 2: // No pertenece
                    mat_resultados[curr_result.cord.x][curr_result.cord.y] = 1;
                    break;
            }

            datos_rec++; 
            cout << "Datos recibidos- " << datos_rec << endl;

            id_slave = curr_result.source; 

            // Verifica si quedan tareas por asignar
            if (!tareas.empty()) {
                curr_task = tareas.back();
                tareas.pop_back(); 
                // Etiqueta 3 porque es una petición de evaluar el punto
                curr_task.send(id_slave, 3, MPI_TASK);
                datos_env++; 

                cout << "Datos env--- " << datos_env << endl;
            }
        }

    cout << "----Comienza el proceso de generacion de resultados------" << endl; 
        string path = "/home/est_licenciatura_cesar.rivera/Evolutivo/Segunda_Parte/Pruebas/im_v3i1024.pgm";     
        ofstream image(path);

        cout << "Path: " << path << endl;  

        if(!image){
            cerr << "Error al abrir el archivo." << endl; 
            return ; 
        }

        image << "P2\n"; 
        image << MAX_PTS << " " << MAX_PTS << "\n"; 
        image << 1 << "\n"; 

        int val; 
        for(int i = 0; i < MAX_PTS; i++){
            for(int j = 0; j < MAX_PTS; j++){
                val = (mat_resultados[j][i]) ? 0:1; 
                image << val << " "; 
            }
            image << "\n"; 
        }

        image.close(); 
	   
    cout << "--------Se inicia el termino de los proceso slaves---------" << endl; 
    // Finalizar todos los procesos, enviado la etiqueta 4 a cada uno de los esclavos
    for(int i = 1; i < size; i++){
        t.send(i, 4, MPI_TASK); 
    }


    return; 

}

void Esclavo(int rank){ 
    MPI_Datatype MPI_TASK;
    Tarea t(0, 0, 0, 0); 
    t.init(MPI_TASK);  

    //Se comunica con el maestro, etiqueta 0 para mostrar que es la primera interaccion
    // Y que el maestro pueda contemplarlo para la asignacion de tareas
    t.send(0, 0, MPI_TASK); 

    int tag_respuesta, tag_peticion; 

    MPI_Status status; 
    Tarea curr_task(0, 0, 0, 0); 

    // Recibira peticiones mientras la etiqueta sea diferente de 4 se mantendra en el bucle
    while(true){ 
        curr_task.get(0, MPI_TASK); 
        
        tag_peticion = curr_task.tag; 
        //Verifica si el maestro mando la tag de finalizacion 
        if(tag_peticion == 4){
            cout << "Proceso " << rank << " termino <----------" << endl; 
           break; 
        }
        // En otro caso, ejecutamos la evaluacion y regresamos la respuesta. 

        //Determina si el punto esta dentro o fuera (Evaluacion)
        tag_respuesta = (curr_task.eval())? 1:2; 
        //Regresa la repuesta, si esta dentro con etiqueta 1, si esta fuera con etiqueta 2 
        curr_task.send(0, tag_respuesta, MPI_TASK); 
        //0 ya que se envia al maestro. 
    }

}



int main(int argc, char** argv){

    MPI_Init(&argc, &argv); 

    int rank, size; 
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 12) {  
        std::cerr << "Error al crear al menos uno de los procesos" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if(rank==0){
        //Variable para medir el tiempo de ejecucion del proceso maestro
        auto start = chrono::high_resolution_clock::now(); 

        //Proceso maestro
            Maestro(size);  

        auto end = chrono::high_resolution_clock::now(); 

        chrono::duration<double> duration = end - start; 

        int horas = duration.count()/3600, 
            minutos = (duration.count() - horas*3600)/60,  
            segundos = duration.count() - horas * 3600 - minutos * 60; 

        cout << "Tiempo de ejecucion " << horas << ":" << minutos << ":" << segundos << endl; 
    }
    else{

        Esclavo(rank);  
    
    }

    MPI_Finalize(); 

    return 0; 
}




























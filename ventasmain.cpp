#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <vector>
#include <limits>
#include "Ventas.h"
#include "MenuEstadisticas.h"
#include "MenuModificaciones.h"
#include "MenuConsultas.h"
#include "HashMapList.h"

using namespace std;

//Variables globales
HashMapList<string, Venta> ventasMap(5000);
// Vector que almacena todas las ventas cargadas, para facilitar el recorrido en consultas, estadísticas y modificaciones
vector<Venta> vectorVentas;


string nombreArchivoCSV = "ventas_sudamerica.csv";

// Función para parsear una línea del CSV en un vector de strings
// Separa la linea en campos usando la coma como delimitador
vector<string> parseCSVLine(const string& line) {
    vector<string> result;
    stringstream ss(line);
    string item;

    while (getline(ss, item, ',')) {
        result.push_back(item);
    }

    return result;
}


// Función que lee el archivo CSV completo y carga las ventas
// Llena tanto ventasMap (HashMapList) como vectorVentas (vector)
// Valida cada línea, convierte tipos de datos, calcula el monto total
// Mide tiempo de ejecución
HashMapList<string, Venta> leerArchivoCSV(const string& nombreArchivo) {
    clock_t begin;
    cout << "Comenzando a medir Tiempo..." << endl;
    begin = clock();

    HashMapList<string, Venta> ventasMapTemp(5000);

    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo!" << endl;
        return ventasMapTemp;
    }

    string linea;
    getline(archivo, linea); // Leer encabezado

    int contadorLineas = 0;
    int lineasInvalidas = 0;

    while (getline(archivo, linea)) {
        contadorLineas++;
        vector<string> campos = parseCSVLine(linea);

        if (campos.size() != 12) {
            cerr << "Linea " << contadorLineas << " malformada, se omite.\n";
            lineasInvalidas++;
            continue;
        }

        Venta v;
        v.setId(campos[0]);
        v.setFecha(campos[1]);
        v.setPais(campos[2]);
        v.setCiudad(campos[3]);
        v.setCliente(campos[4]);
        v.setProducto(campos[5]);
        v.setCategoria(campos[6]);

        try {
            v.setCantidad(stoi(campos[7]));
            v.setPrecioUnitario(stof(campos[8]));
        } catch (const std::invalid_argument& e) {
            cerr << "Error al convertir cantidad/precio en linea " << contadorLineas << ". Se omite.\n";
            lineasInvalidas++;
            continue;
        }

        v.setMedioEnvio(campos[10]);
        v.setEstadoEnvio(campos[11]);

        // Obtener clave
        string clave = v.getClave();

        // Validar clave → evitar claves vacias
        if (clave.find("__") != string::npos || clave == "" || clave == "___" || clave.find(",,") != string::npos) {
            cerr << "Linea " << contadorLineas << " tiene clave vacia o inconsistente → se omite.\n";
            lineasInvalidas++;
            continue;
        }

        // Insertar en el HashMapList con clave válida
        ventasMapTemp.put(clave, v);
        vectorVentas.push_back(v);
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;

    cout << "Se procesaron " << contadorLineas << " lineas de datos.\n";
    cout << "Lineas invalidas u omitidas: " << lineasInvalidas << "\n";
    cout << "Tardo en segundos " << elapsed_secs << "\n" << endl;

    archivo.close();
    return ventasMapTemp;
}



// === Main ===
int main() {
    int opcion;

    do {
        cout << "\n=== MENU PRINCIPAL ===\n";
        cout << "1 - Cargar los datos para la base de datos de ventas\n";
        cout << "2 - Mostrar estadisticas de ventas\n";
        cout << "3 - Modificar ventas (Agregar/Eliminar/Modificar)\n";
        cout << "4 - Consultas dinamicas\n";
        cout << "0 - Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                ventasMap = leerArchivoCSV(nombreArchivoCSV);
                 cout << "Base de datos cargada correctamente.\n";
                break;


            case 2:
                MenuEstadisticas();
                break;

            case 3:
                MenuModificaciones();
                break;

            case 4:
                MenuConsultas();
                break;

            case 0:
                cout << "Saliendo del programa...\n";
                break;

            default:
                cout << "Opcion invalida. Intente nuevamente.\n";
        }

    } while (opcion != 0);

    return 0;
}

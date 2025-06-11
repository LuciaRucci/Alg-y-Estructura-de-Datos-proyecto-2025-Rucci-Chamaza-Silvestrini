#include "MenuModificaciones.h"
#include "Ventas.h"
#include "HashMapList.h"
#include "HashMap.h"
#include "HashEntry.h"
#include "Lista.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// Variables globales
extern vector<Venta> vectorVentas;
extern const string nombreArchivoCSV;
extern HashMapList<string, Venta> leerArchivoCSV(const string& nombreArchivo);

vector<Venta> cargarVectorDesdeCSV(const string& archivoCSV) {
    vector<Venta> ventas;
    ifstream archivo(archivoCSV);
    string linea;
    getline(archivo, linea); // Saltear cabecera

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;
        Venta v;

        getline(ss, campo, ','); v.setId(campo);
        getline(ss, campo, ','); v.setFecha(campo);
        getline(ss, campo, ','); v.setPais(campo);
        getline(ss, campo, ','); v.setCiudad(campo);
        getline(ss, campo, ','); v.setCliente(campo);
        getline(ss, campo, ','); v.setProducto(campo);
        getline(ss, campo, ','); v.setCategoria(campo);
        getline(ss, campo, ','); v.setCantidad(stoi(campo));
        getline(ss, campo, ','); v.setPrecioUnitario(stof(campo));
        getline(ss, campo, ','); /* monto total no lo leo, se calcula en setMontoTotal */
        getline(ss, campo, ','); v.setMedioEnvio(campo);
        getline(ss, campo);     v.setEstadoEnvio(campo);

        ventas.push_back(v);
    }

    return ventas;
}


void MenuModificaciones() {
    int opcion;
    do {
        cout << "\n=== MENU MODIFICACIONES ===\n";
        cout << "1 - Agregar una venta\n";
        cout << "2 - Eliminar una venta (por pais o ciudad)\n";
        cout << "3 - Modificar una venta (por ID)\n";
        cout << "0 - Volver al menu principal\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: agregarVenta(); break;
            case 2: eliminarVenta(); break;
            case 3: modificarVenta(); break;
            case 0: cout << "Volviendo al menu principal...\n"; break;
            default: cout << "Opcion invalida.\n";
        }

    } while (opcion != 0);
}
void agregarVenta() {
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Agregar una venta]\n";
    cout << "Estructura utilizada: vectorVentas + archivo CSV\n";
    cout << "Comenzando a medir Tiempo" << endl;

    Venta v;
    string tempStr;
    int tempInt;
    float tempFloat;

    // Ignorar salto de linea anterior
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Ingrese ID: ";
    getline(cin, tempStr);
    v.setId(tempStr);

    cout << "Ingrese Fecha (YYYY-MM-DD): ";
    getline(cin, tempStr);
    v.setFecha(tempStr);

    cout << "Ingrese Pais: ";
    getline(cin, tempStr);
    v.setPais(tempStr);

    cout << "Ingrese Ciudad: ";
    getline(cin, tempStr);
    v.setCiudad(tempStr);

    cout << "Ingrese Cliente: ";
    getline(cin, tempStr);
    v.setCliente(tempStr);

    cout << "Ingrese Producto: ";
    getline(cin, tempStr);
    v.setProducto(tempStr);

    cout << "Ingrese Categoria: ";
    getline(cin, tempStr);
    v.setCategoria(tempStr);

    // VALIDAR cantidad
    cout << "Ingrese Cantidad: ";
    while (!(cin >> tempInt) || tempInt < 0) {
        contadorIf++;  // if cantidad invalida
        cout << "Numero invalido. Ingrese una cantidad entera no negativa: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    v.setCantidad(tempInt);

    // VALIDAR precio unitario
    cout << "Ingrese Precio Unitario: ";
    while (!(cin >> tempFloat) || tempFloat < 0) {
        contadorIf++;  // if precio invalido
        cout << "Numero invalido. Ingrese un precio unitario valido: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    v.setPrecioUnitario(tempFloat);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Ingrese Medio de Envio: ";
    getline(cin, tempStr);
    v.setMedioEnvio(tempStr);

    cout << "Ingrese Estado de Envio: ";
    getline(cin, tempStr);
    v.setEstadoEnvio(tempStr);

    // Guardar en CSV (append)
    ofstream archivo(nombreArchivoCSV, ios::app);
    if (archivo.is_open()) {
        archivo << v.getId() << ","
                << v.getFecha() << ","
                << v.getPais() << ","
                << v.getCiudad() << ","
                << v.getCliente() << ","
                << v.getProducto() << ","
                << v.getCategoria() << ","
                << v.getCantidad() << ","
                << v.getPrecioUnitario() << ","
                << v.getMontoTotal() << ","
                << v.getMedioEnvio() << ","
                << v.getEstadoEnvio() << "\n";
        archivo.close();
        cout << "Venta agregada exitosamente.\n";
    } else {
        cerr << "Error al abrir el archivo.\n";
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tardo en segundos " << elapsed_secs << "\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << "\n" << endl;

    // Actualizar vectorVentas
    vectorVentas.clear();
    vectorVentas = cargarVectorDesdeCSV(nombreArchivoCSV);
}



void eliminarVenta() {
    clock_t begin = clock();
    cout << "Comenzando a medir Tiempo" << endl;
    int contadorIf = 0;

    cout << "[Proceso: Eliminar una venta por pais o ciudad]\n";
    cout << "Estructura utilizada: vectorVentas + archivo CSV\n";

    string opcionFiltro;
    cout << "Desea eliminar por (1) Pais o (2) Ciudad? Ingrese 1 o 2: ";
    cin >> opcionFiltro;
    cin.ignore();

    string valorFiltro;
    if (opcionFiltro == "1") {
        cout << "Ingrese el nombre del pais: ";
        getline(cin, valorFiltro);
    } else if (opcionFiltro == "2") {
        cout << "Ingrese el nombre de la ciudad: ";
        getline(cin, valorFiltro);
    } else {
        cout << "Opcion invalida.\n";
        return;
    }

    // Mostrar ventas que coinciden
    vector<Venta> ventasFiltradas;
    for (auto& v : vectorVentas) {
        contadorIf++;
        if ((opcionFiltro == "1" && v.getPais() == valorFiltro) ||
            (opcionFiltro == "2" && v.getCiudad() == valorFiltro)) {
            ventasFiltradas.push_back(v);
        }
    }

    if (ventasFiltradas.empty()) {
        cout << "No se encontraron ventas con ese filtro.\n";
        clock_t end = clock();
        double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
        cout << "Tiempo de ejecucion: " << elapsed_secs << " segundos.\n";
        cout << "Cantidad de IF utilizados: " << contadorIf << endl;
        return;
    }

    // Mostrar ventas encontradas
    cout << "Ventas encontradas:\n";
    for (auto& v : ventasFiltradas) {
        v.imprimirVenta();
    }

    // Preguntar qué hacer
    cout << "Desea:\n";
    cout << "1. Eliminar TODAS las ventas que coinciden con el filtro\n";
    cout << "2. Eliminar SOLO una venta por ID\n";
    cout << "Seleccione una opcion (1 o 2): ";
    string opcionEliminar;
    getline(cin, opcionEliminar);

    // Eliminar del vectorVentas y del archivo
    vector<Venta> nuevasVentas;
    bool eliminado = false;

    if (opcionEliminar == "1") {
        // Eliminar todas las ventas que coinciden
        for (auto& v : vectorVentas) {
            contadorIf++;
            if ((opcionFiltro == "1" && v.getPais() == valorFiltro) ||
                (opcionFiltro == "2" && v.getCiudad() == valorFiltro)) {
                eliminado = true;
                cout << "Eliminada venta ID: " << v.getId() << endl;
                continue; // No agregar al nuevo vector
            }
            nuevasVentas.push_back(v);
        }
    } else if (opcionEliminar == "2") {
        cout << "Ingrese el ID de la venta que desea eliminar: ";
        string idEliminar;
        getline(cin, idEliminar);

        for (auto& v : vectorVentas) {
            contadorIf++;
            if (v.getId() == idEliminar) {
                eliminado = true;
                cout << "Eliminada venta ID: " << v.getId() << endl;
                continue; // No agregar al nuevo vector
            }
            nuevasVentas.push_back(v);
        }
    } else {
        cout << "Opcion invalida. No se realizo ninguna eliminacion.\n";
        return;
    }

    // Reescribir CSV
    ofstream archivo(nombreArchivoCSV);
    if (archivo.is_open()) {
        archivo << "ID_Venta,Fecha,Pais,Ciudad,Cliente,Producto,Categoria,Cantidad,Precio_Unitario,Monto_Total,Medio_Envio,Estado_Envio\n";
        for (auto& v : nuevasVentas) {
            archivo << v.getId() << ","
                    << v.getFecha() << ","
                    << v.getPais() << ","
                    << v.getCiudad() << ","
                    << v.getCliente() << ","
                    << v.getProducto() << ","
                    << v.getCategoria() << ","
                    << v.getCantidad() << ","
                    << v.getPrecioUnitario() << ","
                    << v.getMontoTotal() << ","
                    << v.getMedioEnvio() << ","
                    << v.getEstadoEnvio() << "\n";
        }
        archivo.close();

        if (eliminado) {
            cout << "Eliminacion realizada exitosamente.\n";
        } else {
            cout << "No se elimino ninguna venta.\n";
        }
    } else {
        cerr << "Error al abrir el archivo para sobrescribir.\n";
    }

    // Actualizar vectorVentas
    vectorVentas.clear();
    vectorVentas = cargarVectorDesdeCSV(nombreArchivoCSV);

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tiempo de ejecucion: " << elapsed_secs << " segundos.\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << endl;
}

void modificarVenta() {
    cout << "Ingrese el ID de la venta a modificar: ";
    string idModificar;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, idModificar);

    clock_t begin = clock();
    cout << "Comenzando a medir Tiempo" << endl;
    cout << "[Proceso: Modificar una venta por ID]\n";
    cout << "Estructura utilizada: vectorVentas + archivo CSV\n";

    bool encontrado = false;

    for (auto& v : vectorVentas) {
        if (v.getId() == idModificar) {
            encontrado = true;
            cout << "Venta encontrada:\n";
            v.imprimirVenta();

            int opcion;
            do {
                cout << "\nSeleccione el campo a modificar:\n";
                cout << "1. Fecha\n";
                cout << "2. Pais\n";
                cout << "3. Ciudad\n";
                cout << "4. Cliente\n";
                cout << "5. Producto\n";
                cout << "6. Categoria\n";
                cout << "7. Cantidad\n";
                cout << "8. Precio Unitario\n";
                cout << "9. Medio de Envio\n";
                cout << "10. Estado de Envio\n";
                cout << "0. Terminar modificacion\n";
                cout << "Opcion: ";
                cin >> opcion;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                string tempStr;
                int tempInt;
                float tempFloat;

                switch (opcion) {
                    case 1:
                        cout << "Nueva Fecha (YYYY-MM-DD): ";
                        getline(cin, tempStr);
                        v.setFecha(tempStr);
                        break;
                    case 2:
                        cout << "Nuevo Pais: ";
                        getline(cin, tempStr);
                        v.setPais(tempStr);
                        break;
                    case 3:
                        cout << "Nueva Ciudad: ";
                        getline(cin, tempStr);
                        v.setCiudad(tempStr);
                        break;
                    case 4:
                        cout << "Nuevo Cliente: ";
                        getline(cin, tempStr);
                        v.setCliente(tempStr);
                        break;
                    case 5:
                        cout << "Nuevo Producto: ";
                        getline(cin, tempStr);
                        v.setProducto(tempStr);
                        break;
                    case 6:
                        cout << "Nueva Categoria: ";
                        getline(cin, tempStr);
                        v.setCategoria(tempStr);
                        break;
                    case 7:
                        cout << "Nueva Cantidad: ";
                        while (!(cin >> tempInt) || tempInt < 0) {
                            cout << "Numero invalido. Ingrese una cantidad entera no negativa: ";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                        v.setCantidad(tempInt);
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    case 8:
                        cout << "Nuevo Precio Unitario: ";
                        while (!(cin >> tempFloat) || tempFloat < 0) {
                            cout << "Numero invalido. Ingrese un precio unitario valido: ";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                        v.setPrecioUnitario(tempFloat);
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    case 9:
                        cout << "Nuevo Medio de Envio: ";
                        getline(cin, tempStr);
                        v.setMedioEnvio(tempStr);
                        break;
                    case 10:
                        cout << "Nuevo Estado de Envio: ";
                        getline(cin, tempStr);
                        v.setEstadoEnvio(tempStr);
                        break;
                    case 0:
                        cout << "Finalizando modificacion.\n";
                        break;
                    default:
                        cout << "Opcion invalida.\n";
                }

            } while (opcion != 0);

            break; // una vez encontrada la venta, no hace falta seguir recorriendo.
        }
    }

    // Sobrescribir el CSV
    ofstream archivo(nombreArchivoCSV);
    if (archivo.is_open()) {
        // Escribimos cabecera
        archivo << "ID_Venta,Fecha,Pais,Ciudad,Cliente,Producto,Categoria,Cantidad,Precio_Unitario,Monto_Total,Medio_Envio,Estado_Envio\n";

        for (const auto& v : vectorVentas) {
            archivo << v.getId() << ","
                    << v.getFecha() << ","
                    << v.getPais() << ","
                    << v.getCiudad() << ","
                    << v.getCliente() << ","
                    << v.getProducto() << ","
                    << v.getCategoria() << ","
                    << v.getCantidad() << ","
                    << v.getPrecioUnitario() << ","
                    << v.getMontoTotal() << ","
                    << v.getMedioEnvio() << ","
                    << v.getEstadoEnvio() << "\n";
        }

        archivo.close();

        if (encontrado) {
            cout << "Venta modificada exitosamente.\n";
        } else {
            cout << "No se encontró la venta con el ID especificado.\n";
        }
    } else {
        cerr << "Error al abrir el archivo para sobrescribir.\n";
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tardo en segundos " << elapsed_secs << "\n" << endl;

   
    // Actualizar vectorVentas
    vectorVentas.clear();
    vectorVentas = cargarVectorDesdeCSV(nombreArchivoCSV);
}





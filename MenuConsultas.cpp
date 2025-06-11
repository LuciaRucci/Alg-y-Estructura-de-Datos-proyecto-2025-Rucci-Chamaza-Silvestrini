#include "MenuConsultas.h"
#include "Ventas.h"
#include "HashMapList.h"
#include "HashMap.h"
#include "HashEntry.h"
#include "ArbolBinarioAVL.h"
#include "NodoArbolAVL.h"
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <limits>
//MenuConsultas.cpp
// Implementa el menú de Consultas Dinámicas (opción 4 del menú principal).
// Permite realizar diferentes consultas sobre la base de datos de ventas.
// Las funciones recorren el vectorVentas y procesan los datos de acuerdo a la consulta seleccionada.
// Se mide el tiempo de ejecución y la cantidad de IF utilizados en cada proceso.


using namespace std;

extern vector<Venta> vectorVentas;

void MenuConsultas() {
    int opcion;
    do {
        cout << "\n    MENU CONSULTAS DINAMICAS    \n";
        cout << "1 - Listado de ventas en una ciudad especifica\n";
        cout << "2 - Listado de ventas en rango de fechas por pais\n";
        cout << "3 - Comparacion entre dos paises (monto total)\n";
        cout << "4 - Comparacion entre dos productos por pais\n";
        cout << "5 - Productos vendidos por encima de umbral por pais\n";
        cout << "6 - Productos vendidos por debajo de umbral por pais\n";
        cout << "7 - Comparar productos discriminado por todos los paises\n";
        cout << "8 - Comparar productos mas vendidos por pais\n";
        cout << "9 - Comparar medio de envio mas usado por pais\n";
        cout << "0 - Volver al menu principal\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: consultarVentasPorCiudad(); break;
            case 2: consultarVentasPorPaisYRangoFechas(); break;
            case 3: compararPaisesPorMontoTotal(); break;
            case 4: compararProductosPorPais(); break;
            case 5: productosPorPromedioUmbralMayor(); break;
            case 6: productosPorPromedioUmbralMenor(); break;
            case 7: compararProductosTodosPaises(); break;
            case 8: compararProductosMasVendidosPorPais(); break;
            case 9: compararMedioEnvioMasUsadoPorPais(); break;
            case 0: cout << "Volviendo al menu principal...\n"; break;
            default: cout << "Opcion invalida.\n";
        }

    } while (opcion != 0);
}


void consultarVentasPorCiudad() {
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Consultar ventas por ciudad]\n";
    cout << "Estructura utilizada: Arbol AVL\n";

    string ciudadBuscada;
    cout << "Ingrese el nombre de la ciudad: ";
    cin.ignore();
    getline(cin, ciudadBuscada);

    // Creo un AVL de ventas de la ciudad
    ArbolBinarioAVL<Venta> arbolVentasCiudad;

    // Recorro todas las ventas → inserto en el AVL las que coincidan con la ciudad
    for (auto& v : vectorVentas) {
        contadorIf++;
        if (v.getCiudad() == ciudadBuscada) {
            try {
                arbolVentasCiudad.put(v);
            } catch (...) {
                // Si hay dos ventas iguales (fecha + id), el AVL lanza excepción, no pasa nada.
            }
        }
    }

    clock_t end = clock();  // Termina el procesamiento acá

    // Impresión en orden (ordenado por fecha)
    cout << "\nVentas en '" << ciudadBuscada << "' ordenadas por fecha:\n";
    arbolVentasCiudad.inorder();  // Esto imprime por fecha (gracias a operator< de Venta)

    // Si no hay ventas en el AVL
    if (arbolVentasCiudad.esVacio()) {
        cout << "No se encontraron ventas en la ciudad '" << ciudadBuscada << "'.\n";
    }

    // Tiempo → SOLO el procesamiento
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tiempo de ejecucion: " << elapsed_secs << " segundos.\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << endl;
}

void consultarVentasPorPaisYRangoFechas() {
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Consultar ventas por pais y rango de fechas]\n";
    cout << "Estructura utilizada: Arbol AVL\n";


    string paisBuscado;
    string fechaInicio, fechaFin;

    cout << "Ingrese el nombre del pais: ";
    cin.ignore();
    getline(cin, paisBuscado);

    cout << "Ingrese la fecha de inicio (YYYY-MM-DD): ";
    getline(cin, fechaInicio);

    cout << "Ingrese la fecha de fin (YYYY-MM-DD): ";
    getline(cin, fechaFin);

    // Creo un AVL para ordenar por fecha
    ArbolBinarioAVL<Venta> arbolVentasPorFecha;

    for (auto& v : vectorVentas) {
        contadorIf++;
        if (v.getPais() == paisBuscado && v.getFecha() >= fechaInicio && v.getFecha() <= fechaFin) {
            try {
                arbolVentasPorFecha.put(v);
            } catch (...) {
                // Si hay ventas repetidas en fecha + id, ignoro la excepcion
            }
        }
    }

    clock_t end = clock();

    // Muestro las ventas ordenadas por fecha
    if (arbolVentasPorFecha.esVacio()) {
        cout << "No se encontraron ventas en el pais '" << paisBuscado << "' entre " << fechaInicio << " y " << fechaFin << ".\n";
    } else {
        cout << "\nVentas en '" << paisBuscado << "' entre " << fechaInicio << " y " << fechaFin << " ordenadas por fecha:\n";
        arbolVentasPorFecha.inorder();  // Esto ya imprime las ventas ordenadas
    }

    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tiempo de ejecucion: " << elapsed_secs << " segundos.\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << endl;
}

void compararPaisesPorMontoTotal() {
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Comparacion de monto total entre dos paises]\n";
    cout << "Algoritmo: HashMapList\n";

    string pais1, pais2;

    cout << "Ingrese el primer pais: ";
    cin.ignore();
    getline(cin, pais1);

    cout << "Ingrese el segundo pais: ";
    getline(cin, pais2);

    HashMapList<string, float> mapaMontos(50);

    for (auto& v : vectorVentas) {
        contadorIf++;
        string pais = v.getPais();
        float monto = v.getCantidad() * v.getPrecioUnitario();

        try {
            float montoActual = mapaMontos.get(pais);
            mapaMontos.put(pais, montoActual + monto);
        } catch (...) {
            mapaMontos.put(pais, monto);
        }
    }

    float montoTotalPais1 = 0.0f;
    float montoTotalPais2 = 0.0f;

    try { montoTotalPais1 = mapaMontos.get(pais1); } catch (...) {}
    try { montoTotalPais2 = mapaMontos.get(pais2); } catch (...) {}

    cout << "\n=== Comparacion de Monto Total ===\n";
    cout << pais1 << ": $" << montoTotalPais1 << endl;
    cout << pais2 << ": $" << montoTotalPais2 << endl;

    if (montoTotalPais1 > montoTotalPais2) {
        cout << "El pais con mayor monto total de ventas es " << pais1
             << ", con un total de $" << montoTotalPais1 << ".\n";
        contadorIf++;
    } else if (montoTotalPais2 > montoTotalPais1) {
        cout << "El pais con mayor monto total de ventas es " << pais2
             << ", con un total de $" << montoTotalPais2 << ".\n";
        contadorIf++;
    } else {
        cout << "Ambos paises tienen el mismo monto total de ventas: $" << montoTotalPais1 << ".\n";
        contadorIf++;
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tiempo de ejecucion: " << elapsed_secs << " segundos.\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << endl;
}

void compararProductosPorPais() {
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Comparacion entre dos productos por pais]\n";
    cout << "Algoritmo: HashMapList\n";

    string paisBuscado, producto1, producto2;

    cout << "Ingrese el nombre del pais: ";
    cin.ignore();
    getline(cin, paisBuscado);

    cout << "Ingrese el primer producto: ";
    getline(cin, producto1);

    cout << "Ingrese el segundo producto: ";
    getline(cin, producto2);

    HashMapList<string, float> mapaProductos(50);

    for (auto& v : vectorVentas) {
        contadorIf++;
        if (v.getPais() == paisBuscado) {
            string producto = v.getProducto();
            float monto = v.getCantidad() * v.getPrecioUnitario();

            try {
                float montoActual = mapaProductos.get(producto);
                mapaProductos.put(producto, montoActual + monto);
            } catch (...) {
                mapaProductos.put(producto, monto);
            }
        }
    }

    float montoProducto1 = 0.0f;
    float montoProducto2 = 0.0f;

    try { montoProducto1 = mapaProductos.get(producto1); } catch (...) {}
    try { montoProducto2 = mapaProductos.get(producto2); } catch (...) {}

    cout << "\n=== Comparacion de Productos en '" << paisBuscado << "' ===\n";
    cout << producto1 << ": $" << montoProducto1 << endl;
    cout << producto2 << ": $" << montoProducto2 << endl;

    if (montoProducto1 > montoProducto2) {
        cout << "El producto con mayor monto total de ventas en '" << paisBuscado << "' es " << producto1
             << ", con un total de $" << montoProducto1 << ".\n";
        contadorIf++;
    } else if (montoProducto2 > montoProducto1) {
        cout << "El producto con mayor monto total de ventas en '" << paisBuscado << "' es " << producto2
             << ", con un total de $" << montoProducto2 << ".\n";
        contadorIf++;
    } else {
        cout << "Ambos productos tienen el mismo monto total de ventas en '" << paisBuscado << "': $" << montoProducto1 << ".\n";
        contadorIf++;
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tiempo de ejecucion: " << elapsed_secs << " segundos.\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << endl;
}

void productosPorPromedioUmbralMayor() {
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Productos vendidos por encima de umbral por pais]\n";
    cout << "Algoritmo: HashMapList\n";

    string paisBuscado;
    float umbral;

    cout << "Ingrese el nombre del pais: ";
    cin.ignore();
    getline(cin, paisBuscado);

    cout << "Ingrese el umbral (monto promedio): ";
    cin >> umbral;

    HashMapList<string, pair<float, int>> mapaProductoDatos(50);

    for (auto& v : vectorVentas) {
        contadorIf++;
        if (v.getPais() == paisBuscado) {
            string producto = v.getProducto();
            float monto = v.getCantidad() * v.getPrecioUnitario();
            pair<float, int> datos;

            try {
                datos = mapaProductoDatos.get(producto);
                datos.first += monto;
                datos.second += v.getCantidad();
            } catch (...) {
                datos = make_pair(monto, v.getCantidad());
            }

            mapaProductoDatos.put(producto, datos);
        }
    }

    bool seEncontro = false;

    cout << "\n=== Productos con promedio de ventas mayor a $" << umbral << " en '" << paisBuscado << "' ===\n";

    for (int i = 0; i < (int)mapaProductoDatos.getTableSize(); i++) {
        Lista<HashEntry<string, pair<float, int>>>* lista = mapaProductoDatos.getList(i);
        if (lista != nullptr && !lista->esVacia()) {
            Nodo<HashEntry<string, pair<float, int>>>* nodo = lista->getInicio();
            while (nodo != nullptr) {
                string producto = nodo->getDato().getClave();
                float montoTotal = nodo->getDato().getValor().first;
                int cantidad = nodo->getDato().getValor().second;
                float promedio = (cantidad > 0) ? (montoTotal / cantidad) : 0.0f;

                contadorIf++;
                if (promedio > umbral) {
                    cout << "Producto: " << producto
                         << " | Promedio de ventas: $" << promedio << endl;
                    seEncontro = true;
                }

                nodo = nodo->getSiguiente();
            }
        }
    }

    if (!seEncontro) {
        cout << "No se encontraron productos con promedio mayor a $" << umbral << " en '" << paisBuscado << "'.\n";
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tiempo de ejecucion: " << elapsed_secs << " segundos.\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << endl;
}

void productosPorPromedioUmbralMenor() {
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Productos vendidos por debajo de umbral por pais]\n";
    cout << "Algoritmo: HashMapList\n";

    string paisBuscado;
    float umbral;

    cout << "Ingrese el nombre del pais: ";
    cin.ignore();
    getline(cin, paisBuscado);

    cout << "Ingrese el umbral (monto promedio): ";
    cin >> umbral;

    HashMapList<string, pair<float, int>> mapaProductoDatos(50);

    for (auto& v : vectorVentas) {
        contadorIf++;
        if (v.getPais() == paisBuscado) {
            string producto = v.getProducto();
            float monto = v.getCantidad() * v.getPrecioUnitario();
            pair<float, int> datos;

            try {
                datos = mapaProductoDatos.get(producto);
                datos.first += monto;
                datos.second += v.getCantidad();
            } catch (...) {
                datos = make_pair(monto, v.getCantidad());
            }

            mapaProductoDatos.put(producto, datos);
        }
    }

    bool seEncontro = false;

    cout << "\n=== Productos con promedio de ventas menor a $" << umbral << " en '" << paisBuscado << "' ===\n";

    for (int i = 0; i < (int)mapaProductoDatos.getTableSize(); i++) {
        Lista<HashEntry<string, pair<float, int>>>* lista = mapaProductoDatos.getList(i);
        if (lista != nullptr && !lista->esVacia()) {
            Nodo<HashEntry<string, pair<float, int>>>* nodo = lista->getInicio();
            while (nodo != nullptr) {
                string producto = nodo->getDato().getClave();
                float montoTotal = nodo->getDato().getValor().first;
                int cantidad = nodo->getDato().getValor().second;
                float promedio = (cantidad > 0) ? (montoTotal / cantidad) : 0.0f;

                contadorIf++;
                if (promedio < umbral) {
                    cout << "Producto: " << producto
                         << " | Promedio de ventas: $" << promedio << endl;
                    seEncontro = true;
                }

                nodo = nodo->getSiguiente();
            }
        }
    }

    if (!seEncontro) {
        cout << "No se encontraron productos con promedio menor a $" << umbral << " en '" << paisBuscado << "'.\n";
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tiempo de ejecucion: " << elapsed_secs << " segundos.\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << endl;
}

void compararProductosTodosPaises() {
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Comparar productos discriminado por todos los paises]\n";
    cout << "Algoritmo: HashMapList\n";

    string producto1, producto2;

    cout << "Ingrese el primer producto: ";
    cin.ignore();
    getline(cin, producto1);

    cout << "Ingrese el segundo producto: ";
    getline(cin, producto2);

    HashMapList<string, pair<int, float>> mapaProductos(50);

    for (auto& v : vectorVentas) {
        contadorIf++;
        string producto = v.getProducto();
        int cantidad = v.getCantidad();
        float monto = cantidad * v.getPrecioUnitario();

        pair<int, float> datos;

        try {
            datos = mapaProductos.get(producto);
            datos.first += cantidad;
            datos.second += monto;
        } catch (...) {
            datos = make_pair(cantidad, monto);
        }

        mapaProductos.put(producto, datos);
    }

    int cantidadProducto1 = 0;
    float montoProducto1 = 0.0f;

    int cantidadProducto2 = 0;
    float montoProducto2 = 0.0f;

    try {
        pair<int, float> datos1 = mapaProductos.get(producto1);
        cantidadProducto1 = datos1.first;
        montoProducto1 = datos1.second;
    } catch (...) {}

    try {
        pair<int, float> datos2 = mapaProductos.get(producto2);
        cantidadProducto2 = datos2.first;
        montoProducto2 = datos2.second;
    } catch (...) {}

    cout << "\n=== Comparacion de Productos en Todos los Paises ===\n";
    cout << producto1 << ": " << cantidadProducto1 << " unidades, $" << montoProducto1 << " total\n";
    cout << producto2 << ": " << cantidadProducto2 << " unidades, $" << montoProducto2 << " total\n";

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tiempo de ejecucion: " << elapsed_secs << " segundos.\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << endl;
}

void compararProductosMasVendidosPorPais() {
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Comparar productos mas vendidos por pais]\n";
    cout << "Algoritmo: HashMapList\n"; 

    string pais1, pais2;

    cout << "Ingrese el primer pais: ";
    cin.ignore();
    getline(cin, pais1);

    cout << "Ingrese el segundo pais: ";
    getline(cin, pais2);

    HashMapList<string, int> contadorPais1(50);
    HashMapList<string, int> contadorPais2(50);

    for (auto& v : vectorVentas) {
        contadorIf++;
        if (v.getPais() == pais1) {
            try {
                int cant = contadorPais1.get(v.getProducto());
                contadorPais1.put(v.getProducto(), cant + v.getCantidad());
            } catch (...) {
                contadorPais1.put(v.getProducto(), v.getCantidad());
            }
        } else if (v.getPais() == pais2) {
            try {
                int cant = contadorPais2.get(v.getProducto());
                contadorPais2.put(v.getProducto(), cant + v.getCantidad());
            } catch (...) {
                contadorPais2.put(v.getProducto(), v.getCantidad());
            }
        }
    }

    string productoMaxPais1;
    int cantidadMaxPais1 = -1;

    for (int i = 0; i < (int)contadorPais1.getTableSize(); i++) {
        Lista<HashEntry<string, int>>* lista = contadorPais1.getList(i);
        if (lista != nullptr && !lista->esVacia()) {
            Nodo<HashEntry<string, int>>* nodo = lista->getInicio();
            while (nodo != nullptr) {
                contadorIf++;
                if (nodo->getDato().getValor() > cantidadMaxPais1) {
                    cantidadMaxPais1 = nodo->getDato().getValor();
                    productoMaxPais1 = nodo->getDato().getClave();
                }
                nodo = nodo->getSiguiente();
            }
        }
    }

    string productoMaxPais2;
    int cantidadMaxPais2 = -1;

    for (int i = 0; i < (int)contadorPais2.getTableSize(); i++) {
        Lista<HashEntry<string, int>>* lista = contadorPais2.getList(i);
        if (lista != nullptr && !lista->esVacia()) {
            Nodo<HashEntry<string, int>>* nodo = lista->getInicio();
            while (nodo != nullptr) {
                contadorIf++;
                if (nodo->getDato().getValor() > cantidadMaxPais2) {
                    cantidadMaxPais2 = nodo->getDato().getValor();
                    productoMaxPais2 = nodo->getDato().getClave();
                }
                nodo = nodo->getSiguiente();
            }
        }
    }

    cout << "\n=== Productos mas vendidos ===\n";
    if (cantidadMaxPais1 >= 0) {
        cout << pais1 << ": " << productoMaxPais1 << " con " << cantidadMaxPais1 << " unidades.\n";
    } else {
        cout << "No se encontraron ventas en '" << pais1 << "'.\n";
    }

    if (cantidadMaxPais2 >= 0) {
        cout << pais2 << ": " << productoMaxPais2 << " con " << cantidadMaxPais2 << " unidades.\n";
    } else {
        cout << "No se encontraron ventas en '" << pais2 << "'.\n";
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tiempo de ejecucion: " << elapsed_secs << " segundos.\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << endl;
}
void compararMedioEnvioMasUsadoPorPais() {
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Comparar medio de envio mas usado por pais]\n";
    cout << "Algoritmo:  HashMapList\n";

    string pais1, pais2;

    cout << "Ingrese el primer pais: ";
    cin.ignore();
    getline(cin, pais1);

    cout << "Ingrese el segundo pais: ";
    getline(cin, pais2);

    auto encontrarMasFrecuente = [&](const string& pais) {
        HashMapList<string, int> mapaMedios(50);

        for (auto& v : vectorVentas) {
            contadorIf++;
            if (v.getPais() == pais) {
                string medio = v.getMedioEnvio();
                try {
                    int cantActual = mapaMedios.get(medio);
                    mapaMedios.put(medio, cantActual + 1);
                } catch (...) {
                    mapaMedios.put(medio, 1);
                }
            }
        }

        string medioMax;
        int cantidadMax = -1;

        for (int i = 0; i < (int)mapaMedios.getTableSize(); i++) {
            Lista<HashEntry<string, int>>* listaMedios = mapaMedios.getList(i);
            if (listaMedios != nullptr && !listaMedios->esVacia()) {
                Nodo<HashEntry<string, int>>* nodoMedio = listaMedios->getInicio();
                while (nodoMedio != nullptr) {
                    contadorIf++;
                    if (nodoMedio->getDato().getValor() > cantidadMax) {
                        cantidadMax = nodoMedio->getDato().getValor();
                        medioMax = nodoMedio->getDato().getClave();
                    }
                    nodoMedio = nodoMedio->getSiguiente();
                }
            }
        }

        return make_pair(medioMax, cantidadMax);
    };

    auto resultado1 = encontrarMasFrecuente(pais1);
    auto resultado2 = encontrarMasFrecuente(pais2);

    cout << "\n=== Medios de Envio mas usados ===\n";
    if (resultado1.second >= 0) {
        cout << pais1 << ": " << resultado1.first << " con " << resultado1.second << " envios.\n";
    } else {
        cout << "No se encontraron datos para el pais '" << pais1 << "'.\n";
    }

    if (resultado2.second >= 0) {
        cout << pais2 << ": " << resultado2.first << " con " << resultado2.second << " envios.\n";
    } else {
        cout << "No se encontraron datos para el pais '" << pais2 << "'.\n";
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tiempo de ejecucion: " << elapsed_secs << " segundos.\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << endl;
}


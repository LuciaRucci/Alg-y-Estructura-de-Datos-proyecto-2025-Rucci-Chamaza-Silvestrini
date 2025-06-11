#include "MenuEstadisticas.h"
#include "Ventas.h"
#include "HashMapList.h"
#include "HashMap.h"
#include "Lista.h"
#include "Nodo.h"
#include "quickSort.h"
#include <ctime>
#include <iostream>
#include <string>
#include <limits> 
#include <vector>

using namespace std;

extern vector<Venta> vectorVentas;


void MenuEstadisticas() {
    int opcion;
    do {
        cout << "\n=== MENU ESTADISTICAS DE VENTAS ===\n";
        cout << "1 - Ciudad con mayor monto total de ventas por pais\n";
        cout << "2 - Producto mas vendido en cantidad\n";
        cout << "3 - Producto menos vendido en cantidad\n";
        cout << "4 - Dia con mayor monto total de ventas\n";
        cout << "5 - Monto total vendido por producto y pais\n";
        cout << "6 - Promedio de ventas por categoria y pais\n";
        cout << "7 - Medio de envio mas utilizado por pais\n";
        cout << "8 - Medio de envio mas utilizado por categoria\n";
        cout << "9 - Estado de envio mas frecuente por pais\n";
        cout << "0 - Volver al menu principal\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: mostrarTop5CiudadesPorPais(); break;
            case 2: mostrarProductoMasVendido(); break;
            case 3: mostrarProductoMenosVendido(); break;
            case 4: mostrarDiaMayorVenta(); break;
            case 5: mostrarMontoTotalPorProductoYPais(); break;
            case 6: mostrarPromedioPorCategoriaYPais(); break;
            case 7: mostrarMedioEnvioMasUtilizadoporPais(); break;
            case 8: mostrarMedioEnvioMasUtilizadoPorCategoria(); break;
            case 9: mostrarEstadoEnvioFrecuente(); break;
            case 0: cout << "Volviendo al menu principal...\n"; break;
            default: cout << "Opcion invalida.\n";
        }

    } while (opcion != 0);
}


void mostrarTop5CiudadesPorPais() {
    cout << "Comenzando a medir Tiempo\n";
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Top 5 ciudades con mayor monto total de ventas por pais]\n";
    cout << "Algoritmo: HashMapList + QuickSort\n";

    HashMapList<string, HashMapList<string, float>*> mapaPaisCiudadMonto(50);

    // Paso 1 Agrupo las ventas
    for (auto& v : vectorVentas) {
        string pais = v.getPais();
        string ciudad = v.getCiudad();
        float monto = v.getCantidad() * v.getPrecioUnitario();

        contadorIf++;  // if pais.empty
        if (pais.empty()) continue;

        contadorIf++;  // if ciudad.empty
        if (ciudad.empty()) continue;

        HashMapList<string, float>* mapaCiudades;

        try {
            mapaCiudades = mapaPaisCiudadMonto.get(pais);
        } catch (...) {
            mapaCiudades = new HashMapList<string, float>(50);
        }

        try {
            float montoActual = mapaCiudades->get(ciudad);
            mapaCiudades->put(ciudad, montoActual + monto);
        } catch (...) {
            mapaCiudades->put(ciudad, monto);
        }

        // Guardamos el puntero en el HashMapList de pais → ciudad
        mapaPaisCiudadMonto.put(pais, mapaCiudades);
    }

    // Paso 2 → Recorro el HashMapList y hago el Top 5
    for (int i = 0; i < (int)mapaPaisCiudadMonto.getTableSize(); i++) {
        Lista<HashEntry<string, HashMapList<string, float>*>>* lista = mapaPaisCiudadMonto.getList(i);
        if (lista != nullptr && !lista->esVacia()) {
            Nodo<HashEntry<string, HashMapList<string, float>*>>* nodo = lista->getInicio();
            while (nodo != nullptr) {
                string pais = nodo->getDato().getClave();
                HashMapList<string, float>* mapaCiudades = nodo->getDato().getValor();

                vector<pair<string, float>> vectorCiudades;

                for (int j = 0; j < (int)mapaCiudades->getTableSize(); j++) {
                    Lista<HashEntry<string, float>>* listaCiudades = mapaCiudades->getList(j);
                    if (listaCiudades != nullptr && !listaCiudades->esVacia()) {
                        Nodo<HashEntry<string, float>>* nodoCiudad = listaCiudades->getInicio();
                        while (nodoCiudad != nullptr) {
                            vectorCiudades.push_back(make_pair(nodoCiudad->getDato().getClave(), nodoCiudad->getDato().getValor()));
                            nodoCiudad = nodoCiudad->getSiguiente();
                        }
                    }
                }

                // Ordeno con QuickSort
                quickSort(vectorCiudades, 0, vectorCiudades.size() - 1,
                    [](const pair<string, float>& a, const pair<string, float>& b) {
                        return a.second > b.second;
                    });

                cout << "\nPais: " << pais << endl;
                int top = min(5, (int)vectorCiudades.size());
                contadorIf++;  // if para top

                for (int k = 0; k < top; k++) {
                    cout << k + 1 << ". Ciudad: " << vectorCiudades[k].first
                         << " | Monto total: $" << vectorCiudades[k].second << endl;
                }

                nodo = nodo->getSiguiente();
            }
        }
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tardo en segundos " << elapsed_secs << "\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << "\n" << endl;
}



// Recorre todas las ventas cargadas en vectorVentas.
// Para cada producto, acumula la cantidad total de unidades vendidas en un HashMapList (producto → cantidad).
// Luego recorre el HashMapList para encontrar el producto con mayor cantidad vendida.
// Finalmente muestra el producto más vendido y la cantidad de unidades vendidas.
void mostrarProductoMasVendido() {
    cout << "Comenzando a medir Tiempo\n";
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Producto mas vendido en cantidad]\n";
    cout << "Algoritmo: HashMapList\n";

    HashMapList<string, int> mapaProductoCantidad(50);

    for (auto& v : vectorVentas) {
        string producto = v.getProducto();
        int cantidad = v.getCantidad();

        contadorIf++;
        if (producto.empty()) continue;

        try {
            int cantidadActual = mapaProductoCantidad.get(producto);
            mapaProductoCantidad.put(producto, cantidadActual + cantidad);
        } catch (...) {
            mapaProductoCantidad.put(producto, cantidad);
        }
    }

    string productoMax;
    int cantidadMax = -1;

    for (int i = 0; i < (int)mapaProductoCantidad.getTableSize(); i++) {
        Lista<HashEntry<string, int>>* lista = mapaProductoCantidad.getList(i);
        if (lista != nullptr && !lista->esVacia()) {
            Nodo<HashEntry<string, int>>* nodo = lista->getInicio();
            while (nodo != nullptr) {
                contadorIf++;
                if (nodo->getDato().getValor() > cantidadMax) {
                    cantidadMax = nodo->getDato().getValor();
                    productoMax = nodo->getDato().getClave();
                }
                nodo = nodo->getSiguiente();
            }
        }
    }

    contadorIf++;
    if (cantidadMax >= 0) {
        cout << "El producto mas vendido es " << productoMax
             << ", con un total de " << cantidadMax << " unidades vendidas." << endl;
    } else {
        cout << "No se encontraron productos en el registro." << endl;
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tardo en segundos " << elapsed_secs << "\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << "\n" << endl;
}

// Recorre todas las ventas cargadas en vectorVentas.
// Para cada producto, acumula la cantidad total de unidades vendidas en un HashMapList (producto → cantidad).
// Luego recorre el HashMapList para encontrar el producto con menor cantidad vendida (el que menos unidades tiene).
// Finalmente muestra el producto menos vendido y la cantidad de unidades vendidas.
void mostrarProductoMenosVendido() {
    cout << "Comenzando a medir Tiempo\n";
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Producto menos vendido en cantidad]\n";
    cout << "Algoritmo: HashMapList\n";

    HashMapList<string, int> mapaProductoCantidad(50);

    for (auto& v : vectorVentas) {
        string producto = v.getProducto();
        int cantidad = v.getCantidad();

        contadorIf++;
        if (producto.empty()) continue;

        try {
            int cantidadActual = mapaProductoCantidad.get(producto);
            mapaProductoCantidad.put(producto, cantidadActual + cantidad);
        } catch (...) {
            mapaProductoCantidad.put(producto, cantidad);
        }
    }

    string productoMin;
    int cantidadMin = numeric_limits<int>::max();

    for (int i = 0; i < (int)mapaProductoCantidad.getTableSize(); i++) {
        Lista<HashEntry<string, int>>* lista = mapaProductoCantidad.getList(i);
        if (lista != nullptr && !lista->esVacia()) {
            Nodo<HashEntry<string, int>>* nodo = lista->getInicio();
            while (nodo != nullptr) {
                contadorIf++;
                if (nodo->getDato().getValor() < cantidadMin) {
                    cantidadMin = nodo->getDato().getValor();
                    productoMin = nodo->getDato().getClave();
                }
                nodo = nodo->getSiguiente();
            }
        }
    }

    contadorIf++;
    if (cantidadMin < numeric_limits<int>::max()) {
        cout << "El producto menos vendido es " << productoMin
             << ", con un total de " << cantidadMin << " unidades vendidas." << endl;
    } else {
        cout << "No se encontraron productos en el registro." << endl;
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tardo en segundos " << elapsed_secs << "\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << "\n" << endl;
}

// Recorre todas las ventas cargadas en vectorVentas.
// Para cada día (fecha), acumula el monto total de ventas en un HashMapList (fecha → monto total).
// Luego recorre el HashMapList para encontrar el día con mayor monto total de ventas.
// Finalmente muestra ese día y el monto total correspondiente.
void mostrarDiaMayorVenta() {
    cout << "Comenzando a medir Tiempo\n";
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Dia con mayor monto total de ventas]\n";
    cout << "Algoritmo: HashMapList\n";

    HashMapList<string, float> mapaDiaMonto(50);

    for (auto& v : vectorVentas) {
        string dia = v.getFecha();
        float monto = v.getCantidad() * v.getPrecioUnitario();

        contadorIf++;
        if (dia.empty()) continue;

        try {
            float montoActual = mapaDiaMonto.get(dia);
            mapaDiaMonto.put(dia, montoActual + monto);
        } catch (...) {
            mapaDiaMonto.put(dia, monto);
        }
    }

    string diaMax;
    float montoMax = -1.0f;

    for (int i = 0; i < (int)mapaDiaMonto.getTableSize(); i++) {
        Lista<HashEntry<string, float>>* lista = mapaDiaMonto.getList(i);
        if (lista != nullptr && !lista->esVacia()) {
            Nodo<HashEntry<string, float>>* nodo = lista->getInicio();
            while (nodo != nullptr) {
                contadorIf++;
                if (nodo->getDato().getValor() > montoMax) {
                    montoMax = nodo->getDato().getValor();
                    diaMax = nodo->getDato().getClave();
                }
                nodo = nodo->getSiguiente();
            }
        }
    }

    contadorIf++;
    if (montoMax >= 0.0f) {
        cout << "El dia con mayor monto total de ventas es " << diaMax
             << ", con un total de $" << montoMax << "." << endl;
    } else {
        cout << "No se encontraron registros de ventas por dia." << endl;
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tardo en segundos " << elapsed_secs << "\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << "\n" << endl;
}


// Recorre todas las ventas cargadas en vectorVentas.
// Para cada país, acumula el monto total vendido de cada producto utilizando un HashMapList anidado:
// mapaPaisProductoMonto: país → (producto → monto total).
// Luego recorre ese mapa para mostrar, por cada país, el monto total vendido de cada producto.
void mostrarMontoTotalPorProductoYPais() {
    cout << "Comenzando a medir Tiempo\n";
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Monto total vendido por producto y pais]\n";
    cout << "Algoritmo: HashMapList\n";

    HashMapList<string, HashMapList<string, float>*> mapaPaisProductoMonto(50);

    for (auto& v : vectorVentas) {
        string pais = v.getPais();
        string producto = v.getProducto();
        float monto = v.getCantidad() * v.getPrecioUnitario();

        contadorIf++;
        if (pais.empty() || producto.empty()) continue;

        HashMapList<string, float>* mapaProductos;

        try {
            mapaProductos = mapaPaisProductoMonto.get(pais);
        } catch (...) {
            mapaProductos = new HashMapList<string, float>(50);
        }

        try {
            float montoActual = mapaProductos->get(producto);
            mapaProductos->put(producto, montoActual + monto);
        } catch (...) {
            mapaProductos->put(producto, monto);
        }

        mapaPaisProductoMonto.put(pais, mapaProductos);
    }

    for (int i = 0; i < (int)mapaPaisProductoMonto.getTableSize(); i++) {
        Lista<HashEntry<string, HashMapList<string, float>*>>* lista = mapaPaisProductoMonto.getList(i);
        if (lista != nullptr && !lista->esVacia()) {
            Nodo<HashEntry<string, HashMapList<string, float>*>>* nodo = lista->getInicio();
            while (nodo != nullptr) {
                string pais = nodo->getDato().getClave();
                HashMapList<string, float>* mapaProductos = nodo->getDato().getValor();

                cout << "\nPais: " << pais << endl;
                for (int j = 0; j < (int)mapaProductos->getTableSize(); j++) {
                    Lista<HashEntry<string, float>>* listaProductos = mapaProductos->getList(j);
                    if (listaProductos != nullptr && !listaProductos->esVacia()) {
                        Nodo<HashEntry<string, float>>* nodoProducto = listaProductos->getInicio();
                        while (nodoProducto != nullptr) {
                            cout << "Producto: " << nodoProducto->getDato().getClave()
                                 << " | Monto total: $" << nodoProducto->getDato().getValor() << endl;
                            nodoProducto = nodoProducto->getSiguiente();
                        }
                    }
                }

                nodo = nodo->getSiguiente();
            }
        }
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tardo en segundos " << elapsed_secs << "\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << "\n" << endl;
}


// Recorre todas las ventas cargadas en vectorVentas.
// Para cada país, acumula el monto total y la cantidad de ventas de cada categoría, usando un HashMapList anidado:
// mapaPaisCategoria: país → (categoría → (monto total, cantidad de ventas)).
// Luego recorre ese mapa y calcula el promedio de ventas por categoría en cada país: promedio = monto total / cantidad de ventas.
// Finalmente muestra el promedio por categoría para cada país.
void mostrarPromedioPorCategoriaYPais() {
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Promedio de ventas por categoria y pais]\n";
    cout << "Algoritmo: Recorrido secuencial + agrupacion con HashMapList anidado + calculo de promedio\n";
    cout << "Estructura utilizada: vectorVentas + HashMapList<string, HashMapList<string, pair<float, int>>>\n";

    HashMapList<string, HashMapList<string, pair<float, int>>*> mapaPaisCategoria(50);

    for (auto& v : vectorVentas) {
        string pais = v.getPais();
        string categoria = v.getCategoria();
        float monto = v.getCantidad() * v.getPrecioUnitario();

        contadorIf++;
        if (pais.empty() || categoria.empty()) continue;

        HashMapList<string, pair<float, int>>* mapaCategorias;

        try {
            mapaCategorias = mapaPaisCategoria.get(pais);
        } catch (...) {
            mapaCategorias = new HashMapList<string, pair<float, int>>(50);
        }

        try {
            pair<float, int> datos = mapaCategorias->get(categoria);
            datos.first += monto;
            datos.second += 1;
            mapaCategorias->put(categoria, datos);
        } catch (...) {
            mapaCategorias->put(categoria, make_pair(monto, 1));
        }

        mapaPaisCategoria.put(pais, mapaCategorias);
    }

    for (int i = 0; i < (int)mapaPaisCategoria.getTableSize(); i++) {
        Lista<HashEntry<string, HashMapList<string, pair<float, int>>*>>* lista = mapaPaisCategoria.getList(i);
        if (lista != nullptr && !lista->esVacia()) {
            Nodo<HashEntry<string, HashMapList<string, pair<float, int>>*>>* nodo = lista->getInicio();
            while (nodo != nullptr) {
                string pais = nodo->getDato().getClave();
                HashMapList<string, pair<float, int>>* mapaCategorias = nodo->getDato().getValor();

                cout << "\nPais: " << pais << endl;

                for (int j = 0; j < (int)mapaCategorias->getTableSize(); j++) {
                    Lista<HashEntry<string, pair<float, int>>>* listaCategorias = mapaCategorias->getList(j);
                    if (listaCategorias != nullptr && !listaCategorias->esVacia()) {
                        Nodo<HashEntry<string, pair<float, int>>>* nodoCategoria = listaCategorias->getInicio();
                        while (nodoCategoria != nullptr) {
                            float montoTotal = nodoCategoria->getDato().getValor().first;
                            int cantidad = nodoCategoria->getDato().getValor().second;
                            float promedio = (cantidad > 0) ? (montoTotal / cantidad) : 0.0f;

                            contadorIf++;
                            cout << "Categoria: " << nodoCategoria->getDato().getClave()
                                 << " | Promedio de ventas: $" << promedio << endl;

                            nodoCategoria = nodoCategoria->getSiguiente();
                        }
                    }
                }

                nodo = nodo->getSiguiente();
            }
        }
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tardo en segundos " << elapsed_secs << "\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << "\n" << endl;
}

// Recorre todas las ventas cargadas en vectorVentas.
// Para cada país, cuenta la cantidad de veces que se utilizó cada medio de envío, usando un HashMapList anidado:
// mapaPaisMedioEnvioCantidad: país → (medio de envío → cantidad de envíos).
// Luego recorre ese mapa y determina cuál es el medio de envío más utilizado en cada país.
// Finalmente muestra el medio de envío más usado por país y su cantidad.
void mostrarMedioEnvioMasUtilizadoporPais() {
    cout << "Comenzando a medir Tiempo\n";
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Medio de envio mas utilizado por pais]\n";
    cout << "Algoritmo: HashMapList\n";

    HashMapList<string, HashMapList<string, int>*> mapaPaisMedioEnvioCantidad(50);

    for (auto& v : vectorVentas) {
        string pais = v.getPais();
        string medioEnvio = v.getMedioEnvio();

        contadorIf++;
        if (pais.empty() || medioEnvio.empty()) continue;

        HashMapList<string, int>* mapaMedios;

        try {
            mapaMedios = mapaPaisMedioEnvioCantidad.get(pais);
        } catch (...) {
            mapaMedios = new HashMapList<string, int>(50);
        }

        try {
            int cantActual = mapaMedios->get(medioEnvio);
            mapaMedios->put(medioEnvio, cantActual + 1);
        } catch (...) {
            mapaMedios->put(medioEnvio, 1);
        }

        mapaPaisMedioEnvioCantidad.put(pais, mapaMedios);
    }

    for (int i = 0; i < (int)mapaPaisMedioEnvioCantidad.getTableSize(); i++) {
        Lista<HashEntry<string, HashMapList<string, int>*>>* lista = mapaPaisMedioEnvioCantidad.getList(i);
        if (lista != nullptr && !lista->esVacia()) {
            Nodo<HashEntry<string, HashMapList<string, int>*>>* nodo = lista->getInicio();
            while (nodo != nullptr) {
                string pais = nodo->getDato().getClave();
                HashMapList<string, int>* mapaMedios = nodo->getDato().getValor();

                string medioMax;
                int cantidadMax = -1;

                for (int j = 0; j < (int)mapaMedios->getTableSize(); j++) {
                    Lista<HashEntry<string, int>>* listaMedios = mapaMedios->getList(j);
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

                cout << "El medio de envio mas utilizado en " << pais
                     << " es '" << medioMax << "' con " << cantidadMax << " envios." << endl;

                nodo = nodo->getSiguiente();
            }
        }
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tardo en segundos " << elapsed_secs << "\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << "\n" << endl;
}

// Recorre todas las ventas cargadas en vectorVentas.
// Para cada categoría, cuenta la cantidad de veces que se utilizó cada medio de envío, usando un HashMapList anidado:
// mapaCategoriaMedioEnvioCantidad: categoría → (medio de envío → cantidad de envíos).
// Luego recorre ese mapa y determina cuál es el medio de envío más utilizado en cada categoría.
// Finalmente muestra el medio de envío más usado por categoría y su cantidad.
void mostrarMedioEnvioMasUtilizadoPorCategoria() {
    cout << "Comenzando a medir Tiempo\n";
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Medio de envio mas utilizado por categoria]\n";
    cout << "Algoritmo: HashMapList \n";

    HashMapList<string, HashMapList<string, int>*> mapaCategoriaMedioEnvioCantidad(50);

    for (auto& v : vectorVentas) {
        string categoria = v.getCategoria();
        string medioEnvio = v.getMedioEnvio();

        contadorIf++;
        if (categoria.empty() || medioEnvio.empty()) continue;

        HashMapList<string, int>* mapaMedios;

        try {
            mapaMedios = mapaCategoriaMedioEnvioCantidad.get(categoria);
        } catch (...) {
            mapaMedios = new HashMapList<string, int>(50);
        }

        try {
            int cantActual = mapaMedios->get(medioEnvio);
            mapaMedios->put(medioEnvio, cantActual + 1);
        } catch (...) {
            mapaMedios->put(medioEnvio, 1);
        }

        mapaCategoriaMedioEnvioCantidad.put(categoria, mapaMedios);
    }

    for (int i = 0; i < (int)mapaCategoriaMedioEnvioCantidad.getTableSize(); i++) {
        Lista<HashEntry<string, HashMapList<string, int>*>>* lista = mapaCategoriaMedioEnvioCantidad.getList(i);
        if (lista != nullptr && !lista->esVacia()) {
            Nodo<HashEntry<string, HashMapList<string, int>*>>* nodo = lista->getInicio();
            while (nodo != nullptr) {
                string categoria = nodo->getDato().getClave();
                HashMapList<string, int>* mapaMedios = nodo->getDato().getValor();

                string medioMax;
                int cantidadMax = -1;

                for (int j = 0; j < (int)mapaMedios->getTableSize(); j++) {
                    Lista<HashEntry<string, int>>* listaMedios = mapaMedios->getList(j);
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

                cout << "El medio de envio mas utilizado en la categoria '" << categoria
                     << "' es '" << medioMax << "' con " << cantidadMax << " envios." << endl;

                nodo = nodo->getSiguiente();
            }
        }
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tardo en segundos " << elapsed_secs << "\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << "\n" << endl;
}

// Recorre todas las ventas cargadas en vectorVentas.
// Para cada país, cuenta la cantidad de veces que se utilizó cada estado de envío, usando un HashMapList anidado:
// mapaPaisEstadoEnvioCantidad: país → (estado de envío → cantidad de ocurrencias).
// Luego recorre ese mapa y determina cuál es el estado de envío más frecuente en cada país.
// Finalmente muestra el estado de envío más frecuente por país y su cantidad.
void mostrarEstadoEnvioFrecuente() {
    cout << "Comenzando a medir Tiempo\n";
    clock_t begin = clock();
    int contadorIf = 0;

    cout << "[Proceso: Estado de envio mas frecuente por pais]\n";
    cout << "Algoritmo: HashMapList \n";

    HashMapList<string, HashMapList<string, int>*> mapaPaisEstadoEnvioCantidad(50);

    for (auto& v : vectorVentas) {
        string pais = v.getPais();
        string estadoEnvio = v.getEstadoEnvio();

        contadorIf++;
        if (pais.empty() || estadoEnvio.empty()) continue;

        HashMapList<string, int>* mapaEstados;

        try {
            mapaEstados = mapaPaisEstadoEnvioCantidad.get(pais);
        } catch (...) {
            mapaEstados = new HashMapList<string, int>(50);
        }

        try {
            int cantActual = mapaEstados->get(estadoEnvio);
            mapaEstados->put(estadoEnvio, cantActual + 1);
        } catch (...) {
            mapaEstados->put(estadoEnvio, 1);
        }

        mapaPaisEstadoEnvioCantidad.put(pais, mapaEstados);
    }

    for (int i = 0; i < (int)mapaPaisEstadoEnvioCantidad.getTableSize(); i++) {
        Lista<HashEntry<string, HashMapList<string, int>*>>* lista = mapaPaisEstadoEnvioCantidad.getList(i);
        if (lista != nullptr && !lista->esVacia()) {
            Nodo<HashEntry<string, HashMapList<string, int>*>>* nodo = lista->getInicio();
            while (nodo != nullptr) {
                string pais = nodo->getDato().getClave();
                HashMapList<string, int>* mapaEstados = nodo->getDato().getValor();

                string estadoMax;
                int cantidadMax = -1;

                for (int j = 0; j < (int)mapaEstados->getTableSize(); j++) {
                    Lista<HashEntry<string, int>>* listaEstados = mapaEstados->getList(j);
                    if (listaEstados != nullptr && !listaEstados->esVacia()) {
                        Nodo<HashEntry<string, int>>* nodoEstado = listaEstados->getInicio();
                        while (nodoEstado != nullptr) {
                            contadorIf++;
                            if (nodoEstado->getDato().getValor() > cantidadMax) {
                                cantidadMax = nodoEstado->getDato().getValor();
                                estadoMax = nodoEstado->getDato().getClave();
                            }
                            nodoEstado = nodoEstado->getSiguiente();
                        }
                    }
                }

                cout << "El estado de envio mas frecuente en " << pais
                     << " es '" << estadoMax << "' con " << cantidadMax << " envios." << endl;

                nodo = nodo->getSiguiente();
            }
        }
    }

    clock_t end = clock();
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "Tardo en segundos " << elapsed_secs << "\n";
    cout << "Cantidad de IF utilizados: " << contadorIf << "\n" << endl;
}

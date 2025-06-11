#ifndef MENU_ESTADISTICAS_H
#define MENU_ESTADISTICAS_H

#include <string>
#include "HashMapList.h"
#include "HashMap.h"
#include "HashEntry.h"
#include "Ventas.h" // para la clase Venta

// Estructura auxiliar para ciudad + monto
struct CiudadMonto {
    string ciudad;
    float monto;
};

// Declaración de funciones del menú de estadísticas
void MenuEstadisticas();
void mostrarTop5CiudadesPorPais();
void mostrarMontoTotalPorProductoYPais();
void mostrarPromedioPorCategoriaYPais();
void mostrarMedioEnvioMasUtilizadoporPais();
void mostrarMedioEnvioMasUtilizadoPorCategoria();
void mostrarDiaMayorVenta();
void mostrarEstadoEnvioFrecuente();
void mostrarProductoMasVendido();
void mostrarProductoMenosVendido();

#endif // MENU_ESTADISTICAS_H


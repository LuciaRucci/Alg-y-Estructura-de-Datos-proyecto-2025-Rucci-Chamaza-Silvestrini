#ifndef VENTAS_H
#define VENTAS_H

#include <iostream>
#include <string>
#include <vector>  // Para cargarDesdeCSV
#include "HashMapList.h"  // Necesario para declarar ventasMap

using namespace std;


// Variables privadas que representan los campos de la venta.
// Algunos campos son de tipo string, otros numericos.
// monto_total se calcula automáticamente a partir de cantidad y precio_unitario.

class Venta {
private:
    string id;
    string fecha;
    string pais;
    string ciudad;
    string cliente;
    string producto;
    string categoria;
    int cantidad;
    float precio_unitario;
    float monto_total;
    string medio_envio;
    string estado_envio;

public:
    // Constructor completo: inicializa todos los campos a valores vacios o cero.
// Se utiliza cuando se crea una venta vacia que se va a completar despues.

    Venta()
        : id(""), fecha(""), pais(""), ciudad(""), cliente(""), producto(""), categoria(""),
          cantidad(0), precio_unitario(0.0), monto_total(0.0), medio_envio(""), estado_envio("") {}

    // Getters
    string getId() const { return id; }
    string getFecha() const { return fecha; }
    string getPais() const { return pais; }
    string getCiudad() const { return ciudad; }
    string getCliente() const { return cliente; }
    string getProducto() const { return producto; }
    string getCategoria() const { return categoria; }
    int getCantidad() const { return cantidad; }
    float getPrecioUnitario() const { return precio_unitario; }
    float getMontoTotal() const { return monto_total; }
    string getMedioEnvio() const { return medio_envio; }
    string getEstadoEnvio() const { return estado_envio; }

    // Setters
    void setId(const string& v) { id = v; }
    void setFecha(const string& v) { fecha = v; }
    void setPais(const string& v) { pais = v; }
    void setCiudad(const string& v) { ciudad = v; }
    void setCliente(const string& v) { cliente = v; }
    void setProducto(const string& v) { producto = v; }
    void setCategoria(const string& v) { categoria = v; }
    void setCantidad(int v) { cantidad = v; calcularMonto(); }
    void setPrecioUnitario(float v) { precio_unitario = v; calcularMonto(); }
    void setMedioEnvio(const string& v) { medio_envio = v; }
    void setEstadoEnvio(const string& v) { estado_envio = v; }

    // Método para actualizar el monto total
    void calcularMonto() { monto_total = cantidad * precio_unitario; }

    // Comparadores para permitir ordenar y comparar venta sirve para AVL
    // Los utiliza el AVL para decidir cómo insertar, buscar y recorrer las ventas (por ejemplo en orden de fecha)

// Se considera que dos ventas son iguales si tienen la misma fecha y el mismo ID
    bool operator==(const Venta& otra) const {
        return fecha == otra.fecha && id == otra.id;
    }
// Se usa para ordenar las ventas en el AVL (orden cronologico por fecha)
// Permite recorrer las ventas en orden ascendente de fecha (por ejemplo en inorder())
    bool operator<(const Venta& otra) const {
        return fecha < otra.fecha;
    }
// Complementa al anterior = el AVL lo usa para decidir si una venta va a la rama derecha
    bool operator>(const Venta& otra) const {
        return fecha > otra.fecha;
    }

    // Mostrar la venta
    // Se usa en la mayoría de los menús del programa (consultas, estadísticas, modificaciones).
    void imprimirVenta() const {
        cout << "ID: " << id
             << " | Fecha: " << fecha
             << " | Pais: " << pais
             << " | Ciudad: " << ciudad
             << " | Cliente: " << cliente
             << " | Producto: " << producto
             << " | Categoria: " << categoria
             << " | Cantidad: " << cantidad
             << " | Precio Unitario: " << precio_unitario
             << " | Monto Total: " << monto_total
             << " | Medio Envio: " << medio_envio
             << " | Estado Envio: " << estado_envio
             << endl;
    }

//  (<<) para imprimir una Venta directamente con cout.
// Es necesaria para que Venta se pueda imprimir correctamente en ArbolBinarioAVL

// Al definir este operador como 'friend', le damos acceso a los campos privados de Venta.
// Se pasa la Venta como 'const &' para evitar copiarla y garantizar que no sera modificada durante la impresion
friend ostream& operator<<(ostream& os, const Venta& v) {
    os << "ID: " << v.id
       << " | Fecha: " << v.fecha
       << " | Pais: " << v.pais
       << " | Ciudad: " << v.ciudad
       << " | Cliente: " << v.cliente
       << " | Producto: " << v.producto
       << " | Categoria: " << v.categoria
       << " | Cantidad: " << v.cantidad
       << " | Precio Unitario: " << v.precio_unitario
       << " | Monto Total: " << v.monto_total
       << " | Medio Envio: " << v.medio_envio
       << " | Estado Envio: " << v.estado_envio;
    return os;
}


    // Devuelve una clave única compuesta a partir de pais, ciudad, fecha y producto.
    // Esta clave se usa como índice en HashMapList (ventasMap) para identificar las ventas de forma única.

    string getClave() const {
        
        return pais + "_" + ciudad + "_" + fecha + "_" + producto;
    }

    // Carga una venta a partir de un vector de strings que representa una linea del CSV
    // Se usa en la función leerArchivoCSV() para cargar los datos desde el archivo de ventas.
    void cargarDesdeCSV(const vector<string>& campos) {
        setId(campos[0]);
        setFecha(campos[1]);
        setPais(campos[2]);
        setCiudad(campos[3]);
        setCliente(campos[4]);
        setProducto(campos[5]);
        setCategoria(campos[6]);
        setCantidad(stoi(campos[7])); // campo cantidad → de string a int
        setPrecioUnitario(stof(campos[8])); // campo precio_unitario → de string a float
        // campos[9] es monto_total, no lo tomamos porque se calcula
        setMedioEnvio(campos[10]);
        setEstadoEnvio(campos[11]);
    }
};

// Declaración de la variable global que vamos a usar en todo el proyecto:
extern HashMapList<string, Venta> ventasMap;

#endif // VENTAS_H

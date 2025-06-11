#ifndef U05_HASH_HASHMAP_HASHMAP_H_
#define U05_HASH_HASHMAP_HASHMAP_H_

#include "HashEntry.h"
#include "Lista.h" // <<< AGREGADO: esta versión usa Lista para manejar colisiones
#include <sstream> // <<< AGREGADO: para convertir la clave a string en hashFunc
#include <string>
#include <iostream>

using namespace std;

template <class K, class T>
class HashMap {
private:
    Lista<HashEntry<K, T>>* tabla; // <<< CAMBIADO: usa Lista en cada celda para manejar colisiones
    unsigned int tamanio;
    static unsigned int hashFunc(K clave);
    unsigned int (*hashFuncP)(K clave);

public:
    explicit HashMap(unsigned int k);
    HashMap(unsigned int k, unsigned int (*hashFuncP)(K clave));
    ~HashMap();

    void put(K clave, T valor);
    void remove(K clave);
    T get(K clave);
    bool esVacio();

    Lista<HashEntry<K, T>>* getTabla();

    // <<< AGREGADO: metodo recorrer que permite recorrer todo el mapa
    template <typename Funcion>
    void recorrer(Funcion func) const {
        for (int i = 0; i < (int)tamanio; i++) {
            if (tabla[i].esVacia()) continue;
            Nodo<HashEntry<K, T>>* nodo = tabla[i].getInicio();
            while (nodo != nullptr) {
                HashEntry<K, T> entry = nodo->getDato();
                func(entry.getClave(), entry.getValor());
                nodo = nodo->getSiguiente();
            }
        }
    }
};

// Implementación
template <class K, class T>
HashMap<K, T>::HashMap(unsigned int k) {
    tamanio = k;
    tabla = new Lista<HashEntry<K, T>>[tamanio];
    hashFuncP = hashFunc;
}

template <class K, class T>
HashMap<K, T>::HashMap(unsigned int k, unsigned int (*fp)(K)) {
    tamanio = k;
    tabla = new Lista<HashEntry<K, T>>[tamanio];
    hashFuncP = fp;
}

template <class K, class T>
HashMap<K, T>::~HashMap() {
    delete[] tabla;
}

template <class K, class T>
void HashMap<K, T>::put(K clave, T valor) {
    unsigned int pos = hashFuncP(clave) % tamanio;
    tabla[pos].insertarUltimo(HashEntry<K, T>(clave, valor));
}

template <class K, class T>
void HashMap<K, T>::remove(K clave) {
    unsigned int pos = hashFuncP(clave) % tamanio;
    if (tabla[pos].esVacia()) throw 404;

    Nodo<HashEntry<K, T>>* actual = tabla[pos].getInicio();
    int index = 0;
    while (actual != nullptr) {
        if (actual->getDato().getClave() == clave) {
            tabla[pos].remover(index);
            return;
        }
        actual = actual->getSiguiente();
        index++;
    }
    throw 404;
}

template <class K, class T>
T HashMap<K, T>::get(K clave) {
    unsigned int pos = hashFuncP(clave) % tamanio;
    if (tabla[pos].esVacia()) throw 404;

    Nodo<HashEntry<K, T>>* nodo = tabla[pos].getInicio();
    while (nodo != nullptr) {
        if (nodo->getDato().getClave() == clave) {
            return nodo->getDato().getValor();
        }
        nodo = nodo->getSiguiente();
    }
    throw 404;
}

template <class K, class T>
bool HashMap<K, T>::esVacio() {
    for (int i = 0; i < (int)tamanio; i++) {
        if (!tabla[i].esVacia()) return false;
    }
    return true;
}

template <class K, class T>
unsigned int HashMap<K, T>::hashFunc(K clave) {
    unsigned int hash = 0;
    stringstream ss;
    ss << clave;
    string strClave = ss.str();
    for (char c : strClave) {
        hash = hash * 101 + c;
    }
    return hash;
}

template <class K, class T>
Lista<HashEntry<K, T>>* HashMap<K, T>::getTabla() {
    return tabla;
}

#endif // U05_HASH_HASHMAP_HASHMAP_H_

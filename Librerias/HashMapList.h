#ifndef U05_HASH_HASHMAP_HASHMAPLIST_H_
#define U05_HASH_HASHMAP_HASHMAPLIST_H_

#include "HashEntry.h"
#include "Lista.h"
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

template <class K, class T>
class HashMapList {
private:
    Lista<HashEntry<K, T>>** tabla;
    unsigned int tamanio;
    static unsigned int hashFunc(K clave);
    unsigned int (*hashFuncP)(K clave);

public:
    explicit HashMapList(unsigned int k);
    HashMapList(unsigned int k, unsigned int (*hashFuncP)(K clave));

    Lista<HashEntry<K, T>>* getList(int i);
    void put(K clave, T valor);
    void remove(K clave);
    T get(K clave);
    ~HashMapList();
    bool esVacio();

    unsigned int getTableSize() const { return tamanio; }
    void print();

    template <typename Funcion>
    void recorrer(Funcion func) const {
        for (int i = 0; i < (int)tamanio; i++) {
            if (tabla[i] == nullptr) continue;
            Nodo<HashEntry<K, T>>* nodo = tabla[i]->getInicio();
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
HashMapList<K, T>::HashMapList(unsigned int k) {
    tamanio = k;
    tabla = new Lista<HashEntry<K, T>>*[tamanio];
    for (int i = 0; i < (int)tamanio; i++) {
        tabla[i] = nullptr;
    }
    hashFuncP = hashFunc;
}

template <class K, class T>
HashMapList<K, T>::HashMapList(unsigned int k, unsigned int (*fp)(K)) {
    tamanio = k;
    tabla = new Lista<HashEntry<K, T>>*[tamanio];
    for (int i = 0; i < (int)tamanio; i++) {
        tabla[i] = nullptr;
    }
    hashFuncP = fp;
}

template <class K, class T>
HashMapList<K, T>::~HashMapList() {
    for (int i = 0; i < (int)tamanio; i++) {
        if (tabla[i] != nullptr) {
            delete tabla[i];
        }
    }
    delete[] tabla;
}

template <class K, class T>
void HashMapList<K, T>::put(K clave, T valor) {
    unsigned int pos = hashFuncP(clave) % tamanio;

    if (tabla[pos] == nullptr) {
        tabla[pos] = new Lista<HashEntry<K, T>>();
    } else {
        // Buscar si ya existe un HashEntry con esa clave
        Nodo<HashEntry<K, T>>* nodo = tabla[pos]->getInicio();
        int index = 0;
        while (nodo != nullptr) {
            if (nodo->getDato().getClave() == clave) {
                tabla[pos]->remover(index); // Remover el anterior
                break;
            }
            nodo = nodo->getSiguiente();
            index++;
        }
    }

    tabla[pos]->insertarUltimo(HashEntry<K, T>(clave, valor));
}

template <class K, class T>
void HashMapList<K, T>::remove(K clave) {
    unsigned int pos = hashFuncP(clave) % tamanio;
    if (tabla[pos] == nullptr) throw 404;

    Nodo<HashEntry<K, T>>* actual = tabla[pos]->getInicio();
    int index = 0;
    while (actual != nullptr) {
        if (actual->getDato().getClave() == clave) {
            tabla[pos]->remover(index);
            return;
        }
        actual = actual->getSiguiente();
        index++;
    }
    throw 404;
}

template <class K, class T>
T HashMapList<K, T>::get(K clave) {
    unsigned int pos = hashFuncP(clave) % tamanio;
    if (tabla[pos] == nullptr) throw 404;

    Nodo<HashEntry<K, T>>* nodo = tabla[pos]->getInicio();
    while (nodo != nullptr) {
        if (nodo->getDato().getClave() == clave) {
            return nodo->getDato().getValor();
        }
        nodo = nodo->getSiguiente();
    }
    throw 404;
}

template <class K, class T>
bool HashMapList<K, T>::esVacio() {
    for (int i = 0; i < (int)tamanio; i++) {
        if (tabla[i] != nullptr && !tabla[i]->esVacia()) return false;
    }
    return true;
}

template <class K, class T>
unsigned int HashMapList<K, T>::hashFunc(K clave) {
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
Lista<HashEntry<K, T>>* HashMapList<K, T>::getList(int i) {
    if (i >= 0 && i < (int)tamanio) {
        return tabla[i];
    } else {
        return nullptr;
    }
}

template <class K, class T>
void HashMapList<K, T>::print() {
    for (int i = 0; i < (int)tamanio; i++) {
        if (tabla[i] != nullptr && !tabla[i]->esVacia()) {
            cout << "Índice " << i << ": ";
            Nodo<HashEntry<K, T>>* aux = tabla[i]->getInicio();
            while (aux != nullptr) {
                cout << "(" << aux->getDato().getClave() << ", " << aux->getDato().getValor() << ") ";
                aux = aux->getSiguiente();
            }
            cout << endl;
        }
    }
}

#endif // U05_HASH_HASHMAP_HASHMAPLIST_H_

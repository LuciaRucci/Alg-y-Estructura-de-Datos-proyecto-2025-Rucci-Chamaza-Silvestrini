#pragma once

template<class T>
class Nodo{
private:
    T dato;
    Nodo<T> *siguiente;

public:
    // Constructor por defecto
    Nodo() {
        siguiente = nullptr;
    }

    // Constructor con dato
    Nodo(T d) {
        dato = d;
        siguiente = nullptr;
    }

    T getDato(){
        return dato;
    }

    void setDato(T d){
        dato = d;
    }

    Nodo<T> *getSiguiente(){
        return siguiente;
    }

    void setSiguiente(Nodo<T> *siguiente){
        this->siguiente = siguiente;
    }
};

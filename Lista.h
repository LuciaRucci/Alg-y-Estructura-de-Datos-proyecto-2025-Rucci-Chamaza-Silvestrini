#ifndef U02_LISTAS_LISTA_LISTA_H_
#define U02_LISTAS_LISTA_LISTA_H_
#include "Nodo.h"
#include <iostream>
/**
 * Clase que implementa una Lista Enlazada generica, ya que puede
 * almacenar cualquier tipo de dato T
 * @tparam T cualquier tipo de dato
 */
template <class T>
class Lista {
private:
  Nodo<T> *inicio;

public:
  Lista();

  Lista(const Lista<T> &li);

  ~Lista();

  bool esVacia() const ;

  int getTamanio() const;

  void insertar(int pos, T dato);

  void insertarPrimero(T dato);

  void insertarUltimo(T dato);

  void remover(int pos);

  T getDato(int pos) const;

  void reemplazar(int pos, T dato);

  void vaciar();

  void print();

  Nodo<T>* getInicio() const ;
};

/**
 * Constructor de la clase Lista
 * @tparam T
 */
template <class T>
Lista<T>::Lista() {
  inicio = nullptr;
}

/**
 * Constructor por copia de la clase Lista
 * @tparam T
 * @param li
 */
template <class T>
Lista<T>::Lista(const Lista<T> &li) {
  inicio = li.inicio;
}

/**
 * Destructor de la clase Lista, se encarga de liberar la memoria de todos los
 * nodos utilizados en la lista
 * @tparam T
 */
template <class T>
Lista<T>::~Lista() {
  vaciar();
}

/**
 * Metodo para saber si la lista esta vacia
 * @tparam T
 * @return true si la lista esta vacia, sino false
 */
template <class T>
bool Lista<T>::esVacia() const {
  return inicio == nullptr;
}

/**
 * Metodo para obtener la cantidad de nodos de la lista
 * @tparam T
 * @return la cantidad de nodos de la lista
 */
template <class T>
int Lista<T>::getTamanio() const {
  Nodo<T> *aux = inicio;
  int size = 0;

  while (aux != nullptr) {
    aux = aux->getSiguiente();
    size++;
  }

  return size;
}

/**
 * Inserta un nodo con el dato en la posicion pos
 * @tparam T
 * @param pos lugar donde será insertado el dato
 * @param dato  dato a insertar
 */
template <class T>
void Lista<T>::insertar(int pos, T dato) {
  int posActual = 0;
  Nodo<T> *aux = inicio, *nuevo;
  nuevo = new Nodo<T>;
  nuevo->setDato(dato);

  if (pos == 0) {
    nuevo->setSiguiente(inicio);
    inicio = nuevo;
    return;
  }

  while (aux != nullptr && posActual < pos - 1) {
    aux = aux->getSiguiente();
    posActual++;
  }

  if (aux == nullptr) {
    throw 404;
  }

  nuevo->setSiguiente(aux->getSiguiente());
  aux->setSiguiente(nuevo);
}

/**
 * Inserta un nodo con el dato en la primera posicion
 * @tparam T
 * @param dato dato a insertar
 */
template <class T>
void Lista<T>::insertarPrimero(T dato) {
  insertar(0, dato);
}

/**
 * Inserta un nodo con el dato en la ultima posicion
 * @tparam T
 * @param dato dato a insertar
 */
template <class T>
void Lista<T>::insertarUltimo(T dato) {
  Nodo<T> *aux = inicio, *nuevo;
  nuevo = new Nodo<T>;
  nuevo->setDato(dato);

  if (aux == nullptr) {
    nuevo->setSiguiente(inicio);
    inicio = nuevo;
    return;
  }

  while (aux->getSiguiente() != nullptr) {
    aux = aux->getSiguiente();
  }

  nuevo->setSiguiente(aux->getSiguiente());
  aux->setSiguiente(nuevo);
}

/**
 * Elimina el nodo en la posicion 'pos' de la lista enlazada, reenlazando los nodos
 * adecuadamente.
 * @tparam T
 * @param pos posicion del nodo a eliminar
 */
template <class T>
void Lista<T>::remover(int pos) {
    if (inicio == nullptr) {
        throw 400; // Lista vacía
    }

    Nodo<T>* aux = inicio;

    // Caso especial: eliminar el primer nodo
    if (pos == 0) {
        Nodo<T>* aBorrar = inicio;
        inicio = inicio->getSiguiente();
        delete aBorrar;
        return;
    }

    int posActual = 0;

    // Avanzar hasta el nodo anterior al que queremos eliminar
    while (aux != nullptr && posActual < pos - 1) {
        aux = aux->getSiguiente();
        posActual++;
    }

    if (aux == nullptr || aux->getSiguiente() == nullptr) {
        throw 400; // Posición inválida
    }

    Nodo<T>* aBorrar = aux->getSiguiente();

    // Protección extra: evitar ciclos o autolinks
    if (aBorrar == aux) {
        aux->setSiguiente(nullptr);
        delete aBorrar;
        return;
    }

    aux->setSiguiente(aBorrar->getSiguiente());
    delete aBorrar;
}




/**
 * Obtener el dato del nodo en la posicion pos
 * @tparam T
 * @param pos posicion del dato
 * @return dato almacenado en el nodo
 */
template <class T>
T Lista<T>::getDato(int pos) const{
  Nodo<T> *aux = inicio;
  int posActual = 0;

  while (aux != nullptr && posActual < pos) {
    aux = aux->getSiguiente();
    posActual++;
  }

  if (aux == nullptr) {
    throw 404;
  }

  return aux->getDato();
}

/**
 * Reemplaza el dato almacenado en un nodo por este otro
 * @tparam T
 * @param pos posicion donde se desea reemplazar
 * @param dato nuevo dato a almacenar
 */
template <class T>
void Lista<T>::reemplazar(int pos, T dato) {
  Nodo<T> *aux = inicio;
  int posActual = 0;

  while (aux != nullptr && posActual < pos) {
    aux = aux->getSiguiente();
    posActual++;
  }

  if (aux == nullptr) {
    throw 404;
  }

  aux->setDato(dato);
}

/**
 * Función que vacia la lista enlazada
 * @tparam T
 */
template <class T>
void Lista<T>::vaciar() {
    Nodo<T>* actual = inicio;
    Nodo<T>* siguiente = nullptr;

    while (actual != nullptr) {
        siguiente = actual->getSiguiente();

        // Protección extra → cortar posibles ciclos
        if (siguiente == actual) {
            delete actual;
            break;
        }

        delete actual;
        actual = siguiente;
    }

    inicio = nullptr;
}


/**
 * Imprime la lista por consola
 * @tparam T
 */
template <class T>
void Lista<T>::print() {
  Nodo<T> *aux = inicio;

  while (aux != nullptr) {
    std::cout << aux->getDato() << "->";
    aux = aux->getSiguiente();
  }
  std::cout << "NULL" << std::endl;
}

/**
 * Método que devuelve un puntero al primer nodo de la lista
 * @tparam T
 * @return puntero al primer nodo (inicio)
 */
template <class T>
Nodo<T>* Lista<T>::getInicio() const {
  return inicio;
}

#endif // U02_LISTAS_LISTA_LISTA_H_

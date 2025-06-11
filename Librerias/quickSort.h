#ifndef QUICKSORT_H_
#define QUICKSORT_H_

#include <iostream>
using namespace std;

// QuickSort para int[]
void quickSort(int *arr, int inicio, int fin)
{
    int i, j, medio;
    int pivot, aux;

    medio = (inicio + fin) / 2;
    pivot = arr[medio]; // Se elige el pivote como el elemento medio
    i = inicio;
    j = fin;

    do
    {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;

        if (i <= j)
        {
            aux = arr[i];
            arr[i] = arr[j];
            arr[j] = aux;
            i++;
            j--;
        }
    } while (i <= j);

    if (inicio < j)
        quickSort(arr, inicio, j);
    if (i < fin)
        quickSort(arr, i, fin);
}

// === QuickSort para vector<T> con Compare ===
#include <vector>

template <typename T, typename Compare>
void quickSort(vector<T>& vec, int left, int right, Compare comp) {
    if (left >= right) return;

    int i = left;
    int j = right;
    T pivot = vec[(left + right) / 2];

    while (i <= j) {
        while (comp(vec[i], pivot)) i++;
        while (comp(pivot, vec[j])) j--;

        if (i <= j) {
            swap(vec[i], vec[j]);
            i++;
            j--;
        }
    }

    if (left < j)
        quickSort(vec, left, j, comp);
    if (i < right)
        quickSort(vec, i, right, comp);
}

#endif // QUICKSORT_H_

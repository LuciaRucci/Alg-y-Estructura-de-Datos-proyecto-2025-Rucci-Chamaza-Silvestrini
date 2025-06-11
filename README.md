# Alg-y-Estructura-de-Datos-proyecto-2025-Rucci-Chamaza-Silvestrini
# Algoritmos y Estructura de Datos - Proyecto 2025

### Autores:
- Lucía Rucci 2418308
- Florencia Chamaza 2418092
- Mia Silvestrini 2411741

---

## Descripción del Proyecto

Este proyecto consiste en el desarrollo de un sistema para la gestión y análisis de una base de datos de ventas en Sudamérica.  
El trabajo fue realizado para la materia **Algoritmo y estructura de datos** - Ingeniería en Informatica - Universidad Católica de Córdoba.

Se procesaron **2.500 registros** del archivo `ventas_sudamerica.csv` y se implementaron consultas dinámicas, estadísticas y funcionalidades de modificación sobre los datos, respetando los requisitos de la consigna.

---

## Objetivos cumplidos

- Medición de **tiempo de ejecución** y **cantidad de condicionales (IF)** en cada proceso  
- Implementación de **algoritmos eficientes**   
- Procesamiento en tiempo de lectura del CSV  
- Actualización automática tras modificaciones (Agregar, Eliminar, Modificar ventas)  
- Consultas dinámicas por el usuario  
- Código modular y estructurado en librerías  


## Estructuras utilizadas

- **ArbolBinarioAVL** → ordenación y consultas por fecha  
- **HashMapList** → agrupaciones complejas por país, ciudad, producto, medio de envío, categoría, estado de envío  
- **HashMap** → agrupaciones simples  
- **Lista** → soporte para estructura HashMapList  
- **Vector** → carga principal de las ventas (`vectorVentas`)  

---

## Algoritmos utilizados

- **QuickSort** → ordenamiento descendente de ciudades por monto total de ventas  
- **Recorridos de HashMapList** → agrupación y conteo de registros  
- **Recorrido en Orden (InOrder) de AVL** → consulta ordenada por fecha

---

## Menús implementados

### Menú Principal
- Carga del CSV con medición de tiempo y líneas procesadas

### Menú Estadísticas de Ventas
- Top 5 ciudades por país
- Producto más vendido
- Producto menos vendido
- Día con mayor monto total
- Monto total por producto y país
- Promedio por categoría y país
- Medio de envío más utilizado por país
- Medio de envío más utilizado por categoría
- Estado de envío más frecuente por país

### Menú Modificaciones
- Agregar una venta
- Eliminar venta (por país o ciudad)
- Modificar venta (por ID)

### Menú Consultas Dinámicas
- Ventas por ciudad (AVL)
- Ventas en rango de fechas por país (AVL)
- Comparación entre dos países (monto total)
- Comparación entre dos productos por país
- Productos vendidos por encima / debajo de umbral
- Comparación de productos en todos los países
- Productos más vendidos por país
- Medio de envío más usado por país

---

## Medición de Eficiencia

Cada proceso imprime al usuario:
- Tiempo de ejecución (segundos)
- Cantidad de IF utilizados
- Estructura de datos empleada

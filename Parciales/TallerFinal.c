#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

//1. Estructura del Nodo de FIFO Pedidos de Despacho
typedef struct NodoPedido {
    char nombre_destino[40];    
    int cantidad_solicitada;    
    struct NodoPedido *siguiente; 
} Pedido;

// Estructura para el Nodo del Árbol AVL/ABB Inventario/Lote por Fecha
typedef struct NodoAVL {
    int fecha_vencimiento;      
    int stock_total;            
    char nombre_producto[45];   

    // Conexión Híbrida con la Cola FIFO 
    Pedido *cabeza_pedidos;     
    struct NodoAVL *izquierda;  
    struct NodoAVL *derecha;    
    int altura;                 
} Inventario;

// 2. soporte con AVL Y COLA

int obtenerAltura(Inventario *lote_actual) {
    if (lote_actual == NULL)
        return 0;
    return lote_actual->altura;
}

int obtenerFactorEquilibrio(Inventario *lote) {
    if (lote == NULL)
        return 0;
    return obtenerAltura(lote->izquierda) - obtenerAltura(lote->derecha);
}

Inventario* nuevoNodoInventario(int fecha, int unidades, const char* nombre) {
    Inventario* nodo = (Inventario*)malloc(sizeof(Inventario));
    
    if (nodo == NULL) {
        perror("Error de asignación de memoria para Inventario");
        exit(EXIT_FAILURE);
    }

    nodo->fecha_vencimiento = fecha;
    nodo->stock_total = unidades; 
    strcpy(nodo->nombre_producto, nombre); 
    
    nodo->izquierda = NULL;
    nodo->derecha = NULL;
    nodo->altura = 1; 
    nodo->cabeza_pedidos = NULL; 
    
    return nodo;
}

Inventario *buscarMasAntiguo(Inventario *nodo) {
    Inventario *actual = nodo;
    while (actual != NULL && actual->izquierda != NULL)
        actual = actual->izquierda;
    return actual;
}

int contarPedidos(Pedido *cabeza) {
    int contador = 0;
    Pedido *actual = cabeza;
    while (actual != NULL) {
        contador++;
        actual = actual->siguiente;
    }
    return contador;
}

// 3. ROTACIONES AVL 

Inventario *rotacionDerecha(Inventario *NodoDesbalanceado) {
    Inventario *NodoSuperior = NodoDesbalanceado->izquierda;
    Inventario *SubArbolTemporal = NodoSuperior->derecha;

    NodoSuperior->derecha = NodoDesbalanceado;
    NodoDesbalanceado->izquierda = SubArbolTemporal;

    NodoDesbalanceado->altura =
        (obtenerAltura(NodoDesbalanceado->izquierda) > obtenerAltura(NodoDesbalanceado->derecha) ?
         obtenerAltura(NodoDesbalanceado->izquierda) : obtenerAltura(NodoDesbalanceado->derecha)) + 1;

    NodoSuperior->altura =
        (obtenerAltura(NodoSuperior->izquierda) > obtenerAltura(NodoSuperior->derecha) ?
         obtenerAltura(NodoSuperior->izquierda) : obtenerAltura(NodoSuperior->derecha)) + 1;

    return NodoSuperior; 
}

Inventario *rotacionIzquierda(Inventario *NodoDesbalanceado) {
    Inventario *NodoSuperior = NodoDesbalanceado->derecha;
    Inventario *SubArbolTemporal = NodoSuperior->izquierda;

    NodoSuperior->izquierda = NodoDesbalanceado;
    NodoDesbalanceado->derecha = SubArbolTemporal;

    NodoDesbalanceado->altura =
        (obtenerAltura(NodoDesbalanceado->izquierda) > obtenerAltura(NodoDesbalanceado->derecha) ?
         obtenerAltura(NodoDesbalanceado->izquierda) : obtenerAltura(NodoDesbalanceado->derecha)) + 1;

    NodoSuperior->altura =
        (obtenerAltura(NodoSuperior->izquierda) > obtenerAltura(NodoSuperior->derecha) ?
         obtenerAltura(NodoSuperior->izquierda) : obtenerAltura(NodoSuperior->derecha)) + 1;

    return NodoSuperior; 
}

// 4. REGISTRO DE MERCANCÍAS INSERCIÓN AVL

Inventario* insertarNodoAVL(Inventario *nodo, int fecha, int unidades, const char *nombre) {

    if (nodo == NULL) {
        printf("Lote %d de %s con %d uds. registrado exitosamente.\n", fecha, nombre, unidades);
        return nuevoNodoInventario(fecha, unidades, nombre); 
    }

    if (fecha < nodo->fecha_vencimiento)
        nodo->izquierda = insertarNodoAVL(nodo->izquierda, fecha, unidades, nombre);
    else if (fecha > nodo->fecha_vencimiento)
        nodo->derecha = insertarNodoAVL(nodo->derecha, fecha, unidades, nombre);
    else {
        printf("Error: Ya existe un lote con la fecha %d.\n", fecha);
        return nodo;
    }

    nodo->altura =
        (obtenerAltura(nodo->izquierda) > obtenerAltura(nodo->derecha) ?
         obtenerAltura(nodo->izquierda) : obtenerAltura(nodo->derecha)) + 1;

    int balance = obtenerFactorEquilibrio(nodo);

    if (balance > 1 && fecha < nodo->izquierda->fecha_vencimiento)
        return rotacionDerecha(nodo);

    if (balance < -1 && fecha > nodo->derecha->fecha_vencimiento)
        return rotacionIzquierda(nodo);

    if (balance > 1 && fecha > nodo->izquierda->fecha_vencimiento) {
        nodo->izquierda = rotacionIzquierda(nodo->izquierda);
        return rotacionDerecha(nodo);
    }

    if (balance < -1 && fecha < nodo->derecha->fecha_vencimiento) {
        nodo->derecha = rotacionDerecha(nodo->derecha);
        return rotacionIzquierda(nodo);
    }

    return nodo;
}

// 5. REGISTRAR PEDIDO DE DESPACHO ENCOLAR FIFO

Pedido *nuevoNodoPedido(const char *destino, int unidades) {
    Pedido *nuevo = (Pedido *)malloc(sizeof(Pedido));
    if (nuevo == NULL) {
        perror("Error de asignación de memoria para Pedido");
        exit(EXIT_FAILURE);
    }
    strcpy(nuevo->nombre_destino, destino); 
    nuevo->cantidad_solicitada = unidades; 
    nuevo->siguiente = NULL;
    return nuevo;
}

void encolarPedido(Inventario *lote_prioritario, const char *destino, int unidades) {

    if (lote_prioritario == NULL || unidades <= 0 || unidades > lote_prioritario->stock_total) {
        printf("Error al registrar pedido.\n");
        return;
    }

    Pedido *nuevo = nuevoNodoPedido(destino, unidades);

    if (lote_prioritario->cabeza_pedidos == NULL)
        lote_prioritario->cabeza_pedidos = nuevo;
    else {
        Pedido *actual = lote_prioritario->cabeza_pedidos;
        while (actual->siguiente != NULL)
            actual = actual->siguiente;
        actual->siguiente = nuevo;
    }

    lote_prioritario->stock_total -= unidades;
}

// 6. CANCELACIÓN (ELIMINACIONES)

void liberarCola(Pedido *cabeza) {
    Pedido *actual = cabeza;
    while (actual != NULL) {
        Pedido *tmp = actual;
        actual = actual->siguiente;
        free(tmp);
    }
}

Inventario *valorMinimoNodo(Inventario *nodo) {
    Inventario *actual = nodo;
    while (actual->izquierda != NULL)
        actual = actual->izquierda;
    return actual;
}

Inventario *eliminarNodoAVL(Inventario *raiz, int fecha) {

    if (raiz == NULL)
        return raiz;

    if (fecha < raiz->fecha_vencimiento)
        raiz->izquierda = eliminarNodoAVL(raiz->izquierda, fecha);
    else if (fecha > raiz->fecha_vencimiento)
        raiz->derecha = eliminarNodoAVL(raiz->derecha, fecha);
    else {

        if (raiz->izquierda == NULL || raiz->derecha == NULL) {

            liberarCola(raiz->cabeza_pedidos);

            Inventario *temp = raiz->izquierda ? raiz->izquierda : raiz->derecha;

            if (temp == NULL) {
                free(raiz);
                return NULL;
            } else {
                *raiz = *temp;
                free(temp);
            }

        } else {
            Inventario *temp = valorMinimoNodo(raiz->derecha);

            raiz->fecha_vencimiento = temp->fecha_vencimiento;
            raiz->stock_total = temp->stock_total;
            strcpy(raiz->nombre_producto, temp->nombre_producto);
            raiz->cabeza_pedidos = temp->cabeza_pedidos;

            raiz->derecha = eliminarNodoAVL(raiz->derecha, temp->fecha_vencimiento);
        }
    }

    if (raiz == NULL)
        return raiz;

    raiz->altura =
        (obtenerAltura(raiz->izquierda) > obtenerAltura(raiz->derecha) ?
         obtenerAltura(raiz->izquierda) : obtenerAltura(raiz->derecha)) + 1;

    int balance = obtenerFactorEquilibrio(raiz);

    if (balance > 1 && obtenerFactorEquilibrio(raiz->izquierda) >= 0)
        return rotacionDerecha(raiz);

    if (balance > 1 && obtenerFactorEquilibrio(raiz->izquierda) < 0) {
        raiz->izquierda = rotacionIzquierda(raiz->izquierda);
        return rotacionDerecha(raiz);
    }

    if (balance < -1 && obtenerFactorEquilibrio(raiz->derecha) <= 0)
        return rotacionIzquierda(raiz);

    if (balance < -1 && obtenerFactorEquilibrio(raiz->derecha) > 0) {
        raiz->derecha = rotacionDerecha(raiz->derecha);
        return rotacionIzquierda(raiz);
    }

    return raiz;
}

// 7. REPORTE IN-ORDER

void recorridoInOrder(Inventario *raiz) {
    if (raiz != NULL) {
        recorridoInOrder(raiz->izquierda);
        printf("| %d | %-15s | %-10d | %-8d |\n",
               raiz->fecha_vencimiento,
               raiz->nombre_producto,
               raiz->stock_total,
               contarPedidos(raiz->cabeza_pedidos));
        recorridoInOrder(raiz->derecha);
    }
}

// 8. MAIN Y MENÚ

int main() {
    Inventario *raiz = NULL;

    raiz = insertarNodoAVL(raiz, 20251110, 500, "Leche");
    raiz = insertarNodoAVL(raiz, 20260315, 1200, "Granos");
    raiz = insertarNodoAVL(raiz, 20251125, 300, "Pescado");
    raiz = insertarNodoAVL(raiz, 20260105, 850, "Atun");
    raiz = insertarNodoAVL(raiz, 20251215, 400, "Tuberculos");

    printf("FECHA VENC.  PRODUCTO      STOCK DISP  PEDIDOS\n");
    recorridoInOrder(raiz);

    return 0;
}

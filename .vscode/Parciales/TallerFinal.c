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
    while (cabeza != NULL) {
        contador++;
        cabeza = cabeza->siguiente;
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

    if (nodo == NULL)
        return nuevoNodoInventario(fecha, unidades, nombre);

    if (fecha < nodo->fecha_vencimiento)
        nodo->izquierda = insertarNodoAVL(nodo->izquierda, fecha, unidades, nombre);
    else if (fecha > nodo->fecha_vencimiento)
        nodo->derecha = insertarNodoAVL(nodo->derecha, fecha, unidades, nombre);
    else
        return nodo;

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
    strcpy(nuevo->nombre_destino, destino); 
    nuevo->cantidad_solicitada = unidades; 
    nuevo->siguiente = NULL;
    return nuevo;
}

void encolarPedido(Inventario *lote_prioritario, const char *destino, int unidades) {

    if (lote_prioritario == NULL || unidades <= 0 || unidades > lote_prioritario->stock_total)
        return;

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
    while (cabeza != NULL) {
        Pedido *tmp = cabeza;
        cabeza = cabeza->siguiente;
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

        liberarCola(raiz->cabeza_pedidos);

        if (raiz->izquierda == NULL || raiz->derecha == NULL) {
            Inventario *temp = raiz->izquierda ? raiz->izquierda : raiz->derecha;
            free(raiz);
            return temp;
        } else {
            Inventario *temp = valorMinimoNodo(raiz->derecha);
            raiz->fecha_vencimiento = temp->fecha_vencimiento;
            raiz->stock_total = temp->stock_total;
            strcpy(raiz->nombre_producto, temp->nombre_producto);
            raiz->cabeza_pedidos = NULL;
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

void cancelarPedido(Inventario *lote, const char *destino) {
    Pedido *actual = lote->cabeza_pedidos;
    Pedido *anterior = NULL;

    while (actual != NULL) {
        if (strcmp(actual->nombre_destino, destino) == 0) {
            lote->stock_total += actual->cantidad_solicitada;
            if (anterior == NULL)
                lote->cabeza_pedidos = actual->siguiente;
            else
                anterior->siguiente = actual->siguiente;
            free(actual);
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
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
    int opcion;

    do {
        printf("\n===== SISTEMA LOGISTICO =====\n");
        printf("1. Recepción de Mercancía\n");
        printf("2. Registrar Pedido de Despacho\n");
        printf("3. Cancelaciones\n");
        printf("4. Reporte de Estado\n");
        printf("0. Salir\n");
        printf("Seleccione una opcion: ");
        scanf(" %d", &opcion);

        if (opcion == 1) {
            int fecha, cantidad;
            char producto[40];

            printf("Ingrese fecha (AAAA/MM/DD): ");
            scanf(" %d", &fecha);

            printf("Ingrese nombre del producto: ");
            scanf(" %s", producto);

            printf("Ingrese cantidad: ");
            scanf(" %d", &cantidad);

            raiz = insertarNodoAVL(raiz, fecha, cantidad, producto);
            printf("Producto registrado.\n");
        }
        else if (opcion == 2) {
            char destino[40];
            int cantidad;

            Inventario *lote = buscarMasAntiguo(raiz);
            if (lote != NULL) {
                printf("Destino: ");
                scanf(" %s", destino);

                printf("Cantidad a despachar: ");
                scanf(" %d", &cantidad);

                encolarPedido(lote, destino, cantidad);
                printf("Pedido registrado.\n");
            } else {
                printf("No hay inventario disponible.\n");
            }
        }
        else if (opcion == 3) {
            int sub;
            printf("\n1. Eliminar lote completo\n");
            printf("2. Cancelar pedido\n");
            printf("Seleccione una opcion: ");
            scanf(" %d", &sub);

            if (sub == 1) {
                int fecha;
                printf("Ingrese fecha del lote a eliminar: ");
                scanf(" %d", &fecha);
                raiz = eliminarNodoAVL(raiz, fecha);
                printf("Lote eliminado.\n");
            } 
            else if (sub == 2) {
                char destino[40];
                Inventario *lote = buscarMasAntiguo(raiz);
                if (lote != NULL) {
                    printf("Ingrese destino del pedido a cancelar: ");
                    scanf(" %s", destino);
                    cancelarPedido(lote, destino);
                    printf("Pedido cancelado.\n");
                } else {
                    printf("No hay pedidos disponibles.\n");
                }
            }
        }
        else if (opcion == 4) {
            printf("\nFECHA VENC. | PRODUCTO        | STOCK DISP | PEDIDOS\n");
            printf("--------------------------------------------------\n");
            recorridoInOrder(raiz);
        }

    } while (opcion != 0);

    printf("Sistema finalizado.\n");
    return 0;
}

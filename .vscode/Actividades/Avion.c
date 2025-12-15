#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definición del nodo
typedef struct nodo {
    int documento;
    char nombre[50];
    struct nodo *ant;
    struct nodo *sig;
} nodo;

nodo *cab = NULL;
int capacidadAvion = 0;
int pasajerosActuales = 0;

void capacidad();
void registrar();
void mostrar();
void abordar();

int main() {
    int opc;
    do {
        printf("\n--- MENU ---\n");
        printf("1. Capacidad del avion\n");
        printf("2. Registrar pasajero\n");
        printf("3. Mostrar pasajeros\n");
        printf("4. Abordar pasajero\n");
        printf("5. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opc);
        switch (opc) {
            case 1:
                capacidad();
                break;
            case 2:
                registrar();
                break;
            case 3:
                mostrar();
                break;
            case 4:
                abordar();
                break;
            case 5:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opcion invalida\n");
        }
    } while (opc != 5);
    return 0;
}

void capacidad() {
    printf("Ingrese la capacidad del avion: ");
    scanf("%d", &capacidadAvion);

    int total = capacidadAvion + (capacidadAvion * 0.10);
    printf("Los boletos a vender (10%% de sobrecupo permitido): %d\n", total);
    capacidadAvion = total;
}
void registrar() {
    if (capacidadAvion == 0) {
        printf("Error: Ingrese la capacidad del avion primero.\n");
        return;
    }

    if (pasajerosActuales >= capacidadAvion) {
        printf("Ya no hay cupos disponibles.\n");
        return;
    }

    nodo *nuevo = (nodo *)malloc(sizeof(nodo));
    if (nuevo == NULL) {
        printf("Error al asignar memoria.\n");
        return;
    }

    printf("Ingrese el documento del pasajero: ");
    scanf("%d", &nuevo->documento);

    printf("Ingrese el nombre del pasajero: ");
    fgets(nuevo->nombre, 50, stdin);
    nuevo->nombre[strcspn(nuevo->nombre, "\n")] = '\0'; 

    nuevo->sig = NULL;
    nuevo->ant = NULL;

    if (cab == NULL) {
        cab = nuevo;
    } else {
        nodo *aux = cab;
        while (aux->sig != NULL) {
            aux = aux->sig;
        }
        aux->sig = nuevo;
        nuevo->ant = aux;
    }

    pasajerosActuales++;
    printf("Pasajero registrado con exito.\n");
}

void mostrar() {
    if (cab == NULL) {
        printf("No hay pasajeros registrados.\n");
        return;
    }

    nodo *aux = cab;
    printf("\n--- LISTA DE PASAJEROS ---\n");
    while (aux != NULL) {
        printf("Documento: %d | Nombre: %s\n", aux->documento, aux->nombre);
        aux = aux->sig;
    }
}

void abordar() {
    if (cab == NULL) {
        printf("No hay pasajeros para abordar.\n");
        return;
    }

    int doc;
    printf("Ingrese el documento del pasajero que va a abordar: ");
    scanf("%d", &doc);

    nodo *aux = cab;
    while (aux != NULL && aux->documento != doc) {
        aux = aux->sig;
    }

    if (aux == NULL) {
        printf("No se encontró el pasajero.\n");
        return;
    }

    if (aux->ant != NULL)
        aux->ant->sig = aux->sig;
    else
        cab = aux->sig;

    if (aux->sig != NULL)
        aux->sig->ant = aux->ant;

    free(aux);
    pasajerosActuales--;
    printf("Pasajero abordado correctamente.\n");
}

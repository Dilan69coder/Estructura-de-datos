#include <stdio.h>
#define N 10

void llenarArray(int a[], int n);
void mostrarArray(int a[], int n);
void Ascendente(int a[], int n);
void Descendente(int a[], int n);

int main() {
    int original[N];
    int copia[N];
    int opcion, i;
    int llenado = 0;

    do {
        printf("\n--- MENU ---\n");
        printf("1. Registrar tiempos\n");
        printf("2. Mostrar arreglo original\n");
        printf("3. Orden Ascendente\n");
        printf("4. Orden Descendente\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                llenarArray(original, N);
                llenado = 1;
                break;
            case 2:
                if (llenado)
                    mostrarArray(original, N);
                else
                    printf("Primero registre los tiempos.\n");
                break;
            case 3:
                if (llenado) {
                    for (i = 0; i < N; i++) copia[i] = original[i];
                    Ascendente(copia, N);
                } else
                    printf("Primero registre los tiempos.\n");
                break;
            case 4:
                if (llenado) {
                    for (i = 0; i < N; i++) copia[i] = original[i];
                    Descendente(copia, N);
                } else
                    printf("Primero registre los tiempos.\n");
                break;
            case 0:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion != 0);

    return 0;
}
// Llena el arreglo
void llenarArray(int a[], int n) {
    int i, x;
    for (i = 0; i < n; i++) {
        do {
            printf("Ingrese tiempo %d (entero positivo): ", i + 1);
            scanf("%d", &x);
            if (x <= 0) printf("Debe ser positivo.\n");
        } while (x <= 0);
        a[i] = x;
    }
}
//Muestra original
void mostrarArray(int a[], int n) {
    int i;
    printf("[");
    for (i = 0; i < n; i++) {
        printf("%d", a[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}
//Asendente
void Ascendente(int a[], int n) {
    int i, j, min, temp, paso = 1;
    for (i = 0; i < n - 1; i++) {
        min = i;
        for (j = i + 1; j < n; j++) {
            if (a[j] < a[min]) min = j;
        }
        temp = a[i];
        a[i] = a[min];
        a[min] = temp;

        printf("Paso %d: ", paso++);
        mostrarArray(a, n);
    }
    printf("Resultado final (ascendente): ");
    mostrarArray(a, n);
}
//Descendente
void Descendente(int a[], int n) {
    int i, j, key;
    for (i = 1; i < n; i++) {
        key = a[i];
        j = i - 1;
        while (j >= 0 && a[j] < key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;

        printf("Paso %d: ", i);
        mostrarArray(a, n);
    }
    printf("Resultado final (descendente): ");
    mostrarArray(a, n);
}
// FIN TRABAJO
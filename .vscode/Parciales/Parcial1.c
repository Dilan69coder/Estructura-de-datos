#include <stdio.h>

typedef struct {
    int codigo;
    int nota; 
} Estudiante;

int notaMaxima(Estudiante arr[], int n) {
    if (n == 1) {
        return arr[0].nota;
    } else {
        int ultimo = arr[n - 1].nota;
        int maxAnterior = notaMaxima(arr, n - 1);
        return (ultimo > maxAnterior) ? ultimo : maxAnterior;
    }
}

int sumaNotas(Estudiante arr[], int n) {
    if (n == 0) {
        return 0;
    } else {
        return arr[n - 1].nota + sumaNotas(arr, n - 1);
    }
}
void codigoEstu(Estudiante arr[], int n, int i) {
    if (i == n - 1) return;

    int minIndex = i;
    for (int j = i + 1; j < n; j++) {
        if (arr[j].codigo < arr[minIndex].codigo) {
            minIndex = j;
        }
    }
    Estudiante temp = arr[i];
    arr[i] = arr[minIndex];
    arr[minIndex] = temp;

    codigoEstu(arr, n, i + 1);
}


void mostrarLista(Estudiante arr[], int n) {
    if (n == 0) return;
    mostrarLista(arr, n - 1);
    printf("Codigo: %d | Nota: %d\n", arr[n - 1].codigo, arr[n - 1].nota);
}

int main() {
    Estudiante estudiantes[5] = {
        {2024101, 85},
        {2024102, 73},
        {2024103, 92},
        {2024104, 100},
        {2024105, 60}
    };

    int tam = 5; 
    int opcion = 0;

    do {
        printf("\n--- MENU ---\n");
        printf("1.Ver Nota Maxima\n");
        printf("2.Calcular Promedio\n");
        printf("3.Lista de estudiantes ordenada\n");
        printf("4.Salir\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 1: {
                int maximo = notaMaxima(estudiantes, tam);
                printf("La nota maxima es: %d / 100\n", maximo);
                break;
            }
            case 2: {
                int suma = sumaNotas(estudiantes, tam);
                float promedio = (float)suma / tam;
                printf("El promedio del curso es: %.2f / 100\n", promedio);
                break;
            }
            case 3: {
                printf("\nLista ordenada por codigo:\n");
                codigoEstu(estudiantes, tam, 0);
                mostrarLista(estudiantes, tam);
                break;
            }
            case 4: {
                printf("Saliendo...\n");
                break;
            }
            default: {
                printf("Opcion invalida, intente de nuevo.\n");
            }
        }
    } while (opcion != 4);

    return 0;
}

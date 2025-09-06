#include <iostream>
using namespace std;

// Función para capturar puntuaciones de 8 equipos
void capturarPuntuaciones(int puntuaciones[], int n) {
    for (int i = 0; i < n; i++) {
        cout << "Ingrese la puntuacion del equipo " << (i + 1) << ": ";
        cin >> puntuaciones[i];
    }
}

int main() {
    const int NUM_EQUIPOS = 8;   // cantidad fija de equipos
    int puntuaciones[NUM_EQUIPOS];

    // Llamada a la función
    capturarPuntuaciones(puntuaciones, NUM_EQUIPOS);

    // Mostrar las puntuaciones capturadas
    cout << "\nPuntuaciones ingresadas:\n";
    for (int i = 0; i < NUM_EQUIPOS; i++) {
        cout << "Equipo " << (i + 1) << ": " << puntuaciones[i] << endl;
    }

    return 0;
}

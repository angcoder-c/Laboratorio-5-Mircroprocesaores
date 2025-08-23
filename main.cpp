/*
 * ANGEL GABRIEL CHAVEZ OTZOY
 * 24248
 * LABORATORIO 05
 * 19 / 08 / 2025
 */

#include <iostream>
#include <random>
#include <thread>
#include <tuple>
#include <vector>

using namespace std;

// maximo local de cada hilo
void find_max(tuple<int*, int, int, int, int>* data) {  // Cambio 1: pasar por referencia
    // arr, inicio, fin, max_local, thread_id

    get<3>(*data) = get<0>(*data)[get<1>(*data)];  // Cambio 2: usar * para desreferenciar

    for (int i = get<1>(*data); i <= get<2>(*data); i++) {
        if (get<0>(*data)[i] > get<3>(*data)) {
            get<3>(*data) = get<0>(*data)[i];
        }
    }

    std::cout
    << "[HILO-" << get<4>(*data)
    << "] procesando elementos del índice " + get<1>(*data)
    << " al " + get<2>(*data)
    << " - maximo local: " + get<3>(*data)
    << endl;
}

int main() {
    int cant_elementos;
    int cant_hilos;

    // numero de elementos
    cout << "[PRINCIPAL] Cantidad de elementos: ";
    cin >> cant_elementos;

    // numero de hilos
    cout << "[PRINCIPAL] Cantidad de hilos: ";
    cin >> cant_hilos;

    if (cant_elementos <= cant_hilos) {
        std::cout
        << "[PRINCIPAL] ERROR: La cantidad de elementos ("
        << cant_elementos
        << ") debe ser mayor que la cantidad de hilos ("
        << cant_hilos
        << ")"
        << std::endl;
        return 0;
    }

    // generar numeros aleatorios
    random_device r;
    default_random_engine e1(r());
    uniform_int_distribution<int> uniform_dist(1, 100000);
    int* arr = new int[cant_elementos];  // Cambio 3: usar new para array dinámico

    for (int i = 0; i < cant_elementos; i++) {
        // agregar enteros aleatorios
        arr[i] = uniform_dist(e1);
    }

    // elementos por hilo
    int elementos_por_hilo = cant_elementos / cant_hilos;
    int elementos_restantes = cant_elementos % cant_hilos;

    cout
    << "[PRINCIPAL] Elementos por hilo: "
    << elementos_por_hilo
    << endl;

    if (elementos_restantes > 0) {
        cout
        << "[PRINCIPAL] Elementos restantes: "
        << elementos_restantes
        << endl;
    }

    // array para almacenar los hilos
    vector<thread> hilos(cant_hilos);
    // arr, inicio, fin, max_local, thread_id
    vector<tuple<int*, int, int, int, int>> thread_data(cant_hilos);

    // iniciar los hilos
    cout << "[PRINCIPAL] Iniciando hilos" << endl;

    for (int i = 0; i < cant_hilos; i++) {
        get<0>(thread_data[i]) = arr;
        get<4>(thread_data[i]) = i + 1;
        get<1>(thread_data[i]) = i * elementos_por_hilo;

        if (i == cant_hilos - 1) {
            // elementos restantes
            std::get<2>(thread_data[i]) = (i + 1) * elementos_por_hilo + elementos_restantes - 1;
        } else {
            std::get<2>(thread_data[i]) = (i + 1) * elementos_por_hilo - 1;
        }

        // lanzar el hilo - Cambio 4: pasar dirección de la tupla
        hilos[i] = thread(find_max, &thread_data[i]);
    }

    // esperar a los hilos
    for (int i = 0; i < cant_hilos; i++) {
        cout << "[PRINCIPAL] Terminando hilo " << get<4>(thread_data[i]) << endl;
        hilos[i].join();
    }

    // maximos locales
    int maximo_global = get<3>(thread_data[0]);
    for (int i = 0; i < cant_hilos; i++) {
        cout
        << "HILO-" << get<4>(thread_data[i])
        << ": " << get<3>(thread_data[i])
        << endl;

        if (get<3>(thread_data[i]) > maximo_global) {
            maximo_global = get<3>(thread_data[i]);
        }
    }

    cout << "MAXIMO GLOBAL: " << maximo_global << endl;

    // liberar memoria
    delete[] arr;
    return 0;
}
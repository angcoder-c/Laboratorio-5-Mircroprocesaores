/*
 * ANGEL GABRIEL CHAVEZ OTZOY
 * 24248
 * LABORATORIO 05
 * 19 / 08 / 2025
 */

#include <iostream>
#include <random>

int main() {
    int cant_elementos;
    std::cout << "Cantidad de elementos: ";
    std::cin >> cant_elementos;
    std::random_device r;

    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(1, 100);

    int arr[cant_elementos];

    for (int i = 0; i < cant_elementos; i++) {
        arr[i] = uniform_dist(e1);
    }

    for (int i = 0; i < cant_elementos; i++) {
        std::cout << arr[i] << " " << std::endl;
    }
    return 0;
}
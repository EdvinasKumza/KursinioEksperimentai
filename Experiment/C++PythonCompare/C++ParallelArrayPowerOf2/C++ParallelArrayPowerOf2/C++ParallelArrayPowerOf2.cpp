#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>

void kvadratu(const std::vector<int>& src, std::vector<int>& dst, size_t pradzia, size_t pabaiga) {
    for (size_t i = pradzia; i < pabaiga; ++i) {
        dst[i] = src[i] * src[i];
    }
}

int main() {
    const size_t dydis = 10'000'000;
    std::vector<int> duomenys(dydis), rezultatai(dydis);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 10'000);

    for (size_t i = 0; i < dydis; ++i) {
        duomenys[i] = dist(gen);
    }

    size_t gijuSk = std::thread::hardware_concurrency();
    double bendraTrukme = 0.0;
    const int iteracijos = 100;

    for (int it = 0; it < iteracijos; ++it) {
        auto pradziosLaikas = std::chrono::high_resolution_clock::now();
        size_t dalis = dydis / gijuSk;
        std::vector<std::jthread> gijos;
        gijos.reserve(gijuSk);

        for (size_t i = 0; i < gijuSk; ++i) {
            size_t nuo = i * dalis;
            size_t iki = (i == gijuSk - 1) ? dydis : (i + 1) * dalis;
            gijos.emplace_back(kvadratu, std::cref(duomenys), std::ref(rezultatai), nuo, iki);
        }

        auto pabaigosLaikas = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> trukme = pabaigosLaikas - pradziosLaikas;
        bendraTrukme += trukme.count();
    }

    double vidurkis = bendraTrukme / iteracijos;
    std::cout << "Vidutinis lygiagretaus vykdymo laikas: " << vidurkis << " ms\n";
    return 0;
}

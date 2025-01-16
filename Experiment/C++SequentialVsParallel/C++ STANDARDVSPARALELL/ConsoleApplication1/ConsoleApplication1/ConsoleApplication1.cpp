#include <iostream>
#include <vector>
#include <numeric>
#include <execution>
#include <chrono>
#include <algorithm>
#include <random>
#include <cmath>

std::pair<double, double> skaiciuotiStatistika(const std::vector<double>& laikai) {
    double suma = std::accumulate(laikai.begin(), laikai.end(), 0.0);
    double vidurkis = suma / laikai.size();

    double kvadratuSuma = std::inner_product(laikai.begin(), laikai.end(), laikai.begin(), 0.0);
    double standartinisNukrypimas = std::sqrt(kvadratuSuma / laikai.size() - vidurkis * vidurkis);

    return { vidurkis, standartinisNukrypimas };
}

long long sekvencinisSuma(const std::vector<int>& duomenys) {
    return std::reduce(std::execution::seq, duomenys.begin(), duomenys.end(), 0LL);
}

long long lygiagretusSuma(const std::vector<int>& duomenys) {
    return std::reduce(std::execution::par_unseq, duomenys.begin(), duomenys.end(), 0LL);
}

void rusiavimoEksperimentas(std::vector<int> duomenys, bool naudotiLygiagreciai) {
    if (naudotiLygiagreciai) {
        std::sort(std::execution::par, duomenys.begin(), duomenys.end());
    }
    else {
        std::sort(std::execution::seq, duomenys.begin(), duomenys.end());
    }
}

int main() {
    const size_t duomenuDydis = 10000000;
    const int iteracijos = 100;

    std::vector<int> duomenys(duomenuDydis);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> skirstinys(0, 100);

    for (size_t i = 0; i < duomenuDydis; ++i) {
        duomenys[i] = skirstinys(gen);
    }

    std::vector<double> sekvenciniaiLaikai;
    std::vector<double> lygiagretusLaikai;

    for (int i = 0; i < iteracijos; ++i) {
        auto pradziaSekv = std::chrono::high_resolution_clock::now();
        long long sekvRezultatas = sekvencinisSuma(duomenys);
        auto pabaigaSekv = std::chrono::high_resolution_clock::now();
        sekvenciniaiLaikai.push_back(
            std::chrono::duration<double, std::milli>(pabaigaSekv - pradziaSekv).count()
        );

        auto pradziaLyg = std::chrono::high_resolution_clock::now();
        long long lygRezultatas = lygiagretusSuma(duomenys);
        auto pabaigaLyg = std::chrono::high_resolution_clock::now();
        lygiagretusLaikai.push_back(
            std::chrono::duration<double, std::milli>(pabaigaLyg - pradziaLyg).count()
        );
    }

    auto [sekvVidurkis, sekvNukrypimas] = skaiciuotiStatistika(sekvenciniaiLaikai);
    auto [lygVidurkis, lygNukrypimas] = skaiciuotiStatistika(lygiagretusLaikai);

    std::cout << "Sekvencinis Reduce vidutinis laikas: "
        << sekvVidurkis << " ms (" << sekvNukrypimas << " ms)" << std::endl;
    std::cout << "Lygiagretus Reduce vidutinis laikas: "
        << lygVidurkis << " ms (" << lygNukrypimas << " ms)" << std::endl;

    std::vector<double> rusiavimoSekvLaikai;
    std::vector<double> rusiavimoLygLaikai;

    for (int i = 0; i < iteracijos; ++i) {
        auto duomenysKopija = duomenys;

        auto pradziaSekvRusiav = std::chrono::high_resolution_clock::now();
        rusiavimoEksperimentas(duomenysKopija, false);
        auto pabaigaSekvRusiav = std::chrono::high_resolution_clock::now();
        rusiavimoSekvLaikai.push_back(
            std::chrono::duration<double, std::milli>(pabaigaSekvRusiav - pradziaSekvRusiav).count()
        );

        duomenysKopija = duomenys;
        auto pradziaLygRusiav = std::chrono::high_resolution_clock::now();
        rusiavimoEksperimentas(duomenysKopija, true);
        auto pabaigaLygRusiav = std::chrono::high_resolution_clock::now();
        rusiavimoLygLaikai.push_back(
            std::chrono::duration<double, std::milli>(pabaigaLygRusiav - pradziaLygRusiav).count()
        );
    }

    auto [rusiavimoSekvVidurkis, rusiavimoSekvNukrypimas] = skaiciuotiStatistika(rusiavimoSekvLaikai);
    auto [rusiavimoLygVidurkis, rusiavimoLygNukrypimas] = skaiciuotiStatistika(rusiavimoLygLaikai);

    std::cout << "Sekvencinis Rikiavimas vidutinis laikas: "
        << rusiavimoSekvVidurkis << " ms (" << rusiavimoSekvNukrypimas << " ms)" << std::endl;
    std::cout << "Lygiagretus Rikiavimas vidutinis laikas: "
        << rusiavimoLygVidurkis << " ms (" << rusiavimoLygNukrypimas << " ms)" << std::endl;

    return 0;
}

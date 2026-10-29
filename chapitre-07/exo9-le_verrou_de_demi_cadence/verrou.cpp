#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

struct Statistiques {
    double moyenne;
    double ecartType;
    double maximum;
};

Statistiques Analyser(const std::vector<double> &intervallesMs) {
    if (intervallesMs.empty()) return {0.0, 0.0, 0.0};
    double moyenne = 0.0;
    for (double dt : intervallesMs) moyenne += dt;
    moyenne /= intervallesMs.size();
    double somme = 0.0;
    for (double dt : intervallesMs) somme += (dt - moyenne) * (dt - moyenne);
    return {moyenne, std::sqrt(somme / intervallesMs.size()),
            *std::max_element(intervallesMs.begin(), intervallesMs.end())};
}

int main() {
    std::puts("Mesurer 1000 intervalles de la salle alourdie, sans verrou puis avec verrou, dans Chap7Projet.");
    std::puts("Le verrou vise deux périodes entre les débuts de deux images successives.");
    std::puts("Ne pas remplacer les temps de rendu réels par une attente seule.");
}

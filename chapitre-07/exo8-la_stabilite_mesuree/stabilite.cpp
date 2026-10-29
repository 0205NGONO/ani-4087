#include <cmath>
#include <cstdio>
#include <vector>

struct Resultat {
    double moyenne;
    double ecartType;
    double maximum;
    std::size_t horsBudget;
};

Resultat Analyser(const std::vector<double> &intervallesMs) {
    Resultat r{0.0, 0.0, 0.0, 0};
    for (double dt : intervallesMs) {
        r.moyenne += dt;
        if (dt > r.maximum) r.maximum = dt;
        if (dt > 1000.0 / 90.0) ++r.horsBudget;
    }
    if (intervallesMs.empty()) return r;
    r.moyenne /= intervallesMs.size();
    double somme = 0.0;
    for (double dt : intervallesMs) {
        const double e = dt - r.moyenne;
        somme += e * e;
    }
    r.ecartType = std::sqrt(somme / intervallesMs.size());
    return r;
}

int main() {
    std::puts("Collecter 60 secondes d’intervalles de rendu dans Chap7Projet, après 30 images de chauffe.");
    std::puts("Écarter les images de chauffe et appeler Analyser() sur les intervalles en millisecondes.");
    std::puts("Répéter la mesure avec NK_VK_MAILBOX=1 sous Vulkan.");
}

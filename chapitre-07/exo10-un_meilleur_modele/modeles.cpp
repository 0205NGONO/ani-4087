#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

namespace {

constexpr double kPi = 3.14159265358979323846;
constexpr double kHz = 72.0;
constexpr double kDt = 1.0 / kHz;

struct Erreurs {
    double sommeVitesse = 0.0;
    double maxVitesse = 0.0;
    double sommeAcceleration = 0.0;
    double maxAcceleration = 0.0;
    int nombre = 0;
};

double Doux(double t, double amplitude, double duree) {
    if (t <= 0.0) return 0.0;
    if (t >= duree) return amplitude;
    return amplitude * 0.5 * (1.0 - std::cos(kPi * t / duree));
}

double Geste(const char *nom, double t) {
    const std::string geste(nom);
    if (geste.find("rapide") == 0) return Doux(t, 90.0, 0.400);
    if (geste.find("lent") == 0) return Doux(t, 45.0, 1.200);
    if (geste.find("demi-tour") == 0) return Doux(t, 180.0, 0.120);
    return 120.0 * std::min(t, 0.400);
}

Erreurs Evaluer(const char *nom, double duree) {
    const double fin = duree + 0.200;
    std::vector<double> poses;
    for (int i = 0; i <= static_cast<int>(std::ceil(fin / kDt)); ++i)
        poses.push_back(Geste(nom, i * kDt));

    Erreurs r;
    for (size_t i = 2; i + 1 < poses.size(); ++i) {
        const double vitesse = (poses[i] - poses[i - 1]) / kDt;
        const double vitessePrecedente = (poses[i - 1] - poses[i - 2]) / kDt;
        const double acceleration = (vitesse - vitessePrecedente) / kDt;
        const double vrai = Geste(nom, (i + 1) * kDt);
        const double predV = poses[i] + vitesse * kDt;
        const double predA = predV + 0.5 * acceleration * kDt * kDt;
        const double eV = std::fabs(predV - vrai);
        const double eA = std::fabs(predA - vrai);
        r.sommeVitesse += eV;
        r.maxVitesse = std::max(r.maxVitesse, eV);
        r.sommeAcceleration += eA;
        r.maxAcceleration = std::max(r.maxAcceleration, eA);
        ++r.nombre;
    }
    return r;
}

void Imprimer(const char *nom, double duree) {
    const Erreurs r = Evaluer(nom, duree);
    const double diviseur = r.nombre > 0 ? r.nombre : 1;
    std::printf("%s : vitesse %.6f deg (max %.6f), acceleration %.6f deg (max %.6f)\n",
                nom, r.sommeVitesse / diviseur, r.maxVitesse,
                r.sommeAcceleration / diviseur, r.maxAcceleration);
}

} // namespace

int main() {
    Imprimer("rapide (90 deg en 400 ms)", 0.400);
    Imprimer("lent (45 deg en 1200 ms)", 1.200);
    Imprimer("demi-tour (180 deg en 120 ms)", 0.120);
    Imprimer("arret (120 deg/s, arret a 400 ms)", 0.400);
}

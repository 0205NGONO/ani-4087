#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

namespace {

constexpr double kPi = 3.14159265358979323846;
constexpr double kHz = 72.0;
constexpr double kPeriod = 1.0 / kHz;

struct Mesure {
    double moyenneAvec = 0.0;
    double moyenneSans = 0.0;
};

double Lissage(double t, double amplitude, double duree) {
    if (t <= 0.0) return 0.0;
    if (t >= duree) return amplitude;
    return amplitude * 0.5 * (1.0 - std::cos(kPi * t / duree));
}

double Erreur(double estime, double vrai) {
    return std::fabs(estime - vrai);
}

Mesure Evaluer(double amplitude, double duree) {
    const int nombre = static_cast<int>(std::ceil(duree / kPeriod));
    std::vector<double> eAvec;
    std::vector<double> eSans;
    for (int i = 1; i < nombre; ++i) {
        const double t = i * kPeriod;
        const double courant = Lissage(t, amplitude, duree);
        const double precedent = Lissage(t - kPeriod, amplitude, duree);
        const double vitesse = (courant - precedent) / kPeriod;
        const double vrai = Lissage(t + kPeriod, amplitude, duree);
        eSans.push_back(Erreur(courant, vrai));
        eAvec.push_back(Erreur(courant + vitesse * kPeriod, vrai));
    }

    Mesure resultat;
    for (double e : eAvec) resultat.moyenneAvec += e;
    for (double e : eSans) resultat.moyenneSans += e;
    if (!eAvec.empty()) {
        resultat.moyenneAvec /= eAvec.size();
        resultat.moyenneSans /= eSans.size();
    }
    return resultat;
}

void Imprimer(const char *nom, double amplitude, double duree) {
    const Mesure m = Evaluer(amplitude, duree);
    const double rapport = m.moyenneSans > 0.0 ? m.moyenneAvec / m.moyenneSans : 0.0;
    std::printf("%s (%g deg en %g ms) : avec %.6f deg, sans %.6f deg, rapport %.6f\n",
                nom, amplitude, duree * 1000.0, m.moyenneAvec, m.moyenneSans, rapport);
}

} // namespace

int main() {
    Imprimer("rapide", 90.0, 0.400);
    Imprimer("lent", 45.0, 1.200);
    Imprimer("demi-tour", 180.0, 0.120);
}

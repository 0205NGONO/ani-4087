#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

namespace {

constexpr double kPi = 3.14159265358979323846;
constexpr double kHz = 72.0;
constexpr double kPeriode = 1.0 / kHz;

struct Pose {
    double temps;
    double lacet;
};

struct Statistiques {
    double moyenne = 0.0;
    double maximum = 0.0;
};

double Lacet(double t) {
    const double duree = 0.400;
    if (t <= 1.0) return 0.0;
    if (t >= 1.0 + duree) return 90.0;
    return 45.0 * (1.0 - std::cos(kPi * (t - 1.0) / duree));
}

double Vitesse(const Pose &a, const Pose &b) {
    return b.temps > a.temps ? (b.lacet - a.lacet) / (b.temps - a.temps) : 0.0;
}

double Echantillon(const std::vector<Pose> &poses, double cible, double maintenant) {
    const size_t dernier = static_cast<size_t>(maintenant / kPeriode);
    const size_t debut = dernier + 1 > 64 ? dernier + 1 - 64 : 0;
    const auto debutHistorique = poses.begin() + debut;
    const auto finHistorique = poses.begin() + dernier + 1;
    auto it = std::upper_bound(debutHistorique, finHistorique, cible,
        [](double temps, const Pose &pose) { return temps < pose.temps; });
    if (it == debutHistorique) return poses[debut].lacet;
    return (it - 1)->lacet;
}

Statistiques Erreur(double latence, bool prediction, bool borne) {
    std::vector<Pose> poses;
    for (int i = 0; i <= static_cast<int>(3.0 / kPeriode); ++i) {
        const double t = i * kPeriode;
        poses.push_back({t, Lacet(t)});
    }

    Statistiques stats;
    int nombre = 0;
    for (double t = 1.0; t <= 1.9; t += kPeriode) {
        const double cible = t - latence;
        const double echantillon = Echantillon(poses, cible, t);
        const size_t dernier = static_cast<size_t>(t / kPeriode);
        const size_t debut = dernier + 1 > 64 ? dernier + 1 - 64 : 0;
        const auto debutHistorique = poses.begin() + debut;
        const auto finHistorique = poses.begin() + dernier + 1;
        const auto it = std::upper_bound(debutHistorique, finHistorique, cible,
            [](double temps, const Pose &pose) { return temps < pose.temps; });
        const size_t index = it == debutHistorique ? debut : static_cast<size_t>(it - poses.begin() - 1);
        double estime = echantillon;
        if (prediction && index + 1 < poses.size()) {
            double horizon = t + kPeriode - poses[index].temps;
            if (borne) horizon = std::clamp(horizon, 0.0, 0.100);
            estime += Vitesse(poses[index], poses[index + 1]) * horizon;
        }
        const double erreur = std::fabs(estime - Lacet(t + kPeriode));
        stats.moyenne += erreur;
        stats.maximum = std::max(stats.maximum, erreur);
        ++nombre;
    }
    if (nombre > 0) stats.moyenne /= nombre;
    return stats;
}

} // namespace

int main() {
    for (double ms : {0.0, 10.0, 20.0, 40.0, 80.0}) {
        const double latence = ms / 1000.0;
        const Statistiques avec = Erreur(latence, true, true);
        const Statistiques sans = Erreur(latence, false, true);
        std::printf("latence %.0f ms : avec %.6f deg (max %.6f), sans %.6f deg (max %.6f)\n",
                    ms, avec.moyenne, avec.maximum, sans.moyenne, sans.maximum);
    }
    for (bool borne : {false, true}) {
        const double t = 1.2;
        const double latence = 1.0;
        const double cible = t - latence;
        std::vector<Pose> poses;
        for (int i = 0; i <= static_cast<int>(3.0 / kPeriode); ++i) {
            const double temps = i * kPeriode;
            poses.push_back({temps, Lacet(temps)});
        }
        const size_t dernier = static_cast<size_t>(t / kPeriode);
        const size_t debut = dernier + 1 > 64 ? dernier + 1 - 64 : 0;
        const auto debutHistorique = poses.begin() + debut;
        const auto finHistorique = poses.begin() + dernier + 1;
        const auto it = std::upper_bound(debutHistorique, finHistorique, cible,
            [](double temps, const Pose &pose) { return temps < pose.temps; });
        const size_t index = it == debutHistorique ? debut : static_cast<size_t>(it - poses.begin() - 1);
        const double horizon = t + kPeriode - poses[index].temps;
        const double utilise = borne ? std::clamp(horizon, 0.0, 0.100) : horizon;
        const double v = index + 1 < poses.size() ? Vitesse(poses[index], poses[index + 1]) : 0.0;
        const double predit = poses[index].lacet + v * utilise;
        std::printf("%s : echantillon a %.6f s, horizon %.3f ms, predit %.6f deg, vrai %.6f deg\n",
                    borne ? "avec borne" : "sans borne", poses[index].temps,
                    utilise * 1000.0, predit, Lacet(t + kPeriode));
    }
}

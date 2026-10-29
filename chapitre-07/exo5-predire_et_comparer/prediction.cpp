#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace {

double Lacet(double t) {
    const double duree = 0.400;
    if (t <= 0.0) return 0.0;
    if (t >= duree) return 90.0;
    return 45.0 * (1.0 - std::cos(3.14159265358979323846 * t / duree));
}

} // namespace

int main() {
    const char *texte = std::getenv("MASALLE_HZ");
    const double hz = texte != nullptr ? std::atof(texte) : 72.0;
    if (hz <= 1.0) {
        std::fprintf(stderr, "MASALLE_HZ doit être supérieur à 1.\n");
        return 1;
    }
    std::printf("MASALLE_HZ = %.0f ; periode = %.6f ms\n", hz, 1000.0 / hz);
    std::printf("geste de contrôle : 90 deg en 400 ms ; lacet à t=200 ms : %.6f deg\n",
                Lacet(0.200));
    std::puts("Les erreurs sur 1000 images doivent être mesurées pendant le mouvement réel de la souris.");
}

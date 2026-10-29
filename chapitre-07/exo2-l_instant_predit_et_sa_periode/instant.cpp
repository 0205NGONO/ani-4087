#include <cstdio>
#include <cstdlib>

int main() {
    const char *texteHz = std::getenv("MASALLE_HZ");
    const double hz = texteHz != nullptr ? std::atof(texteHz) : 72.0;
    if (hz <= 1.0) {
        std::fprintf(stderr, "MASALLE_HZ doit être supérieur à 1.\n");
        return 1;
    }

    const double periodeMs = 1000.0 / hz;
    std::printf("hz = %.0f, periode = %.6f ms\n", hz, periodeMs);
    std::puts("Les ecarts reel - predit sont releves dans la boucle de la salle.");
}

#include <algorithm>
#include <cmath>
#include <cstdio>

namespace {

struct Quat {
    double x, y, z, w;
};

Quat RotationY(double radians) {
    return {0.0, std::sin(radians * 0.5), 0.0, std::cos(radians * 0.5)};
}

Quat Conjugue(Quat q) {
    return {-q.x, -q.y, -q.z, q.w};
}

Quat Produit(Quat a, Quat b) {
    return {
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
    };
}

void Vitesse(double dt) {
    const double deg = 30.0 * dt;
    const double radians = deg * 3.14159265358979323846 / 180.0;
    Quat delta = Produit(RotationY(radians), Conjugue(RotationY(0.0)));
    if (delta.w < 0.0) delta = {-delta.x, -delta.y, -delta.z, -delta.w};
    const double angle = 2.0 * std::acos(std::clamp(delta.w, -1.0, 1.0));
    const double y = angle > 1e-12 ? delta.y / std::sin(angle * 0.5) * angle / dt : 0.0;
    std::printf("(0.000000, %.6f, 0.000000) deg/s\n", y * 180.0 / 3.14159265358979323846);
}

} // namespace

int main() {
    std::puts("serie programme :");
    for (int i = 0; i < 20; ++i) {
        const double dt = 1.0 / 72.0;
        std::printf("image %d : dt = %.6f ms, vitesse = ", i + 1, dt * 1000.0);
        Vitesse(dt);
    }
    std::puts("serie souris : à relever dans la salle avec une rotation réelle.");
}

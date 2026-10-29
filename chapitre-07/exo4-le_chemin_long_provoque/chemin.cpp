#include <cmath>
#include <cstdio>

namespace {

constexpr double kPi = 3.14159265358979323846;
constexpr double kDt = 1.0 / 72.0;

struct Quat {
    double y;
    double w;
};

Quat Orientation(double lacetDeg) {
    const double demi = lacetDeg * kPi / 360.0;
    const double signeCanonique = std::cos(demi) < 0.0 ? -1.0 : 1.0;
    return {signeCanonique * std::sin(demi), signeCanonique * std::cos(demi)};
}

double Vitesse(bool cheminCourt, double lacet, double lacetAvant) {
    const Quat a = Orientation(lacetAvant);
    const Quat b = Orientation(lacet);
    double y = b.y * a.w - b.w * a.y;
    double w = b.w * a.w + b.y * a.y;
    if (cheminCourt && w < 0.0) {
        y = -y;
        w = -w;
    }
    const double angle = 2.0 * std::atan2(y, w);
    return angle / kDt * 180.0 / kPi;
}

void Imprimer(bool cheminCourt, int image) {
    const double lacet = image * kDt * 20.0;
    const double avant = (image - 1) * kDt * 20.0;
    const double vitesse = Vitesse(cheminCourt, lacet, avant);
    const Quat q0 = Orientation(avant);
    const Quat q1 = Orientation(lacet);
    const double deltaW = q1.w * q0.w + q1.y * q0.y;
    std::printf("image %d : lacet = %.6f deg, vitesse = (0, %.6f, 0) deg/s, delta.w = %.9f\n",
                image, lacet, vitesse, deltaW);
}

} // namespace

int main() {
    const int image = 648;
    std::puts("sans le test :");
    for (int i = image - 1; i <= image + 1; ++i) Imprimer(false, i);
    std::puts("avec le test :");
    for (int i = image - 1; i <= image + 1; ++i) Imprimer(true, i);
}

#include <cmath>
#include <cstdlib>
#include <iostream>

int main() {
    int imageCount = 0;
    long long delay = 0;
    if (!(std::cin >> imageCount >> delay)) {
        return 0;
    }

    long long positionSum = 0;
    long long angleSum = 0;
    for (int image = 1; image <= imageCount; ++image) {
        long long x1 = 0;
        long long z1 = 0;
        long long angle1 = 0;
        long long x2 = 0;
        long long z2 = 0;
        long long angle2 = 0;
        std::cin >> x1 >> z1 >> angle1 >> x2 >> z2 >> angle2;

        const long double dx = static_cast<long double>(x2) - x1;
        const long double dz = static_cast<long double>(z2) - z1;
        const long long position = static_cast<long long>(
            std::floor(std::sqrt(dx * dx + dz * dz) + 0.5L));
        const long long angle = std::llabs(angle2 - angle1);
        positionSum += position;
        angleSum += angle;

        std::cout << image << " POSITION " << position << " ANGLE " << angle << '\n';
    }

    if (imageCount == 0 || delay == 0) {
        std::cout << "AUCUNE\n";
        return 0;
    }

    std::cout << "MOYENNE POSITION " << positionSum / imageCount << '\n'
              << "MOYENNE ANGLE " << angleSum / imageCount << '\n'
              << "VITESSE " << angleSum / (static_cast<long long>(imageCount) * delay)
              << '\n';
    return 0;
}

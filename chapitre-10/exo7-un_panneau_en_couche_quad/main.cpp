#include <cmath>
#include <iostream>
#include <string>

struct Pose {
    long long x;
    long long y;
    long long z;
    int yaw;
};

void rotate(long long &x, long long &z, int yaw) {
    const long long originalX = x;
    const long long originalZ = z;
    switch (yaw) {
    case 1:
        x = originalZ;
        z = -originalX;
        break;
    case 2:
        x = -originalX;
        z = -originalZ;
        break;
    case 3:
        x = -originalZ;
        z = originalX;
        break;
    default:
        break;
    }
}

Pose compose(const Pose &anchor, const Pose &pose) {
    long long x = pose.x;
    long long z = pose.z;
    rotate(x, z, anchor.yaw);
    return {anchor.x + x, anchor.y + pose.y, anchor.z + z,
            (anchor.yaw + pose.yaw) % 4};
}

int main() {
    Pose anchor{};
    long long eyeX = 0;
    long long eyeY = 0;
    long long eyeZ = 0;
    int panelCount = 0;
    if (!(std::cin >> anchor.x >> anchor.y >> anchor.z >> anchor.yaw
                  >> eyeX >> eyeY >> eyeZ >> panelCount)) {
        return 0;
    }

    for (int panel = 1; panel <= panelCount; ++panel) {
        char space = '-';
        Pose center{};
        long long width = 0;
        long long height = 0;
        std::cin >> space >> center.x >> center.y >> center.z >> center.yaw
                 >> width >> height;

        const bool local = space != 'S';
        if (local) {
            center = compose(anchor, center);
        }
        std::cout << "PANNEAU " << panel << ' '
                  << (local ? "LOCAL" : "STAGE") << '\n';

        const long long halfWidth = width / 2;
        const long long halfHeight = height / 2;
        const long long cornerX[4] = {-halfWidth, halfWidth, halfWidth, -halfWidth};
        const long long cornerY[4] = {-halfHeight, -halfHeight, halfHeight, halfHeight};
        for (int corner = 0; corner < 4; ++corner) {
            long long x = cornerX[corner];
            long long z = 0;
            rotate(x, z, center.yaw);
            std::cout << center.x + x << ' ' << center.y + cornerY[corner]
                      << ' ' << center.z + z << '\n';
        }

        const long double dx = static_cast<long double>(center.x - eyeX);
        const long double dy = static_cast<long double>(center.y - eyeY);
        const long double dz = static_cast<long double>(center.z - eyeZ);
        const long long distance = static_cast<long long>(
            std::floor(std::sqrt(dx * dx + dy * dy + dz * dz) + 0.5L));
        std::cout << "DISTANCE " << distance << '\n';
    }

    std::cout << "PANNEAUX " << panelCount << '\n';
    return 0;
}

#include <iostream>

struct Pose {
    long long x;
    long long y;
    long long z;
    int yaw;
};

Pose compose(const Pose &anchor, const Pose &pose) {
    long long x = pose.x;
    long long z = pose.z;
    switch (anchor.yaw) {
    case 1:
        x = pose.z;
        z = -pose.x;
        break;
    case 2:
        x = -pose.x;
        z = -pose.z;
        break;
    case 3:
        x = -pose.z;
        z = pose.x;
        break;
    default:
        break;
    }

    return {anchor.x + x, anchor.y + pose.y, anchor.z + z,
            (anchor.yaw + pose.yaw) % 4};
}

int main() {
    Pose anchor{};
    int poseCount = 0;
    if (!(std::cin >> anchor.x >> anchor.y >> anchor.z >> anchor.yaw >> poseCount)) {
        return 0;
    }

    int correct = 0;
    int incorrect = 0;
    for (int index = 1; index <= poseCount; ++index) {
        char requestedSpace = 'S';
        char declaredSpace = 'S';
        Pose pose{};
        std::cin >> requestedSpace >> declaredSpace
                 >> pose.x >> pose.y >> pose.z >> pose.yaw;

        const Pose actual = requestedSpace == 'L' ? compose(anchor, pose) : pose;
        const Pose believed = declaredSpace == 'L' ? compose(anchor, pose) : pose;
        const long long dx = believed.x - actual.x;
        const long long dy = believed.y - actual.y;
        const long long dz = believed.z - actual.z;
        const int dk = (believed.yaw - actual.yaw + 4) % 4;

        std::cout << index << " CRU " << believed.x << ' ' << believed.y << ' '
                  << believed.z << ' ' << believed.yaw << '\n'
                  << index << " ECART " << dx << ' ' << dy << ' ' << dz << ' '
                  << dk << '\n';

        if (dx == 0 && dy == 0 && dz == 0 && dk == 0) {
            ++correct;
        } else {
            ++incorrect;
        }
    }

    std::cout << "JUSTES " << correct << '\n'
              << "FAUSSES " << incorrect << '\n';
    return 0;
}

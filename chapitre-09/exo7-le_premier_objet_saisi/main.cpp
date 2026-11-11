#include <cstdint>
#include <iostream>
#include <vector>

struct Pose {
    std::int64_t x;
    std::int64_t y;
    int angle;
};

Pose rotate(const Pose& pose, int turns) {
    switch (turns % 4) {
    case 1:
        return {-pose.y, pose.x, pose.angle};
    case 2:
        return {-pose.x, -pose.y, pose.angle};
    case 3:
        return {pose.y, -pose.x, pose.angle};
    default:
        return pose;
    }
}

Pose compose(const Pose& first, const Pose& second) {
    const Pose translated = rotate(second, first.angle);
    return {first.x + translated.x, first.y + translated.y,
            (first.angle + second.angle) % 4};
}

Pose inverse(const Pose& pose) {
    const int inverseAngle = (4 - pose.angle) % 4;
    const Pose negated{-pose.x, -pose.y, 0};
    const Pose translated = rotate(negated, inverseAngle);
    return {translated.x, translated.y, inverseAngle};
}

int main() {
    int objectCount = 0;
    int frameCount = 0;
    if (!(std::cin >> objectCount >> frameCount)) {
        return 0;
    }

    std::vector<Pose> objects(static_cast<std::size_t>(objectCount));
    for (Pose& object : objects) {
        std::cin >> object.x >> object.y >> object.angle;
    }

    int heldObject = -1;
    Pose attachment{};
    int previousButton = 0;
    int successfulGrabs = 0;
    int missedGrabs = 0;

    for (int frame = 1; frame <= frameCount; ++frame) {
        Pose palm{};
        int button = 0;
        std::cin >> palm.x >> palm.y >> palm.angle >> button;

        const bool pressed = button && !previousButton;
        const bool released = !button && previousButton;
        bool missedThisFrame = false;

        if (pressed) {
            int nearest = -1;
            std::int64_t nearestDistanceSquared = 22501;
            for (int i = 0; i < objectCount; ++i) {
                const std::int64_t dx = objects[static_cast<std::size_t>(i)].x -
                                        palm.x;
                const std::int64_t dy = objects[static_cast<std::size_t>(i)].y -
                                        palm.y;
                if (dx < -150 || dx > 150 || dy < -150 || dy > 150) {
                    continue;
                }
                const std::int64_t distanceSquared = dx * dx + dy * dy;
                if (distanceSquared <= 22500 &&
                    (distanceSquared < nearestDistanceSquared ||
                     (distanceSquared == nearestDistanceSquared &&
                      (nearest < 0 || i < nearest)))) {
                    nearest = i;
                    nearestDistanceSquared = distanceSquared;
                }
            }

            if (nearest < 0) {
                heldObject = -1;
                missedThisFrame = true;
                ++missedGrabs;
            } else {
                heldObject = nearest;
                attachment = compose(inverse(palm),
                                     objects[static_cast<std::size_t>(nearest)]);
                ++successfulGrabs;
            }
        } else if (released) {
            heldObject = -1;
        }

        if (heldObject >= 0) {
            objects[static_cast<std::size_t>(heldObject)] =
                compose(palm, attachment);
            const Pose& held = objects[static_cast<std::size_t>(heldObject)];
            std::cout << frame << " TIENT " << heldObject + 1 << ' '
                      << held.x << ' ' << held.y << ' ' << held.angle << '\n';
        } else if (missedThisFrame) {
            std::cout << frame << " RATE\n";
        } else {
            std::cout << frame << " LIBRE\n";
        }
        previousButton = button;
    }

    std::cout << "SAISIES " << successfulGrabs << '\n'
              << "RATEES " << missedGrabs << '\n';
    for (int i = 0; i < objectCount; ++i) {
        const Pose& object = objects[static_cast<std::size_t>(i)];
        std::cout << "OBJET " << i + 1 << ' ' << object.x << ' ' << object.y
                  << ' ' << object.angle << '\n';
    }
}

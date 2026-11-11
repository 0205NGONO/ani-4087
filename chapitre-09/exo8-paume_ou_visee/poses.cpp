#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

struct Point {
    std::int64_t x;
    std::int64_t y;
    std::int64_t z;
};

struct HeadPose {
    Point position;
    int turns;
};

Point rotate(const Point& point, int turns) {
    switch (turns % 4) {
    case 1:
        return {point.z, point.y, -point.x};
    case 2:
        return {-point.x, point.y, -point.z};
    case 3:
        return {-point.z, point.y, point.x};
    default:
        return point;
    }
}

Point worldPoint(const HeadPose& head, Point local) {
    const Point rotated = rotate(local, head.turns);
    return {head.position.x + rotated.x, head.position.y + rotated.y,
            head.position.z + rotated.z};
}

void printPoint(const char* label, const Point& point) {
    std::cout << label << ' ' << point.x << ' ' << point.y << ' ' << point.z
              << '\n';
}

int nearestObject(const Point& hand, const std::vector<Point>& objects) {
    int nearest = -1;
    std::int64_t nearestDistanceSquared = 22501;
    for (int i = 0; i < static_cast<int>(objects.size()); ++i) {
        const Point& object = objects[static_cast<std::size_t>(i)];
        const std::int64_t dx = object.x - hand.x;
        const std::int64_t dy = object.y - hand.y;
        const std::int64_t dz = object.z - hand.z;
        if (dx < -150 || dx > 150 || dy < -150 || dy > 150 ||
            dz < -150 || dz > 150) {
            continue;
        }
        const std::int64_t distanceSquared = dx * dx + dy * dy + dz * dz;
        if (distanceSquared <= 22500 &&
            (distanceSquared < nearestDistanceSquared ||
             (distanceSquared == nearestDistanceSquared &&
              (nearest < 0 || i < nearest)))) {
            nearest = i;
            nearestDistanceSquared = distanceSquared;
        }
    }
    return nearest;
}

int main() {
    int headCount = 0;
    int objectCount = 0;
    if (!(std::cin >> headCount >> objectCount)) {
        return 0;
    }

    std::vector<HeadPose> heads(static_cast<std::size_t>(headCount));
    for (HeadPose& head : heads) {
        std::cin >> head.position.x >> head.position.y >> head.position.z >>
            head.turns;
    }

    std::vector<Point> objects(static_cast<std::size_t>(objectCount));
    for (Point& object : objects) {
        std::cin >> object.x >> object.y >> object.z;
    }

    for (int i = 0; i < headCount; ++i) {
        const HeadPose& head = heads[static_cast<std::size_t>(i)];
        const Point palmRight =
            worldPoint(head, {180, -300, -250});
        const Point aimRight =
            worldPoint(head, {180, -250, -400});
        const Point palmLeft =
            worldPoint(head, {-180, -300, -250});
        const Point aimLeft =
            worldPoint(head, {-180, -250, -400});

        std::cout << "TETE " << i + 1 << '\n';
        printPoint("PAUME D", palmRight);
        printPoint("VISEE D", aimRight);
        printPoint("PAUME G", palmLeft);
        printPoint("VISEE G", aimLeft);
    }

    const Point palmRight = worldPoint(heads.front(), {180, -300, -250});
    const Point aimRight = worldPoint(heads.front(), {180, -250, -400});
    const int palmObject = nearestObject(palmRight, objects);
    const int aimObject = nearestObject(aimRight, objects);
    std::cout << "SAISIE PAUME ";
    if (palmObject < 0) {
        std::cout << "AUCUNE\n";
    } else {
        std::cout << palmObject + 1 << '\n';
    }
    std::cout << "SAISIE VISEE ";
    if (aimObject < 0) {
        std::cout << "AUCUNE\n";
    } else {
        std::cout << aimObject + 1 << '\n';
    }

    const long double dx =
        static_cast<long double>(palmRight.x - aimRight.x);
    const long double dy =
        static_cast<long double>(palmRight.y - aimRight.y);
    const long double dz =
        static_cast<long double>(palmRight.z - aimRight.z);
    const auto distance = static_cast<std::int64_t>(
        std::llround(std::sqrt(dx * dx + dy * dy + dz * dz)));
    std::cout << "ECART " << distance << '\n';
}

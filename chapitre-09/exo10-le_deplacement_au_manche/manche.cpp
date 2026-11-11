#include <iostream>
#include <string>
#include <unordered_set>

int main() {
    double step = 0.0;
    int frameCount = 0;
    if (!(std::cin >> step >> frameCount)) {
        return 0;
    }

    double offsetX = 0.0;
    double offsetZ = 0.0;
    int diagonalFrames = 0;

    for (int frame = 1; frame <= frameCount; ++frame) {
        int turns = 0;
        int keyCount = 0;
        std::cin >> turns >> keyCount;
        std::unordered_set<std::string> held;
        for (int i = 0; i < keyCount; ++i) {
            std::string key;
            std::cin >> key;
            held.insert(key);
        }

        const int x = static_cast<int>(held.count("F_DROITE") != 0) -
                      static_cast<int>(held.count("F_GAUCHE") != 0);
        const int y = static_cast<int>(held.count("F_HAUT") != 0) -
                      static_cast<int>(held.count("F_BAS") != 0);
        diagonalFrames += static_cast<int>(x != 0 && y != 0);

        int forwardX = 0;
        int forwardZ = -1;
        int rightX = 1;
        int rightZ = 0;
        switch (turns % 4) {
        case 1:
            forwardX = -1;
            forwardZ = 0;
            rightX = 0;
            rightZ = -1;
            break;
        case 2:
            forwardX = 0;
            forwardZ = 1;
            rightX = -1;
            rightZ = 0;
            break;
        case 3:
            forwardX = 1;
            forwardZ = 0;
            rightX = 0;
            rightZ = 1;
            break;
        default:
            break;
        }

        offsetX += step * (y * forwardX + x * rightX);
        offsetZ += step * (y * forwardZ + x * rightZ);
        std::cout << frame << ' ' << x << ',' << y << ' ' << offsetX << ' '
                  << offsetZ << '\n';
    }

    std::cout << "DECALAGE " << offsetX << ' ' << offsetZ << '\n'
              << "DIAGONALES " << diagonalFrames << '\n';
}

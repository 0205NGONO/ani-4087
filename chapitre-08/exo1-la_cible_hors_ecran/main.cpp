#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

struct Target {
    std::uint64_t width;
    std::uint64_t height;
};

int main() {
    std::uint64_t windowWidth = 0;
    std::uint64_t windowHeight = 0;
    std::uint64_t bufferCount = 0;
    std::uint64_t orderCount = 0;
    if (!(std::cin >> windowWidth >> windowHeight >> bufferCount >> orderCount)) {
        return 0;
    }

    std::uint64_t renderWidth = windowWidth;
    std::uint64_t renderHeight = windowHeight;
    std::uint64_t currentTarget = 0;
    std::uint64_t images = 0;
    bool offscreenEnabled = false;
    bool blocked = false;
    std::vector<Target> targets;
    std::vector<bool> busy(static_cast<std::size_t>(bufferCount), false);

    for (std::uint64_t order = 0; order < orderCount; ++order) {
        std::string command;
        std::cin >> command;

        if (command == "allume") {
            std::string subsystem;
            std::cin >> subsystem;
            if (!blocked && subsystem == "OFFSCREEN") {
                offscreenEnabled = true;
            }
        } else if (command == "cible") {
            std::uint64_t width = 0;
            std::uint64_t height = 0;
            std::cin >> width >> height;
            if (blocked) {
                continue;
            }
            if (!offscreenEnabled) {
                std::cout << "CIBLE NULLE\n";
            } else {
                targets.push_back({width, height});
                std::cout << "CIBLE " << targets.size() << ' ' << width << ' '
                          << height << '\n';
            }
        } else if (command == "vers") {
            std::uint64_t target = 0;
            std::cin >> target;
            if (blocked) {
                continue;
            }
            if (target == 0 || target <= targets.size()) {
                currentTarget = target;
            } else {
                std::cout << "VERS REFUSE\n";
            }
        } else if (command == "taille") {
            std::uint64_t width = 0;
            std::uint64_t height = 0;
            std::cin >> width >> height;
            if (!blocked) {
                renderWidth = width;
                renderHeight = height;
            }
        } else if (command == "image" && !blocked) {
            const std::uint64_t imageNumber = images + 1;
            const std::size_t buffer =
                static_cast<std::size_t>((imageNumber - 1) % bufferCount);
            if (busy[buffer]) {
                std::cout << "BLOQUE " << imageNumber << '\n';
                blocked = true;
                continue;
            }

            ++images;
            if (currentTarget == 0) {
                std::cout << "IMAGE " << imageNumber << " FENETRE\n";
            } else {
                std::cout << "IMAGE " << imageNumber << " CIBLE "
                          << currentTarget << '\n';
                const Target& target =
                    targets[static_cast<std::size_t>(currentTarget - 1)];
                if (renderWidth > target.width || renderHeight > target.height) {
                    busy[buffer] = true;
                }
            }
        }
    }

    std::cout << "IMAGES " << images << '\n';
    std::cout << "ETAT " << (blocked ? "BLOQUE" : "LIBRE") << '\n';
}

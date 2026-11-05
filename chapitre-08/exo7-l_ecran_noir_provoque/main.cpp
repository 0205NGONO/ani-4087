#include <cstdint>
#include <iostream>
#include <string>

int main() {
    std::uint64_t orderCount = 0;
    if (!(std::cin >> orderCount)) {
        return 0;
    }

    std::uint64_t currentTarget = 0;
    std::uint64_t savedTarget = 0;
    std::uint64_t nextHandle = 101;
    std::uint64_t imageCount = 0;
    std::uint64_t displayed = 0;
    std::uint64_t black = 0;
    std::uint64_t stale = 0;

    for (std::uint64_t i = 0; i < orderCount; ++i) {
        std::string order;
        std::cin >> order;
        if (order == "cible") {
            std::uint64_t width = 0;
            std::uint64_t height = 0;
            std::cin >> width >> height;
            currentTarget = nextHandle++;
            std::cout << "CIBLE " << currentTarget << ' ' << width << ' '
                      << height << '\n';
        } else if (order == "chaine") {
            savedTarget = 0;
        } else if (order == "enregistre") {
            savedTarget = currentTarget;
        } else if (order == "image") {
            ++imageCount;
            if (savedTarget == 0) {
                std::cout << "IMAGE " << imageCount << " NOIR\n";
                ++black;
            } else if (savedTarget == currentTarget) {
                std::cout << "IMAGE " << imageCount << " AFFICHE "
                          << savedTarget << '\n';
                ++displayed;
            } else {
                std::cout << "IMAGE " << imageCount << " PERIMEE "
                          << savedTarget << '\n';
                ++stale;
            }
        }
    }

    std::cout << "AFFICHEES " << displayed << '\n';
    std::cout << "NOIRES " << black << '\n';
    std::cout << "PERIMEES " << stale << '\n';
}

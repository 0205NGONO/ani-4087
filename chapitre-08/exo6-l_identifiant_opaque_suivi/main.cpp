#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

int main() {
    std::uint64_t imageCount = 0;
    std::uint64_t orderCount = 0;
    if (!(std::cin >> imageCount >> orderCount)) {
        return 0;
    }

    std::vector<std::uint64_t> slots(static_cast<std::size_t>(imageCount), 0);
    std::unordered_set<std::uint64_t> textures;
    std::uint64_t deadReads = 0;
    std::uint64_t largeHandles = 0;

    for (std::uint64_t i = 0; i < orderCount; ++i) {
        std::string order;
        std::uint64_t value = 0;
        std::cin >> order >> value;
        if (order == "cree") {
            textures.insert(value);
        } else if (order == "detruit") {
            textures.erase(value);
        } else if (order == "set") {
            std::uint64_t handle = 0;
            std::cin >> handle;
            if (value >= imageCount) {
                std::cout << "SET IGNORE " << value << '\n';
            } else {
                slots[static_cast<std::size_t>(value)] = handle;
                if (handle > UINT64_C(4294967295)) {
                    ++largeHandles;
                }
            }
        } else if (order == "get") {
            const std::uint64_t handle =
                value < imageCount ? slots[static_cast<std::size_t>(value)] : 0;
            if (handle == 0) {
                std::cout << "GET " << value << " 0 VIDE\n";
            } else if (textures.find(handle) != textures.end()) {
                std::cout << "GET " << value << ' ' << handle << " VIVANTE\n";
            } else {
                std::cout << "GET " << value << ' ' << handle << " MORTE\n";
                ++deadReads;
            }
        }
    }

    std::cout << "MORTES " << deadReads << '\n';
    std::cout << "LARGES " << largeHandles << '\n';
}

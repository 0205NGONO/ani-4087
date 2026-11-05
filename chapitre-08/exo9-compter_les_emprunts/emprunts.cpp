#include <cstdint>
#include <iostream>
#include <string>

int main() {
    std::uint64_t imageCount = 0;
    std::uint64_t frameCount = 0;
    if (!(std::cin >> imageCount >> frameCount)) {
        return 0;
    }

    bool acquired = false;
    std::uint64_t successfulAcquires = 0;
    std::uint64_t refusedAcquires = 0;
    std::uint64_t releases = 0;
    std::uint64_t firstLeak = 0;

    for (std::uint64_t frame = 1; frame <= frameCount; ++frame) {
        std::string path;
        std::cin >> path;
        if (acquired) {
            std::cout << frame << " BLOQUEE\n";
            ++refusedAcquires;
            continue;
        }

        acquired = true;
        ++successfulAcquires;
        if (path == "sortie") {
            std::cout << frame << " FUITE\n";
            if (firstLeak == 0) {
                firstLeak = frame;
            }
        } else if (path == "double") {
            ++refusedAcquires;
            acquired = false;
            ++releases;
            std::cout << frame << " DOUBLE\n";
        } else {
            acquired = false;
            ++releases;
            std::cout << frame << " OK\n";
        }
    }

    std::cout << "REUSSIES " << successfulAcquires << '\n';
    std::cout << "REFUSEES " << refusedAcquires << '\n';
    std::cout << "LIBERATIONS " << releases << '\n';
    std::cout << "ECART "
              << static_cast<std::int64_t>(successfulAcquires) -
                     static_cast<std::int64_t>(releases)
              << '\n';
    if (firstLeak == 0) {
        std::cout << "PREMIERE FUITE AUCUNE\n";
    } else {
        std::cout << "PREMIERE FUITE " << firstLeak << '\n';
    }
}

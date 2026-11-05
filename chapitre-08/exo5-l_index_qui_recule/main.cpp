#include <cstdint>
#include <iostream>

int main() {
    std::uint64_t count = 0;
    if (!(std::cin >> count)) {
        return 0;
    }

    std::uint64_t refused = 0;
    std::uint64_t signedErrors = 0;
    std::uint64_t unsignedErrors = 0;

    for (std::uint64_t i = 0; i < count; ++i) {
        std::uint64_t imageCount = 0;
        std::uint64_t releases = 0;
        std::cin >> imageCount >> releases;

        if (imageCount == 0 || imageCount > 4) {
            std::cout << imageCount << ' ' << releases << " REFUSEE\n";
            ++refused;
            continue;
        }

        const std::uint64_t next = releases % imageCount;
        const std::uint64_t correct =
            (next + imageCount - 1) % imageCount;
        const int signedValue =
            (static_cast<int>(next) - 1) % static_cast<int>(imageCount);
        const std::uint32_t unsignedValue =
            (static_cast<std::uint32_t>(next) - std::uint32_t{1}) %
            static_cast<std::uint32_t>(imageCount);

        if (signedValue != static_cast<int>(correct)) {
            ++signedErrors;
        }
        if (unsignedValue != correct) {
            ++unsignedErrors;
        }

        std::cout << imageCount << ' ' << releases << ' ' << next << ' '
                  << correct << ' ' << signedValue << ' ' << unsignedValue
                  << '\n';
    }

    std::cout << "REFUSEES " << refused << '\n';
    std::cout << "FAUX SIGNE " << signedErrors << '\n';
    std::cout << "FAUX NON SIGNE " << unsignedErrors << '\n';
}

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

int main() {
    std::uint64_t imageCount = 0;
    std::uint64_t frameCount = 0;
    std::int64_t startPosition = 0;
    std::int64_t step = 0;
    if (!(std::cin >> imageCount >> frameCount >> startPosition >> step)) {
        return 0;
    }

    std::string modes;
    std::cin >> modes;
    std::vector<std::uint64_t> painted(
        static_cast<std::size_t>(imageCount), 0);
    std::uint64_t nextImage = 0;
    bool previousWasVisible = false;
    std::int64_t previousPosition = 0;
    std::vector<std::int64_t> jumps;

    for (std::uint64_t frame = 1; frame <= frameCount; ++frame) {
        const std::uint64_t acquiredImage = nextImage;
        painted[static_cast<std::size_t>(acquiredImage)] = frame;
        const char mode = modes[static_cast<std::size_t>(frame - 1)];

        std::uint64_t submittedImage = acquiredImage;
        if (mode == 'P') {
            submittedImage = (nextImage + imageCount - 1) % imageCount;
        }

        nextImage = (nextImage + 1) % imageCount;
        if (mode == 'D') {
            submittedImage = (nextImage + imageCount - 1) % imageCount;
        }

        const std::uint64_t shownFrame =
            painted[static_cast<std::size_t>(submittedImage)];
        if (shownFrame == 0) {
            std::cout << frame << " NOIR -\n";
            previousWasVisible = false;
            continue;
        }

        const std::int64_t position =
            startPosition + step * static_cast<std::int64_t>(shownFrame);
        std::cout << frame << ' ' << position << ' ';
        if (!previousWasVisible) {
            std::cout << "-\n";
        } else {
            const std::int64_t jump = position - previousPosition;
            std::cout << jump << '\n';
            jumps.push_back(jump);
        }
        previousPosition = position;
        previousWasVisible = true;
    }

    if (jumps.empty()) {
        std::cout << "SAUTS AUCUN\n";
    } else {
        const auto bounds = std::minmax_element(jumps.begin(), jumps.end());
        std::cout << "SAUT MIN " << *bounds.first << '\n';
        std::cout << "SAUT MAX " << *bounds.second << '\n';
        std::cout << "TREMBLE "
                  << (*bounds.first != *bounds.second ? "OUI" : "NON") << '\n';
    }
}

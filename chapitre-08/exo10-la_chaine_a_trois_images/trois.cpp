#include <cstdint>
#include <iostream>

int main() {
    std::uint64_t scenarioCount = 0;
    if (!(std::cin >> scenarioCount)) {
        return 0;
    }

    for (std::uint64_t scenario = 0; scenario < scenarioCount; ++scenario) {
        std::uint64_t imageCount = 0;
        std::uint64_t holdFrames = 0;
        std::uint64_t frameCount = 0;
        std::cin >> imageCount >> holdFrames >> frameCount;

        std::cout << "CIBLES " << imageCount << ' ' << holdFrames;
        std::uint64_t conflicts = 0;
        for (std::uint64_t frame = 1; frame <= frameCount; ++frame) {
            const std::uint64_t image = (frame - 1) % imageCount;
            std::cout << ' ' << image;

            bool conflict = false;
            for (std::uint64_t age = 1; age <= holdFrames && age < frame; ++age) {
                const std::uint64_t previouslySubmittedImage =
                    (frame - age - 1) % imageCount;
                if (image == previouslySubmittedImage) {
                    conflict = true;
                    break;
                }
            }
            if (conflict) {
                ++conflicts;
            }
        }
        std::cout << '\n';
        std::cout << "CONFLITS " << imageCount << ' ' << holdFrames << ' '
                  << conflicts << '\n';
    }
}

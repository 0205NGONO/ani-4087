#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::uint64_t imageCount = 0;
    std::uint64_t orderCount = 0;
    if (!(std::cin >> imageCount >> orderCount)) {
        return 0;
    }

    std::uint64_t nextImage = 0;
    std::uint64_t acquiredImage = 0;
    std::uint64_t releasedImage = 0;
    bool acquired = false;
    bool acquiredImageWasDrawn = false;
    bool hasUnsubmittedRelease = false;
    bool releasedImageWasDrawn = false;
    std::uint64_t submitted = 0;
    std::uint64_t refused = 0;
    std::uint64_t outsideDraws = 0;
    std::vector<bool> drawn(static_cast<std::size_t>(imageCount), false);

    for (std::uint64_t i = 0; i < orderCount; ++i) {
        std::string order;
        std::cin >> order;

        if (order == "acquire") {
            if (acquired) {
                std::cout << "ACQUIRE REFUSE\n";
                ++refused;
            } else {
                acquiredImage = nextImage;
                acquiredImageWasDrawn = false;
                acquired = true;
                std::cout << "ACQUIRE " << acquiredImage << '\n';
            }
        } else if (order == "draw") {
            if (acquired) {
                acquiredImageWasDrawn = true;
                std::cout << "DRAW " << acquiredImage << '\n';
            } else {
                std::cout << "DRAW HORS EMPRUNT\n";
                ++outsideDraws;
            }
        } else if (order == "release") {
            if (!acquired) {
                std::cout << "RELEASE REFUSE\n";
                ++refused;
            } else {
                releasedImage = acquiredImage;
                releasedImageWasDrawn = acquiredImageWasDrawn;
                drawn[static_cast<std::size_t>(releasedImage)] =
                    releasedImageWasDrawn;
                nextImage = (nextImage + 1) % imageCount;
                acquired = false;
                hasUnsubmittedRelease = true;
                std::cout << "RELEASE OK\n";
            }
        } else if (order == "submit") {
            if (acquired) {
                std::cout << "SUBMIT REFUSE EMPRUNTEE\n";
                ++refused;
            } else if (!hasUnsubmittedRelease) {
                std::cout << "SUBMIT REFUSE RIEN\n";
                ++refused;
            } else {
                std::cout << "SUBMIT " << releasedImage;
                if (!drawn[static_cast<std::size_t>(releasedImage)]) {
                    std::cout << " VIDE";
                }
                std::cout << '\n';
                ++submitted;
                hasUnsubmittedRelease = false;
            }
        }
    }

    std::cout << "SOUMISES " << submitted << '\n';
    std::cout << "REFUS " << refused << '\n';
    std::cout << "HORS EMPRUNT " << outsideDraws << '\n';
}

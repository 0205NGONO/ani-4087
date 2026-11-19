#include <iostream>
#include <string>

int main() {
    int imageCount = 0;
    int frameCount = 0;
    if (!(std::cin >> imageCount >> frameCount)) {
        return 0;
    }

    if (imageCount < 1 || imageCount > 4) {
        std::cout << "CHAINE REFUSEE\n";
        return 0;
    }

    std::string shouldRender;
    if (frameCount > 0) {
        std::cin >> shouldRender;
    }

    int nextImage = 0;
    int rendered = 0;
    int skipped = 0;

    for (int frame = 1; frame <= frameCount; ++frame) {
        if (shouldRender[frame - 1] == '1') {
            std::cout << frame << " OUI 1 " << frame - 1 << ' ' << nextImage << '\n';
            nextImage = (nextImage + 1) % imageCount;
            ++rendered;
        } else {
            std::cout << frame << " NON 0\n";
            ++skipped;
        }
    }

    std::cout << "FINS " << frameCount << '\n'
              << "RENDUES " << rendered << '\n'
              << "SAUTEES " << skipped << '\n'
              << "PROCHAINE " << nextImage << '\n';
    return 0;
}

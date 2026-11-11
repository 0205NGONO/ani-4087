#include <iostream>
#include <string>

int main() {
    int frameCount = 0;
    if (!(std::cin >> frameCount)) {
        return 0;
    }

    std::string states;
    if (frameCount > 0) {
        std::cin >> states;
    }

    int previousCurrent = 0;
    int presses = 0;
    int releases = 0;
    int falseReleases = 0;
    int heldFrames = 0;

    for (int i = 0; i < frameCount; ++i) {
        const char state = states[static_cast<std::size_t>(i)];
        const int active = static_cast<int>(state != 'x');
        const int current = static_cast<int>(state == '1');
        const int changed = static_cast<int>(current != previousCurrent);

        if (current && changed) {
            ++presses;
        }
        if (!current && changed) {
            ++releases;
            if (!active) {
                ++falseReleases;
            }
        }
        heldFrames += current;
        std::cout << i + 1 << ' ' << current << ' ' << changed << ' '
                  << active << '\n';
        previousCurrent = current;
    }

    std::cout << "APPUIS " << presses << '\n'
              << "RELACHES " << releases << '\n'
              << "FAUX RELACHES " << falseReleases << '\n'
              << "TENUES " << heldFrames << '\n';
}

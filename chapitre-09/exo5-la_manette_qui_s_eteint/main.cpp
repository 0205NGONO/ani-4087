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

    bool previousCurrent = false;
    bool heldWith = false;
    bool heldWithout = false;
    bool fallenWithout = false;
    int pickupsWith = 0;
    int pickupsWithout = 0;
    int fallsWithout = 0;
    int stuckWith = 0;
    int differences = 0;

    for (int i = 0; i < frameCount; ++i) {
        const char state = states[static_cast<std::size_t>(i)];
        const bool active = state != 'x';
        const bool current = state == '1';
        const bool changed = current != previousCurrent;

        if (active && changed) {
            if (current) {
                heldWith = true;
                ++pickupsWith;
            } else {
                heldWith = false;
            }
        }

        if (!fallenWithout && changed) {
            if (current) {
                heldWithout = true;
                ++pickupsWithout;
            } else if (!active) {
                if (heldWithout) {
                    heldWithout = false;
                    fallenWithout = true;
                    ++fallsWithout;
                }
            } else {
                heldWithout = false;
            }
        }

        if (state == '0' && heldWith) {
            ++stuckWith;
        }
        differences += static_cast<int>(heldWith != heldWithout);
        std::cout << i + 1 << ' ' << (heldWith ? "TENU" : "LIBRE") << ' '
                  << (heldWithout ? "TENU" : "LIBRE") << '\n';
        previousCurrent = current;
    }

    std::cout << "PRISES AVEC " << pickupsWith << '\n'
              << "PRISES SANS " << pickupsWithout << '\n'
              << "CHUTES SANS " << fallsWithout << '\n'
              << "COINCES AVEC " << stuckWith << '\n'
              << "DIFFERENCES " << differences << '\n';
}

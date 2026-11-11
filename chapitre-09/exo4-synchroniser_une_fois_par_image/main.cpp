#include <iostream>

int main() {
    int frameCount = 0;
    if (!(std::cin >> frameCount)) {
        return 0;
    }

    int current = 0;
    int changed = 0;
    int active = 0;
    int realPresses = 0;
    int grabs = 0;
    int refusedSyncs = 0;
    int previousPhysical = 0;

    for (int frame = 1; frame <= frameCount; ++frame) {
        int physical = 0;
        int syncCount = 0;
        int focused = 0;
        std::cin >> physical >> syncCount >> focused;

        if (physical && !previousPhysical) {
            ++realPresses;
        }
        previousPhysical = physical;

        for (int sync = 0; sync < syncCount; ++sync) {
            if (!focused) {
                ++refusedSyncs;
                continue;
            }
            changed = static_cast<int>(physical != current);
            current = physical;
            active = 1;
        }

        const bool grabbed = current && changed;
        grabs += static_cast<int>(grabbed);
        std::cout << frame << ' ' << current << ' ' << changed << ' '
                  << active << ' ' << (grabbed ? "SAISIT" : "-") << '\n';
    }

    std::cout << "APPUIS REELS " << realPresses << '\n'
              << "SAISIES " << grabs << '\n'
              << "SYNC REFUSEES " << refusedSyncs << '\n';
}

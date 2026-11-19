#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

int main() {
    long long speed = 0;
    int frameCount = 0;
    if (!(std::cin >> speed >> frameCount)) {
        return 0;
    }

    int falseFrames = 0;
    long long worstHeadset = 0;
    long long worstSimulator = 0;

    for (int frame = 1; frame <= frameCount; ++frame) {
        long long predicted = 0;
        long long submitted = 0;
        long long displayed = 0;
        char mode = 'R';
        std::cin >> predicted >> submitted >> displayed >> mode;

        const long long renderedPose = speed * predicted;
        const long long submittedPose = mode == 'R' ? renderedPose : speed * submitted;
        const long long headset = submittedPose - renderedPose;
        const long long simulator = speed * displayed - renderedPose;

        if (headset != 0) {
            ++falseFrames;
        }
        worstHeadset = std::max(worstHeadset, std::llabs(headset));
        worstSimulator = std::max(worstSimulator, std::llabs(simulator));
        std::cout << frame << " CASQUE " << headset
                  << " SIMULATEUR " << simulator << '\n';
    }

    std::cout << "FAUSSES " << falseFrames << '\n'
              << "PIRE CASQUE " << worstHeadset << '\n'
              << "PIRE SIMULATEUR " << worstSimulator << '\n';
    return 0;
}

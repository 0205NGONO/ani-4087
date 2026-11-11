#include <algorithm>
#include <cstdint>
#include <iostream>

int main() {
    int callCount = 0;
    if (!(std::cin >> callCount)) {
        return 0;
    }

    int goodCalls = 0;
    int longCalls = 0;
    int silentCalls = 0;
    std::int64_t totalDuration = 0;

    for (int call = 1; call <= callCount; ++call) {
        std::int64_t amplitude = 0;
        std::int64_t duration = 0;
        std::int64_t frequency = 0;
        std::cin >> amplitude >> duration >> frequency;

        amplitude = std::clamp<std::int64_t>(amplitude, 0, 100);
        const bool minimumDuration = duration <= 0;
        const bool defaultFrequency = frequency <= 0;
        const std::int64_t effectiveDuration =
            minimumDuration ? 0 : duration;
        const char* verdict = nullptr;

        if (amplitude == 0) {
            verdict = "MUETTE";
            ++silentCalls;
        } else if (duration < 50) {
            verdict = "BREVE";
        } else if (duration <= 200) {
            verdict = "BONNE";
            ++goodCalls;
        } else {
            verdict = "LONGUE";
            ++longCalls;
        }

        totalDuration += effectiveDuration;
        std::cout << call << ' ' << amplitude << ' ';
        if (minimumDuration) {
            std::cout << "MIN";
        } else {
            std::cout << duration;
        }
        std::cout << ' ';
        if (defaultFrequency) {
            std::cout << "DEFAUT";
        } else {
            std::cout << frequency;
        }
        std::cout << ' ' << verdict << '\n';
    }

    std::cout << "BONNES " << goodCalls << '\n'
              << "LONGUES " << longCalls << '\n'
              << "MUETTES " << silentCalls << '\n'
              << "TEMPS " << totalDuration << '\n';
}

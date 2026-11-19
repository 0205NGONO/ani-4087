#include <iostream>
#include <string>

int main() {
    std::string policy;
    int frameCount = 0;
    if (!(std::cin >> policy >> frameCount)) {
        return 0;
    }

    std::string shouldRender;
    if (frameCount > 0) {
        std::cin >> shouldRender;
    }

    bool frameOpen = false;
    int rendered = 0;
    int lost = 0;
    int errors = 0;
    int firstWaitRefusal = 0;

    for (int frame = 1; frame <= frameCount; ++frame) {
        bool render = false;
        const bool waitAccepted = !frameOpen;
        if (!waitAccepted) {
            ++errors;
            if (firstWaitRefusal == 0) {
                firstWaitRefusal = frame;
            }
        } else {
            render = shouldRender[frame - 1] == '1';
        }

        const bool beginAccepted = !frameOpen;
        if (!beginAccepted) {
            ++errors;
        } else {
            frameOpen = true;
        }

        if (render) {
            ++rendered;
        }

        const bool callEnd = policy == "TOUJOURS" || render;
        std::string endResult = "-";
        if (callEnd) {
            if (frameOpen) {
                frameOpen = false;
                endResult = "OK";
            } else {
                ++errors;
                endResult = "REFUSE";
            }
        }

        if (shouldRender[frame - 1] == '1' && !render) {
            ++lost;
        }

        std::cout << frame << ' ' << (waitAccepted ? "OK" : "REFUSE") << ' '
                  << (beginAccepted ? "OK" : "REFUSE") << ' '
                  << (render ? "OUI" : "NON") << ' ' << endResult << '\n';
    }

    std::cout << "RENDUES " << rendered << '\n'
              << "PERDUES " << lost << '\n'
              << "ERREURS " << errors << '\n'
              << "PREMIER REFUS "
              << (firstWaitRefusal == 0 ? "AUCUN" : std::to_string(firstWaitRefusal))
              << '\n';
    return 0;
}

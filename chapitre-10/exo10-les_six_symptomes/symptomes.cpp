#include <iostream>
#include <string>

int main() {
    int applicationCount = 0;
    if (!(std::cin >> applicationCount)) {
        return 0;
    }

    int silentWithSymptoms = 0;
    for (int application = 1; application <= applicationCount; ++application) {
        std::string end;
        int left = 0;
        int right = 0;
        char instant = 'P';
        char pose = 'R';
        char space = 'S';
        std::cin >> end >> left >> right >> instant >> pose >> space;

        std::string symptoms;
        const auto append = [&symptoms](const char *symptom) {
            if (!symptoms.empty()) {
                symptoms += ' ';
            }
            symptoms += symptom;
        };

        if (end == "JAMAIS") {
            append("FIGE");
        } else if (end == "SI_RENDU") {
            append("NE_REPART_PAS");
        }
        if (left == 0) {
            append("OEIL_NOIR_G");
        }
        if (right == 0) {
            append("OEIL_NOIR_D");
        }
        if (instant == 'N') {
            append("TREMBLE");
        }
        if (pose == 'F') {
            append("DE_TRAVERS");
        }
        if (space == 'L') {
            append("DECALE");
        }

        const bool simulatorSpeaks = end != "TOUJOURS" || left == 0 || right == 0;
        std::cout << application << " CASQUE "
                  << (symptoms.empty() ? "AUCUN" : symptoms) << '\n'
                  << application << " SIMULATEUR "
                  << (simulatorSpeaks ? "DIT" : "MUET") << '\n';
        if (!symptoms.empty() && !simulatorSpeaks) {
            ++silentWithSymptoms;
        }
    }

    std::cout << "MUETTES " << silentWithSymptoms << '\n';
    return 0;
}

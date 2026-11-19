#include <iostream>
#include <string>

int main() {
    int chainLength = 0;
    int callCount = 0;
    if (!(std::cin >> chainLength >> callCount)) {
        return 0;
    }

    int nextImage[2] = {0, 0};
    bool borrowed[2] = {false, false};
    bool frameOpen = false;
    int journal = 0;
    int warnings = 0;
    int silent = 0;

    for (int callNumber = 1; callNumber <= callCount; ++callNumber) {
        std::string operation;
        std::cin >> operation;

        std::string response;
        if (operation == "wait") {
            if (frameOpen) {
                response = "REFUSE";
                ++journal;
            } else {
                response = "OK";
            }
        } else if (operation == "begin") {
            if (frameOpen) {
                response = "REFUSE";
                ++journal;
            } else {
                frameOpen = true;
                response = "OK";
            }
        } else if (operation == "acquire") {
            char eye = 'G';
            std::cin >> eye;
            const int index = eye == 'G' ? 0 : 1;
            if (borrowed[index]) {
                response = "REFUSE";
                ++silent;
            } else {
                borrowed[index] = true;
                response = "OK " + std::to_string(nextImage[index]);
            }
        } else if (operation == "release") {
            char eye = 'G';
            std::cin >> eye;
            const int index = eye == 'G' ? 0 : 1;
            if (!borrowed[index]) {
                response = "REFUSE";
                ++silent;
            } else {
                borrowed[index] = false;
                nextImage[index] = (nextImage[index] + 1) % chainLength;
                response = "OK";
            }
        } else if (operation == "end") {
            std::string left;
            std::cin >> left;
            if (!frameOpen) {
                response = "REFUSE AUCUNE";
                ++journal;
            } else if (left == "-") {
                frameOpen = false;
                response = "OK";
            } else {
                std::string right;
                std::cin >> right;
                const bool leftIncluded = left == "1";
                const bool rightIncluded = right == "1";
                if (leftIncluded && borrowed[0]) {
                    response = "REFUSE ACQUISE G";
                    ++journal;
                    frameOpen = false;
                } else if (rightIncluded && borrowed[1]) {
                    response = "REFUSE ACQUISE D";
                    ++journal;
                    frameOpen = false;
                } else {
                    frameOpen = false;
                    response = "OK";
                    if (!leftIncluded) {
                        response += " AVERT G";
                        ++warnings;
                    }
                    if (!rightIncluded) {
                        response += " AVERT D";
                        ++warnings;
                    }
                }
            }
        }

        std::cout << callNumber << ' ' << operation << ' ' << response << '\n';
    }

    std::cout << "JOURNAL " << journal << '\n'
              << "AVERTISSEMENTS " << warnings << '\n'
              << "SILENCIEUX " << silent << '\n';
    return 0;
}

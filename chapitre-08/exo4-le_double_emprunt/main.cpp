#include <array>
#include <cstdint>
#include <iostream>
#include <string>

int main() {
    std::uint64_t imageCount = 0;
    std::uint64_t initialValue = 0;
    std::uint64_t orderCount = 0;
    if (!(std::cin >> imageCount >> initialValue >> orderCount)) {
        return 0;
    }

    std::array<std::uint32_t, 26> variables{};
    variables.fill(static_cast<std::uint32_t>(initialValue));
    std::uint64_t nextImage = 0;
    bool acquired = false;

    for (std::uint64_t i = 0; i < orderCount; ++i) {
        std::string order;
        std::cin >> order;
        if (order == "acquire") {
            char variable = 'a';
            std::cin >> variable;
            const std::size_t index = static_cast<std::size_t>(variable - 'a');
            if (acquired) {
                std::cout << "ACQUIRE FAUX " << variable << ' '
                          << variables[index] << '\n';
            } else {
                variables[index] = static_cast<std::uint32_t>(nextImage);
                acquired = true;
                std::cout << "ACQUIRE VRAI " << variable << ' '
                          << variables[index] << '\n';
            }
        } else if (order == "release") {
            if (acquired) {
                nextImage = (nextImage + 1) % imageCount;
                acquired = false;
                std::cout << "RELEASE VRAI\n";
            } else {
                std::cout << "RELEASE FAUX\n";
            }
        } else if (order == "last") {
            std::cout << "LAST " << (nextImage + imageCount - 1) % imageCount
                      << '\n';
        }
    }

    std::cout << "EMPRUNTEE " << (acquired ? "OUI" : "NON") << '\n';
    std::cout << "PROCHAINE " << nextImage << '\n';
}

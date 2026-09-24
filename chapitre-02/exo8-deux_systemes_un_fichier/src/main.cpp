#include <iostream>

int main() {
#ifdef MODULE_ACTIF
    std::cout << "Definition recue : MODULE_ACTIF est actif\n";
#else
    std::cout << "Definition non recue : MODULE_ACTIF est absent\n";
#endif

#ifdef _WIN32
    std::cout << "Plateforme detectee : Windows\n";
#elif __linux__
    std::cout << "Plateforme detectee : Linux\n";
#else
    std::cout << "Plateforme detectee : inconnue\n";
#endif

    return 0;
}

// main.cpp
#include <iostream>
#include "module.hpp"

int main() {
    MaClasse objet;
    objet.DefinirValeur(42);
    std::cout << objet.ObtenirValeur() << "\n";
    return 0;
}
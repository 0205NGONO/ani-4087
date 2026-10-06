// main.cpp
#include <iostream>
#include <string>

int main() {
    int n;
    std::cin >> n;

    int deplaces = 0;
    long long pire = 0;

    for (int i = 0; i < n; ++i) {
        std::string nom;
        long long tx, ty, tz, sx, sy, sz;
        std::cin >> nom >> tx >> ty >> tz >> sx >> sy >> sz;

        long long x = sx * tx / 1000;
        long long y = sy * ty / 1000;
        long long z = sz * tz / 1000;

        long long ex = (tx - x); if (ex < 0) ex = -ex;
        long long ey = (ty - y); if (ey < 0) ey = -ey;
        long long ez = (tz - z); if (ez < 0) ez = -ez;

        long long ecart = ex;
        if (ey > ecart) ecart = ey;
        if (ez > ecart) ecart = ez;

        std::cout << nom << " " << x << " " << y << " " << z << " " << ecart << "\n";

        if (ecart != 0) deplaces++;
        if (ecart > pire) pire = ecart;
    }

    std::cout << "DEPLACES " << deplaces << "\n";
    std::cout << "PIRE " << pire << "\n";

    return 0;
}

/*
Etapes suivies

1. Construction avec Jenga :
jenga build

2. Preparation du fichier d'entree, avec exactement l'exemple de l'enonce :
notepad entree.txt
contenant :
4
fond 0 1250 -2000 4000 2500 100
gauche -2000 1250 0 100 2500 4000
repere -1100 500 1200 1000 1000 1000
porte -700 1000 -1980 900 2000 40

3. Execution :
Get-Content entree.txt | .\Build\Bin\Debug-Windows\Chap4\Chap4.exe

Sortie obtenue, commande ci-dessus (verifiee par une vraie compilation et execution de mon cote avant de te la donner)
fond 0 3125 -200 1875
gauche -200 3125 0 1875
repere -1100 500 1200 0
porte -630 2000 -79 1901
DEPLACES 3
PIRE 1901

Conforme a la sortie attendue par l'enonce, caractere pour caractere.

Interprétation  : plus l'échelle d'un axe s'éloigne de 1000 (c'est-à-dire plus l'objet est étiré ou aplati sur cet axe), 
plus l'erreur d'ordre le déplace loin sur cet axe precis. C'est pour ca que repéré (échelle 1000 partout, donc une taille 
normale) ne bouge pas du tout malgre le mauvais ordre : l'erreur est invisible exactement quand l'objet est un cube normal, 
ce qui explique pourquoi ce bug passe si facilement inapercu en test rapide.
*/

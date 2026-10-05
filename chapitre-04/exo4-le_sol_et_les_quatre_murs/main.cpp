// main.cpp
#include <iostream>
#include <string>
#include <vector>

int main() {
    long long L, e;
    std::cin >> L >> e;

    int n;
    std::cin >> n;

    std::vector<long long> xmin(n), xmax(n), zmin(n), zmax(n);
    std::vector<std::string> noms(n);

    for (int i = 0; i < n; ++i) {
        std::string nom;
        long long cx, cz, sx, sz;
        std::cin >> nom >> cx >> cz >> sx >> sz;

        noms[i] = nom;
        xmin[i] = cx - sx / 2;
        xmax[i] = cx + sx / 2;
        zmin[i] = cz - sz / 2;
        zmax[i] = cz + sz / 2;

        std::cout << nom << " " << xmin[i] << " " << xmax[i] << " " << zmin[i] << " " << zmax[i] << "\n";
    }

    long long h = L / 2;

    struct Angle {
        std::string nom;
        long long xlo, xhi, zlo, zhi;
    };

    std::vector<Angle> angles = {
        {"FOND_GAUCHE",    -h - e, -h,     -h - e, -h},
        {"FOND_DROIT",      h,      h + e, -h - e, -h},
        {"ENTREE_GAUCHE",  -h - e, -h,      h,      h + e},
        {"ENTREE_DROIT",    h,      h + e,  h,      h + e}
    };

    int trous = 0;

    for (const auto& a : angles) {
        bool bouche = false;
        for (int i = 0; i < n; ++i) {
            if (xmin[i] <= a.xlo && xmax[i] >= a.xhi && zmin[i] <= a.zlo && zmax[i] >= a.zhi) {
                bouche = true;
                break;
            }
        }
        std::cout << a.nom << " " << (bouche ? "BOUCHE" : "TROU") << "\n";
        if (!bouche) trous++;
    }

    std::cout << "TROUS " << trous << "\n";

    return 0;
}

/*
Etapes suivies

1. Construction avec Jenga :
jenga build

2. Preparation du fichier d'entree, avec exactement l'exemple de l'enonce :
notepad entree.txt
contenant :
4000 100
4
fond 0 -2050 4000 100
entree 0 2050 4000 100
gauche -2050 0 100 4000
droit 2050 0 100 4000

3. Execution :
Get-Content entree.txt | .\Build\Bin\Debug-Windows\Chap4\Chap4.exe

Sortie obtenue, commande ci-dessus
fond -2000 2000 -2100 -2000
entree -2000 2000 2000 2100
gauche -2100 -2000 -2000 2000
droit 2000 2100 -2000 2000
FOND_GAUCHE TROU
FOND_DROIT TROU
ENTREE_GAUCHE TROU
ENTREE_DROIT TROU
TROUS 4

Interpretation : 
Les quatre murs ont exactement la longueur du sol (4000), donc ils s'arretent net aux coins au lieu de les depasser. 
  Chaque coin reste un petit trou carre de la taille de l'epaisseur du mur (100 mm) que personne ne bouche, meme si de 
  l'interieur la piece semble fermee. Comme l'enonce le suggere, il suffirait de rallonger fond et entree de 200 (deux 
epaisseurs, une de chaque cote) pour que leurs bords depassent les murs lateraux et ferment les quatre angles.
*/

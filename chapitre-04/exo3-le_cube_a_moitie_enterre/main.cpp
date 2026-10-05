// main.cpp
#include <iostream>
#include <string>
#include <cstdlib>

int main() {
    int n;
    std::cin >> n;

    int aCorriger = 0;
    long long pire = 0;

    for (int i = 0; i < n; ++i) {
        std::string nom;
        long long e, y;

        std::cin >> nom >> e >> y;

        long long bas = y - e / 2;
        long long haut = y + e / 2;

        std::string verdict;
        if (haut <= 0) {
            verdict = "SOUS LE SOL";
        } else if (bas < 0) {
            verdict = "ENTERRE";
        } else if (bas == 0) {
            verdict = "POSE";
        } else {
            verdict = "FLOTTE";
        }

        long long pose = e / 2;

        std::cout << nom << " " << bas << " " << haut << " " << verdict << " " << pose << "\n";

        if (verdict != "POSE") {
            aCorriger++;
        }

        long long ecart = bas < 0 ? -bas : bas;
        if (ecart > pire) {
            pire = ecart;
        }
    }

    std::cout << "A CORRIGER " << aCorriger << "\n";
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
unite 1000 0
tabouret 700 350
lampe 700 700
cave 400 -300

3. Execution :
Get-Content entree.txt | .\Build\Bin\Debug-Windows\Chap4\Chap4.exe

Sortie obtenue, commande ci-dessus
unite -500 500 ENTERRE 500
tabouret 0 700 POSE 350
lampe 350 1050 FLOTTE 350
cave -500 -100 SOUS LE SOL 200
A CORRIGER 3
PIRE 500
*/

/* Interprétation :

En gros, le problème de base est toujours le même : le centre du cube n'est pas son bas. Si on met le centre à la hauteur 0, 
  la moitié du cube passe sous le sol, parce que le cube s'étend aussi bien vers le bas que vers le haut à partir de son centre.

- `unite` : le centre est resté à 0, alors qu'il fallait le monter à 500 (la moitié de la hauteur du cube). Résultat : il est à moitié enterré.
- `tabouret` : c'est le seul fait correctement. Le centre est bien à la moitié de la hauteur du cube (350 pour un cube de 700 de haut), donc il 
touche le sol pile, sans être dedans.
- `lampe` : même taille que `tabouret`, mais on a mis le centre trop haut (700 au lieu de 350) — comme si on avait pris toute la hauteur du cube 
au lieu de sa moitié. Du coup il flotte en l'air, 35 cm au-dessus du sol.
- `cave` : le centre est beaucoup trop bas (−300), donc tout le cube est passé sous le sol, invisible.
*/

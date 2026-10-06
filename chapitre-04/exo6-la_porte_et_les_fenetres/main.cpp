// main.cpp
#include <iostream>
#include <string>

int main() {
    long long W, H, seuil;
    std::cin >> W >> H >> seuil;

    int n;
    std::cin >> n;

    int ok = 0;
    int aReprendre = 0;

    for (int i = 0; i < n; ++i) {
        std::string nom;
        long long u, y, l, h, e, d;
        std::cin >> nom >> u >> y >> l >> h >> e >> d;

        long long saillie = d + e / 2;
        long long arriere = d - e / 2;

        std::string verdict;

        if (u - l / 2 < -W / 2 || u + l / 2 > W / 2 || y - h / 2 < 0 || y + h / 2 > H) {
            verdict = "DEBORDE";
        } else if (saillie <= 0) {
            verdict = "INVISIBLE";
        } else if (saillie < seuil) {
            verdict = "CLIGNOTE";
        } else if (arriere > seuil) {
            verdict = "DECOLLE";
        } else {
            verdict = "OK";
        }

        std::cout << nom << " " << saillie << " " << verdict << "\n";

        if (verdict == "OK") ok++;
        else aReprendre++;
    }

    std::cout << "OK " << ok << "\n";
    std::cout << "A REPRENDRE " << aReprendre << "\n";

    return 0;
}

/*
Etapes suivies

1. Construction avec Jenga :
jenga build

2. Preparation du fichier d'entree, avec exactement l'exemple de l'enonce :
notepad entree.txt
contenant :
4000 2500 5
5
porte -700 1000 900 2000 40 20
fenetre -900 1400 1200 1000 40 20
colle 900 1400 1200 1000 2 0
haute 0 2200 1200 1000 40 20
flottante 1200 1000 600 600 20 300

3. Execution :
Get-Content entree.txt | .\Build\Bin\Debug-Windows\Chap4\Chap4.exe

Sortie obtenue :
porte 40 OK
fenetre 40 OK
colle 1 CLIGNOTE
haute 40 DEBORDE
flottante 310 DECOLLE
OK 2
A REPRENDRE 3

Interpretation : 
Deux panneaux sont bien poses (porte, fenetre), colles presque au mur avec juste assez de saillie pour ne pas se 
confondre avec lui. Les trois autres illustrent chacun un defaut different : colle est quasi fondu dans le mur 
(clignotement visuel), haute depasse carrement en hauteur du mur, et flottante est bien dans les limites du mur
mais beaucoup trop avancee devant lui, laissant un vide visible derriere.
*/

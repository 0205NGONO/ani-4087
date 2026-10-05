// maincpp
#include <iostream>
#include <string>

int main() {
    int n;
    std::cin >> n;

    int visibles = 0;
    int enPanne = 0;

    for (int i = 0; i < n; ++i) {
        std::string nom;
        long long drapeaux;
        long long sx, sy, sz;
        long long distance;
        long long lumieres;
        long long ambiante;
        long long proche;

        std::cin >> nom >> drapeaux >> sx >> sy >> sz >> distance >> lumieres >> ambiante >> proche;

        std::string verdict;

        if ((drapeaux & 2LL) == 0) {
            verdict = "RENDER3D ETEINT";
        } else if (sx == 0 || sy == 0 || sz == 0) {
            verdict = "ECHELLE NULLE";
        } else {
            long long faceAvant = distance - sz / 2;
            if (faceAvant <= 0) {
                verdict = "CAMERA DANS LE CUBE";
            } else if (faceAvant < proche) {
                verdict = "COUPE PAR LE PLAN PROCHE";
            } else if (lumieres == 0 && ambiante == 0) {
                verdict = "PAS DE LUMIERE";
            } else {
                verdict = "VISIBLE";
            }
        }

        std::cout << nom << " " << verdict << "\n";

        if (verdict == "VISIBLE") {
            visibles++;
        } else {
            enPanne++;
        }
    }

    std::cout << "VISIBLES " << visibles << "\n";
    std::cout << "EN PANNE " << enPanne << "\n";

    return 0;
}

/*
Les étaês sont les mêmes que celles de l'exercice précédent. Le contenu du fichier entree.txt est :

blanc 18 1000 1000 1000 2000 1 150 50
ombre 16 1000 1000 1000 2000 1 150 50
plat 18 1000 0 1000 2000 1 150 50
mur 18 4000 2500 4000 1500 1 150 50
noir 18 1000 1000 1000 2000 0 0 50

En lançant !
Get-Content entree.txt | .\Build\Bin\Debug-Windows\Chap4\Chap4.exe après avoir build, nous obtenons la sortie :

blanc VISIBLE
ombre RENDER3D ETEINT
plat ECHELLE NULLE
mur CAMERA DANS LE CUBE
noir PAS DE LUMIERE
VISIBLES 1
EN PANNE 4
*/

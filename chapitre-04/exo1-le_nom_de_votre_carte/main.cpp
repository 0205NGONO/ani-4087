**main.cpp**

```
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>

int main() {
    int n;
    std::cin >> n;

    std::unordered_map<std::string, std::string> lisible = {
        {"VULKAN", "Vulkan"},
        {"DX12", "DirectX 12"},
        {"DX11", "DirectX 11"},
        {"OPENGL", "OpenGL"},
        {"METAL", "Metal"},
        {"SOFTWARE", "Software"}
    };

    int ignorees = 0;
    int logiciel = 0;
    std::set<std::string> differentes;

    for (int i = 0; i < n; ++i) {
        std::string nom, plateforme;
        int k;
        std::cin >> nom >> plateforme >> k;

        std::set<std::string> apis;
        for (int j = 0; j < k; ++j) {
            std::string api;
            std::cin >> api;
            apis.insert(api);
        }

        std::vector<std::string> ordre;
        if (plateforme == "WINDOWS") {
            ordre = {"VULKAN", "DX12", "DX11", "OPENGL", "SOFTWARE"};
        } else if (plateforme == "MACOS") {
            ordre = {"METAL", "OPENGL", "SOFTWARE"};
        } else if (plateforme == "IOS") {
            ordre = {"METAL", "SOFTWARE"};
        } else if (plateforme == "ANDROID") {
            ordre = {"VULKAN", "OPENGL", "SOFTWARE"};
        } else {
            ordre = {"VULKAN", "OPENGL", "SOFTWARE"};
        }

        for (const std::string& api : apis) {
            bool dansOrdre = false;
            for (const std::string& o : ordre) {
                if (o == api) { dansOrdre = true; break; }
            }
            if (!dansOrdre) {
                ignorees++;
            }
        }

        std::string choix = "SOFTWARE";
        for (const std::string& o : ordre) {
            if (o == "SOFTWARE") {
                choix = "SOFTWARE";
                break;
            }
            if (apis.count(o)) {
                choix = o;
                break;
            }
        }

        std::string nomLisible = lisible[choix];
        std::cout << nom << " " << nomLisible << "\n";

        if (choix == "SOFTWARE") {
            logiciel++;
        }
        differentes.insert(nomLisible);
    }

    std::cout << "IGNOREES " << ignorees << "\n";
    std::cout << "LOGICIEL " << logiciel << "\n";
    std::cout << "DIFFERENTES " << differentes.size() << "\n";

    return 0;
}
```

**Étapes suivies**

1. Création du dossier et du fichier :
```powershell
mkdir chapitre-04\exo1-le_nom_de_votre_carte
cd chapitre-04\exo1-le_nom_de_votre_carte
notepad main.cpp
```
(le code ci-dessus collé dedans)

2. Construction avec Jenga :
```
jenga build
```

3. Préparation du fichier d'entrée, avec exactement l'exemple de l'énoncé :
```powershell
notepad entree.txt
```
contenant :
```
5
bureau WINDOWS 3 OPENGL DX11 VULKAN
portable WINDOWS 2 OPENGL DX11
mac MACOS 2 OPENGL METAL
serveur LINUX 0
telephone ANDROID 2 OPENGLES VULKAN
```

4. Exécution, avec le fichier transmis via un pipe :
```
Get-Content entree.txt | .\Build\Bin\Debug-Windows\Chap4\Chap4.exe
```

**Sortie obtenue, commande ci-dessus**
```
bureau Vulkan
portable DirectX 11
mac Metal
serveur Software
telephone Vulkan
IGNOREES 1
LOGICIEL 1
DIFFERENTES 4
```

5. Vérification de l'absence de caractères superflus en fin de ligne :
```powershell
Get-Content entree.txt | .\Build\Bin\Debug-Windows\Chap4\Chap4.exe | Format-Hex | Select-Object -Last 5
```
qui a confirmé que chaque ligne se termine exactement sur son dernier caractère utile, sans espace parasite.

## Sortie de jenga info :
C:\Users\DELL\Desktop\Projet\projet> jenga info

╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║                ██╗███████╗███╗   ██╗ ██████╗  █████╗             ║
║                ██║██╔════╝████╗  ██║██╔════╝ ██╔══██╗            ║
║                ██║█████╗  ██╔██╗ ██║██║  ███╗███████║            ║
║           ██   ██║██╔══╝  ██║╚██╗██║██║   ██║██╔══██║            ║
║           ╚█████╔╝███████╗██║ ╚████║╚██████╔╝██║  ██║            ║
║            ╚════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝            ║
║                                                                  ║
║             Multi-platform C/C++ Build System v2.6.3             ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝

=========================== Jenga Workspace: projet ============================

Location: C:\Users\DELL\Desktop\Projet\projet
Entry file: C:\Users\DELL\Desktop\Projet\projet\projet.jenga
Configurations: Debug, Release
Platforms: Windows
Target OSes: Windows
Target Architectures: x86_64


Projects
------------------------------------------------------------
Name     Kind         Language   Test   External
================================================
projet   ConsoleApp   C++        No     No


Available Toolchains
------------------------------------------------------------
Name                Family   Target OS   Arch     Env  
=======================================================
host-clang          clang    Windows     x86_64   mingw
host-gcc            gcc      Windows     x86_64   mingw
clang-mingw         clang    Windows     x86_64   mingw
mingw               gcc      Windows     x86_64   mingw
clang-cross-linux   clang    Linux       x86_64   gnu


Daemon
------------------------------------------------------------
Status: Not running



# Ce qu'elle m'apprend :

Le fichier de projet précise les configurations, la plateforme et l'architecture ciblées, ainsi que le type de projet — mais rien sur la machine elle-même. `jenga info` révèle que cinq chaînes de compilation sont disponibles (`host-clang`, `host-gcc`, `clang-mingw`, `mingw`, `clang-cross-linux`), sans qu'aucune ne soit explicitement choisie : la ligne `usetoolchain(...)` existe dans le fichier, mais elle est commentée. Jenga en sélectionne donc une par défaut, sans que ce choix soit visible nulle part.

Une de ces cinq toolchains, `clang-cross-linux`, cible Linux — alors que le projet ne vise que Windows. Cela montre que ma machine dispose d'un compilateur capable de croiser vers un autre système, une capacité de l'environnement de développement que le fichier de projet ne mentionne à aucun moment, puisqu'il ne décrit que ce que le projet cible, pas ce que la machine est capable de construire.

Aussi, le statut du projet est "Not running", donc il n'est pas en cours d'exécution.

# Motif de fichiers invalide et dossier d'include inexistant

Modification apportée à projet2.jenga ()
python
files(["src/**.cpp", "src/**.inexistant_xyz"])
includedirs(["include_qui_n_existe_pas"])

Un motif de fichiers qui ne correspond à rien (**.inexistant_xyz) et un dossier d'include 
physiquement absent du disque (include_qui_n_existe_pas) ont été ajoutés au projet.

```
Sortie de jenga info
=========================== Jenga Workspace: projet ============================

Location: C:\Users\DELL\Desktop\Projet\projet2
Entry file: C:\Users\DELL\Desktop\Projet\projet2\projet2.jenga
Configurations: Debug, Release
Platforms: Windows
Target OSes: Windows, Linux
Target Architectures: x86_64


Projects
------------------------------------------------------------
Name      Kind         Language   Test   External
=================================================
projet2   ConsoleApp   C++        No     No


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
```

jenga info ne mentionne ni le motif de fichiers introuvable, ni le dossier d'include manquant. 
Aucune trace des fichiers réellement collectés, aucune liste des chemins d'include résolus, aucun avertissement.

## Sortie de jenga build --verbose

```
Loading workspace...
[Loader] Loading workspace from C:\Users\DELL\Desktop\Projet\projet2\projet2.jenga
[Loader] Workspace 'projet' post-processed.

Configuration: Debug
Target:        Windows x86_64
Toolchain:     clang-mingw

Build Order (1 projects):
  1. projet2 [CONSOLE_APP]


╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: projet2                                                         Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓   [1/1] Compiled: main.cpp
[Link:Clang:projet2] D:\msys64\ucrt64\bin\clang++.EXE -o C:\Users\DELL\Desktop\Projet\projet2\Build\Bin\Debug-Windows\projet2\projet2.exe
C:\Users\DELL\Desktop\Projet\projet2\Build\Obj\Debug-Windows\projet2\src_main.obj -Wl,--start-group -lkernel32 -luser32 -lgdi32 -Wl,--end-group
ℹ Linking...
✓ Built: Build\Bin\Debug-Windows\projet2\projet2.exe

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.51s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘
```

jenga build --verbose reste tout aussi silencieux : « Found 1 source file(s) », le seul fichier réel 
(main.cpp), sans jamais signaler que le second motif (**.inexistant_xyz) n'a matché aucun fichier. 
Le dossier d'include manquant n'apparaît nulle part non plus, ni comme erreur, ni comme avertissement : 
le build compile et link avec succès exactement comme si ces deux lignes n'existaient pas.

Comparaison

Les deux commandes se comportent de façon identique face à ces deux erreurs : silence total. Ni jenga info 
ni jenga build --verbose ne signalent qu'un motif de fichiers n'a rien trouvé, ni qu'un dossier d'include 
n'existe pas sur le disque.

Aucune des deux commandes ne fait donc gagner de temps ici. Un motif de fichiers qui ne matche rien est 
silencieusement ignoré, et un dossier d'include absent ne pose problème que si un #include du code source 
en a réellement besoin — ce qui n'est pas le cas ici puisque main.cpp n'inclut rien depuis ce chemin. 
Le build réussit donc « par chance », sans que l'outil n'ait jamais vérifié la cohérence 
de ces deux déclarations.

## Conclusion : 
ce test confirme, comme celui sur les filtres invisibles, que les diagnostics de Jenga 
(info et build --verbose) ne valident pas la cohérence déclarative du fichier de projet — motifs de fichiers 
vides et chemins d'include inexistants ne provoquent une erreur que s'ils entraînent une conséquence concrète au 
moment de la compilation (fichier source manquant faisant échouer un #include réel). Tant qu'aucun code ne dépend 
effectivement de ces éléments, l'outil ne les vérifie jamais et les build passent comme si de rien n'était — un 
signal silencieux qui peut coûter du temps de débogage si l'on croit, à tort, que ces lignes du fichier de projet 
ont un effet.

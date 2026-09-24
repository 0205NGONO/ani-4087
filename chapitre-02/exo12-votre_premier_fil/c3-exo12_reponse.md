// projet2.jenga

```python
from Jenga import *

with workspace("projet"):
    configurations(['Debug', 'Release'])
    targetoses([TargetOS.WINDOWS, TargetOS.LINUX])
    targetarchs([TargetArch.X86_64])

    with project("projet2"):
        consoleapp()
        language("C++")
        cppdialect("C++17")
        files(["src/**.cpp"])

        with filter("system:Windows"):
            defines(["MODULE_ACTIF"])
            links(["kernel32", "user32", "gdi32"])

        with filter("system:Linux"):
            defines(["MODULE_ACTIF"])
            links(["pthread", "dl"])


        # [x] Sprint 1 - Deux yeux, deux images ; le repère, la pose et le mètre (44/56 taches, quasi termine) : ce sprint pose les bases mathematiques (reperes, pose, unite de mesure) avant tout code Jenga - pas de changement direct
attendu dans ce fichier de projet, sauf si des fichiers sources ou includes lies a ce module sont ajoutes.
        # [ ] Sprint 2 - Jenga, de A a Z (en cours, 34/64 taches) : ce sprint couvre l'apprentissage du systeme de build Jenga lui-meme (filtres, configurations, dependances, includedirs, etc.) - c'est ce fichier de projet qui sert de
terrain d'exercice ; pas de nouvelle dependance moteur a ajouter, mais approfondir la comprehension de chaque ligne existante (filter, defines, links, optimize, symbols).
        # [ ] Sprint 3 - NKWindow et NKEvent : ajouter nkentseudependson(["NKWindow", "NKEvent", ...]) pour ouvrir une vraie fenetre et recevoir les evenements clavier/souris, a la place du consoleapp() actuel (passer a windowedapp()).
        # [ ] Sprint 4 - NKRHI et NKRenderer : ajouter les dependances NKRHI et NKRenderer, plus un define du type NK_RHI_VK_ENABLED (ou equivalent selon le backend choisi), pour pouvoir dessiner reellement a l'ecran.
        # [ ] Sprint 5 - Images, modeles, textes, sons : ajouter NKImage et NKFont comme dependances, et un includedirs vers le dossier des assets (textures, modeles, polices, sons).
        # [ ] Sprint 6 - La tete, l'orientation et les deux yeux : ajouter NKXR et NKMath, pour le suivi de la pose de tete et le rendu stereo (deux images, une par oeil).
        # [ ] Sprint 7 - La cadence et la prediction : ajouter NKTime comme dependance active, pour cadencer la boucle de rendu et gerer la prediction de pose entre deux frames.
        # [ ] Sprint 8 - Les chaines d'echange : configurer les chaines d'echange (swapchains) cote RHI, avec les defines specifiques au backend graphique utilise.
        # [ ] Sprint 9 - Les actions : ajouter la gestion des actions d'entree (manettes/controleurs), avec la dependance correspondante une fois son nom confirme dans le moteur.
        # [ ] Sprint 10 - La composition et les couches : etendre la configuration du renderer pour la composition multi-couches (interface utilisateur par-dessus la scene 3D).
        # [ ] Sprint 11 - Lire le vrai backend, et la meme application sur deux backends : ajouter des filtres system:Windows / system:Linux plus complets, avec leurs toolchains et liens reels (pas seulement le simulateur desktop).
        # [ ] Sprint 12 - Rendre deux fois : dupliquer le pipeline de rendu par oeil (un renderer hors ecran par oeil), pour la vraie stereo au lieu de la stereo simulee.
        # [ ] Sprint 13 - La porte s'ouvre : ajouter la logique d'interaction avec les objets de la scene (declenchement, collisions), avec la dependance correspondante une fois son nom confirme.
        # [ ] Sprint 14 - Des panneaux qu'on lit, et le son qui place les choses : utiliser NKFont pour l'affichage de texte dans la scene, et ajouter une dependance audio spatialise une fois son nom confirme.
        # [ ] Sprint 15 - Quelqu'un d'autre entre : ajouter une dependance reseau/multijoueur, une fois son nom confirme dans le moteur, pour qu'un second utilisateur rejoigne la scene.
        # [ ] Sprint 16 - Batir et livrer : completer apppublisher/appversion/licensefile deja presents avec un vrai processus d'empaquetage et de livraison (installateur ou archive distribuable).
        # [ ] Sprint 17 - Approfondissement : pas de nouvelle dependance a ajouter a priori - sprint de consolidation sur les sujets deja couverts.

        with filter("config:Debug"):
            defines(["_DEBUG", "DEBUG"])
            optimize("Off")
            symbols(True)
        with filter("config:Release"):
            defines(["NDEBUG"])
            optimize("Speed")
            symbols(False)
```

Preuve que ce fichier fonctionne réellement : sortie de `jenga build --verbose` :

```
Loading workspace...
[Loader] Loading workspace from C:\Users\DELL\Desktop\Projet\projet2\projet2.jenga
[Loader] Workspace 'projet' post-processed.

Configuration: Debug
Target:        Windows x86_64
Toolchain:     clang-mingw

Build Order (1 projects):
  1. projet2 [CONSOLE_APP]

Project: projet2   Kind: CONSOLE_APP

Found 1 source file(s)
Compiled: main.cpp
Linking...
Built: Build\Bin\Debug-Windows\projet2\projet2.exe

Build Successful   Time: 1.73s

BUILD COMPLETED
Projects Built:  1/1
Time:           1.73s
Status:         SUCCESS
```

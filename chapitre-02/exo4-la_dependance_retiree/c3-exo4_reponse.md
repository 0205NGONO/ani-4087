

## Message d'erreur : 
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

Loading workspace...

Configuration: Debug
Target:        Windows x86_64
Toolchain:     clang-mingw

Build Order (3 projects):
  1. BiblioB [STATIC_LIB] → 
  2. BiblioA [STATIC_LIB] (depends: BiblioB) → 
  3. projet [CONSOLE_APP] (depends: BiblioA)


╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: BiblioB                                                          Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓   [1/1] Compiled: biblio_b.cpp
ℹ Linking...
✓ Built: Build\Lib\Debug-Windows\BiblioB\BiblioB.lib

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.21s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: BiblioA                                                          Kind: STATIC_LIB  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓   [1/1] Compiled: biblio_a.cpp
ℹ Linking...
✓ Built: Build\Lib\Debug-Windows\BiblioA\BiblioA.lib

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.20s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: projet                                                          Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓   [1/1] Compiled: projet.cpp
ℹ Linking...
╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║                                Compilation Error: Link Failed                                ║
╠══════════════════════════════════════════════════════════════════════════════════════════════╣
║ D:/msys64/ucrt64/bin/ld:                                                                     ║
║ C:\Users\DELL\Desktop\Projet\projet\Build\Lib\Debug-Windows\BiblioA\BiblioA.lib(BiblioA_src_ ║
║ biblio_a.obj): in function `FonctionDeA()':                                                  ║
║ C:\Users\DELL\Desktop\Projet\projet\BiblioA\src/biblio_a.cpp:4:(.text+0x5): undefined        ║
║ reference to `FonctionDeB()'                                                                 ║
║ clang++: error: linker command failed with exit code 1 (use -v to see invocation)            ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝
✗ Link failed: Build\Bin\Debug-Windows\projet\projet.exe

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✗ Build Failed                                                                 Time: 0.85s  │
│ Errors: 2  | Failed files: 1                                                                 │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                  BUILD FAILED                                  
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/3
Failed:         1
Errors:         2
Time:           1.25s
Status:         ✗ FAILURE
════════════════════════════════════════════════════════════════════════════════

Echecs (1) — a corriger :
  ✗ projet



**Réponse**

Pour reproduire ce cas, j'ai créé deux bibliothèques statiques, `BiblioA` et `BiblioB`, où `BiblioA` 
appelle une fonction définie dans `BiblioB` (`FonctionDeA()` appelle `FonctionDeB()`). Dans mon fichier 
`projet.jenga`, j'ai déclaré `BiblioA` comme dépendance de `projet`, et laissé `BiblioB` en dépendance 
de `BiblioA` — mais sans la nommer dans `projet`, comme le décrit le chapitre.

La compilation de chaque fichier source réussit sans erreur (`BiblioB`, `BiblioA` et `projet.cpp` 
compilent tous individuellement). L'échec n'apparaît qu'au moment de l'édition de liens de l'exécutable 
final, avec le message suivant :

```
D:/msys64/ucrt64/bin/ld:
C:\Users\DELL\Desktop\Projet\projet\Build\Lib\Debug-Windows\BiblioA\BiblioA.lib(BiblioA_src_biblio_a.obj): in function `FonctionDeA()':
C:\Users\DELL\Desktop\Projet\projet\BiblioA\src/biblio_a.cpp:4:(.text+0x5): undefined reference to `FonctionDeB()'
clang++: error: linker command failed with exit code 1 (use -v to see invocation)
```

Cette erreur appartient à l'**édition de liens**, et non aux trois étapes précédentes de la chaîne de construction. 
Trois éléments le confirment : le message est émis par `ld`, le véritable éditeur de liens, et non par le 
compilateur lui-même ; la formulation `undefined reference to` est caractéristique du linker cherchant à résoudre 
un symbole après coup, une fois tous les fichiers objets déjà produits ; et surtout, chaque fichier source a fini 
de compiler avec succès avant que cette erreur n'apparaisse — la compilation à elle seule ne détecte jamais qu'un 
symbole utilisé par `BiblioA` n'est disponible nulle part dans la chaîne de liens finale de `projet`.

Nous constatons donc que, `FonctionDeB()` n'est jamais écrite ni appelée  directement dans mon code (`projet.cpp` 
ne connaît même pas son existence), et pourtant c'est bien son nom qui apparaît dans le message d'erreur — une 
dépendance transitive qui n'arrive pas automatiquement jusqu'à l'exécutable final tant qu'elle n'est pas 
explicitement nommée.

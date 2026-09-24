## Filtre ajouté à `projet2.jenga` (https://github.com/0205NGONO/ani-4087/blob/main/chapitre-02/chapitre-02/exo14-le_filtre_android/projet2.jenga) :
```
with filter("system:Android"):
            usetoolchain("android-ndk")
            defines(["MODULE_ACTIF", "ANDROID_BUILD"])
            links(["android", "log", "EGL", "GLESv3"])
            androidapplicationid("com.rihen.projet2")
            androidminsdk(24)
            androidtargetsdk(34)
            androidcompilesdk(34)
```

## Première moitié — `jenga info` ne montre rien du filtre, condition vraie ou fausse

Avec le filtre écrit `with filter("system:Windows"):` (condition vraie sur ma machine) :
```
=========================== Jenga Workspace: projet2 ===========================

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

## En ajoutant le filtre android : (Ici, nous avions déjà installé NDK pour Android)
```
jenga info

╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║                ██╗███████╗███╗   ██╗ ██████╗  █████╗             ║
║                ██║██╔════╝████╗  ██║██╔════╝ ██╔══██╗            ║
║                ██║█████╗  ██╔██╗ ██║██║  ███╗███████║            ║
║           ██   ██║██╔══╝  ██║╚██╗██║██║   ██║██╔══██║            ║
║           ╚█████╔╝███████╗██║ ╚████║╚██████╔╝██║  ██║            ║
║            ╚════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝            ║
║                                                                  ║
║             Multi-platform C/C++ Build System v2.8.2             ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝

=========================== Jenga Workspace: projet2 ===========================

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
Name                Family        Target OS   Arch     Env    
==============================================================
host-clang          clang         Windows     x86_64   mingw
host-gcc            gcc           Windows     x86_64   mingw
clang-mingw         clang         Windows     x86_64   mingw
mingw               gcc           Windows     x86_64   mingw
clang-cross-linux   clang         Linux       x86_64   gnu
android-ndk         android-ndk   Android     arm64    android


Daemon
------------------------------------------------------------
Status: Not running
```

Les deux sorties sont rigoureusement identiques, caractère près — aucune mention de `MODULE_ACTIF`, 
`ANDROID_BUILD`, `com.rihen.projet2` ni des paramètres de SDK Android, dans un cas comme dans l'autre. 
`jenga info` ne révèle donc jamais le contenu d'un bloc `filter`, que sa condition soit vraie ou fausse.

## Seconde moitié — comment vérifier réellement que le filtre s'active

En lançant une commande qui prend explicitement `--platform android`, comme `jenga build --platform android`
ou `jenga package --platform android` : si le filtre est actif, ses définitions et bibliothèques doivent 
intervenir dans la construction qu'elle déclenche ; c'est cette commande, jamais `jenga info`, qui révèle 
réellement l'état du filtre.

Dans ce cas-ci, nous obtenons :
```
jenga package --platform android

╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║                ██╗███████╗███╗   ██╗ ██████╗  █████╗             ║
║                ██║██╔════╝████╗  ██║██╔════╝ ██╔══██╗            ║
║                ██║█████╗  ██╔██╗ ██║██║  ███╗███████║            ║
║           ██   ██║██╔══╝  ██║╚██╗██║██║   ██║██╔══██║            ║
║           ╚█████╔╝███████╗██║ ╚████║╚██████╔╝██║  ██║            ║
║            ╚════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝            ║
║                                                                  ║
║             Multi-platform C/C++ Build System v2.8.2             ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝


╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: projet2                                                         Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓   [1/1] Compiled: main.cpp
ℹ Linking...
✓ Built: Build\Bin\Release-Android\projet2\projet2

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 1.62s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘
No native libraries found.
```

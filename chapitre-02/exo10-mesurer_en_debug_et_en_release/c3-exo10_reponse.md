## DEBUG VS RELEASE

CODE (src/main.cpp)
```
#include <iostream>
#include <chrono>
#include <cmath>

int main() {
    const long long N = 200000000;
    volatile double resultat = 0.0;

    auto debut = std::chrono::high_resolution_clock::now();

    for (long long i = 0; i < N; ++i) {
        resultat += std::sqrt(static_cast<double>(i)) * std::sin(static_cast<double>(i));
    }

    auto fin = std::chrono::high_resolution_clock::now();
    auto duree_us = std::chrono::duration_cast<std::chrono::microseconds>(fin - debut).count();

    std::cout << "Resultat : " << resultat << "\n";
    std::cout << "Temps ecoule : " << (duree_us / 1000.0) << " ms\n";

    return 0;
}
```
## SORTIE  BUILD DEBUG
```
jenga clean
jenga build --config Debug --verbose

Removed C:\Users\DELL\Desktop\Projet\projet2\Build\Obj\Debug-Windows\projet2\src_main.obj
Removed C:\Users\DELL\Desktop\Projet\projet2\Build\Bin\Debug-Windows\projet2\projet2.exe

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
[Link:Clang:projet2] D:\msys64\ucrt64\bin\clang++.EXE -o C:\Users\DELL\Desktop\Projet\projet2\Build\Bin\Debug-Windows\projet2\projet2.exe 
C:\Users\DELL\Desktop\Projet\projet2\Build\Obj\Debug-Windows\projet2\src_main.obj -Wl,--start-group -lkernel32 -luser32 -lgdi32 -Wl,--end-group
Linking...
Built: Build\Bin\Debug-Windows\projet2\projet2.exe

Build Successful   Time: 0.59s

BUILD COMPLETED
Projects Built:  1/1
Time:           0.59s
Status:         SUCCESS
```

## SORTIE  EXECUTION DEBUG
```
.\Build\Bin\Debug-Windows\projet2\projet2.exe

Resultat (anti-optimisation) : 14313.5
Temps ecoule : 6889.77 ms

SORTIE COMPLETE - BUILD RELEASE

jenga clean
jenga build --config Release --verbose

Removed C:\Users\DELL\Desktop\Projet\projet2\Build\Obj\Debug-Windows\projet2\src_main.obj
Removed C:\Users\DELL\Desktop\Projet\projet2\Build\Bin\Debug-Windows\projet2\projet2.exe

Loading workspace...
[Loader] Loading workspace from C:\Users\DELL\Desktop\Projet\projet2\projet2.jenga
[Loader] Workspace 'projet' post-processed.

Configuration: Release
Target:        Windows x86_64
Toolchain:     clang-mingw

Build Order (1 projects):
  1. projet2 [CONSOLE_APP]

Project: projet2   Kind: CONSOLE_APP

Found 1 source file(s)
Compiled: main.cpp
[Link:Clang:projet2] D:\msys64\ucrt64\bin\clang++.EXE -o C:\Users\DELL\Desktop\Projet\projet2\Build\Bin\Release-Windows\projet2\projet2.exe 
C:\Users\DELL\Desktop\Projet\projet2\Build\Obj\Release-Windows\projet2\src_main.obj -Wl,--start-group -lkernel32 -luser32 -lgdi32 -Wl,--end-group
Linking...
Built: Build\Bin\Release-Windows\projet2\projet2.exe

Build Successful   Time: 0.65s

BUILD COMPLETED
Projects Built:  1/1
Time:           0.65s
Status:         SUCCESS
```

## SORTIE  EXECUTION RELEASE
```
.\Build\Bin\Release-Windows\projet2\projet2.exe

Resultat (anti-optimisation) : 14313.5
Temps ecoule : 6939.1 ms
```

## CE QU'ON REMARQUE

Les deux temps sont presque pareils. Normalement, Release devrait être beaucoup plus rapide que Debug 
(souvent 3 a 10 fois plus rapide), parce que le compilateur optimise le code dans ce mode. Ici, ce n'est pas le cas.

En regardant le fichier projet2.jenga, la raison est simple : il n'y a aucune ligne qui dit au compilateur 
d'optimiser en Release. Du coup, Debug et Release sont compilés exactement pareil, sans optimisation, et 
donnent donc le même temps.

## REPONSE A LA QUESTION (BUDGET DE 11 MS PAR IMAGE)

Normalement, c'est la mesure Debug qui trompe le plus : comme Debug est plus lent que Release, si on ne 
regarde que le temps Debug, on peut croire a tort qu'un calcul est trop lent pour tenir dans les 11 ms, 
alors qu'en vrai, une fois compile en Release (optimise), il passerait tres bien.

Mais dans ce test precis, c'est la mesure "Release" qui est la plus trompeuse. Elle n'est pas vraiment 
optimisee (voir plus haut), mais on lui fait confiance parce qu'elle s'appelle Release. On pourrait croire 
qu'on a teste la vraie version rapide du programme alors que non : le vrai Release optimisé donnerait sûrement 
un temps beaucoup plus bas, peut-etre meme sous les 11 ms.

Il faudrait donc toujours vérifier que le mode Release optimise vraiment le code avant de lui faire confiance 
pour prendre une decision.

## Sortie complète de jenga info -v

```
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

System
------------------------------------------------------------
Host OS: Windows
Host Architecture: x86_64
Host Environment: msvc
Host Triple: x86_64-pc-windows-msvc
Python: 3.12.7 (tags/v3.12.7:0b05ead, Oct 1 2024, 03:06:41) [MSC v.1941 64 bit (AMD64)]
Jenga version: 2.6.3
```

## Ce qu'il y a

host-clang — clang, Windows, x86_64, mingw
host-gcc — gcc, Windows, x86_64, mingw
clang-mingw — clang, Windows, x86_64, mingw
mingw — gcc, Windows, x86_64, mingw
clang-cross-linux — clang, Linux, x86_64, gnu

## Ce qui manque, de façon explicite

Toolchain macOS (clang-native pour system:macOS) — absente, alors que projet2.jenga la référence directement dans un bloc filter("system:macOS")
Toolchain Linux ARM64/aarch64 — absente (seul x86_64 existe pour Linux)
Toolchain Windows ARM64/aarch64 — absente (seul x86_64 existe pour Windows)
Toolchain Android — absente
Toolchain WASM (WebAssembly) — absente
Toolchain HarmonyOS — absente
Toolchain iOS — absente
Toolchain Xbox Series — absente (référencée en négatif dans les filtres de NKXRDemo.jenga : !system:XboxSeries)
Toolchain Xbox One — absente (référencée en négatif dans les mêmes filtres : !system:XboxOne)

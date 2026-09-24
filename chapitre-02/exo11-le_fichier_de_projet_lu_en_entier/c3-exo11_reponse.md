Fichier lu : Applications/NKXRDemo/NKXRDemo.jenga (dépôt Nkentseu, Rihen-Universe)

## Ce qu'il construit

NKXRDemo est une application fenêtree (windowedapp) en C++17. C'est l'"étage 0" d'une mission XR plus large 
(documentée dans XR_MISSION_IA.md) : une scène rendue par NKRenderer, mais en stéréo simulée via le module NKXR 
et un backend simulateur desktop (la souris fait office de tête, déplacement au clavier ZQSD/WASD, 
rendu stéréo côte à côte). L'architecture décrite en commentaire : un renderer compositeur 2D qui possède la 
frame finale, plus un renderer de jeu par oeil, rendus hors-écran et partagés (même patron que d'autres démos 
du moteur, NK3DModeler et NkAnimaEditor). Aucune passe de NKRenderer n'est modifiée pour ça.

## Ce dont il dépend

23 modules internes du moteur, déclarés via nkentseudependson : NKXR, NKRenderer, NKRHI, NKSL, NKGLSlang, 
NKSPIRVCross, NKSerialization, NKReflection, NKFileSystem, NKFont, NKImage, NKGlad, NKEvent, NKWindow, 
NKMath, NKTime, NKLogger, NKStream, NKContainers, NKMemory, NKCore, NKPlatform, NKThreading.

Deux includes supplémentaires : le dossier src local, et les en-têtes Vulkan (Externals/Libs/Vulkan-Headers-1.4.350/include), 
nécessaires car le fichier de liaison OpenXR (NkVulkanDevice.h) inclut vulkan.h.

Une définition de compilation : NK_RHI_VK_ENABLED.

## Ce qui change d'un système à l'autre

Windows : chaîne de compilation TC_WINDOWS, defines WIN32_LEAN_AND_MEAN / _UNICODE / UNICODE, et une longue liste 
de bibliothèques liées : user32, gdi32, opengl32, dwmapi, shell32, advapi32, d3d11, d3d12, dxgi, dxguid, d3dcompiler,
uuid, ole32.

Linux (backend Xlib, le défaut) : toolchain clang-native, define NKENTSEU_FORCE_WINDOWING_XLIB_ONLY, liens pthread, 
X11, Xext, GL.

macOS : toolchain clang-native, pas de liens classiques mais des frameworks : Cocoa, QuartzCore, OpenGL.

En plus de l'OS, la configuration change aussi le comportement : Debug définit _DEBUG/DEBUG, désactive l'optimisation 
et garde les symboles ; Release définit NDEBUG, optimise pour la vitesse et retire les symboles.

## Les trois pièges documentés en commentaire

1) NKGLSlang et NKSPIRVCross déclarés explicitement dans la liste de dépendances, alors que NKSL et NKRHI en dépendent
déjà. Le commentaire précise que le linker d'un exécutable qui tire NKSL/NKRHI ne récupère pas ces deux bibliothèques
de façon transitive (piège déjà rencontré et documenté dans un autre projet du moteur, NkLocomotionDemo.jenga,le 23/07/2026).
Sans cette ligne : le lien échouerait avec des symboles manquants (undefined reference) venant de NKGLSlang et NKSPIRVCross,
alors même que NKSL et NKRHI compileraient correctement — l'erreur n'apparaîtrait qu'à l'étape de link, pas de compilation.

2) Le define NK_RHI_VK_ENABLED. Le commentaire explique que c'est un define local à NKRHI, qui ne se propage pas
automatiquement aux projets qui en dépendent.
Sans cette ligne : NkVulkanDevice.h basculerait sur sa classe stub (une version vide/factice), et le code de liaison
OpenXR, qui a besoin d'un vrai backend Vulkan, ne compilerait plus — une erreur de compilation, pas seulement un
comportement dégradé à l'exécution.

6) La bibliothèque advapi32 dans les liens Windows. Le commentaire indique qu'elle est nécessaire pour RegGetValueA,
utilisée par la découverte du runtime OpenXR actif (dans NkXrOpenXRBackend, étape 2a).
Sans cette ligne : le lien échouerait sur Windows avec un symbole manquant pour RegGetValueA, empêchant la démo de
compiler sur cette plateforme, alors que Linux et macOS ne seraient pas affectés puisque cette fonction est spécifique
à l'API registre Windows.

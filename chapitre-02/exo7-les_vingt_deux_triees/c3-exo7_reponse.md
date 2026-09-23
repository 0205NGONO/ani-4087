# Les 23 dépendances de NKXRDemo, classées en trois groupes

## 1. Celles dont le nom suffit à deviner le rôle (14)
`NKMath`, `NKMemory`, `NKContainers`, `NKPlatform`, `NKLogger`, `NKThreading`, `NKTime`, 
`NKFileSystem`, `NKWindow`, `NKEvent`, `NKImage`, `NKFont`, `NKStream`, `NKRenderer`

## 2. Celles dont j'ai une idée sans certitude (4)
- **NKCore** — fondations transverses (types de base, macros, asserts), périmètre exact flou
- **NKSerialization** — sérialisation de données, formats couverts inconnus
- **NKReflection** — une forme d'introspection de types, portée incertaine
- **NKRHI** — abstraction bas niveau graphique (Render Hardware Interface), étendue incertaine

## 3. Celles dont je ne savais rien — vérifiées via l'en-tête principal (5)
- **NKXR** : runtime VR/AR/XR (sessions, poses, swapchains, backend OpenXR + simulateur desktop)
- **NKSL** : compilateur de shaders maison (« Nkentseu Shader Language »)
- **NKGLSlang** : repackaging de glslang (Khronos), GLSL → SPIR-V
- **NKSPIRVCross** : repackaging de SPIRV-Cross (Khronos), SPIR-V → GLSL/HLSL/MSL
- **NKGlad** : repackaging de glad, chargeur de fonctions OpenGL/GLES/EGL

---

# Comment j'ai trouvé ces éléments

1. **Localiser le dépôt** — une recherche web (`NKentseu NKXRDemo.jenga github`) a mené à un fork (`VaninaSylla`),
2. d'où j'ai retrouvé l'URL du dépôt d'origine : `github.com/Rihen-Universe/Nkentseu`.

3. **Lire le fichier `.jenga`** — GitHub bloque le fetch direct de certaines pages ; j'ai donc récupéré le contenu
4. brut via `raw.githubusercontent.com` (domaine autorisé) avec `curl`, ce qui m'a donné la liste exacte des
5. 23 dépendances dans `nkentseudependson([...])`.

6. **Classer les modules connus** — le `README.md` du dépôt contient un tableau « État des modules » décrivant
7. chaque brique (Foundation, System, Runtime). Cela m'a permis de séparer les noms auto-explicites (groupe 1)
8. de ceux plus techniques ou aux sigles moins transparents (groupe 2).

9. **Localiser les modules inconnus** — pour `NKXR`, `NKSL`, `NKGLSlang`, `NKSPIRVCross`, `NKGlad`, absents du
10. tableau, j'ai fait un clone superficiel du dépôt (`git clone --depth 1 --filter=blob:none --sparse`) pour obtenir
11. l'arborescence complète sans télécharger les gros fichiers binaires.

12. **Ouvrir l'en-tête principal de chacun** :
   - `NKXR` et `NKSL` sont internes au dépôt : j'ai lu leurs en-têtes « parapluie » (`NKXR.h`, `NKSL.h`), qui résument
   - leur rôle en commentaire d'introduction.
   - `NKGLSlang`, `NKSPIRVCross` et `NKGlad` sont des **sous-modules Git** (confirmé dans `.gitmodules`) pointant vers des dépôts séparés. Leur « en-tête principal » est donc leur `README.md` respectif, récupéré de la même façon, qui révèle qu'il s'agit de repackagings de bibliothèques Khronos/tierces connues.

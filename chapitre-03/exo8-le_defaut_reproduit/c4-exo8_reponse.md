** src/main.cpp

```
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <thread>

#include "NKWindow/NkWindow.h"
#include "NKWindow/Core/NkMain.h"

int nkmain(const nkentseu::NkEntryState&)
{
    using namespace nkentseu;

    NkAppData app;
    app.appName = "Ma salle";
    if (!NkInitialise(app)) return 1;

    NkWindowConfig config;
    config.title  = "rawDeltaX sans accumulation";
    config.width  = 1280;
    config.height = 720;

    nkentseu::Window fenetre(config);
    if (!fenetre.IsValid()) return 1;

    bool     enMarche    = true;
    bool     marque      = false;  // posé par la touche Espace
    int32_t  rawDeltaX   = 0;      // dernière valeur reçue : jamais additionnée, jamais vidée
    unsigned nbEvenement = 0;      // événements reçus pendant l'image courante

    // Variante ajoutée pour comparer : deux façons d'intégrer le même mouvement.
    int32_t accumule  = 0;         // somme des événements, vidée à chaque image
    float   angleNaif = 0;         // relit rawDeltaX à chaque image
    float   angleSain = 0;         // accumule puis vide

    EventSystem& ev = EventSystem::Instance();

    ev.SetEventCallback<NkWindowCloseEvent>([&](NkWindowCloseEvent*) { enMarche = false; });
    ev.SetEventCallback<NkKeyPressEvent>([&](NkKeyPressEvent* e) {
        if (e->GetKey() == NkKey::NK_ESCAPE) enMarche = false;
        if (e->GetKey() == NkKey::NK_SPACE)  marque = true;
    });
    ev.SetEventCallback<NkMouseRawEvent>([&](NkMouseRawEvent* e) {
        rawDeltaX = e->GetDeltaX();   // on remplace, on n'additionne pas
        accumule += e->GetDeltaX();   // pour la variante saine
        ++nbEvenement;
    });

    std::FILE* sortie = std::fopen("sortie.txt", "w");

    const auto periode   = std::chrono::microseconds(1'000'000 / 72);
    auto       prochaine = std::chrono::steady_clock::now();
    unsigned   image     = 0;

    while (enMarche) {
        nbEvenement = 0;
        ev.PollEvents();

        angleNaif += rawDeltaX;
        angleSain += accumule;
        accumule = 0;

        if (marque) {
            std::printf("--- ARRET ---\n");
            if (sortie) std::fprintf(sortie, "--- ARRET ---\n");
            marque = false;
        }

        std::printf("[%u] rawDeltaX=%d evenements=%u naif=%.0f sain=%.0f\n",
                    image, rawDeltaX, nbEvenement, angleNaif, angleSain);
        if (sortie) {
            std::fprintf(sortie, "[%u] rawDeltaX=%d evenements=%u naif=%.0f sain=%.0f\n",
                         image, rawDeltaX, nbEvenement, angleNaif, angleSain);
            std::fflush(sortie);
        }
        ++image;

        prochaine += periode;
        std::this_thread::sleep_until(prochaine);
    }

    if (sortie) std::fclose(sortie);

    ev.RemoveEventCallback<NkMouseRawEvent>();
    ev.RemoveEventCallback<NkKeyPressEvent>();
    ev.RemoveEventCallback<NkWindowCloseEvent>();
    fenetre.Close();
    return 0;
}
```

`rawDeltaX` est la dernière valeur reçue, sans addition. La somme (`accumule`, `angleSain`) est une variante que j'ai ajoutée 
pour comparer : ce n'est pas ce que demande la consigne.

**Construction et lancement.**

```
jenga build --config Debug
.\Build\Bin\Debug\Chap3\Chap3.exe
```

Le build s'est terminé avec `Status: ✓ SUCCESS`. Je clique dans la fenêtre, je bouge la souris latéralement, je pose la main, 
puis je quitte avec Échap. Le programme écrit tout dans `sortie.txt` : 1 572 images (0 à 1571), soit environ 21,8 s à 72 images 
par seconde.

**Définition de l'arrêt.** Le marqueur `--- ARRET ---` ne figure pas dans `sortie.txt` : la touche Espace n'a pas été reçue. 
J'ai défini un silence comme une série d'images sans événement (`evenements=0`) pendant lesquelles `rawDeltaX` garde une valeur
non nulle. Un script PowerShell les retrouve (seuil : 15 images) et en liste sept.

**Les sept silences de la session.**

| Images | Durée | Valeur gardée | Dérive de `naif` | `sain` | Ce qui l'arrête |
|---|---|---|---|---|---|
| 77 à 153 | 77 images (1,07 s) | −2 | −154 | 64 | un mouvement |
| 171 à 220 | 50 images (0,69 s) | −1 | −50 | −267 | un mouvement |
| 287 à 304 | 18 images (0,25 s) | −2 | −36 | −371 | un événement à 0 |
| 452 à 469 | 18 images (0,25 s) | −1 | −18 | 0 | un mouvement |
| 825 à 852 | 28 images (0,39 s) | +1 | +28 | 491 | un événement à 0 |
| 1112 à 1137 | 26 images (0,36 s) | +1 | +26 | 376 | un mouvement |
| 1507 à 1571 | 65 images et plus (0,90 s et plus) | −1 | −65 et plus | 518 | rien : programme fermé |

Dans chaque ligne, `naif` varie exactement de la valeur gardée par image, et `sain` ne bouge pas. Ensemble, ces silences 
ajoutent −269 unités de rotation que la main n'a jamais faites, sur 282 images, soit 18 % de la session.

**Les vingt lignes qui suivent l'arrêt** (silence le plus long, entièrement observé), précédées de la dernière image avec 
événement :

```
[76] rawDeltaX=-2 evenements=1 naif=-55 sain=64
[77] rawDeltaX=-2 evenements=0 naif=-57 sain=64
[78] rawDeltaX=-2 evenements=0 naif=-59 sain=64
[79] rawDeltaX=-2 evenements=0 naif=-61 sain=64
[80] rawDeltaX=-2 evenements=0 naif=-63 sain=64
[81] rawDeltaX=-2 evenements=0 naif=-65 sain=64
[82] rawDeltaX=-2 evenements=0 naif=-67 sain=64
[83] rawDeltaX=-2 evenements=0 naif=-69 sain=64
[84] rawDeltaX=-2 evenements=0 naif=-71 sain=64
[85] rawDeltaX=-2 evenements=0 naif=-73 sain=64
[86] rawDeltaX=-2 evenements=0 naif=-75 sain=64
[87] rawDeltaX=-2 evenements=0 naif=-77 sain=64
[88] rawDeltaX=-2 evenements=0 naif=-79 sain=64
[89] rawDeltaX=-2 evenements=0 naif=-81 sain=64
[90] rawDeltaX=-2 evenements=0 naif=-83 sain=64
[91] rawDeltaX=-2 evenements=0 naif=-85 sain=64
[92] rawDeltaX=-2 evenements=0 naif=-87 sain=64
[93] rawDeltaX=-2 evenements=0 naif=-89 sain=64
[94] rawDeltaX=-2 evenements=0 naif=-91 sain=64
[95] rawDeltaX=-2 evenements=0 naif=-93 sain=64
[96] rawDeltaX=-2 evenements=0 naif=-95 sain=64
```

**Lecture :**

- **La dernière valeur reste affichée sans qu'aucun événement n'arrive.** `rawDeltaX` vaut −2 sur 77 images consécutives
(77 à 153), sans événement.
- **Seul le prochain événement arrête la dérive**, quel qu'il soit : un mouvement dans quatre silences, un événement de
déplacement nul dans deux, et aucun dans le dernier, qui aurait continué si je n'avais pas fermé le programme.
- **Quand la dernière valeur est 0, il n'y a pas de dérive.** Les images 306 à 421 (116 images) restent figées à `naif=-493`.
- **Les valeurs gardées sont toujours petites** (±1 ou ±2) : le mouvement s'éteint par une traîne de petits déplacements, et
le dernier reste.
- **Les deux angles divergent aussi hors des silences.** Quand plusieurs événements arrivent dans la même image (jusqu'à 7),
`naif` n'en retient qu'un seul. À la fin de la session, `naif` vaut −46 et `sain` 518.

**Ce que ces lignes prouvent, en une phrase : ** Main posée, `rawDeltaX` garde la dernière valeur reçue (−2) pendant 77 images 
sans qu'aucun événement n'arrive, et l'angle qui la relit à chaque image dérive de −154 alors que l'angle accumulé reste figé 
à 64 : sans accumuler soi-même et vider une fois par image, la tête continue de tourner toute seule.

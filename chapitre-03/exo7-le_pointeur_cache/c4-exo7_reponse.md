## Souris cachée et confinée :

**Code** 
// main.cpp

```cpp
#define NOMINMAX
#include <windows.h>

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
    config.title  = "Souris cachee et confinee";
    config.width  = 1280;
    config.height = 720;

    nkentseu::Window fenetre(config);
    if (!fenetre.IsValid()) return 1;

    // Curseur caché et confiné via l'API Windows (voir Limites).
    HWND hwnd = FindWindowA(nullptr, "Souris cachee et confinee");
    RECT zone{};
    if (hwnd) {
        GetClientRect(hwnd, &zone);
        POINT haut{zone.left, zone.top}, bas{zone.right, zone.bottom};
        ClientToScreen(hwnd, &haut);
        ClientToScreen(hwnd, &bas);
        zone = {haut.x, haut.y, bas.x, bas.y};
        ClipCursor(&zone);
    }
    ShowCursor(FALSE);

    bool    enMarche = true;
    int32_t x = 0, y = 0;
    int32_t rawX = 0, rawY = 0;   // brut accumulé depuis l'image précédente

    EventSystem& ev = EventSystem::Instance();

    ev.SetEventCallback<NkWindowCloseEvent>([&](NkWindowCloseEvent*) { enMarche = false; });
    ev.SetEventCallback<NkKeyPressEvent>([&](NkKeyPressEvent* e) {
        if (e->GetKey() == NkKey::NK_ESCAPE) enMarche = false;
    });

    ev.SetEventCallback<NkMouseMoveEvent>([&](NkMouseMoveEvent* e) {
        x = e->GetX();
        y = e->GetY();
    });

    ev.SetEventCallback<NkMouseRawEvent>([&](NkMouseRawEvent* e) {
        rawX += e->GetDeltaX();   // accumuler, jamais écraser
        rawY += e->GetDeltaY();
    });

    std::FILE* csv = std::fopen("series.csv", "w");
    std::fprintf(csv, "image;x;y;rawDx;rawDy\n");

    const auto periode   = std::chrono::microseconds(1'000'000 / 72);
    auto       prochaine = std::chrono::steady_clock::now();
    unsigned   image     = 0;

    while (enMarche) {
        ev.PollEvents();

        std::printf("[%u] x=%d y=%d rawDelta=(%d,%d)\n", image, x, y, rawX, rawY);
        std::fprintf(csv, "%u;%d;%d;%d;%d\n", image, x, y, rawX, rawY);
        ++image;

        // Contournement : le moteur ne remet jamais à zéro le déplacement brut.
        // Sans ce vidage, la dernière valeur serait relue à chaque image et la
        // tête dériverait même souris immobile. Ne pas supprimer.
        rawX = rawY = 0;

        prochaine += periode;
        std::this_thread::sleep_until(prochaine);
    }

    std::fclose(csv);
    ShowCursor(TRUE);
    ClipCursor(nullptr);

    ev.RemoveEventCallback<NkMouseRawEvent>();
    ev.RemoveEventCallback<NkMouseMoveEvent>();
    ev.RemoveEventCallback<NkKeyPressEvent>();
    ev.RemoveEventCallback<NkWindowCloseEvent>();
    fenetre.Close();
    return 0;
}
```

**Construction et lancement.**

```
jenga build --config Debug
.\Build\Bin\Debug\Chap3\Chap3.exe
```

Le build s'est terminé avec `Status: ✓ SUCCESS` (18 avertissements, sans conséquence). Je clique dans la fenêtre, je pousse la souris vers les 
bords en m'y maintenant quelques instants, puis je quitte avec Échap pour que le curseur soit rendu au système. Les séries sont écrites dans 
`series.csv`, dans le dossier d'où j'ai lancé l'exécutable.

**Observations.**

- Le curseur n'est jamais apparu dans la fenêtre.
- Sur les 1 219 images enregistrées (images 0 à 1218, environ 17 s à 72 images par seconde), `x` et `y` restent dans la fenêtre : `x` varie de 0 à 1124 et `y` de 0 à 719.
- Au repos, le brut vaut (0, 0) : de l'image 698 à l'image 935 (238 images, environ 3,3 s), puis de 939 à 1218 (280 images).

**Les deux séries, au bord haut** (images 315 à 338) :

| Image | x | y | rawΔx | rawΔy |
|---|---|---|---|---|
| 315 | 261 | 0 | 0 | 0 |
| 316 | 248 | 0 | −13 | −11 |
| 318 | 183 | 0 | −26 | −50 |
| 320 | 127 | 0 | −21 | −68 |
| 324 | 105 | 0 | −1 | −60 |
| 332 | 107 | 0 | 3 | −18 |
| 336 | 116 | 0 | 5 | −33 |
| 338 | 127 | 0 | 10 | −46 |

**Les passages au bord, résumés.** Le brut est cumulé sur les images où la position est déjà collée au bord ; l'image d'arrivée est exclue, car une 
partie de son mouvement sert à atteindre la butée.

| Passage | Images | Position | Brut cumulé |
|---|---|---|---|
| Bord haut | 316 à 346 | `y` = 0 pendant 31 images | y : −680 |
| Coin haut-gauche | 376 à 379 | (0, 0) | (−57, −168) |
| Bord gauche | 512 à 518 | `x` = 0 | x : −156 |
| Bord gauche | 678 à 684 | `x` = 0 | x : −278 |
| Bord haut | 695 à 697 | `y` = 0 | y : −27 |
| Bord haut | 936 à 938 | `y` = 0 | y : −43 |
| Bord bas | 156 à 159 | `y` = 719 | y : +17 |

**Laquelle continue de bouger ?** Le déplacement brut. Au bord haut, `y` reste à 0 pendant 31 images alors que la main a remonté d'environ 680 unités. 
Le blocage se fait axe par axe : pendant ce temps, `x` continue de varier (de 261 à 105, puis jusqu'à 162), parce que seule la butée du haut joue. 
Aux images 937 et 938, `x` passe de 222 à 229 pendant que `y` reste à 0.

**Pourquoi c'est lui qu'il faut.**

- **La position est une butée.** Elle dit où est le pointeur sur l'écran. Une fois contre le bord, elle ne sait plus rien du geste : une tête commandée
- par `x` et `y` se bloquerait en plein virage.
- **Le pointeur est caché et confiné**, donc sa position ne représente plus rien à l'écran.
- **La position embarque l'accélération du système, le brut non.** Entre les images 209 et 219, la position fait (−233, −247) pour un brut de (−210, −216),
soit 11 à 14 % de plus. Entre 316 et 324, elle fait −156 pour un brut de −122, soit 28 % de plus. Quand la main va lentement (images 209 et 210), les deux
se suivent à quelques unités près. Une tête qui tourne plus vite quand on bouge vite est le défaut que le chapitre reproche au delta accéléré.
- **Le brut vient du matériel** (`NkMouseRawData` : « mouvement brut sans accélération », issu de `WM_INPUT`), sans butée d'écran ni accélération.

**Pourquoi le vidage à chaque image.** On additionne chaque événement dans un compteur à soi, jamais on n'écrase, puis on lit le total et on le remet à zéro 
une fois par image. Le compteur retombe à zéro quand la main s'arrête, ce que montrent les 238 images à (0, 0) : la tête s'arrête avec la main. Le moteur, lui, 
ne remet jamais son accumulateur à zéro (défaut signalé par le chapitre), d'où le contournement commenté dans le code.

**Limites.**

- **Le relevé couvre la session entière** (images 0 à 1218), mais une seule session.
- **Trois bords sont testés** (gauche, haut, bas), ainsi que le coin haut-gauche. Le bord droit ne l'est pas : `x` n'a pas dépassé 1124 sur 1280.
- **Le bord bas ne l'est que sur 4 images**, au début de la session, avec +17 de brut cumulé. C'est la donnée la plus faible du lot : elle va dans le même sens
que les autres, mais je ne m'appuie pas sur elle.
- **Les poussées au bord sont courtes** (de 3 à 31 images), parce qu'une main ne peut pas pousser indéfiniment sur un tapis.
- **Le curseur est caché et confiné avec l'API Windows** (`ShowCursor`, `ClipCursor`), pas avec le moteur : mes recherches n'ont trouvé dans `IWindowImpl.h`
que `CaptureMouse(bool)`, qui n'est pas un confinement.
- **L'explication de l'écart par l'accélération de pointeur est une hypothèse.** Je n'ai pas lu le réglage de souris de ma machine.
- **Des images à (0, 0) apparaissent en plein mouvement.** Environ une sur six entre les images 579 et 615, mais de façon plus irrégulière ailleurs (une sur
cinq vers 157 à 177, une sur dix vers 316 à 346). Je pense à un décalage entre le rythme des événements et celui des images à 72 par seconde, mais je n'ai pas
compté les événements reçus, donc c'est une hypothèse.

**Ce que cela montre.** La position et le déplacement brut répondent à deux questions différentes. La première dit où est le pointeur, la seconde dit ce que fait 
la main. Pour faire tourner une tête, il faut la seconde : elle ne rencontre pas de bord et n'est pas déformée par l'accélération.

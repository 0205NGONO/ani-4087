## État et événement : deux compteurs sur la touche Espace

**Consigne :** 
**Code
// src/main.cpp

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
    using Horloge = std::chrono::steady_clock;

    NkAppData app;
    app.appName = "Ma salle";
    if (!NkInitialise(app)) return 1;

    NkWindowConfig config;
    config.title  = "Etat vs evenement";
    config.width  = 1280;
    config.height = 720;

    nkentseu::Window fenetre(config);
    if (!fenetre.IsValid()) return 1;

    bool     enMarche      = true;
    bool     mesureEnCours = false;
    bool     espaceTenue   = false;  // état reconstruit à la main (voir Limites)
    uint32_t etat          = 0;      // images où Espace est tenue
    uint32_t appuis        = 0;      // NkKeyPressEvent sur Espace
    uint32_t repetitions   = 0;      // NkKeyRepeatEvent sur Espace
    uint32_t images        = 0;
    Horloge::time_point debut;

    EventSystem& ev = EventSystem::Instance();

    ev.SetEventCallback<NkWindowCloseEvent>([&](NkWindowCloseEvent*) {
        enMarche = false;
    });

    ev.SetEventCallback<NkKeyPressEvent>([&](NkKeyPressEvent* e) {
        if (e->GetKey() == NkKey::NK_ESCAPE) { enMarche = false; return; }
        if (e->GetKey() != NkKey::NK_SPACE) return;
        if (!mesureEnCours) {
            mesureEnCours = true;
            etat = appuis = repetitions = images = 0;
            debut = Horloge::now();
        }
        ++appuis;
        espaceTenue = true;
    });

    ev.SetEventCallback<NkKeyRepeatEvent>([&](NkKeyRepeatEvent* e) {
        if (e->GetKey() != NkKey::NK_SPACE || !mesureEnCours) return;
        ++repetitions;
    });

    ev.SetEventCallback<NkKeyReleaseEvent>([&](NkKeyReleaseEvent* e) {
        if (e->GetKey() != NkKey::NK_SPACE) return;
        espaceTenue = false;
        if (!mesureEnCours) return;
        mesureEnCours = false;
        double duree = std::chrono::duration<double>(Horloge::now() - debut).count();
        std::printf("Duree de l'appui                  : %.2f s\n", duree);
        std::printf("Images ecoulees                   : %u\n", images);
        std::printf("Compteur ETAT                     : %u\n", etat);
        std::printf("Compteur EVENEMENT (Press)        : %u\n", appuis);
        std::printf("Evenements REPEAT (a part)        : %u\n\n", repetitions);
        std::fflush(stdout);
    });

    const auto periode   = std::chrono::microseconds(1'000'000 / 72);
    auto       prochaine = Horloge::now();

    while (enMarche) {
        ev.PollEvents();
        if (mesureEnCours) {
            ++images;
            if (espaceTenue) ++etat;
        }
        prochaine += periode;
        std::this_thread::sleep_until(prochaine);
    }

    ev.RemoveEventCallback<NkKeyReleaseEvent>();
    ev.RemoveEventCallback<NkKeyRepeatEvent>();
    ev.RemoveEventCallback<NkKeyPressEvent>();
    ev.RemoveEventCallback<NkWindowCloseEvent>();
    fenetre.Close();
    return 0;
}
```

**Construction et lancement :**

```
jenga build --config Debug
.\Build\Bin\Debug\Chap3\Chap3.exe
```

Le build s'est terminé avec `Status: ✓ SUCCESS` (18 avertissements, sans conséquence). Je lance en Debug pour avoir la console, je clique dans la fenêtre 
pour lui donner le focus, je tiens Espace environ une seconde, puis je relâche. Quatre essais.

**Résultats :**

| Essai | Durée | Images | ÉTAT | Press | REPEAT | Images / s | REPEAT / s |
|---|---|---|---|---|---|---|---|
| 1 | 1,33 s | 96 | 96 | 1 | 25 | 72,2 | 18,8 |
| 2 | 1,41 s | 101 | 101 | 1 | 27 | 71,6 | 19,1 |
| 3 | 1,53 s | 111 | 111 | 1 | 32 | 72,5 | 20,9 |
| 4 | 1,38 s | 99 | 99 | 1 | 27 | 71,7 | 19,6 |

J'ai tenu la touche entre 1,3 et 1,5 s, et non exactement une seconde. Le compteur d'état vaut 96 à 111 et le compteur d'événement vaut 1 à chaque essai.

**Explication de l'écart :**

- **L'état est une photographie du présent.** À chaque image, la question est « Espace est-elle tenue maintenant ? », et la réponse reste oui pendant tout l'appui.
Le compteur mesure donc une durée en images : 1,33 s × 72 ≈ 96. Il dépend de la cadence : à 144 images par seconde, le même geste donnerait environ le double.
- **L'événement est le récit de ce qui est arrivé.** Un appui a eu lieu, une seule fois. Le compteur vaut 1 aux quatre essais, quelle que soit la durée de l'appui
et la cadence de la boucle.
- **Les répétitions sont envoyées à part.** Le moteur émet les répétitions automatiques du système dans `NkKeyRepeatEvent` (type `NK_KEY_REPEAT`), jamais dans
`NkKeyPressEvent`. Il n'y a pas de `IsRepeat()` à tester sur le rappel d'appui : `NkKeyPressEvent` n'en a pas, et c'est pourquoi le compilateur a refusé mon premier
essai. Cela explique que Press vaille exactement 1.

**Ce que cela implique pour un tir.** En lisant l'état, on tire 96 à 111 balles pour ce geste. En écoutant `NkKeyPressEvent`, on en tire une. En écoutant aussi 
`NkKeyRepeatEvent` par inadvertance, on en tirerait 26 à 33, à un rythme fixé par les réglages clavier de l'utilisateur et non par le code. L'état sert pour ce 
qui dure (avancer), l'événement pour ce qui arrive (tirer, saisir).

**Limites :**

- **Le compteur d'état n'utilise pas l'API d'état du moteur.** L'énoncé parle de `NkInput.IsKeyDown`. Ce nom n'existe pas dans mon moteur : la compilation l'a refusé
(`use of undeclared identifier 'NkInput'`), et une recherche de `IsKeyDown|IsKeyPressed|IsKeyHeld|GetKeyState` dans les `.h` n'a rien trouvé. J'ai donc reconstruit
l'état avec un booléen mis à jour par les événements d'appui et de relâchement. Par construction, ÉTAT est égal à Images dans chaque essai : ce que je mesure vraiment
est la cadence de ma boucle, pas une lecture d'état du moteur.
- **La cadence de 72 images par seconde est imposée par mon code** (`sleep_until`), et non mesurée sur le moteur. Elle correspond à la cadence du chapitre.
- **Les répétitions sont cohérentes avec un délai initial de 0,5 s puis environ 30 par seconde** : (1,33 − 0,5) × 30 ≈ 25 pour l'essai 1, et 27 pour l'essai 2. Je n'ai
pas vérifié les réglages clavier de ma machine, donc c'est une hypothèse. Le nombre de répétitions dépend de ces réglages et de la version de Windows.
- **Quatre essais seulement**, avec une durée d'appui non contrôlée.

**Ce que cela montre.** L'état et l'événement répondent à deux questions différentes. Le premier dit si la chose dure à cet instant, le second dit si elle vient d'arriver. 
Les confondre donne soit 96 balles par appui, soit un seul pas quand on voulait avancer.

---

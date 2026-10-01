## Rappels de fermeture et sortie unique

// main.cpp

```
#include "NKWindow/NkWindow.h"
#include "NKWindow/Core/NkMain.h"

int nkmain(const nkentseu::NkEntryState&)
{
    using namespace nkentseu;

    NkAppData app;
    app.appName = "Ma salle";
    if (!NkInitialise(app)) return 1;

    NkWindowConfig config;
    config.title  = "Ma salle";
    config.width  = 1280;
    config.height = 720;

    nkentseu::Window fenetre(config);
    if (!fenetre.IsValid()) return 1;

    bool enMarche = true;
    EventSystem& evenements = EventSystem::Instance();

    // Chemin 1 : la croix de la fenêtre.
    evenements.SetEventCallback<NkWindowCloseEvent>([&](NkWindowCloseEvent*) {
        enMarche = false;
    });

    // Chemin 2 : la touche Échap.
    evenements.SetEventCallback<NkKeyPressEvent>([&](NkKeyPressEvent* ev) {
        if (ev->GetKey() == NkKey::NK_ESCAPE) enMarche = false;
    });

    while (enMarche) {
        evenements.PollEvents();
    }

    // Point de sortie unique : les deux chemins arrivent ici.
    evenements.RemoveEventCallback<NkWindowCloseEvent>();
    evenements.RemoveEventCallback<NkKeyPressEvent>();
    fenetre.Close();
    return 0;
}
```

**Build et lancement**

```
jenga build --config Release
.\Build\Bin\Release\Chap3\Chap3.exe
```

**Observations.** Sous Windows, build Release, j'ai testé trois cas :
- un clic sur la croix ferme la fenêtre et arrête le programme ;
- la touche Échap fait de même ;
- une autre touche (une lettre) ne fait rien.

`Stop-Process -Name Chap3` arrête aussi le programme, mais de l'extérieur : le processus est tué sans passer par le nettoyage de fin de `nkmain`, 
donc ce n'est pas un troisième chemin de sortie du programme.

**Pourquoi les deux chemins doivent aboutir au même endroit.** La croix et Échap ne ferment rien elles-mêmes : chacune met seulement `enMarche` à 
faux. La boucle s'arrête alors à son prochain tour, et tout le nettoyage (retrait des rappels, fermeture de la fenêtre, `return 0`) se fait après 
la boucle, à un seul endroit.

Cela évite que deux sorties aient chacune leur propre nettoyage : l'une pourrait en oublier une partie, et chaque nouvelle façon de quitter (une 
manette, un menu) obligerait à tout recopier. C'est aussi plus sûr, car on ne détruit pas la fenêtre depuis l'intérieur d'un rappel, alors que le 
système d'événements est encore en train de travailler. Enfin, les rappels pointent vers `enMarche`, une variable locale : il faut les retirer 
avant la fin de `nkmain`, et les deux chemins passent forcément par ce retrait.

Le moteur ne ferme pas la fenêtre de lui-même quand on clique sur la croix : il signale seulement la demande, et c'est à l'application de décider. 
Sans rappel, la croix ne fait donc rien.

**Différence avec le chapitre.** Dans ce moteur, il n'y a ni `AddEventCallback` ni garde : l'API est `SetEventCallback<T>(...)`, avec un seul rappel par type, et `RemoveEventCallback<T>()` pour le retirer à la main.

**Limite.** Comme la boucle ne regarde plus `IsOpen()`, si la fenêtre était fermée par un autre moyen, la boucle tournerait sans fin. `while (enMarche && fenetre.IsOpen())` couvrirait ce cas, mais ce n'est pas ce que l'énoncé demande.

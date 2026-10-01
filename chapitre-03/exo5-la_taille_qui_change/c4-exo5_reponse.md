## Événements de redimensionnement : lent contre d'un coup

**Code** 
// main.cpp

```
#include <cstdio>

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
    unsigned nbEvenements = 0;
    EventSystem& evenements = EventSystem::Instance();

    evenements.SetEventCallback<NkWindowCloseEvent>([&](NkWindowCloseEvent*) {
        enMarche = false;
    });

    evenements.SetEventCallback<NkKeyPressEvent>([&](NkKeyPressEvent* ev) {
        if (ev->GetKey() == NkKey::NK_ESCAPE) enMarche = false;
    });

    evenements.SetEventCallback<NkWindowResizeEvent>([&](NkWindowResizeEvent* ev) {
        std::printf("[%u] %ux%u\n", ++nbEvenements, ev->GetWidth(), ev->GetHeight());
        std::fflush(stdout);
    });

    while (enMarche) {
        evenements.PollEvents();
    }

    evenements.RemoveEventCallback<NkWindowResizeEvent>();
    evenements.RemoveEventCallback<NkWindowCloseEvent>();
    evenements.RemoveEventCallback<NkKeyPressEvent>();
    fenetre.Close();
    return 0;
}
```

**Construction et lancement.**

```
jenga build --config Debug
.\Build\Bin\Debug\Chap3\Chap3.exe
```

Le build s'est terminé avec `Status: ✓ SUCCESS` (18 avertissements, sans conséquence).

**Série 1 : redimensionnement lent.** J'ai tiré un coin de la fenêtre lentement, puis relâché. Sortie complète :

```
[1] 1280x720
[2] 1282x720
[3] 1288x720
[4] 1290x720
[5] 1298x720
[6] 1300x720
[7] 1306x720
[8] 1308x720
[9] 1316x720
[10] 1319x720
[11] 1327x720
[12] 1329x720
[13] 1335x720
[14] 1338x720
[15] 1342x720
[16] 1343x720
[17] 1346x720
[18] 1349x720
[19] 1351x720
[20] 1352x720
[21] 1354x720
[22] 1356x720
[23] 1357x720
[24] 1358x720
[25] 1359x720
[26] 1362x720
[27] 1363x720
[28] 1364x720
[29] 1365x720
[30] 1370x720
[31] 1371x720
[32] 1373x720
[33] 1375x720
[34] 1376x720
[35] 1377x720
[36] 1378x720
[37] 1380x720
[38] 1381x720
[39] 1382x720
[40] 1383x720
[41] 1384x720
[42] 1385x720
[43] 1386x720
[44] 1387x720
[45] 1388x720
[46] 1389x720
[47] 1390x720
[48] 1390x721
[49] 1390x725
[50] 1390x726
[51] 1390x728
[52] 1390x729
[53] 1390x731
[54] 1390x733
[55] 1390x735
[56] 1390x736
[57] 1390x737
[58] 1390x738
[59] 1390x739
[60] 1390x740
[61] 1390x741
[62] 1390x742
[63] 1390x743
[64] 1390x744
[65] 1390x745
[66] 1390x746
[67] 1390x747
[68] 1390x748
[69] 1390x749
[70] 1390x751
[71] 1390x752
[72] 1391x752
[73] 1392x752
[74] 1393x752
[75] 1394x752
[76] 1395x752
[77] 1396x752
[78] 1398x752
[79] 1399x752
[80] 1400x752
[81] 1402x752
[82] 1403x752
[83] 1404x752
[84] 1405x752
[85] 1406x752
[86] 1407x752
[87] 1408x752
[88] 1408x753
[89] 1408x759
[90] 1408x761
[91] 1408x765
[92] 1408x766
[93] 1408x768
[94] 1408x769
[95] 1408x772
[96] 1408x773
[97] 1408x775
[98] 1408x778
[99] 1408x779
[100] 1408x780
[101] 1408x781
[102] 1408x782
[103] 1408x783
[104] 1408x784
[105] 1408x785
[106] 1408x786
[107] 1408x787
[108] 1408x788
[109] 1408x789
[110] 1408x790
[111] 1408x791
[112] 1408x792
[113] 1408x793
[114] 1408x794
[115] 1408x795
```

**Série 2 : redimensionnement d'un coup.** J'ai agrandi la fenêtre en un seul geste, puis l'ai restaurée. Sortie complète :

```
[1] 1280x720
[2] 1920x1009
[3] 1280x720
```

**Lecture des résultats.**

- **Série 1 : 115 lignes.** La première (`1280x720`) correspond à la taille de départ, pas à un changement : il y a donc **114 changements réels**,
- de 1280×720 à 1408×795. Le déplacement total est d'environ 128 pixels en largeur et 75 en hauteur, soit à peu près un événement tous les 1 à 2 pixels.
- Le plus grand écart entre deux lignes consécutives est de 8 pixels.
- **Série 2 : 3 lignes.** La première est la taille de départ ; il reste **2 changements** : l'agrandissement (1280×720 vers 1920×1009) et la restauration
- (retour à 1280×720). Chacun est un saut de plusieurs centaines de pixels, signalé en un seul événement.

**Conclusion.** Le nombre d'événements ne dépend pas de la distance parcourue : il dépend du nombre de tailles intermédiaires que Windows annonce à 
l'application. En tirant lentement, Windows annonce chaque taille traversée, ce qui donne 114 événements pour environ 200 pixels. En agrandissant d'un 
coup, la fenêtre saute directement à sa taille finale, et un seul événement suffit pour un écart de 640 pixels en largeur. Un événement de redimensionnement 
ne représente donc pas « un redimensionnement » : c'est une taille parmi d'autres, et il peut en arriver des dizaines pendant un seul geste.

**Conséquence pratique.** Le rappel ne doit pas faire de travail lourd (recréer une surface graphique, par exemple), car il peut être appelé plus de cent fois pendant un tirage lent. Le moteur définit aussi un événement `NkWindowResizeBeginEvent`, qui permettrait peut-être de ne réagir qu'au début ou à la fin du geste ; je ne l'ai pas testé.
- Je pense que la première ligne vient d'un événement émis à la création de la fenêtre, mais je ne l'ai pas vérifié dans le code.

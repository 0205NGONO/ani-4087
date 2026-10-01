// main.cpp
#include <chrono>

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

    // Expérience : on pompe les événements pendant 2 s, puis on arrête.
    auto debut = std::chrono::steady_clock::now();
    while (fenetre.IsOpen()) {
        bool pompe = std::chrono::steady_clock::now() - debut < std::chrono::seconds(2);
        if (pompe) EventSystem::Instance().PollEvents();
    }
    return 0;
}

## Boucle testée (`src/main.cpp`) :

```cpp
    while (fenetre.IsOpen()) {
        // EventSystem::Instance().PollEvents();
    }
```

**Construction.**

```powershell
jenga build --config Debug
```

Fin de la sortie :

```
════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED                                 
════════════════════════════════════════════════════════════════════════════════
Projects Built:  2/2
Warnings:       18
Time:           7.66s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════
```

Le log complet du build est à coller ici si le rendu l'exige : je ne l'ai pas en entier.

**Méthode de mesure.** Le script lance l'exécutable, attend que la fenêtre existe, démarre le chrono, puis interroge Windows toutes les 200 ms avec `IsHungAppWindow`. Je clique dans la fenêtre juste après le début du chrono, car Windows ne tranche qu'après une interaction restée sans réponse.

```powershell
if (-not ([System.Management.Automation.PSTypeName]'W').Type) {
Add-Type @"
using System; using System.Runtime.InteropServices;
public class W {
  [DllImport("user32.dll", CharSet=CharSet.Unicode)]
  public static extern IntPtr FindWindow(string c, string t);
  [DllImport("user32.dll")]
  public static extern bool IsHungAppWindow(IntPtr h);
}
"@
}

$p = Start-Process .\Build\Bin\Debug\Chap3\Chap3.exe -PassThru
while ($p.MainWindowHandle -eq [IntPtr]::Zero) { Start-Sleep -Milliseconds 100; $p.Refresh() }
$h = $p.MainWindowHandle
Write-Host ">>> CLIQUE DANS LA FENÊTRE MAINTENANT <<<"
$t = Get-Date
while (-not [W]::IsHungAppWindow($h) -and ((Get-Date) - $t).TotalSeconds -lt 60) {
    Start-Sleep -Milliseconds 200
}
if ([W]::IsHungAppWindow($h)) { "Bloquée après {0:N1} s" -f ((Get-Date) - $t).TotalSeconds }
else { "Pas déclarée bloquée en 60 s" }
Write-Host ">>> PRENDS LA CAPTURE MAINTENANT (Win+Maj+S), 10 s <<<"
Start-Sleep 10
Stop-Process $p
```

## Résultats : Deux essais, avec le même script :

```
>>> CLIQUE DANS LA FENÊTRE MAINTENANT <<<
Bloquée après 6,8 s
```

```
Bloquée après 6,3 s
```

La fenêtre est donc déclarée bloquée environ **6,5 secondes** après le début du chrono. L'écart de 0,5 s entre les deux essais correspond au délai variable entre le démarrage du chrono et mon clic.

## Observations : Pendant l'expérience, la croix de la fenêtre ne ferme plus rien, alors que le programme tourne toujours à plein régime. Le titre « (Ne répond pas) » n'est pas apparu sur ma machine : le blocage a été détecté par `IsHungAppWindow`, pas par un affichage. En Debug, le moteur ouvre aussi une console qui appartient au processus, et la fermer arrête le programme.

## Limites : Ce chiffre dépend de l'instant de mon clic et de ma version de Windows, et je ne l'ai pas comparé à un délai documenté. Des essais antérieurs, avec un script moins rigoureux dont un « 0,9 s », étaient invalides : le chrono démarrait avant que la fenêtre existe, ou sans interaction. Je ne les retiens pas.

## Ce que cela montre : Une fenêtre non pompée n'est pas plantée : le système lui envoie des messages et compte qu'elle les lise. Sans `PollEvents`, personne ne relève ce courrier, et Windows finit par la déclarer bloquée. C'est le contrat du système d'exploitation, pas une règle propre au moteur.

## Retour à l'état normal : `PollEvents()` a été remis dans la boucle, à chaque tour.

Capture de la fenêtre bloquée : https://github.com/0205NGONO/ani-4087/blob/main/chapitre-03/exo2-la_fenetre_qui_ne_repond_pas/bloc1.png

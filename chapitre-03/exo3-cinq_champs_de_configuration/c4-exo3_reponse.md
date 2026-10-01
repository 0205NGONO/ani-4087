## Cinq champs que le chapitre ne montre pas :
**1. `config.centered = false;`** Je m'attendais à ce que la fenêtre ne soit plus centrée sur l'écran et apparaisse en `(x, y) = (100, 100)`, 
valeurs par défaut de la structure. C'est ce que j'ai observé : elle a quitté le centre, et ses dimensions sont restées à 1280×720. Cela 
s'explique par le code : le backend Win32 lit `centered` et, quand il est faux, place la fenêtre avec `config.x` et `config.y`.

**2. `config.visible = false;`** Je m'attendais à ce que la fenêtre soit créée sans jamais être affichée. Elle n'est en effet jamais apparue, 
alors que le processus tournait toujours, et j'ai dû l'arrêter avec `Stop-Process`. Le backend lit `visible` avant d'afficher la fenêtre (même 
fichier, vers la ligne 211), ce qui explique ce comportement.

**3. `config.bgColor = 0xC0392BFF;`** Je m'attendais à un fond rouge brique, mais rien n'a changé. D'après les sources, c'est normal : sous 
Windows, ce champ n'est lu nulle part. L'en-tête du backend annonce « Pas de SetBackgroundColor / GetBackgroundColor », et mes recherches de 
`bgColor` dans ce dossier ne donnent rien. Le champ fonctionne sous Linux, où j'ai mesuré un pixel (192, 57, 43), mais pas sous Windows.

**4. `config.resizable = false;`** Je m'attendais à une fenêtre de taille fixe, sans poignées de redimensionnement, et je n'ai observé aucun 
changement. Je pense que le champ ne sert à rien : une recherche de `resizable` dans toutes les sources du moteur ne trouve que sa déclaration 
dans `NkWindowConfig.h`, donc aucun backend ne le lit. C'est un champ déclaré mais inutilisé.

**5. `config.frame = false;`** Je m'attendais à une fenêtre sans bordure ni barre de titre, et rien n'a changé. Le backend Win32 lit pourtant 
ce champ, mais le style qu'il choisit quand `frame` est faux reste `WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | …` (lignes 129 à 132). 
Il conserve donc `WS_CAPTION` et `WS_THICKFRAME`, ce qui donne une fenêtre toujours décorée. C'est un défaut apparent du moteur, pas de mon code.

**Bilan.** Sur les cinq champs, deux ont l'effet attendu (`centered`, `visible`) et trois n'en ont aucun sous Windows (`bgColor`, `resizable`, 
`frame`). Le nom d'un champ dans la structure ne garantit donc pas que le backend de la plateforme le prenne en compte.

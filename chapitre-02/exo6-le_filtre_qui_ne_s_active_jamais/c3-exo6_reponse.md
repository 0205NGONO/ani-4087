Voici le lien du projet : 

## Démarche :

Le filtre `filter("system:Windows")` conditionne l'ajout de la définition `MODULE_ACTIF` 
à la plateforme cible. Pour vérifier que ce filtre est réellement pris en compte lors de la 
compilation — et non simplement ignoré — j'ai testé les deux valeurs possibles de la condition, 
sur une machine Windows.

## Test 1 — condition fausse (`system:Linux`)

Avec le filtre réglé sur `system:Linux` (faux sur ma machine Windows), après `jenga clean` puis 
`jenga build --verbose`, le programme compilé affiche :
```
Definition non recue : MODULE_ACTIF est absent
```

## Test 2 — condition vraie (`system:Windows`)

En changeant uniquement `"Linux"` en `"Windows"` dans le fichier de projet, puis en refaisant 
`jenga clean` et `jenga build --verbose`, le même programme (recompilé) affiche cette fois :
```
Definition recue : MODULE_ACTIF est actif
```

## Conclusion :

Un seul mot change dans le fichier de projet (`Linux` → `Windows`), mais le comportement du 
binaire produit change entièrement. Cela prouve que le filtre `system:...` agit bien sur la 
génération des définitions de compilation, bien que `jenga info` et `jenga build --verbose` 
ne l'affichent nulle part explicitement dans leur sortie. La seule façon fiable de vérifier 
l'effet du filtre est donc d'observer le comportement du programme compilé lui-même, et non 
les journaux de l'outil de build.

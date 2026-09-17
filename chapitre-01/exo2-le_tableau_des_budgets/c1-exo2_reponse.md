# Chapitre 2 — Le budget des vingt millisecondes : sources mesurées

Tableau des cinq étapes du pipeline, avec une source mesurée et attribuable pour chacune quand elle existe :

| Étape (cours) | Valeur mesurée trouvée | Source |
|---|---|---|
| **Les capteurs mesurent le mouvement** | 1 - 3 mS ms | https://anr.fr/Colloques/WISG2013/presentations/AAP10_DIAMS.pdf|
| **Le système transmet la mesure** | ~1 ms (transmission USB) | Article *Game Developer*, *"Bringing VR to Life"*<br>« the latency incurred by USB is 1 millisecond » |
| **Votre application décide et dessine** | Introuvable| - |
| **Le compositeur assemble** | Introuvable| - |
| **L'écran affiche la ligne** | 2 - 4 mS | https://xinreality.com/wiki/Latency |

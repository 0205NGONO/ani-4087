Liens des fichiers :
https://github.com/0205NGONO/ani-4087/blob/main/chapitre-02/exo5-le_define_qui_manque/module.hpp


## Message 1 — compilé avec le define (`g++ -DMODULE_ACTIF -o programme main.cpp module.cpp`)

Aucune erreur de compilation. Le programme s'exécute et affiche :
42

## Message 2 — même code, sans le define (`g++ -o programme main.cpp module.cpp`)
main.cpp: In function 'int main()':
main.cpp:7:11: error: 'class MaClasse' has no member named 'DefinirValeur'
    7 |     objet.DefinirValeur(42);
      |           ^~~~~~~~~~~~~
main.cpp:8:24: error: 'class MaClasse' has no member named 'ObtenirValeur'
    8 |     std::cout << objet.ObtenirValeur() << "\n";
      |                        ^~~~~~~~~~~~~
module.cpp:3:5: error: no declaration matches 'int MaClasse::ObtenirValeur() const'
    3 | int MaClasse::ObtenirValeur() const { return valeur; }
      |     ^~~~~~~~
module.cpp:3:5: note: no functions named 'int MaClasse::ObtenirValeur() const'
In file included from module.cpp:2:
module.hpp:6:7: note: 'class MaClasse' defined here
    6 | class MaClasse {
      |       ^~~~~~~~
module.cpp:4:6: error: no declaration matches 'void MaClasse::DefinirValeur(int)'
    4 | void MaClasse::DefinirValeur(int v) { valeur = v; }
      |      ^~~~~~~~
module.cpp:4:6: note: no functions named 'void MaClasse::DefinirValeur(int)'
module.hpp:6:7: note: 'class MaClasse' defined here
    6 | class MaClasse {
      |       ^~~~~~~~


## Conclusion

Le message d'erreur obtenu sans le define est celui que j'aurais su diagnostiquer seule : il indique précisément le membre manquant (`DefinirValeur`, `ObtenirValeur`) et sa ligne exacte, ce qui suffit à remonter jusqu'à l'en-tête et à comprendre qu'une classe attendue complète ne l'est pas. Le premier cas, lui, ne pose aucun problème à diagnostiquer puisqu'il n'y a pas d'erreur.

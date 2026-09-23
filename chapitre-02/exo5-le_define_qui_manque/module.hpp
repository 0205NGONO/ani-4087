#ifndef MODULE_HPP
#define MODULE_HPP

// Si MODULE_ACTIF est defini, la classe est complete.
// Sinon, c'est une coquille vide : le nom existe, mais rien dedans.
class MaClasse {
public:
#ifdef MODULE_ACTIF
    int ObtenirValeur() const;
    void DefinirValeur(int v);
private:
    int valeur;
#endif
};

#endif
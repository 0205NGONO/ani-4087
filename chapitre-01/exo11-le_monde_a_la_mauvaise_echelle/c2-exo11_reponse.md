#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

struct Dimensions {
    double largeur, longueur, hauteur;
};

struct Meuble {
    string nom;
    Dimensions dimensions;
};

struct Salle {
    Dimensions dimensions;
    vector<Meuble> mobilier;
};

Dimensions Multiplier_dimensions(const Dimensions &d, double facteur) {
    return { d.largeur * facteur, d.longueur * facteur, d.hauteur * facteur };
}

void Afficher_dimensions(const string &nom, const Dimensions &d) {
    cout << "  " << nom << " : " << d.largeur << " m x " << d.longueur
         << " m x " << d.hauteur << " m (l x L x h)\n";
}

int main() {
    cout << fixed << setprecision(3);

    Salle salle;
    salle.dimensions = { 4.5, 6.0, 2.7 };
    salle.mobilier = {
        { "Table",   { 1.4, 0.9, 0.75 } },
        { "Chaise",  { 0.45, 0.45, 0.85 } },
        { "Armoire", { 1.0, 0.6, 2.1 } }
    };

    double facteur;
    cout << "Facteur d'echelle : ";
    cin >> facteur;

    cout << "\nDimensions apres application du facteur " << facteur << " :\n";
    Afficher_dimensions("Salle", Multiplier_dimensions(salle.dimensions, facteur));
    for (const auto &meuble : salle.mobilier) {
        Afficher_dimensions(meuble.nom, Multiplier_dimensions(meuble.dimensions, facteur));
    }

    return 0;
}

## Avis pour différentes échelles

Facteur d'echelle : 5

Dimensions apres application du facteur 5.000 :
  Salle : 22.500 m x 30.000 m x 13.500 m (l x L x h)
  Table : 7.000 m x 4.500 m x 3.750 m (l x L x h)
  Chaise : 2.250 m x 2.250 m x 4.250 m (l x L x h)
  Armoire : 5.000 m x 3.000 m x 10.500 m (l x L x h)

## - Avis : "C'est grand hein... C'est une maison ou le hall d'un hôtel ?"

Facteur d'echelle : 0.8

Dimensions apres application du facteur 0.800 :
  Salle : 3.600 m x 4.800 m x 2.160 m (l x L x h)
  Table : 1.120 m x 0.720 m x 0.600 m (l x L x h)
  Chaise : 0.360 m x 0.360 m x 0.680 m (l x L x h)
  Armoire : 0.800 m x 0.480 m x 1.680 m (l x L x h)

## - Avis : "Pas mal... C'est une chambre, ou une cuisine ? mais la hauteur est un peu petite pour
un espace habitable quand-même"

Facteur d'echelle : 0.1

Dimensions apres application du facteur 0.100 :
  Salle : 0.450 m x 0.600 m x 0.270 m (l x L x h)
  Table : 0.140 m x 0.090 m x 0.075 m (l x L x h)
  Chaise : 0.045 m x 0.045 m x 0.085 m (l x L x h)
  Armoire : 0.100 m x 0.060 m x 0.210 m (l x L x h)

## - Avis : "QU'est-ce que tu as fait petit comme ça ? C'est pour un oiseau ou bien ?"

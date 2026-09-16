#include <iostream>
#include <iomanip>

using namespace std;

struct vecteur_3D {
    double x, y, z;
};

double produit_scalaire(const vecteur_3D a, const vecteur_3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vecteur_3D Avant()  { return { 0.0,  0.0, -1.0 }; } //Puisque l'avant regarde les Z négatifs
vecteur_3D Haut()   { return { 0.0,  1.0,  0.0 }; } //Puisque le haut regarde les Y positifs
vecteur_3D Droite() { return { 1.0,  0.0,  0.0 }; } //Puisque la droite regarde les X positifs

int main() {
    vecteur_3D p{};

    cout << "Entrez les coordonnées du vecteur p (x y z) : ";
    cin >> p.x >> p.y >> p.z;

    cout << fixed << setprecision(4);
    cout << "Avant : " << produit_scalaire(p, Avant())  << "\n";
    cout << "Haut : " << produit_scalaire(p, Haut())   << "\n";
    cout << "Droite : " << produit_scalaire(p, Droite()) << "\n";

    return 0;
}

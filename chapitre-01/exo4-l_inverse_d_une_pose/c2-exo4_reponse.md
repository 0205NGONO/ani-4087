#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

struct Vecteur_3D {
    double x, y, z;
};

struct Quaternion {
    double x, y, z, t;
};

struct Pose {
    Vecteur_3D position;
    Quaternion orientation;
};

Vecteur_3D Addition_3D(const Vecteur_3D &a, const Vecteur_3D &b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

Vecteur_3D Oppose_3D(const Vecteur_3D &v) {
    return { -v.x, -v.y, -v.z };
}

Vecteur_3D Soustraction_3D(const Vecteur_3D &a, const Vecteur_3D &b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

Quaternion Multiplication_quaterion(const Quaternion &a, const Quaternion &b) {
    return {
        a.t * b.x + a.x * b.t + a.y * b.z - a.z * b.y,
        a.t * b.y - a.x * b.z + a.y * b.t + a.z * b.x,
        a.t * b.z + a.x * b.y - a.y * b.x + a.z * b.t,
        a.t * b.t - a.x * b.x - a.y * b.y - a.z * b.z
    };
}

// Conjugué : q⁻¹ = q̄
Quaternion Conjugue(const Quaternion &q) {
    return { -q.x, -q.y, -q.z, q.t };
}

Vecteur_3D Tourner(const Quaternion &q, const Vecteur_3D &v) {
    Quaternion vPur{ v.x, v.y, v.z, 0.0 };
    Quaternion resultat = Multiplication_quaterion(Multiplication_quaterion(q, vPur), Conjugue(q));
    return { resultat.x, resultat.y, resultat.z };
}

// p_espace = orientation * p_entite + position
Vecteur_3D Appliquer_pose(const Pose &pose, const Vecteur_3D &point) {
    Vecteur_3D tourne = Tourner(pose.orientation, point);
    return Addition_3D(tourne, pose.position);
}

/* Inverse d'une pose :
    orientation_inverse = conjugué(orientation)
    position_inverse = orientation_inverse appliquée à (-position)
    L'inverse analytique évite l'inversion générale, qui pourrait silencieusement renvoyer l'identité en cas de matrice singulière.
*/
Pose Inverse(const Pose &pose) {
    Quaternion orientation_inverse = Conjugue(pose.orientation);
    Vecteur_3D position_inverse = Tourner(orientation_inverse, Oppose_3D(pose.position));
    return { position_inverse, orientation_inverse };
}

int main() {
    Pose pose{};
    Vecteur_3D point{};

    cout << "Position de la pose (x y z) : ";
    cin >> pose.position.x >> pose.position.y >> pose.position.z;

    cout << "Quaternion de la pose (x y z t) : ";
    cin >> pose.orientation.x >> pose.orientation.y >> pose.orientation.z >> pose.orientation.t;

    cout << "Point à transformer (x y z) : ";
    cin >> point.x >> point.y >> point.z;

    Vecteur_3D transforme = Appliquer_pose(pose, point);

    Pose pose_inverse = Inverse(pose);
    Vecteur_3D point_revenu = Appliquer_pose(pose_inverse, transforme);

    Vecteur_3D ecart = Soustraction_3D(point_revenu, point);

    cout << fixed << setprecision(4);
    cout << "Point transforme : "
         << transforme.x << " " << transforme.y << " " << transforme.z << "\n";
    cout << "Point apres inverse : "
         << point_revenu.x << " " << point_revenu.y << " " << point_revenu.z << "\n";
    cout << "Ecart au point de depart : "
         << ecart.x << " " << ecart.y << " " << ecart.z << "\n";

    return 0;
}
/*
### Exemple
Position de la pose (x y z) : Quaternion de la pose (x y z t) : Point ├á transformer (x y z) : Point transforme : 2.9122 9.7842 9.1737
Point apres inverse : 2.0002 5.0005 7.0007
Ecart au point de depart : 0.0002 0.0005 0.0007
*/

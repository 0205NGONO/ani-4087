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

// Conjugué : q⁻¹ = q̄ pour un quaternion unitaire ou normalisé
Quaternion Conjugue(const Quaternion &q) {
    return { -q.x, -q.y, -q.z, q.t };
}

// Rotation par multiplication complète : v' = q * v * q̄
Vecteur_3D Tourner(const Quaternion &q, const Vecteur_3D &v) {
    Quaternion vPur{ v.x, v.y, v.z, 0.0 };
    Quaternion resultat = Multiplication_quaterion(Multiplication_quaterion(q, vPur), Conjugue(q));
    return { resultat.x, resultat.y, resultat.z };
}

// Applique une pose à un point : rotation d'abord, translation ensuite.
// p_espace = orientation * p_entite + position
Vecteur_3D Appliquer_pose(const Pose &pose, const Vecteur_3D &point) {
    Vecteur_3D tourne = Tourner(pose.orientation, point);
    return Addition_3D(tourne, pose.position);
}

// Compose deux poses : le parent d'abord, l'enfant ensuite
Pose Composer(const Pose &parent, const Pose &enfant) {
    Quaternion orientation_composee = Multiplication_quaterion(parent.orientation, enfant.orientation);
    Vecteur_3D position_composee = Appliquer_pose(parent, enfant.position);
    return { position_composee, orientation_composee };
}

int main() {
    Pose pose_parent{};
    Pose pose_enfant{};
    Vecteur_3D point{};

    cout << "Pose parent - position (x y z) : \n";
    cin >> pose_parent.position.x >> pose_parent.position.y >> pose_parent.position.z;
    cout << "Pose parent - quaternion (x y z t) : \n";
    cin >> pose_parent.orientation.x >> pose_parent.orientation.y
        >> pose_parent.orientation.z >> pose_parent.orientation.t;

    cout << "Pose enfant - position (x y z) : \n";
    cin >> pose_enfant.position.x >> pose_enfant.position.y >> pose_enfant.position.z;
    cout << "Pose enfant - quaternion (x y z t) : \n";
    cin >> pose_enfant.orientation.x >> pose_enfant.orientation.y
        >> pose_enfant.orientation.z >> pose_enfant.orientation.t;

    cout << "Point local (x y z) : \n";
    cin >> point.x >> point.y >> point.z;

    // Chemin 1 : composer les deux poses, puis appliquer une seule fois
    Pose pose_composee = Composer(pose_parent, pose_enfant);
    Vecteur_3D point_via_composition = Appliquer_pose(pose_composee, point);

    // Chemin 2 : appliquer l'une après l'autre, l'enfant d'abord, le parent ensuite
    Vecteur_3D point_intermediaire = Appliquer_pose(pose_enfant, point);
    Vecteur_3D point_via_etapes = Appliquer_pose(pose_parent, point_intermediaire);

    Vecteur_3D ecart = Soustraction_3D(point_via_composition, point_via_etapes);

    cout << fixed << setprecision(4);
    cout << "Point via pose composee : "
         << point_via_composition.x << " " << point_via_composition.y << " " << point_via_composition.z << "\n";
    cout << "Point via deux etapes   : "
         << point_via_etapes.x << " " << point_via_etapes.y << " " << point_via_etapes.z << "\n";
    cout << "Ecart entre les deux    : "
         << ecart.x << " " << ecart.y << " " << ecart.z << "\n";

    return 0;
}

/* Exemple :
echo "3 -1 2 0.2440 0.2440 0.2440 0.9063 0.5 1.5 -0.5 0 0.3007 0 0.9537 2 0.5 -1" | .\exo5-la_composition.exe
Pose parent - position (x y z) : 
Pose parent - quaternion (x y z t) : 
Pose enfant - position (x y z) : 
Pose enfant - quaternion (x y z t) : 
Point local (x y z) : 
Point via pose composee : 2.1612 2.1991 0.7381
Point via deux etapes   : 2.1612 2.1991 0.7381
Ecart entre les deux    : 0.0000 -0.0000 0.0000
*/

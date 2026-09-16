#include <iostream>
#include <iomanip>

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
Vecteur_3D Tourner(const Quaternion q, const Vecteur_3D v) {
    Quaternion vPur{ v.x, v.y, v.z, 0.0 };          // v devient un quaternion pur
    Quaternion resultat = Multiplication_quaterion(Multiplication_quaterion(q, vPur), Conjugue(q));
    return { resultat.x, resultat.y, resultat.z };  // partie vectorielle du résultat
}

// Rotation d'abord, translation ensuite.
// p_espace = orientation * p_entite + position
Vecteur_3D Appliquer_pose(const Pose pose, const Vecteur_3D point) {
    Vecteur_3D tourne = Tourner(pose.orientation, point);
    return Addition_3D(tourne, pose.position);
}

// Translation d'abord, rotation ensuite.
// p_espace = orientation * (p_entite + position)
Vecteur_3D Appliquer_pose_inverse(const Pose pose, const Vecteur_3D point) {
    Vecteur_3D v_deplace = Addition_3D(point, pose.position);
    return Tourner(pose.orientation, v_deplace);
}

int main() {
    Pose pose{};
    Vecteur_3D point{};

    cout << "Position de la pose (x y z) : ";
    cin >> pose.position.x >> pose.position.y >> pose.position.z;

    cout << "Quaternion de la pose (x y z t) : ";
    cin >> pose.orientation.x >> pose.orientation.y
              >> pose.orientation.z >> pose.orientation.t;

    cout << "Point à transformer (x y z) : ";
    cin >> point.x >> point.y >> point.z;

    Vecteur_3D resultat_rotation_puis_translation = Appliquer_pose(pose, point);
    Vecteur_3D resultat_translation_puis_rotation = Appliquer_pose_inverse(pose, point);

    cout << fixed << setprecision(4);
    cout << "Rotation puis translation : "
         << resultat_rotation_puis_translation.x << " "
         << resultat_rotation_puis_translation.y << " "
         << resultat_rotation_puis_translation.z << "\n";
    cout << "Translation puis rotation : "
         << resultat_translation_puis_rotation.x << " "
         << resultat_translation_puis_rotation.y << " "
         << resultat_translation_puis_rotation.z << "\n";

    return 0;
}

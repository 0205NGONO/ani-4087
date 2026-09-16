#include <iostream>

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

// Conjugué : q⁻¹ = q̄ pour un quaternion unitaire
Quaternion Conjugue(const Quaternion &q) {
    return { -q.x, -q.y, -q.z, q.t };
}

// Rotation par multiplication complète : v' = q * v * q̄
Vecteur_3D Tourner(const Quaternion &q, const Vecteur_3D &v) {
    Quaternion v_quat_Pur{ v.x, v.y, v.z, 0.0 };          // v devient un quaternion pur
    Quaternion resultat = Multiplication_quaterion(Multiplication_quaterion(q, v_quat_Pur), Conjugue(q));
    return { resultat.x, resultat.y, resultat.z };  // partie vectorielle du résultat
}

// On pplique une pose à un point : rotation d'abord, translation ensuite.
// p_espace = orientation * p_entite + position
Vecteur_3D Appliquer_pose(const Pose &pose, const Vecteur_3D &point) {
    Vecteur_3D tourne = Tourner(pose.orientation, point);
    return Addition_3D(tourne, pose.position);
}

int main() {
    Pose pose{};
    Vecteur_3D  point{};

    cout << "Position de la pose (x y z) : ";
    cin >> pose.position.x >> pose.position.y >> pose.position.z;

    cout << "Quaternion de la pose (x y z t) : ";
    cin >> pose.orientation.x >> pose.orientation.y
              >> pose.orientation.z >> pose.orientation.t;

    cout << "Point à transformer (x y z) : ";
    cin >> point.x >> point.y >> point.z;

    Vecteur_3D resultat = Appliquer_pose(pose, point);

    cout << fixed << setprecision(4);
    cout << resultat.x << " " << resultat.y << " " << resultat.z << "\n";

    return 0;
}

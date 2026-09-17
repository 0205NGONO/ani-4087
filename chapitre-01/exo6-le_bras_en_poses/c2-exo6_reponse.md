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

// Compose deux poses : le parent d'abord, l'enfant ensuite.
Pose Composer(const Pose &parent, const Pose &enfant) {
    Quaternion orientation_composee = Multiplication_quaterion(parent.orientation, enfant.orientation);
    Vecteur_3D position_composee = Appliquer_pose(parent, enfant.position);
    return { position_composee, orientation_composee };
}

int main() {
    const double LONGUEUR_BRAS = 0.30;         // en mètres
    const double LONGUEUR_AVANT_BRAS = 0.25;    // en mètres

    // chaque articulation porte sa pose dans le repere de la precedente. Le bras "pend" le long de -Y local.
    Pose pose_coude_locale{ { 0.0, -LONGUEUR_BRAS, 0.0 }, { 0, 0, 0, 1 } };
    Pose pose_main_locale{ { 0.0, -LONGUEUR_AVANT_BRAS, 0.0 }, { 0, 0, 0, 1 } };

    // La seule pose qu'on va faire varier : celle de l'epaule, dans le monde. 
    Pose pose_epaule_monde{};
    cout << "Position de l'epaule dans le monde (x y z) : \n";
    cin >> pose_epaule_monde.position.x >> pose_epaule_monde.position.y >> pose_epaule_monde.position.z;
    cout << "Orientation de l'epaule (x y z t) : \n";
    cin >> pose_epaule_monde.orientation.x >> pose_epaule_monde.orientation.y
        >> pose_epaule_monde.orientation.z >> pose_epaule_monde.orientation.t;

    // Le coude dans le monde = composition epaule (parent) + coude (enfant).
    Pose pose_coude_monde = Composer(pose_epaule_monde, pose_coude_locale);

    /* La main dans le monde = composition coude-dans-le-monde (parent) + main (enfant).
     NB : chaque articulation ne connait que son parent immediat.*/
    Pose pose_main_monde = Composer(pose_coude_monde, pose_main_locale);

    cout << fixed << setprecision(4);
    cout << "Position du coude dans le monde : \n"
         << pose_coude_monde.position.x << " "
         << pose_coude_monde.position.y << " "
         << pose_coude_monde.position.z << "\n";
    cout << " Position de la main dans le monde  : \n"
         << pose_main_monde.position.x << " "
         << pose_main_monde.position.y << " "
         << pose_main_monde.position.z << "\n";

    return 0;
}

## - Position de l'épaule, de la main et du coude :

echo "0.8 2.0 -1.5 0 0 0 1" | .\exo6-le_bras_en_poses.exe
-- Position de l'épaule dans le monde (x y z) : 
Orientation de l'epaule (x y z t) : 
-- Position du coude dans le monde : 
0.8000 1.7000 -1.5000
-- Position de la main dans le monde  : 
0.8000 1.4500 -1.5000

## Afin de vérifier si la main suit les mouvements de l'épaule, 
nous allons conserver la position de l'épaule, en changeant juste le quaternion :

-- L'épaule tournée à 45° :

echo "0.8 2.0 -1.5 0.0 0.0 0.3827 0.9239" | .\exo6-le_bras_en_poses.exe
-- Position de l'épaule dans le monde (x y z) : 
Orientation de l'epaule (x y z t) : 
-- Position du coude dans le monde : 
1.0121 1.7879 -1.5000
-- Position de la main dans le monde  : 
1.1889 1.6111 -1.5000

-- L'épaule tournée à 90° :

echo "0.8 2.0 -1.5 0.0 0.0 0.7071 0.7071" | .\exo6-le_bras_en_poses.exe
-- Position de l'épaule dans le monde (x y z) : 
Orientation de l'epaule (x y z t) : 
-- Position du coude dans le monde : 
1.1000 2.0000 -1.5000
-- Position de la main dans le monde  : 
1.3500 2.0000 -1.5000

-- L'épaule tournée à 180° :

echo "0.8 2.0 -1.5 0.0 0.0 0.7071 0.7071" | .\exo6-le_bras_en_poses.exe
-- Position de l'épaule dans le monde (x y z) : 
Orientation de l'epaule (x y z t) : 
-- Position du coude dans le monde : 
1.1000 2.0000 -1.5000
-- Position de la main dans le monde  : 
1.3500 2.0000 -1.5000

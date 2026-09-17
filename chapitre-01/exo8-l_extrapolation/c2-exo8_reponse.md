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

Vecteur_3D Multiplier_scalaire_3D(const Vecteur_3D &v, double s) {
    return { v.x * s, v.y * s, v.z * s };
}

double Norme_3D(const Vecteur_3D &v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Quaternion Multiplication_quaterion(const Quaternion &a, const Quaternion &b) {
    return {
        a.t * b.x + a.x * b.t + a.y * b.z - a.z * b.y,
        a.t * b.y - a.x * b.z + a.y * b.t + a.z * b.x,
        a.t * b.z + a.x * b.y - a.y * b.x + a.z * b.t,
        a.t * b.t - a.x * b.x - a.y * b.y - a.z * b.z
    };
}

double Norme_quaternion(const Quaternion &q) {
    return sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.t * q.t);
}

Quaternion Normaliser_quaternion(const Quaternion &q) {
    double norme = Norme_quaternion(q);
    if (norme < 1e-12) {
        // Quaternion degenere : on retombe sur l'identite plutot que diviser par ~0.
        return { 0.0, 0.0, 0.0, 1.0 };
    }
    return { q.x / norme, q.y / norme, q.z / norme, q.t / norme };
}

/* Construit le quaternion representant une rotation d'angle "angle"
autour de l'axe unitaire "axe". */
Quaternion Quaternion_depuis_axe_angle(const Vecteur_3D &axe, double angle) {
    double demi_angle = angle / 2.0;
    double s = sin(demi_angle);
    return { axe.x * s, axe.y * s, axe.z * s, cos(demi_angle) };
}

/* Quaternion "increment" correspondant a une vitesse angulaire constante
appliquee pendant dt secondes.
Si la vitesse angulaire est nulle (ou quasi nulle), il n'y a pas d'axe de
rotation defini : on evite la division par sa norme et on renvoie
directement l'identite (aucune rotation ne se produit). */
Quaternion Quaternion_increment_rotation(const Vecteur_3D &vitesse_angulaire, double dt) {
    const double EPSILON = 1e-9;

    double norme_omega = Norme_3D(vitesse_angulaire);
    if (norme_omega < EPSILON) {
        return { 0.0, 0.0, 0.0, 1.0 };
    }

    Vecteur_3D axe = Multiplier_scalaire_3D(vitesse_angulaire, 1.0 / norme_omega);
    double angle = norme_omega * dt;
    return Quaternion_depuis_axe_angle(axe, angle);
}

/* Avance une pose de dt secondes a vitesses constantes.
- La position translate simplement le long de la vitesse lineaire.
- L'orientation est composee avec le quaternion increment issu de la
  vitesse angulaire, puis renormalisee pour compenser l'erreur numerique
  qui s'accumule a chaque pas. */
Pose Avancer_pose(const Pose &pose, const Vecteur_3D &vitesse_lineaire,
                   const Vecteur_3D &vitesse_angulaire, double dt) {
    Vecteur_3D deplacement = Multiplier_scalaire_3D(vitesse_lineaire, dt);
    Vecteur_3D nouvelle_position = Addition_3D(pose.position, deplacement);

    Quaternion increment = Quaternion_increment_rotation(vitesse_angulaire, dt);
    Quaternion nouvelle_orientation = Multiplication_quaterion(pose.orientation, increment);
    nouvelle_orientation = Normaliser_quaternion(nouvelle_orientation);

    return { nouvelle_position, nouvelle_orientation };
}

void Afficher_pose(const string &nom, const Pose &pose) {
    cout << nom << " :\n";
    cout << "  Position    : (" << pose.position.x << ", " << pose.position.y
         << ", " << pose.position.z << ")\n";
    cout << "  Orientation : (" << pose.orientation.x << ", " << pose.orientation.y
         << ", " << pose.orientation.z << ", " << pose.orientation.t << ")\n";
}

int main() {
    cout << fixed << setprecision(6);

    Pose pose;
    Vecteur_3D vitesse_lineaire;
    Vecteur_3D vitesse_angulaire;
    double dt;

    cout << "Position initiale (x y z) : ";
    cin >> pose.position.x >> pose.position.y >> pose.position.z;

    cout << "Orientation initiale, quaternion (x y z t) : ";
    cin >> pose.orientation.x >> pose.orientation.y >> pose.orientation.z >> pose.orientation.t;
    pose.orientation = Normaliser_quaternion(pose.orientation);

    cout << "Vitesse lineaire (x y z) : ";
    cin >> vitesse_lineaire.x >> vitesse_lineaire.y >> vitesse_lineaire.z;

    cout << "Vitesse angulaire (x y z), en rad/s : ";
    cin >> vitesse_angulaire.x >> vitesse_angulaire.y >> vitesse_angulaire.z;

    cout << "Duree dt (secondes) : ";
    cin >> dt;

    cout << "\n";
    Afficher_pose("Pose initiale", pose);

    Pose pose_extrapolee = Avancer_pose(pose, vitesse_lineaire, vitesse_angulaire, dt);

    cout << "\n";
    Afficher_pose("Pose extrapolee apres dt = " + to_string(dt) + " s", pose_extrapolee);

    return 0;
}

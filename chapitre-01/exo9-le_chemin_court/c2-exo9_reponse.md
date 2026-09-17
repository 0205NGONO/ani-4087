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

Vecteur_3D Multiplier_scalaire_3D(const Vecteur_3D &v, double s) {
    return { v.x * s, v.y * s, v.z * s };
}

double Norme_3D(const Vecteur_3D &v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Quaternion Conjugue(const Quaternion &q) {
    return { -q.x, -q.y, -q.z, q.t };
}

Quaternion Oppose_quaternion(const Quaternion &q) {
    return { -q.x, -q.y, -q.z, -q.t };
}

Quaternion Multiplication_quaterion(const Quaternion &a, const Quaternion &b) {
    return {
        a.t * b.x + a.x * b.t + a.y * b.z - a.z * b.y,
        a.t * b.y - a.x * b.z + a.y * b.t + a.z * b.x,
        a.t * b.z + a.x * b.y - a.y * b.x + a.z * b.t,
        a.t * b.t - a.x * b.x - a.y * b.y - a.z * b.z
    };
}

double Produit_scalaire_quaternion(const Quaternion &a, const Quaternion &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.t * b.t;
}

Quaternion Quaternion_depuis_axe_angle(const Vecteur_3D &axe, double angle) {
    double demi_angle = angle / 2.0;
    double s = sin(demi_angle);
    return { axe.x * s, axe.y * s, axe.z * s, cos(demi_angle) };
}

void Axe_angle_depuis_quaternion(const Quaternion &q, Vecteur_3D &axe, double &angle) {
    const double EPSILON = 1e-9;
    Vecteur_3D partie_vectorielle{ q.x, q.y, q.z };
    double norme_v = Norme_3D(partie_vectorielle);

    angle = 2.0 * atan2(norme_v, q.t);

    if (norme_v < EPSILON) {
        axe = { 0.0, 0.0, 0.0 };
    } else {
        axe = Multiplier_scalaire_3D(partie_vectorielle, 1.0 / norme_v);
    }
}

Vecteur_3D Vitesse_angulaire_moyenne_avec_forcage(const Quaternion &q1, const Quaternion &q2, double dt) {
    Quaternion q2_ajuste = q2;
    if (Produit_scalaire_quaternion(q1, q2) < 0.0) {
        q2_ajuste = Oppose_quaternion(q2);
    }
    Quaternion diff = Multiplication_quaterion(q2_ajuste, Conjugue(q1));
    Vecteur_3D axe; double angle;
    Axe_angle_depuis_quaternion(diff, axe, angle);
    return Multiplier_scalaire_3D(axe, angle / dt);
}

Vecteur_3D Vitesse_angulaire_moyenne_sans_forcage(const Quaternion &q1, const Quaternion &q2, double dt) {
    Quaternion diff = Multiplication_quaterion(q2, Conjugue(q1));
    Vecteur_3D axe; double angle;
    Axe_angle_depuis_quaternion(diff, axe, angle);
    return Multiplier_scalaire_3D(axe, angle / dt);
}

int main() {
    cout << fixed << setprecision(6);
    const double PI = acos(-1.0);

    Quaternion q1{ 0.0, 0.0, 0.0, 1.0 };

    double angle_reel = 5.0 * PI / 180.0;
    Vecteur_3D axe_z{ 0.0, 0.0, 1.0 };
    double dt = 0.1;

    Quaternion q2_correct = Quaternion_depuis_axe_angle(axe_z, angle_reel);
    Quaternion q2_absurde = Oppose_quaternion(q2_correct);

    cout << "q1         = (" << q1.x << ", " << q1.y << ", " << q1.z << ", " << q1.t << ")\n";
    cout << "q2_absurde = (" << q2_absurde.x << ", " << q2_absurde.y << ", "
         << q2_absurde.z << ", " << q2_absurde.t << ")\n\n";

    Vecteur_3D vitesse_avec = Vitesse_angulaire_moyenne_avec_forcage(q1, q2_absurde, dt);
    Vecteur_3D vitesse_sans = Vitesse_angulaire_moyenne_sans_forcage(q1, q2_absurde, dt);

    cout << "Avec forcage : (" << vitesse_avec.x << ", " << vitesse_avec.y
         << ", " << vitesse_avec.z << ") rad/s\n";
    cout << "Sans forcage : (" << vitesse_sans.x << ", " << vitesse_sans.y
         << ", " << vitesse_sans.z << ") rad/s\n";

    return 0;
}


## Résultats :
q1         = (0.000000, 0.000000, 0.000000, 1.000000)
q2_absurde = (-0.000000, -0.000000, -0.043619, -0.999048)

Avec forcage : (0.000000, 0.000000, 0.872665) rad/s
Sans forcage : (0.000000, 0.000000, -61.959188) rad/s

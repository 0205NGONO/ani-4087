#include <iostream>
#include <iomanip>
#include <cmath>
#include <array>

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

struct Matrice4x4 {
    double m[4][4];
};

Vecteur_3D Addition_3D(const Vecteur_3D &a, const Vecteur_3D &b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

Vecteur_3D Oppose_3D(const Vecteur_3D &v) {
    return { -v.x, -v.y, -v.z };
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

// Rotation par multiplication complete : v' = q * v * q_barre
Vecteur_3D Tourner(const Quaternion &q, const Vecteur_3D &v) {
    Quaternion vPur{ v.x, v.y, v.z, 0.0 };
    Quaternion resultat = Multiplication_quaterion(Multiplication_quaterion(q, vPur), Conjugue(q));
    return { resultat.x, resultat.y, resultat.z };
}


/* Construit la matrice 4x4 homogene d'une pose : les trois premieres
colonnes sont les axes locaux tournés, la quatrieme est la position,
la derniere ligne est (0, 0, 0, 1) comme toute transformation rigide.*/

Matrice4x4 Pose_vers_matrice(const Pose &pose) {
    Vecteur_3D axe_x = Tourner(pose.orientation, { 1, 0, 0 });
    Vecteur_3D axe_y = Tourner(pose.orientation, { 0, 1, 0 });
    Vecteur_3D axe_z = Tourner(pose.orientation, { 0, 0, 1 });

    Matrice4x4 mat;
    mat.m[0][0] = axe_x.x; mat.m[0][1] = axe_y.x; mat.m[0][2] = axe_z.x; mat.m[0][3] = pose.position.x;
    mat.m[1][0] = axe_x.y; mat.m[1][1] = axe_y.y; mat.m[1][2] = axe_z.y; mat.m[1][3] = pose.position.y;
    mat.m[2][0] = axe_x.z; mat.m[2][1] = axe_y.z; mat.m[2][2] = axe_z.z; mat.m[2][3] = pose.position.z;
    mat.m[3][0] = 0;       mat.m[3][1] = 0;       mat.m[3][2] = 0;       mat.m[3][3] = 1;
    return mat;
}


// VERSION 1 : inversion generale par elimination de Gauss-Jordan.
Matrice4x4 Inverser_generale(const Matrice4x4 &source) {
    const double EPSILON = 1e-9;

    double travail[4][8];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            travail[i][j] = source.m[i][j];
            travail[i][j + 4] = (i == j) ? 1.0 : 0.0;
        }
    }

    for (int colonne = 0; colonne < 4; ++colonne) {
        int ligne_pivot = colonne;
        double meilleur = fabs(travail[colonne][colonne]);
        for (int i = colonne + 1; i < 4; ++i) {
            if (fabs(travail[i][colonne]) > meilleur) {
                meilleur = fabs(travail[i][colonne]);
                ligne_pivot = i;
            }
        }

        if (meilleur < EPSILON) {
            /* S'il n'y a pas de pivot exploitable : au lieu de signaler l'echec,
            on saute cette colonne.*/
            continue;
        }

        if (ligne_pivot != colonne) {
            for (int j = 0; j < 8; ++j) swap(travail[colonne][j], travail[ligne_pivot][j]);
        }

        double pivot = travail[colonne][colonne];
        for (int j = 0; j < 8; ++j) travail[colonne][j] /= pivot;

        for (int i = 0; i < 4; ++i) {
            if (i == colonne) continue;
            double facteur = travail[i][colonne];
            for (int j = 0; j < 8; ++j) {
                travail[i][j] -= facteur * travail[colonne][j];
            }
        }
    }

    Matrice4x4 resultat;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            resultat.m[i][j] = travail[i][j + 4];
        }
    }
    return resultat;
}


// VERSION 2 : inverse analytique, ecrite a la main pour une pose.
// orientation_inverse = conjugue(orientation)
// position_inverse    = orientation_inverse appliquee à (-position)

Pose Inverser_analytique(const Pose &pose) {
    Quaternion orientation_inverse = Conjugue(pose.orientation);
    Vecteur_3D position_inverse = Tourner(orientation_inverse, Oppose_3D(pose.position));
    return { position_inverse, orientation_inverse };
}

void Afficher_matrice(const string &nom, const Matrice4x4 &mat) {
    cout << nom << " :\n";
    for (int i = 0; i < 4; ++i) {
        cout << "  ";
        for (int j = 0; j < 4; ++j) {
            cout << setw(9) << mat.m[i][j] << " ";
        }
        cout << "\n";
    }
}

void Comparer_seize_coefficients(const Matrice4x4 &a, const Matrice4x4 &b) {
    cout << "Ecart coefficient par coefficient (general - analytique) :\n";
    double plus_grand_ecart = 0.0;
    for (int i = 0; i < 4; ++i) {
        cout << "  ";
        for (int j = 0; j < 4; ++j) {
            double ecart = a.m[i][j] - b.m[i][j];
            plus_grand_ecart = max(plus_grand_ecart, fabs(ecart));
            cout << setw(9) << ecart << " ";
        }
        cout << "\n";
    }
    cout << "Plus grand ecart absolu sur les 16 coefficients : " << plus_grand_ecart << "\n";
}

int main() {
    cout << fixed << setprecision(4);

    // PARTIE 1 : une pose normale, bien formee (quaternion unitaire).

    Pose pose_normale{ { 2.0, -1.0, 3.0 }, { 0.1788, 0.3577, 0.5365, 0.7431 } };
    double norme = pose_normale.orientation.x * pose_normale.orientation.x
                 + pose_normale.orientation.y * pose_normale.orientation.y
                 + pose_normale.orientation.z * pose_normale.orientation.z
                 + pose_normale.orientation.t * pose_normale.orientation.t;
    cout << " Cas normal (quaternion unitaire, norme au carre = "
         << norme << ") \n\n";

    Matrice4x4 matrice_pose = Pose_vers_matrice(pose_normale);
    Matrice4x4 inverse_generale = Inverser_generale(matrice_pose);

    Pose pose_inverse_analytique = Inverser_analytique(pose_normale);
    Matrice4x4 inverse_analytique = Pose_vers_matrice(pose_inverse_analytique);

    Afficher_matrice("Inverse par la methode generale (Gauss-Jordan)", inverse_generale);
    cout << "\n";
    Afficher_matrice("Inverse analytique (conjugue + translation opposee)", inverse_analytique);
    cout << "\n";
    Comparer_seize_coefficients(inverse_generale, inverse_analytique);

    // PARTIE 2 : une pose degeneree -- quaternion (0,0,0,0), le cas

    cout << "\n Cas degenere (quaternion (0,0,0,0), non initialise) \n\n";

    Pose pose_degeneree{ { 5.0, 2.0, 3.0 }, { 0.0, 0.0, 0.0, 0.0 } };
    Matrice4x4 matrice_degeneree = Pose_vers_matrice(pose_degeneree);

    Afficher_matrice("Matrice de la pose degeneree (bloc de rotation nul)", matrice_degeneree);
    cout << "\n";

    Matrice4x4 inverse_de_la_degeneree = Inverser_generale(matrice_degeneree);
    Afficher_matrice("Ce que rend l'inversion GENERALE sur cette matrice", inverse_de_la_degeneree);

    cout << "La fonction generale a rendu une matrice presque identite,\n";
    cout << "silencieusement, alors que la matrice source n'avait\n";
    cout << "mathematiquement aucune inverse (bloc de rotation nul,\n";
    cout << "determinant = 0).\n";

    return 0;
}


## Comparaison et résultats
Cas normal (quaternion unitaire, norme au carre = 0.9999) 

Inverse par la methode generale (Gauss-Jordan) :
     0.1684    0.9254   -0.3398    1.6079 
    -0.6695    0.3604    0.6496   -0.2494 
     0.7235    0.1181    0.6802   -3.3695 
     0.0000    0.0000    0.0000    1.0000 

Inverse analytique (conjugue + translation opposee) :
     0.1684    0.9253   -0.3398    1.6078 
    -0.6694    0.3603    0.6495   -0.2494 
     0.7235    0.1181    0.6801   -3.3692 
     0.0000    0.0000    0.0000    1.0000 

Ecart coefficient par coefficient (general - analytique) :
     0.0000    0.0001   -0.0000    0.0002 
    -0.0001    0.0000    0.0001   -0.0000 
     0.0001    0.0000    0.0001   -0.0003 
     0.0000    0.0000    0.0000    0.0000 
Plus grand ecart absolu sur les 16 coefficients : 0.0003

 Cas degenere (quaternion (0,0,0,0), non initialise) 

Matrice de la pose degeneree (bloc de rotation nul) :
     0.0000    0.0000    0.0000    5.0000 
     0.0000    0.0000    0.0000    2.0000 
     0.0000    0.0000    0.0000    3.0000 
     0.0000    0.0000    0.0000    1.0000 

Ce que rend l'inversion GENERALE sur cette matrice :
     1.0000    0.0000    0.0000   -5.0000 
     0.0000    1.0000    0.0000   -2.0000 
     0.0000    0.0000    1.0000   -3.0000 
     0.0000    0.0000    0.0000    1.0000 

## Conclusion

La fonction generale a rendu une matrice presque identite,
silencieusement, alors que la matrice source n'avait
mathematiquement aucune inverse (bloc de rotation nul,
determinant = 0).


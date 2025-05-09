#include "Labyrinthe.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

// COULEURS POUR LE CHEMIN
const std::string VERT = "\033[32m";
const std::string ROUGE = "\033[31m";
const std::string RESET = "\033[0m";

// Constructeur par défaut
Labyrinthe::Labyrinthe() : largeur(0), hauteur(0) {}

// Constructeur avec un vecteur de chaînes
Labyrinthe::Labyrinthe(const std::vector<std::string>& grilleLabyrinthe) {
    hauteur = grilleLabyrinthe.size();
    largeur = (hauteur > 0) ? grilleLabyrinthe.size() : 0;
    
    // Convertir les chaînes en matrice 2D de caractères
    grille.resize(hauteur, std::vector<char>(largeur));
    for (size_t i = 0; i < static_cast<size_t>(hauteur); ++i) {
        for (size_t j = 0; j < static_cast<size_t>(largeur); ++j) {
            grille[i][j] = (j < grilleLabyrinthe[i].size()) ? grilleLabyrinthe[i][j] : ' ';
        }
    }
    
    trouverPositionsSpeciales();
}

// Constructeur avec une matrice 2D
Labyrinthe::Labyrinthe(const std::vector<std::vector<char>>& grilleMatrice) {
    grille = grilleMatrice;
    hauteur = grille.size();
    largeur = (hauteur > 0) ? grille[0].size() : 0;
    
    trouverPositionsSpeciales();
}

// Getters
int Labyrinthe::getLargeur() const { return largeur; }
int Labyrinthe::getHauteur() const { return hauteur; }
char Labyrinthe::getCase(int x, int y) const {
    if (x >= 0 && x < hauteur && y >= 0 && y < largeur)
        return grille[x][y];
    return '#'; // Considérer hors limites comme un mur
}

std::pair<int, int> Labyrinthe::getPositionDepart() const { return positionDepart; }
std::pair<int, int> Labyrinthe::getPositionArrivee() const { return positionArrivee; }
std::pair<int, int> Labyrinthe::getPositionEpee() const { return positionEpee; }
std::pair<int, int> Labyrinthe::getPositionBouclier() const { return positionBouclier; }
std::pair<int, int> Labyrinthe::getPositionCouronne() const { return positionCouronne; }
std::pair<int, int> Labyrinthe::getPositionPorte1() const { return positionPorte1; }
std::pair<int, int> Labyrinthe::getPositionPorte2() const { return positionPorte2; }
std::pair<int, int> Labyrinthe::getPositionTNT() const { return positionTNT; }

// Setters
void Labyrinthe::setCase(int x, int y, char valeur) {
    if (x >= 0 && x < hauteur && y >= 0 && y < largeur)
        grille[x][y] = valeur;
}

// Méthodes pour vérifier la validité d'une position
bool Labyrinthe::estPositionValide(int x, int y) const {
    return (x >= 0 && x < hauteur && y >= 0 && y < largeur && !estMur(x, y));
}

bool Labyrinthe::estMur(int x, int y) const {
    if (getCase(x,y) == '#' || getCase(x,y) == 'M') {
        return true; 
    } else {
        return false; 
    }
}

// Méthode pour trouver les positions spéciales dans le labyrinthe
void Labyrinthe::trouverPositionsSpeciales() {
    // Valeurs par défaut
    positionDepart = {-1, -1};
    positionArrivee = {-1, -1};
    positionEpee = {-1, -1};
    positionBouclier = {-1, -1};
    positionCouronne = {-1, -1};
    positionPorte1 = {-1, -1};
    positionPorte2 = {-1, -1};
    positionTNT = {-1, -1};
    
    // Parcourir les grilles pour trouver les positions spéciales
    for (int i = 0; i < hauteur; ++i) {
        for (int j = 0; j < largeur; ++j) {
            char cellule = grille[i][j];
            switch (cellule) {
                case 'D': positionDepart = {i, j}; break;
                case 'A': positionArrivee = {i, j}; break;
                case 'E': positionEpee = {i, j}; break;
                case 'B': positionBouclier = {i, j}; break;
                case 'C': positionCouronne = {i, j}; break;
                case '1': positionPorte1 = {i, j}; break;
                case '2': positionPorte2 = {i, j}; break;
                case 'T': positionTNT = {i, j}; break;
            }
        }
    }
}

// Afficher le labyrinthe
void Labyrinthe::afficher() const {
    for (int i = 0; i < hauteur; ++i) {
        for (int j = 0; j < largeur; ++j) {
            std::cout << grille[i][j];
        }
        std::cout << std::endl;
    }
}

// Afficher le labyrinthe avec le chemin trouvé
void Labyrinthe::afficherAvecChemin(const std::vector<std::pair<int, int>>& chemin, const bool pathObjectCollect) const {
    // Créer une copie temporaire de la grille
    std::vector<std::vector<char>> grilleTemp = grille;
    
    // Marquer le chemin avec des '*'
    for (const std::pair<int, int>& position : chemin) {
        int x = position.first;
        int y = position.second;
        
        // Ne pas remplacer les positions spéciales
        char cellule = grilleTemp[x][y];
        if (cellule != 'D' && cellule != 'A' && cellule != 'E' && 
            cellule != 'B' && cellule != 'C' && 
            cellule != '1' && cellule != '2' && cellule != 'T') {
            grilleTemp[x][y] = '*'; // Marquer le chemin
        }
    }
    
    // Si pathObjectCollect est vrai, afficher le chemin en rouge
    // Sinon, afficher le chemin en vert
    if (pathObjectCollect) {
        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                char c = grilleTemp[i][j];
                if (c == '*') {
                    std::cout << ROUGE << c << RESET;
                } else if (c == 'E' || c == 'B' || c == 'C') {
                    std::cout << ROUGE << c << RESET;
                } else {
                    std::cout << c; 
                }

            }
            std::cout << std::endl;
        }
    } else {
        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                char c = grilleTemp[i][j];
                if (c == '*') {
                    std::cout << VERT << c << RESET; 
                } else {
                    std::cout << c; 
                }
            }
            std::cout << std::endl;
        }
    }
    
}

// Charger plusieurs labyrinthes depuis un fichier
std::vector<Labyrinthe> Labyrinthe::loadFile(const std::string& nomFichier) {
    std::vector<Labyrinthe> labyrinthes;
    std::ifstream fichier(nomFichier);
    
    if (!fichier.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier " << nomFichier << std::endl;
        return labyrinthes;
    }
    
    std::vector<std::vector<char>> grilleTemp;
    std::string ligne;
    
    while (std::getline(fichier, ligne)) {
        if (ligne.empty() && !grilleTemp.empty()) {
            labyrinthes.push_back(Labyrinthe(grilleTemp));
            grilleTemp.clear();
            continue;
        }
        
        // Vérifier si la ligne contient une tabulation (qui sépare deux labyrinthes côte à côte)
        size_t tabPos = ligne.find('\t');
        if (tabPos != std::string::npos) {
            // Prendre seulement la première partie (avant la tabulation)
            ligne = ligne.substr(0, tabPos);
        }
        
        if (!ligne.empty()) {
            std::vector<char> ligneCar(ligne.begin(), ligne.end());
            grilleTemp.push_back(ligneCar);
        }
    }
    
    // Ajouter le dernier labyrinthe s'il reste des données
    if (!grilleTemp.empty()) {
        labyrinthes.push_back(Labyrinthe(grilleTemp));
    }
    
    fichier.close();
    
    // Maintenant, lisons le labyrinthe 2 Prime (après la tabulation)
    fichier.open(nomFichier);
    grilleTemp.clear();
    int compteurLignes = 0;
    bool auMoinsUneTabulationTrouvee = false;
    
    while (std::getline(fichier, ligne)) {
        // Ignorer les lignes jusqu'au deuxième labyrinthe
        // 20 lignes du premier labyrinthe + 1 ligne vide
        // (Oui j'ai compté)
        if (compteurLignes < 21) { 
            compteurLignes++;
            continue;
        }
        
        // Vérifier si la ligne contient une tabulation
        size_t tabPos = ligne.find('\t');
        if (tabPos != std::string::npos) {
            auMoinsUneTabulationTrouvee = true;
            // Prendre la deuxième partie (après la tabulation)
            if (tabPos + 1 < ligne.length()) {
                std::string ligneDroite = ligne.substr(tabPos + 1);
                if (!ligneDroite.empty()) {
                    std::vector<char> ligneCar(ligneDroite.begin(), ligneDroite.end());
                    grilleTemp.push_back(ligneCar);
                }
            }
        } else if (ligne.empty() && !grilleTemp.empty() && auMoinsUneTabulationTrouvee) {
            // Fin du labyrinthe 2 Prime
            break;
        }
    }
    
    // Ajouter le labyrinthe 2 Prime s'il a été trouvé
    if (!grilleTemp.empty()) {
        labyrinthes.push_back(Labyrinthe(grilleTemp));
    }
    
    fichier.close();
    
    // Afficher des informations pour le débogage
    // std::cout << "Nombre de labyrinthes chargés: " << labyrinthes.size() << std::endl;
    // for (size_t i = 0; i < labyrinthes.size(); ++i) {
    //     std::cout << "Labyrinthe " << i << ":" << std::endl;
    //     std::cout << "  Dimensions: " << labyrinthes[i].getLargeur() << "x" << labyrinthes[i].getHauteur() << std::endl;
    //     std::cout << "  Position de la porte 1: " << labyrinthes[i].getPositionPorte1().first << ", " 
    //               << labyrinthes[i].getPositionPorte1().second << std::endl;
    //     std::cout << "  Position de T: " << labyrinthes[i].getPositionTNT().first << ", " 
    //               << labyrinthes[i].getPositionTNT().second << std::endl;
    //     std::cout << "  Position de la porte 2: " << labyrinthes[i].getPositionPorte2().first << ", " 
    //               << labyrinthes[i].getPositionPorte2().second << std::endl;
    // }
    
    return labyrinthes;
}

// Backtracking séquentiel
bool Labyrinthe::backtrackingSequentiel(int x, int y, int finX, int finY, 
                                      std::vector<std::vector<bool>>& visite, 
                                      std::vector<std::pair<int, int>>& chemin) {
    // Vérifier si la position est hors limites ou est un mur
    if (!estPositionValide(x, y) || visite[x][y]) {
        return false;
    }
    
    // Ajouter la position actuelle au chemin
    chemin.push_back({x, y});
    visite[x][y] = true;
    
    // Si nous avons atteint la destination
    if (x == finX && y == finY) {
        return true;
    }
    
    // Haut, droite, bas, gauche
    const int dx[] = {-1, 0, 1, 0};
    const int dy[] = {0, 1, 0, -1};
    
    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        if (backtrackingSequentiel(nx, ny, finX, finY, visite, chemin)) {
            return true;
        }
    }
    
    // Si aucune direction ne mène à la solution, retirer cette position du chemin
    chemin.pop_back();
    return false;
}

bool Labyrinthe::trouverChemin(
    std::pair<int, int> debut, 
    std::pair<int, int> fin,                
    std::vector<std::pair<int, int>>& chemin
) {
    std::vector<std::vector<bool>> visite(hauteur, std::vector<bool>(largeur, false));
    chemin.clear();
    
    return backtrackingSequentiel(debut.first, debut.second, fin.first, fin.second, visite, chemin);
}

// Collecter les objets dans l'ordre C, B, E
bool Labyrinthe::collecterObjets(std::vector<std::pair<int, int>>& chemin) {
    std::vector<std::pair<int, int>> cheminPartiel;
    
    // On suppose qu'on part de la position d'arrivée
    std::pair<int, int> positionActuelle = positionArrivee;
    
    if (!trouverChemin(positionActuelle, positionCouronne, cheminPartiel)) {
        return false;
    }
    
    if (!cheminPartiel.empty()) {
        chemin.insert(chemin.end(), cheminPartiel.begin() + 1, cheminPartiel.end());
    }
    
    positionActuelle = positionCouronne;
    
    cheminPartiel.clear();
    if (!trouverChemin(positionActuelle, positionPorte2, cheminPartiel)) {
        return false;
    }
    
    if (!cheminPartiel.empty()) {
        chemin.insert(chemin.end(), cheminPartiel.begin() + 1, cheminPartiel.end());
    }
    
    positionActuelle = positionPorte2;
    
    cheminPartiel.clear();
    if (!trouverChemin(positionActuelle, positionBouclier, cheminPartiel)) {
        return false;
    }
    
    if (!cheminPartiel.empty()) {
        chemin.insert(chemin.end(), cheminPartiel.begin() + 1, cheminPartiel.end());
    }
    
    positionActuelle = positionBouclier;
    
    cheminPartiel.clear();
    if (!trouverChemin(positionActuelle, positionPorte1, cheminPartiel)) {
        return false;
    }
    
    if (!cheminPartiel.empty()) {
        chemin.insert(chemin.end(), cheminPartiel.begin() + 1, cheminPartiel.end());
    }
    
    // Mise à jour de la position actuelle
    positionActuelle = positionPorte1;
    
    cheminPartiel.clear();
    if (!trouverChemin(positionActuelle, positionEpee, cheminPartiel)) {
        return false;
    }
    
    if (!cheminPartiel.empty()) {
        chemin.insert(chemin.end(), cheminPartiel.begin() + 1, cheminPartiel.end());
    }
    
    // Mise à jour de la position actuelle
    positionActuelle = positionEpee;
    
    cheminPartiel.clear();
    if (!trouverChemin(positionActuelle, positionDepart, cheminPartiel)) {
        return false;
    }
    
    if (!cheminPartiel.empty()) {
        chemin.insert(chemin.end(), cheminPartiel.begin() + 1, cheminPartiel.end());
    }
    
    return true;
}

// Résoudre le premier labyrinthe (D → 1)
bool Labyrinthe::resoudreLabyrinthe1(std::vector<std::pair<int, int>>& chemin) {
    return trouverChemin(positionDepart, positionPorte1, chemin);
}

std::ostream& operator<<(std::ostream& os, const std::pair<int, int>& p) {
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

// Résoudre le deuxième labyrinthe (1 → T → 2)
bool Labyrinthe::resoudreLabyrinthe2(std::vector<std::pair<int, int>>& chemin) {
    std::vector<std::pair<int, int>> cheminPartiel1;
    std::vector<std::pair<int, int>> cheminPartiel2;
    chemin.clear();
    
    // Pour le labyrinthe 2, nous devons utiliser le même labyrinthe pour les deux chemins
    // Étape 1: De 1 à T
    if (!trouverChemin(positionPorte1, positionTNT, cheminPartiel1)) {
        std::cout << "Échec: Impossible de trouver un chemin de la porte 1 à T" << std::endl;
        return false;
    }
    
    // Ajouter le chemin partiel au chemin complet
    chemin = cheminPartiel1;
    
    std::cout << "\nChemin de la porte 1 à T:" << std::endl;
    afficherAvecChemin(cheminPartiel1, false);
    
    return true;
}

// Gérer spécifiquement le labyrinthe2Prime
bool Labyrinthe::resoudreLabyrinthe2Prime(std::vector<std::pair<int, int>>& chemin) {
    std::vector<std::pair<int, int>> cheminPartiel;
    chemin.clear();
    
    if (!trouverChemin(positionTNT, positionPorte2, cheminPartiel)) {
        std::cout << "Échec: Impossible de trouver un chemin de T à la porte 2" << std::endl;
        return false;
    }
    
    chemin = cheminPartiel;
    
    std::cout << "\nChemin de T à la porte 2:" << std::endl;
    afficherAvecChemin(cheminPartiel, false);
    
    return true;
}

// Résoudre le troisième labyrinthe (2 → A)
bool Labyrinthe::resoudreLabyrinthe3(std::vector<std::pair<int, int>>& chemin) {
    return trouverChemin(positionPorte2, positionArrivee, chemin);
}

// Collecter la couronne (A → C)
bool Labyrinthe::collecterCouronne(std::vector<std::pair<int, int>>& chemin) {
    return trouverChemin(positionArrivee, positionCouronne, chemin);
}

// Collecter le bouclier (Porte 2 → B)
bool Labyrinthe::collecterBouclier(std::vector<std::pair<int, int>>& chemin) {
    return trouverChemin(positionPorte2, positionBouclier, chemin);
}

// Collecter l'épée (Porte 1 → E)
bool Labyrinthe::collecterEpee(std::vector<std::pair<int, int>>& chemin) {
    return trouverChemin(positionPorte1, positionEpee, chemin);
}

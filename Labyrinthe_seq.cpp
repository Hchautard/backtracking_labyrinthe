#include "Labyrinthe.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

// Constructeur par défaut
Labyrinthe::Labyrinthe() : largeur(0), hauteur(0) {}

// Constructeur avec un vecteur de chaînes
Labyrinthe::Labyrinthe(const std::vector<std::string>& grilleLabyrinthe) {
    hauteur = grilleLabyrinthe.size();
    largeur = (hauteur > 0) ? grilleLabyrinthe[0].size() : 0;
    
    // Convertir les chaînes en matrice 2D de caractères
    grille.resize(hauteur, std::vector<char>(largeur));
    for (int i = 0; i < hauteur; ++i) {
        for (int j = 0; j < largeur; ++j) {
            grille[i][j] = (j < grilleLabyrinthe[i].size()) ? grilleLabyrinthe[i][j] : ' ';
        }
    }
    
    // Initialiser les positions spéciales
    trouverPositionsSpeciales();
}

// Constructeur avec une matrice 2D
Labyrinthe::Labyrinthe(const std::vector<std::vector<char>>& grilleMatrice) {
    grille = grilleMatrice;
    hauteur = grille.size();
    largeur = (hauteur > 0) ? grille[0].size() : 0;
    
    // Initialiser les positions spéciales
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
    return getCase(x, y) == '#';
}

// Méthode pour trouver les positions spéciales dans le labyrinthe
void Labyrinthe::trouverPositionsSpeciales() {
    // Valeurs par défaut (position invalide)
    positionDepart = {-1, -1};
    positionArrivee = {-1, -1};
    positionEpee = {-1, -1};
    positionBouclier = {-1, -1};
    positionCouronne = {-1, -1};
    positionPorte1 = {-1, -1};
    positionPorte2 = {-1, -1};
    positionTNT = {-1, -1};
    
    // Parcourir la grille pour trouver les positions spéciales
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

// Méthode pour afficher le labyrinthe
void Labyrinthe::afficher() const {
    for (int i = 0; i < hauteur; ++i) {
        for (int j = 0; j < largeur; ++j) {
            std::cout << grille[i][j];
        }
        std::cout << std::endl;
    }
}

// Méthode pour afficher le labyrinthe avec le chemin trouvé
void Labyrinthe::afficherAvecChemin(const std::vector<std::pair<int, int>>& chemin) const {
    // Créer une copie temporaire de la grille
    std::vector<std::vector<char>> grilleTemp = grille;
    
    // Marquer le chemin avec des '*'
    for (const auto& position : chemin) {
        int x = position.first;
        int y = position.second;
        
        // Ne pas remplacer les positions spéciales
        char cellule = grilleTemp[x][y];
        if (cellule != 'D' && cellule != 'A' && cellule != 'E' && 
            cellule != 'B' && cellule != 'C' && 
            cellule != '1' && cellule != '2') {
            grilleTemp[x][y] = '*';
        }
    }
    
    // Afficher la grille avec le chemin
    for (int i = 0; i < hauteur; ++i) {
        for (int j = 0; j < largeur; ++j) {
            std::cout << grilleTemp[i][j];
        }
        std::cout << std::endl;
    }
}

// Méthode pour charger plusieurs labyrinthes depuis un fichier
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
        // Si la ligne est vide et nous avons déjà des données, créer un nouveau labyrinthe
        if (ligne.empty() && !grilleTemp.empty()) {
            labyrinthes.push_back(Labyrinthe(grilleTemp));
            grilleTemp.clear();
            continue;
        }
        
        // Ajouter la ligne à la grille temporaire
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
    return labyrinthes;
}

// Méthode principale de backtracking séquentiel
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
    
    // Directions: haut, droite, bas, gauche
    const int dx[] = {-1, 0, 1, 0};
    const int dy[] = {0, 1, 0, -1};
    
    // Explorer les quatre directions
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

// Méthode pour trouver un chemin entre deux points
bool Labyrinthe::trouverChemin(std::pair<int, int> debut, std::pair<int, int> fin, 
                             std::vector<std::pair<int, int>>& chemin) {
    std::vector<std::vector<bool>> visite(hauteur, std::vector<bool>(largeur, false));
    chemin.clear();
    
    return backtrackingSequentiel(debut.first, debut.second, fin.first, fin.second, visite, chemin);
}

// Méthode pour résoudre le premier labyrinthe (D → E → 1)
bool Labyrinthe::resoudreLabyrinthe1(std::vector<std::pair<int, int>>& chemin) {
    std::vector<std::pair<int, int>> cheminPartiel;
    chemin.clear();
    
    // Étape 1: De D à E
    if (!trouverChemin(positionDepart, positionEpee, cheminPartiel)) {
        return false;
    }
    
    // Ajouter le chemin partiel au chemin complet
    chemin.insert(chemin.end(), cheminPartiel.begin(), cheminPartiel.end());
    
    // Étape 2: De E à 1
    cheminPartiel.clear();
    if (!trouverChemin(positionEpee, positionPorte1, cheminPartiel)) {
        return false;
    }
    
    // Ajouter le reste du chemin (sans dupliquer la position E)
    chemin.insert(chemin.end(), cheminPartiel.begin() + 1, cheminPartiel.end());
    
    return true;
}

bool Labyrinthe::resoudreLabyrinthe2(std::vector<std::pair<int, int>>& chemin) {
    std::vector<std::pair<int, int>> cheminPartiel;
    chemin.clear();
    
    // De 1 à B
    if (!trouverChemin(positionPorte1, positionBouclier, cheminPartiel)) {
        return false;
    }
    
    chemin.insert(chemin.end(), cheminPartiel.begin(), cheminPartiel.end());
    
    // De B à 2
    cheminPartiel.clear();
    if (!trouverChemin(positionBouclier, positionPorte2, cheminPartiel)) {
        return false;
    }
    
    chemin.insert(chemin.end(), cheminPartiel.begin() + 1, cheminPartiel.end());
    
    return true;
}

bool Labyrinthe::resoudreLabyrinthe3(std::vector<std::pair<int, int>>& chemin) {
    std::vector<std::pair<int, int>> cheminPartiel;
    chemin.clear();
    
    // De 2 à C
    if (!trouverChemin(positionPorte2, positionCouronne, cheminPartiel)) {
        return false;
    }
    
    chemin.insert(chemin.end(), cheminPartiel.begin(), cheminPartiel.end());
    
    // De C à A
    cheminPartiel.clear();
    if (!trouverChemin(positionCouronne, positionArrivee, cheminPartiel)) {
        return false;
    }
    
    chemin.insert(chemin.end(), cheminPartiel.begin() + 1, cheminPartiel.end());
    
    return true;
}

bool Labyrinthe::resoudreLabyrinthes(std::vector<std::pair<int, int>>& chemin) {
    std::vector<std::pair<int, int>> chemin1, chemin2, chemin3;
    
    // Résoudre le premier labyrinthe
    if (!resoudreLabyrinthe1(chemin1)) {
        std::cout << "Aucun chemin trouvé dans le premier labyrinthe!" << std::endl;
        return false;
    }
    
    // Résoudre le deuxième labyrinthe
    if (!resoudreLabyrinthe2(chemin2)) {
        std::cout << "Aucun chemin trouvé dans le deuxième labyrinthe!" << std::endl;
        return false;
    }
    
    // Résoudre le troisième labyrinthe
    if (!resoudreLabyrinthe3(chemin3)) {
        std::cout << "Aucun chemin trouvé dans le troisième labyrinthe!" << std::endl;
        return false;
    }
    
    // Assembler les chemins
    chemin.clear();
    chemin.insert(chemin.end(), chemin1.begin(), chemin1.end());
    chemin.insert(chemin.end(), chemin2.begin(), chemin2.end());
    chemin.insert(chemin.end(), chemin3.begin(), chemin3.end());
    
    return true;
}
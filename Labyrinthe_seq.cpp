#include "Labyrinthe.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

// Constructeur par défaut
Labyrinthe::Labyrinthe() : largeur(0), hauteur(0) {
    positionDepart = std::make_pair(-1, -1);
    positionArrivee = std::make_pair(-1, -1);
    positionEpee = std::make_pair(-1, -1);
    positionBouclier = std::make_pair(-1, -1);
    positionCouronne = std::make_pair(-1, -1);
    positionPorte1 = std::make_pair(-1, -1);
    positionPorte2 = std::make_pair(-1, -1);
    positionTNT = std::make_pair(-1, -1);
}

// Constructeur avec une grille
Labyrinthe::Labyrinthe(const std::vector<std::string>& grilleLabyrinthe) : grille(grilleLabyrinthe) {
    hauteur = grille.size();
    largeur = hauteur > 0 ? grille[0].size() : 0;
    trouverPositionsSpeciales();
}

// Accesseurs
int Labyrinthe::getLargeur() const {
    return largeur;
}

int Labyrinthe::getHauteur() const {
    return hauteur;
}

char Labyrinthe::getCase(int x, int y) const {
    if (estPositionValide(x, y)) {
        return grille[y][x];
    }
    return '#'; // Par défaut, on considère les cases hors grille comme des murs
}

std::pair<int, int> Labyrinthe::getPositionDepart() const {
    return positionDepart;
}

std::pair<int, int> Labyrinthe::getPositionArrivee() const {
    return positionArrivee;
}

std::pair<int, int> Labyrinthe::getPositionEpee() const {
    return positionEpee;
}

std::pair<int, int> Labyrinthe::getPositionBouclier() const {
    return positionBouclier;
}

std::pair<int, int> Labyrinthe::getPositionCouronne() const {
    return positionCouronne;
}

std::pair<int, int> Labyrinthe::getPositionPorte1() const {
    return positionPorte1;
}

std::pair<int, int> Labyrinthe::getPositionPorte2() const {
    return positionPorte2;
}

std::pair<int, int> Labyrinthe::getPositionTNT() const {
    return positionTNT;
}

// Modificateurs
void Labyrinthe::setCase(int x, int y, char valeur) {
    if (estPositionValide(x, y)) {
        grille[y][x] = valeur;
    }
}

void Labyrinthe::afficher() const {
    for (const auto& ligne : grille) {
        std::cout << ligne << std::endl;
    }
}

void Labyrinthe::afficherAvecChemin(const std::vector<std::pair<int, int>>& chemin) const {
    // Créer une copie temporaire de la grille pour afficher le chemin
    std::vector<std::string> grilleTemp = grille;
    
    // Marquer le chemin avec le caractère '*'
    for (const auto& pos : chemin) {
        int x = pos.first;
        int y = pos.second;
        if (estPositionValide(x, y)) {
            // Préserver D, E et 1 sur le chemin mais marquer les autres cases
            char c = grilleTemp[y][x];
            if (c != 'D' && c != 'E' && c != '1') {
                grilleTemp[y][x] = '*';
            }
        }
    }
    
    // Afficher la grille avec le chemin
    for (const auto& ligne : grilleTemp) {
        std::cout << ligne << std::endl;
    }
}

// Méthode statique pour charger plusieurs labyrinthes depuis un fichier
std::vector<Labyrinthe> Labyrinthe::loadFile(const std::string& nomFichier) {
    std::ifstream fichier(nomFichier);
    
    if (!fichier.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier " << nomFichier << std::endl;
        return {};
    }
    
    std::string ligne;
    std::vector<std::vector<std::string>> labyrinthesExtraits;
    std::vector<std::string> labyrintheCourant;
    
    while (std::getline(fichier, ligne)) {
        // Si la ligne est vide et nous avons déjà des lignes de labyrinthe, 
        // c'est la fin d'un labyrinthe
        if (ligne.empty() && !labyrintheCourant.empty()) {
            // Ajouter le labyrinthe courant à la liste des labyrinthes
            labyrinthesExtraits.push_back(labyrintheCourant);
            
            // Réinitialiser pour le prochain labyrinthe
            labyrintheCourant.clear();
            continue;
        }
        
        // Si la ligne n'est pas vide, l'ajouter au labyrinthe actuel
        if (!ligne.empty()) {
            labyrintheCourant.push_back(ligne);
        }
    }
    
    // Ajouter le dernier labyrinthe si nécessaire
    if (!labyrintheCourant.empty()) {
        labyrinthesExtraits.push_back(labyrintheCourant);
    }
    
    fichier.close();
    
    // Créer des objets Labyrinthe à partir des données extraites
    std::vector<Labyrinthe> labyrinthes;
    for (const auto& grilleLabyrinthe : labyrinthesExtraits) {
        labyrinthes.push_back(Labyrinthe(grilleLabyrinthe));
    }
    
    return labyrinthes;
}

// Méthodes de validation
bool Labyrinthe::estPositionValide(int x, int y) const {
    return x >= 0 && x < largeur && y >= 0 && y < hauteur;
}

bool Labyrinthe::estMur(int x, int y) const {
    return getCase(x, y) == '#';
}

// Méthode pour trouver et initialiser les positions spéciales
void Labyrinthe::trouverPositionsSpeciales() {
    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            char c = grille[y][x];
            switch (c) {
                case 'D':
                    positionDepart = std::make_pair(x, y);
                    break;
                case 'A':
                    positionArrivee = std::make_pair(x, y);
                    break;
                case 'E':
                    positionEpee = std::make_pair(x, y);
                    break;
                case 'B':
                    positionBouclier = std::make_pair(x, y);
                    break;
                case 'C':
                    positionCouronne = std::make_pair(x, y);
                    break;
                case '1':
                    positionPorte1 = std::make_pair(x, y);
                    break;
                case '2':
                    positionPorte2 = std::make_pair(x, y);
                    break;
                case 'T':
                    positionTNT = std::make_pair(x, y);
                    break;
            }
        }
    }
}

// Méthode principale de backtracking séquentiel
bool Labyrinthe::backtrackingSequentiel(int x, int y, int finX, int finY,
                                   std::vector<std::vector<bool>>& visite,
                                   std::vector<std::pair<int, int>>& chemin) {
    // Vérifier si on est hors des limites ou sur un mur ou déjà visité
    if (!estPositionValide(x, y) || estMur(x, y) || visite[y][x]) {
        return false;
    }
    
    // Marquer la case comme visitée
    visite[y][x] = true;
    
    // Si on a atteint la destination
    if (x == finX && y == finY) {
        chemin.push_back(std::make_pair(x, y));
        return true;
    }
    
    // Directions: haut, droite, bas, gauche
    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {-1, 0, 1, 0};
    
    // Explorer les quatre directions
    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        if (backtrackingSequentiel(nx, ny, finX, finY, visite, chemin)) {
            chemin.push_back(std::make_pair(x, y));
            return true;
        }
    }
    
    // Aucun chemin trouvé à partir de cette position
    return false;
}

// Méthode pour trouver un chemin entre deux points
bool Labyrinthe::trouverChemin(std::pair<int, int> debut, std::pair<int, int> fin,
                           std::vector<std::pair<int, int>>& chemin) {
    // Vérifier si les positions de début et de fin sont valides
    if (!estPositionValide(debut.first, debut.second) || !estPositionValide(fin.first, fin.second)) {
        return false;
    }
    
    // Initialiser la matrice de visite
    std::vector<std::vector<bool>> visite(hauteur, std::vector<bool>(largeur, false));
    
    // Vider le chemin actuel
    chemin.clear();
    
    // Démarrer la recherche récursive
    bool trouve = backtrackingSequentiel(debut.first, debut.second, fin.first, fin.second, visite, chemin);
    
    // Si un chemin est trouvé, l'inverser pour qu'il soit dans l'ordre du début à la fin
    if (trouve) {
        std::reverse(chemin.begin(), chemin.end());
    }
    
    return trouve;
}

// Méthode pour résoudre le labyrinthe 1 (D → E → 1)
bool Labyrinthe::resoudreLabyrinthe1(std::vector<std::pair<int, int>>& chemin) {
    // Vérifier que les positions nécessaires existent
    if (positionDepart.first == -1 || positionEpee.first == -1 || positionPorte1.first == -1) {
        std::cout << "Positions nécessaires manquantes dans le labyrinthe." << std::endl;
        return false;
    }

    // Vider le chemin actuel
    chemin.clear();
    
    // Trouver un chemin en deux étapes: D → E puis E → 1
    std::vector<std::pair<int, int>> cheminDE;
    std::vector<std::pair<int, int>> cheminE1;
    
    // Étape 1: D → E (Départ vers Épée)
    if (!trouverChemin(positionDepart, positionEpee, cheminDE)) {
        std::cout << "Aucun chemin trouvé de D à E" << std::endl;
        return false;
    }
    
    // Étape 2: E → 1 (Épée vers Porte 1)
    if (!trouverChemin(positionEpee, positionPorte1, cheminE1)) {
        std::cout << "Aucun chemin trouvé de E à 1" << std::endl;
        return false;
    }
    
    // Ajouter le premier chemin partiel au chemin complet
    chemin.insert(chemin.end(), cheminDE.begin(), cheminDE.end());
    
    // Ajouter le second chemin partiel au chemin complet (en excluant le premier point qui est déjà inclus)
    if (!cheminE1.empty()) {
        cheminE1.erase(cheminE1.begin());
    }
    chemin.insert(chemin.end(), cheminE1.begin(), cheminE1.end());
    
    return true;
}
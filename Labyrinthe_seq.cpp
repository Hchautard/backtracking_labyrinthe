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

// Méthodes pour charger et afficher le labyrinthe
bool Labyrinthe::chargerDepuisFichier(const std::string& nomFichier) {
    std::ifstream fichier(nomFichier);
    
    if (!fichier.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier " << nomFichier << std::endl;
        return false;
    }
    
    std::string ligne;
    grille.clear();
    
    while (std::getline(fichier, ligne)) {
        if (!ligne.empty()) {
            grille.push_back(ligne);
        }
    }
    
    fichier.close();
    
    // Mise à jour des dimensions
    hauteur = grille.size();
    largeur = hauteur > 0 ? grille[0].size() : 0;
    
    // Trouver les positions spéciales dans le labyrinthe
    trouverPositionsSpeciales();
    
    return true;
}

void Labyrinthe::afficher() const {
    for (const auto& ligne : grille) {
        std::cout << ligne << std::endl;
    }
}

// Méthodes de validation
bool Labyrinthe::estPositionValide(int x, int y) const {
    return x >= 0 && x < largeur && y >= 0 && y < hauteur;
}

bool Labyrinthe::estMur(int x, int y) const {
    return getCase(x, y) == '#';
}

// Méthode pour gérer l'explosion de TNT
void Labyrinthe::exploserTNT() {
    if (positionTNT.first != -1 && positionTNT.second != -1) {
        int x = positionTNT.first;
        int y = positionTNT.second;
        
        // Simuler l'explosion en supprimant les murs adjacents
        // On peut ajuster la portée de l'explosion selon les besoins
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int newX = x + i;
                int newY = y + j;
                if (estPositionValide(newX, newY) && estMur(newX, newY)) {
                    setCase(newX, newY, ' '); // Remplacer le mur par un espace vide
                }
            }
        }
    }
}

// Méthode pour gérer les transitions entre labyrinthes
std::pair<int, int> Labyrinthe::getNextStartPosition(char currentPosition) const {
    switch (currentPosition) {
        case 'D':
            return positionPorte1;
        case '1':
            return positionTNT;
        case 'T':
            return positionPorte2;
        case '2':
            return positionArrivee;
        default:
            return std::make_pair(-1, -1);
    }
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
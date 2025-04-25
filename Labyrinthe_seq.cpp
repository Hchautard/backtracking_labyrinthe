#include "Labyrinthe.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>

using namespace std;

// Couleur 
const std::string RESET = "\033[0m"; 
const std::string VERT = "\033[32m";

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

// Constructeur avec un vecteur de chaînes (pour compatibilité)
Labyrinthe::Labyrinthe(const std::vector<std::string>& grilleLabyrinthe) {
    hauteur = grilleLabyrinthe.size();
    largeur = hauteur > 0 ? grilleLabyrinthe[0].size() : 0;
    
    // Convertir le vecteur de chaînes en matrice 2D
    grille.resize(hauteur, std::vector<char>(largeur));
    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            grille[y][x] = grilleLabyrinthe[y][x];
        }
    }
    
    trouverPositionsSpeciales();
}

// Constructeur avec une matrice 2D (nouveau)
Labyrinthe::Labyrinthe(const std::vector<std::vector<char>>& grilleMatrice) : grille(grilleMatrice) {
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
    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            std::cout << grille[y][x];
        }
        std::cout << std::endl;
    }
}

void Labyrinthe::afficherAvecChemin(const std::vector<std::pair<int, int>>& chemin) const {
    // Définition des codes ANSI pour les couleurs
    const std::string RESET = "\033[0m";
    const std::string VERT = "\033[32m";
    
    // Créer un ensemble pour une recherche plus rapide des positions du chemin
    std::set<std::pair<int, int>> positionsChemin;
    for (const auto& pos : chemin) {
        positionsChemin.insert(pos);
    }
    
    // Afficher la grille avec le chemin en vert
    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            char c = grille[y][x];
            bool estSurChemin = positionsChemin.find(std::make_pair(x, y)) != positionsChemin.end();
            
            if (estSurChemin) {
                // Afficher en vert les cases du chemin, tout en conservant les caractères spéciaux
                std::cout << VERT << "*" << RESET;
            } else {
                // Afficher normalement les autres cases
                std::cout << c;
            }
        }
        std::cout << std::endl;
    }
}

// Méthode statique pour charger plusieurs labyrinthes depuis un fichier
std::vector<Labyrinthe> Labyrinthe::loadFile(const std::string& nomFichier) {
    std::ifstream fichier(nomFichier);
    
    if (!fichier.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier " << nomFichier << std::endl;
        return {};
    }
    
    std::vector<std::vector<std::string>> labyrinthesExtraits;
    
    // Lire tout le fichier
    std::vector<std::string> lignes;
    std::string ligne;
    while (std::getline(fichier, ligne)) {
        lignes.push_back(ligne);
    }
    
    // Traiter le cas spécial où deux labyrinthes sont côte à côte
    size_t i = 0;
    while (i < lignes.size()) {
        // Si ligne vide, avancer
        if (lignes[i].empty()) {
            i++;
            continue;
        }
        
        // Chercher une tabulation dans la ligne qui indiquerait deux labyrinthes côte à côte
        size_t pos = lignes[i].find('\t');
        
        if (pos != std::string::npos) {
            // Deux labyrinthes côte à côte détectés
            std::vector<std::string> labyrinthe1, labyrinthe2;
            
            // Déterminer la hauteur de ces labyrinthes
            size_t hauteur = 0;
            size_t j = i;
            while (j < lignes.size() && !lignes[j].empty()) {
                hauteur++;
                j++;
            }
            
            // Extraire les deux labyrinthes
            for (size_t k = 0; k < hauteur; k++) {
                if (i + k >= lignes.size()) break;
                
                const std::string& ligneCourante = lignes[i + k];
                pos = ligneCourante.find('\t');
                
                if (pos != std::string::npos) {
                    // Diviser la ligne en deux
                    labyrinthe1.push_back(ligneCourante.substr(0, pos));
                    labyrinthe2.push_back(ligneCourante.substr(pos + 1));
                } else {
                    // Si une ligne n'a pas de tabulation, considérer qu'elle appartient entièrement au premier labyrinthe
                    labyrinthe1.push_back(ligneCourante);
                    // Et ajouter une ligne vide au second
                    labyrinthe2.push_back(std::string(labyrinthe1.back().length(), ' '));
                }
            }
            
            // Ajouter les deux labyrinthes extraits
            labyrinthesExtraits.push_back(labyrinthe1);
            labyrinthesExtraits.push_back(labyrinthe2);
            
            // Avancer i après ces labyrinthes
            i += hauteur;
        } else {
            // Un seul labyrinthe, extraire normalement
            std::vector<std::string> labyrinthe;
            
            while (i < lignes.size() && !lignes[i].empty()) {
                labyrinthe.push_back(lignes[i]);
                i++;
            }
            
            labyrinthesExtraits.push_back(labyrinthe);
        }
    }
    
    fichier.close();
    
    // Créer des objets Labyrinthe à partir des données extraites
    std::vector<Labyrinthe> labyrinthes;
    for (const auto& grilleLabyrinthe : labyrinthesExtraits) {
        labyrinthes.push_back(Labyrinthe(grilleLabyrinthe));
    }
    
    std::cout << "Nombre de labyrinthes chargés: " << labyrinthes.size() << std::endl;
    
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

// Méthode pour résoudre le labyrinthe 1 
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

bool Labyrinthe::resoudreLabyrinthe2(std::vector<std::pair<int, int>>& chemin) {
    
    if (positionTNT.first != -1) {
        
        std::vector<std::pair<int, int>> chemin1T;
        std::vector<std::pair<int, int>> cheminTB;
        std::vector<std::pair<int, int>> cheminB2;
        
        bool cheminViaTNT = 
            trouverChemin(positionPorte1, positionTNT, chemin1T) &&
            trouverChemin(positionTNT, positionBouclier, cheminTB) &&
            trouverChemin(positionBouclier, positionPorte2, cheminB2);
        
        if (cheminViaTNT) {
            
            chemin.insert(chemin.end(), chemin1T.begin(), chemin1T.end());
            
            // Ajouter T → B
            if (!cheminTB.empty()) {
                cheminTB.erase(cheminTB.begin());
                chemin.insert(chemin.end(), cheminTB.begin(), cheminTB.end());
            }
            
            // Ajouter B → 2
            if (!cheminB2.empty()) {
                cheminB2.erase(cheminB2.begin());
                chemin.insert(chemin.end(), cheminB2.begin(), cheminB2.end());
            }
            
            return true;
        }
    }
    
    // Si on arrive ici, aucun chemin n'a été trouvé
    return false;

}

// Résoudre le labyrinthe 2 après l'explosion de la TNT
bool Labyrinthe::resoudreLabyrinthe2Prime(std::vector<std::pair<int, int>>& chemin) {
    // Cette fonction résout la deuxième partie du labyrinthe 2 
    // à partir du TNT jusqu'à la porte 2 en passant par le bouclier
    
    // Vérifier que les positions nécessaires existent
    if (positionTNT.first == -1) {
        std::cout << "Position TNT manquante dans le labyrinthe modifié." << std::endl;
        // Si la position TNT n'est pas présente dans le labyrinthe modifié, 
        // nous pouvons essayer de l'initialiser manuellement en utilisant la même
        // position que dans le labyrinthe original
        
        // Par exemple, rechercher la position dans la grille modifiée où il devrait y avoir 
        // la TNT (une zone maintenant vide où le mur était détruit)
        for (int y = 0; y < hauteur; ++y) {
            for (int x = 0; x < largeur; ++x) {
                // Chercher la zone qui pourrait représenter la position où était la TNT
                // Ce critère est arbitraire et peut nécessiter des ajustements
                if (grille[y][x] == ' ' && 
                    (x > 0 && grille[y][x-1] == ' ') && 
                    (y > 0 && grille[y-1][x] == ' ')) {
                    positionTNT = std::make_pair(x, y);
                    break;
                }
            }
            if (positionTNT.first != -1) break;
        }
        
        // Si toujours pas trouvé, essayer un emplacement spécifique dans le labyrinthe
        if (positionTNT.first == -1) {
            positionTNT = std::make_pair(18, 13); // Position probable basée sur le labyrinthe original
        }
    }
    
    if (positionBouclier.first == -1) {
        std::cout << "Position bouclier manquante dans le labyrinthe." << std::endl;
        return false;
    }
    
    if (positionPorte2.first == -1) {
        std::cout << "Position porte 2 manquante dans le labyrinthe." << std::endl;
        return false;
    }

    // Vider le chemin actuel
    chemin.clear();
    
    // Étape 1: T → B (TNT vers Bouclier)
    std::vector<std::pair<int, int>> cheminTB;
    if (!trouverChemin(positionTNT, positionBouclier, cheminTB)) {
        std::cout << "Aucun chemin trouvé de T à B" << std::endl;
        return false;
    }
    
    // Étape 2: B → 2 (Bouclier vers Porte 2)
    std::vector<std::pair<int, int>> cheminB2;
    if (!trouverChemin(positionBouclier, positionPorte2, cheminB2)) {
        std::cout << "Aucun chemin trouvé de B à 2" << std::endl;
        return false;
    }
    
    // Ajouter T → B au chemin complet
    chemin.insert(chemin.end(), cheminTB.begin(), cheminTB.end());
    
    // Ajouter B → 2 (sans le premier point qui est déjà inclus)
    if (!cheminB2.empty()) {
        cheminB2.erase(cheminB2.begin());
        chemin.insert(chemin.end(), cheminB2.begin(), cheminB2.end());
    }
    
    return true;
}

bool Labyrinthe::resoudreLabyrinthe3(std::vector<std::pair<int, int>>& chemin) {
    std::vector<std::pair<int, int>> chemin2C;
    std::vector<std::pair<int, int>> cheminCA;

    bool chemin3 = false;
    if (trouverChemin(positionPorte2, positionCouronne, chemin2C) 
        && trouverChemin(positionCouronne, positionArrivee, cheminCA)) {
        chemin3 = true;
    } else {
        chemin3 = false;
    }
    
    if (chemin3) {

        chemin.insert(chemin.end(), chemin2C.begin(), chemin2C.end());
        
        if (!cheminCA.empty()) {
            cheminCA.erase(cheminCA.begin());
            chemin.insert(chemin.end(), cheminCA.begin(), cheminCA.end());
        }
        
        return true;
    }
    
    // Si on arrive ici, aucun chemin n'a été trouvé
    return false;
}

bool Labyrinthe::resoudreLabyrinthes(std::vector<std::pair<int, int>>& chemin) {
    
    chemin.clear();
    
    // Gestion du 1er labyrinthe
    if (positionDepart.first != -1 && positionEpee.first != -1 && positionPorte1.first != -1) {
        return resoudreLabyrinthe1(chemin);
    }
    
    // Gestion du 2ème labyrinthe
    if (positionPorte1.first != -1 && positionBouclier.first != -1 && positionPorte2.first != -1) {
        return resoudreLabyrinthe2(chemin);
    }
    
    // Gestion du 3ème labyrinthe
    if (positionPorte2.first != -1 && positionCouronne.first != -1 && positionArrivee.first != -1) {
        return resoudreLabyrinthe3(chemin);
    }
    
    return false;
}
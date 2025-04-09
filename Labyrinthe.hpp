#ifndef LABYRINTHE_HPP
#define LABYRINTHE_HPP

#include <vector>
#include <string>
#include <utility> 

class Labyrinthe {
private:
    std::vector<std::vector<char>> grille; // Matrice 2D de caractères
    int largeur;
    int hauteur;
    std::pair<int, int> positionDepart; // Position D
    std::pair<int, int> positionArrivee; // Position A
    std::pair<int, int> positionEpee;    // Position E
    std::pair<int, int> positionBouclier; // Position B
    std::pair<int, int> positionCouronne; // Position C
    std::pair<int, int> positionPorte1;   // Position 1 (entrée/sortie)
    std::pair<int, int> positionPorte2;   // Position 2 (entrée/sortie)
    std::pair<int, int> positionTNT;      // Position T (TNT)
    
    // Méthode pour trouver et initialiser les positions spéciales
    void trouverPositionsSpeciales();
    
    // Méthode interne de backtracking récursif
    bool backtrackingSequentiel(int x, int y, int finX, int finY, 
                              std::vector<std::vector<bool>>& visite, 
                              std::vector<std::pair<int, int>>& chemin);

public:
    // Constructeur par défaut
    Labyrinthe();
    
    // Constructeur avec un vecteur de chaînes (pour compatibilité)
    Labyrinthe(const std::vector<std::string>& grilleLabyrinthe);
    
    // Constructeur avec une matrice 2D (nouveau)
    Labyrinthe(const std::vector<std::vector<char>>& grilleMatrice);
    
    // Getters
    int getLargeur() const;
    int getHauteur() const;
    char getCase(int x, int y) const;
    std::pair<int, int> getPositionDepart() const;
    std::pair<int, int> getPositionArrivee() const;
    std::pair<int, int> getPositionEpee() const;
    std::pair<int, int> getPositionBouclier() const;
    std::pair<int, int> getPositionCouronne() const;
    std::pair<int, int> getPositionPorte1() const;
    std::pair<int, int> getPositionPorte2() const;
    std::pair<int, int> getPositionTNT() const;
    
    // Setters
    void setCase(int x, int y, char valeur);
    
    // Méthodes pour charger et afficher le labyrinthe
    void afficher() const;
    void afficherAvecChemin(const std::vector<std::pair<int, int>>& chemin) const;
    
    // Méthode pour charger plusieurs labyrinthes depuis un fichier
    static std::vector<Labyrinthe> loadFile(const std::string& nomFichier);
    
    // Méthodes de validation
    bool estPositionValide(int x, int y) const;
    bool estMur(int x, int y) const;
    
    // Méthode principale de résolution du chemin
    bool trouverChemin(std::pair<int, int> debut, std::pair<int, int> fin, 
                     std::vector<std::pair<int, int>>& chemin);
    
    // Méthode pour résoudre le labyrinthe 1 (D → E → 1)
    bool resoudreLabyrinthe1(std::vector<std::pair<int, int>>& chemin);
    bool resoudreLabyrinthe2(std::vector<std::pair<int, int>>& chemin);
    bool resoudreLabyrinthe3(std::vector<std::pair<int, int>>& chemin);
    
    // Méthode pour résoudre un labyrinthe complet
    bool resoudreLabyrinthes(std::vector<std::pair<int, int>>& chemin);
};

#endif // LABYRINTHE_HPP
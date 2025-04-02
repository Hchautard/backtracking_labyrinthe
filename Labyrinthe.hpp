#ifndef LABYRINTHE_HPP
#define LABYRINTHE_HPP

#include <vector>
#include <string>
#include <utility> // pour std::pair

class Labyrinthe {
private:
    std::vector<std::string> grille;
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

public:
    // Constructeur par défaut
    Labyrinthe();
    
    // Constructeur avec une grille
    Labyrinthe(const std::vector<std::string>& grilleLabyrinthe);
    
    // Accesseurs
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
    
    // Modificateurs
    void setCase(int x, int y, char valeur);
    
    // Méthodes pour charger et afficher le labyrinthe
    bool chargerDepuisFichier(const std::string& nomFichier);
    void afficher() const;
    
    // Méthode pour charger plusieurs labyrinthes depuis un fichier
    static std::vector<Labyrinthe> loadFile(const std::string& nomFichier);
    
    // Méthodes de validation
    bool estPositionValide(int x, int y) const;
    bool estMur(int x, int y) const;
    
    // Méthode pour gérer l'explosion de TNT
    void exploserTNT();
    
    // Méthode pour gérer les transitions entre labyrinthes
    std::pair<int, int> getNextStartPosition(char currentPosition) const;
    
    // Méthodes pour trouver et initialiser les positions spéciales
    void trouverPositionsSpeciales();
};

#endif // LABYRINTHE_HPP
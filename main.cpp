#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Labyrinthe.hpp"

int main() {
    // Chargement des labyrinthes à partir du fichier
    std::vector<Labyrinthe> labyrinthes = Labyrinthe::loadFile("labyrinthe.txt");
    
    if (labyrinthes.empty()) {
        std::cerr << "Aucun labyrinthe chargé." << std::endl;
        return 1;
    }
    
    // Nous ne traitons que le premier labyrinthe
    if (labyrinthes.size() >= 1) {
        std::cout << "Résolution du labyrinthe 1 (D → E → 1):" << std::endl;
        
        Labyrinthe& labyrinthe = labyrinthes[0];
        
        // Afficher le labyrinthe original
        std::cout << "Labyrinthe original:" << std::endl;
        labyrinthe.afficher();
        std::cout << std::endl;
        
        // Résoudre le labyrinthe 1 en allant de D à 1 en passant par l'épée
        std::vector<std::pair<int, int>> chemin;
        bool cheminTrouve = labyrinthe.resoudreLabyrinthe1(chemin);
        
        if (cheminTrouve) {
            std::cout << "Chemin trouvé de D à 1 en passant par l'épée!" << std::endl;
            std::cout << "Longueur du chemin: " << chemin.size() << " cases" << std::endl;
            std::cout << "Affichage du labyrinthe avec le chemin (carrés verts):" << std::endl;
            labyrinthe.afficherAvecChemin(chemin);
        } else {
            std::cout << "Aucun chemin n'a pu être trouvé de D à 1 en passant par l'épée." << std::endl;
        }
    }
    
    return 0;
}
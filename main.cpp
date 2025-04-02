#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Labyrinthe.hpp"

int main() {
    // Chargement des labyrinthes à partir du fichier avec la méthode statique loadFile
    std::vector<Labyrinthe> labyrinthes = Labyrinthe::loadFile("labyrinthe.txt");
    
    // Afficher chaque labyrinthe
    for (size_t i = 0; i < labyrinthes.size(); ++i) {
        std::cout << "Labyrinthe " << (i + 1) << ":" << std::endl;
        labyrinthes[i].afficher();
        
        // Afficher les positions spéciales
        auto depart = labyrinthes[i].getPositionDepart();
        auto arrivee = labyrinthes[i].getPositionArrivee();
        auto epee = labyrinthes[i].getPositionEpee();
        auto bouclier = labyrinthes[i].getPositionBouclier();
        auto couronne = labyrinthes[i].getPositionCouronne();
        auto porte1 = labyrinthes[i].getPositionPorte1();
        auto porte2 = labyrinthes[i].getPositionPorte2();
        auto tnt = labyrinthes[i].getPositionTNT();
        
        std::cout << "Positions spéciales:" << std::endl;
        std::cout << "  Départ (D): (" << depart.first << ", " << depart.second << ")" << std::endl;
        std::cout << "  Arrivée (A): (" << arrivee.first << ", " << arrivee.second << ")" << std::endl;
        std::cout << "  Épée (E): (" << epee.first << ", " << epee.second << ")" << std::endl;
        std::cout << "  Bouclier (B): (" << bouclier.first << ", " << bouclier.second << ")" << std::endl;
        std::cout << "  Couronne (C): (" << couronne.first << ", " << couronne.second << ")" << std::endl;
        std::cout << "  Porte 1: (" << porte1.first << ", " << porte1.second << ")" << std::endl;
        std::cout << "  Porte 2: (" << porte2.first << ", " << porte2.second << ")" << std::endl;
        std::cout << "  TNT (T): (" << tnt.first << ", " << tnt.second << ")" << std::endl;
        
        std::cout << std::endl;
    }
    
    return 0;
}
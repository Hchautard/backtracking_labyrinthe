#include "Labyrinthe.hpp"
#include <iostream>
#include <vector>
#include <chrono>

int main() {
    // Charger les labyrinthes depuis le fichier
    std::vector<Labyrinthe> labyrinthes = Labyrinthe::loadFile("labyrinthe.txt");
    
    if (labyrinthes.empty()) {
        std::cerr << "Erreur: Aucun labyrinthe n'a été chargé." << std::endl;
        return 1;
    }
    
    std::cout << "Nombre de labyrinthes chargés: " << labyrinthes.size() << std::endl;
    
    // Variables pour stocker le chemin complet
    std::vector<std::pair<int, int>> chemin;
    
    // Mesurer le temps d'exécution
    auto debut = std::chrono::high_resolution_clock::now();
    
    bool succes = false;
    
    // Résoudre le premier labyrinthe (D → E → 1)
    std::cout << "Résolution du labyrinthe 1 (D → E → 1)..." << std::endl;
    succes = labyrinthes[0].resoudreLabyrinthe1(chemin);
    
    if (succes) {
        std::cout << "Chemin trouvé pour le labyrinthe 1!" << std::endl;
        std::cout << "Longueur du chemin: " << chemin.size() << " pas" << std::endl;
        labyrinthes[0].afficherAvecChemin(chemin);
    } else {
        std::cout << "Aucun chemin trouvé pour le labyrinthe 1." << std::endl;
    }
    
    // Résoudre le deuxième labyrinthe (1 → B → 2)
    std::cout << "\nRésolution du labyrinthe 2 (1 → B → 2)..." << std::endl;
    succes = labyrinthes[1].resoudreLabyrinthe2(chemin);
    
    if (succes) {
        std::cout << "Chemin trouvé pour le labyrinthe 2!" << std::endl;
        std::cout << "Longueur du chemin: " << chemin.size() << " pas" << std::endl;
        labyrinthes[1].afficherAvecChemin(chemin);
    } else {
        std::cout << "Aucun chemin trouvé pour le labyrinthe 2." << std::endl;
    }
    
    // Résoudre le troisième labyrinthe (2 → C → A)
    std::cout << "\nRésolution du labyrinthe 3 (2 → C → A)..." << std::endl;
    succes = labyrinthes[2].resoudreLabyrinthe3(chemin);
    
    if (succes) {
        std::cout << "Chemin trouvé pour le labyrinthe 3!" << std::endl;
        std::cout << "Longueur du chemin: " << chemin.size() << " pas" << std::endl;
        labyrinthes[2].afficherAvecChemin(chemin);
    } else {
        std::cout << "Aucun chemin trouvé pour le labyrinthe 3." << std::endl;
    }
    
    // Résoudre le labyrinthe complet (D → E → 1 → B → 2 → C → A)
    std::cout << "\nRésolution du labyrinthe complet..." << std::endl;
    succes = labyrinthes[0].resoudreLabyrinthes(chemin);
    
    auto fin = std::chrono::high_resolution_clock::now();
    auto duree = std::chrono::duration_cast<std::chrono::milliseconds>(fin - debut).count();
    
    if (succes) {
        std::cout << "Chemin complet trouvé!" << std::endl;
        std::cout << "Longueur du chemin: " << chemin.size() << " pas" << std::endl;
        std::cout << "Temps d'exécution: " << duree << " ms" << std::endl;
        
        // Afficher les étapes du chemin
        std::cout << "\nÉtapes du chemin:" << std::endl;
        for (size_t i = 0; i < chemin.size(); ++i) {
            std::cout << "(" << chemin[i].first << ", " << chemin[i].second << ")";
            
            // Identifier les points spéciaux du chemin
            char cellule = labyrinthes[0].getCase(chemin[i].first, chemin[i].second);
            if (cellule == 'D' || cellule == 'A' || cellule == 'E' || 
                cellule == 'B' || cellule == 'C' || 
                cellule == '1' || cellule == '2') {
                std::cout << " [" << cellule << "]";
            }
            
            std::cout << std::endl;
        }
    } else {
        std::cout << "Aucun chemin complet trouvé." << std::endl;
        std::cout << "Temps d'exécution: " << duree << " ms" << std::endl;
    }
    
    return 0;
}
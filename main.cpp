#include <iostream>
#include <vector>
#include <string>
#include "Labyrinthe.hpp"

// Fonction pour résoudre la traversée complète à travers tous les labyrinthes
bool resoudreTraverseeComplete(
    std::vector<Labyrinthe>& labyrinthes, 
    std::vector<std::vector<std::pair<int, int>>>& cheminsParLabyrinthe) {
    
    // Vérifier qu'il y a au moins les trois labyrinthes
    if (labyrinthes.size() < 3) {
        std::cout << "Il manque des labyrinthes pour la traversée complète." << std::endl;
        return false;
    }
    
    // Vider les chemins existants
    cheminsParLabyrinthe.clear();
    cheminsParLabyrinthe.resize(labyrinthes.size());
    
    // --- Étape 1: Labyrinthe 1 (D → E → 1) ---
    std::vector<std::pair<int, int>>& chemin1 = cheminsParLabyrinthe[0];
    if (!labyrinthes[0].resoudreLabyrinthe1(chemin1)) {
        std::cout << "Impossible de trouver un chemin dans le labyrinthe 1." << std::endl;
        return false;
    }
    
    std::cout << "Étape 1 réussie: Chemin trouvé de D à 1 via E (" << chemin1.size() << " cases)." << std::endl;
    
    // --- Étape 2: Labyrinthe 2 (1 → B → 2) ---
    std::vector<std::pair<int, int>>& chemin2 = cheminsParLabyrinthe[1];
    
    // Récupérer les positions des portes et objets importants
    auto porte1_lab2 = labyrinthes[1].getPositionPorte1();
    auto bouclier = labyrinthes[1].getPositionBouclier();
    auto porte2_lab2 = labyrinthes[1].getPositionPorte2();
    auto tnt = labyrinthes[1].getPositionTNT();
    
    // Vérifier que les positions essentielles existent
    if (porte1_lab2.first == -1 || bouclier.first == -1 || porte2_lab2.first == -1) {
        std::cout << "Positions nécessaires manquantes dans le labyrinthe 2." << std::endl;
        return false;
    }
    
    // Tenter d'abord le chemin avec TNT si disponible
    bool cheminTrouveLab2 = false;
    
    if (tnt.first != -1) {
        std::vector<std::pair<int, int>> chemin1T;
        std::vector<std::pair<int, int>> cheminTB;
        std::vector<std::pair<int, int>> cheminB2;
        
        if (labyrinthes[1].trouverChemin(porte1_lab2, tnt, chemin1T) &&
            labyrinthes[1].trouverChemin(tnt, bouclier, cheminTB) &&
            labyrinthes[1].trouverChemin(bouclier, porte2_lab2, cheminB2)) {
            
            // Construire le chemin complet
            chemin2 = chemin1T;
            
            // Ajouter T → B (sans le premier point)
            if (!cheminTB.empty()) {
                cheminTB.erase(cheminTB.begin());
                chemin2.insert(chemin2.end(), cheminTB.begin(), cheminTB.end());
            }
            
            // Ajouter B → 2 (sans le premier point)
            if (!cheminB2.empty()) {
                cheminB2.erase(cheminB2.begin());
                chemin2.insert(chemin2.end(), cheminB2.begin(), cheminB2.end());
            }
            
            cheminTrouveLab2 = true;
            std::cout << "Étape 2 réussie: Chemin trouvé de 1 à 2 via T et B (" << chemin2.size() << " cases)." << std::endl;
        }
    }
    
    // Si on n'a pas trouvé de chemin avec TNT, essayer le chemin direct
    if (!cheminTrouveLab2) {
        std::vector<std::pair<int, int>> chemin1B;
        std::vector<std::pair<int, int>> cheminB2;
        
        if (labyrinthes[1].trouverChemin(porte1_lab2, bouclier, chemin1B) &&
            labyrinthes[1].trouverChemin(bouclier, porte2_lab2, cheminB2)) {
            
            // Construire le chemin complet
            chemin2 = chemin1B;
            
            // Ajouter B → 2 (sans le premier point)
            if (!cheminB2.empty()) {
                cheminB2.erase(cheminB2.begin());
                chemin2.insert(chemin2.end(), cheminB2.begin(), cheminB2.end());
            }
            
            cheminTrouveLab2 = true;
            std::cout << "Étape 2 réussie: Chemin trouvé de 1 à 2 via B (" << chemin2.size() << " cases)." << std::endl;
        }
    }
    
    if (!cheminTrouveLab2) {
        std::cout << "Impossible de trouver un chemin dans le labyrinthe 2." << std::endl;
        return false;
    }
    
    // --- Étape 3: Labyrinthe 3 (2 → C → A) ---
    std::vector<std::pair<int, int>>& chemin3 = cheminsParLabyrinthe[2];
    
    // Récupérer les positions des portes et objets importants
    auto porte2_lab3 = labyrinthes[2].getPositionPorte2();
    auto couronne = labyrinthes[2].getPositionCouronne();
    auto arrivee = labyrinthes[2].getPositionArrivee();
    
    // Vérifier que les positions essentielles existent
    if (porte2_lab3.first == -1 || couronne.first == -1 || arrivee.first == -1) {
        std::cout << "Positions nécessaires manquantes dans le labyrinthe 3." << std::endl;
        return false;
    }
    
    std::vector<std::pair<int, int>> chemin2C;
    std::vector<std::pair<int, int>> cheminCA;
    
    if (!labyrinthes[2].trouverChemin(porte2_lab3, couronne, chemin2C) ||
        !labyrinthes[2].trouverChemin(couronne, arrivee, cheminCA)) {
        std::cout << "Impossible de trouver un chemin dans le labyrinthe 3." << std::endl;
        return false;
    }
    
    // Construire le chemin complet
    chemin3 = chemin2C;
    
    // Ajouter C → A (sans le premier point)
    if (!cheminCA.empty()) {
        cheminCA.erase(cheminCA.begin());
        chemin3.insert(chemin3.end(), cheminCA.begin(), cheminCA.end());
    }
    
    std::cout << "Étape 3 réussie: Chemin trouvé de 2 à A via C (" << chemin3.size() << " cases)." << std::endl;
    
    // Tout s'est bien passé!
    return true;
}

// Fonction principale qui lance l'exécution
int main() {
    // Chargement des labyrinthes à partir du fichier
    std::vector<Labyrinthe> labyrinthes = Labyrinthe::loadFile("labyrinthe.txt");
    
    if (labyrinthes.empty()) {
        std::cerr << "Aucun labyrinthe chargé." << std::endl;
        return 1;
    }
    
    std::cout << "======== Résolution des labyrinthes ========" << std::endl;
    
    // Afficher tous les labyrinthes originaux
    // for (size_t i = 0; i < labyrinthes.size(); i++) {
    //     std::cout << "Labyrinthe no." << i + 1 << ":" << std::endl;
    //     labyrinthes[i].afficher();
    //     std::cout << std::endl;
    // }
    
    // Résolution individuelle (optionnelle, pour déboguer)
    // for (size_t i = 0; i < labyrinthes.size(); i++) {
    //     std::cout << "====== Résolution individuelle - Labyrinthe " << i + 1 << " ======" << std::endl;
        
    //     std::vector<std::pair<int, int>> chemin;
    //     bool cheminTrouve = labyrinthes[i].resoudreLabyrinthes(chemin);
        
    //     if (cheminTrouve) {
    //         std::cout << "Chemin trouvé! Longueur: " << chemin.size() << " cases." << std::endl;
    //         labyrinthes[i].afficherAvecChemin(chemin);
    //         std::cout << std::endl;
    //     } else {
    //         std::cout << "Aucun chemin trouvé dans ce labyrinthe." << std::endl;
    //     }
    // }
    
    // // Résolution complète à travers tous les labyrinthes
    std::cout << "====== Résolution complète à travers tous les labyrinthes ======" << std::endl;
    
    std::vector<std::vector<std::pair<int, int>>> cheminsParLabyrinthe;
    bool traverseeReussie = resoudreTraverseeComplete(labyrinthes, cheminsParLabyrinthe);
    
    if (traverseeReussie) {
        std::cout << "\n=== Résultat final ===" << std::endl;
        std::cout << "Traversée complète réussie!" << std::endl;
        
        // Calculer la longueur totale du chemin
        int longueurTotale = 0;
        for (const auto& chemin : cheminsParLabyrinthe) {
            longueurTotale += chemin.size();
        }
        std::cout << "Longueur totale du parcours: " << longueurTotale << " cases." << std::endl;
        
        // Afficher chaque labyrinthe avec son chemin
        for (size_t i = 0; i < labyrinthes.size(); i++) {
            std::cout << "\nLabyrinthe " << i + 1 << " avec chemin:" << std::endl;
            labyrinthes[i].afficherAvecChemin(cheminsParLabyrinthe[i]);
        }
    } else {
        std::cout << "\n=== Résultat final ===" << std::endl;
        std::cout << "Impossible de réaliser la traversée complète à travers tous les labyrinthes." << std::endl;
    }
    
    return 0;
}
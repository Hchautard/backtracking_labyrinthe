#include "Labyrinthe.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

// Fonction pour tester la résolution parallèle par bifurcations
void resolutionParalleleParBifurcations(std::vector<Labyrinthe>& labyrinthes) {
    // Variables pour stocker le chemin
    std::vector<std::pair<int, int>> chemin;
    bool succes = false;
    
    std::cout << "\n--- RESOLUTION PARALLELE PAR BIFURCATIONS ---\n" << std::endl;
    
    // Mesurer le temps d'exécution
    auto debut = std::chrono::high_resolution_clock::now();
    
    // Test du labyrinthe 1 (D → 1)
    std::cout << "Test parallèle du labyrinthe 1 (D → 1)..." << std::endl;
    succes = labyrinthes[0].trouverCheminParalleleBifurcations(
        labyrinthes[0].getPositionDepart(), 
        labyrinthes[0].getPositionPorte1(), 
        chemin
    );
    
    if (succes) {
        std::cout << "Chemin trouvé pour le labyrinthe 1!" << std::endl;
        std::cout << "Longueur du chemin: " << chemin.size() << " pas" << std::endl;
        labyrinthes[0].afficherAvecChemin(chemin, false);
    } else {
        std::cout << "Aucun chemin trouvé pour le labyrinthe 1." << std::endl;
    }
    
    // Test du labyrinthe 2 (1 → T)
    std::cout << "\nTest parallèle du labyrinthe 2 (1 → T)..." << std::endl;
    chemin.clear();
    succes = labyrinthes[1].trouverCheminParalleleBifurcations(
        labyrinthes[1].getPositionPorte1(), 
        labyrinthes[1].getPositionTNT(), 
        chemin
    );
    
    if (succes) {
        std::cout << "Chemin trouvé de 1 à T dans le labyrinthe 2!" << std::endl;
        std::cout << "Longueur du chemin: " << chemin.size() << " pas" << std::endl;
        labyrinthes[1].afficherAvecChemin(chemin, false);
    } else {
        std::cout << "Aucun chemin trouvé de 1 à T dans le labyrinthe 2." << std::endl;
    }
    
    // Test du labyrinthe 2 Prime (T → 2)
    std::cout << "\nTest parallèle du labyrinthe 2 Prime (T → 2)..." << std::endl;
    chemin.clear();
    succes = labyrinthes[3].trouverCheminParalleleBifurcations(
        labyrinthes[1].getPositionTNT(), 
        labyrinthes[3].getPositionPorte2(), 
        chemin
    );
    
    if (succes) {
        std::cout << "Chemin trouvé de T à 2 dans le labyrinthe 2 Prime!" << std::endl;
        std::cout << "Longueur du chemin: " << chemin.size() << " pas" << std::endl;
        labyrinthes[3].afficherAvecChemin(chemin, false);
    } else {
        std::cout << "Aucun chemin trouvé de T à 2 dans le labyrinthe 2 Prime." << std::endl;
    }
    
    // Test du labyrinthe 3 (2 → A)
    std::cout << "\nTest parallèle du labyrinthe 3 (2 → A)..." << std::endl;
    chemin.clear();
    succes = labyrinthes[2].trouverCheminParalleleBifurcations(
        labyrinthes[2].getPositionPorte2(), 
        labyrinthes[2].getPositionArrivee(), 
        chemin
    );
    
    if (succes) {
        std::cout << "Chemin trouvé pour le labyrinthe 3!" << std::endl;
        std::cout << "Longueur du chemin: " << chemin.size() << " pas" << std::endl;
        labyrinthes[2].afficherAvecChemin(chemin, false);
    } else {
        std::cout << "Aucun chemin trouvé pour le labyrinthe 3." << std::endl;
    }
    
    // Calculer le temps total
    auto fin = std::chrono::high_resolution_clock::now();
    auto duree = std::chrono::duration_cast<std::chrono::milliseconds>(fin - debut).count();
    std::cout << "\nTemps d'exécution parallèle par bifurcations: " << duree << " ms" << std::endl;
}

// Test de collecte des objets en parallèle
void collectionDesObjetsParallele(std::vector<Labyrinthe>& labyrinthes) {
    bool succes = false;
    
    std::cout << "\n--- COLLECTE DES OBJETS EN PARALLELE ---\n" << std::endl;
    
    // Mesurer le temps d'exécution
    auto debut = std::chrono::high_resolution_clock::now();
    
    // Test de collecte de la couronne (A → C)
    std::vector<std::pair<int, int>> cheminC;
    std::cout << "- Collecte de la couronne (A → C)..." << std::endl;
    succes = labyrinthes[2].trouverCheminParalleleBifurcations(
        labyrinthes[2].getPositionArrivee(),
        labyrinthes[2].getPositionCouronne(),
        cheminC
    );

    if (succes) {
        std::cout << "Chemin trouvé pour la collecte de la couronne!" << std::endl;
        std::cout << "Longueur: " << cheminC.size() << " pas" << std::endl;
        labyrinthes[2].afficherAvecChemin(cheminC, true);
    } else {
        std::cout << "Impossible de collecter la couronne." << std::endl;
    }

    // Test du chemin C → 2
    std::vector<std::pair<int, int>> cheminC2;
    std::cout << "- Chemin de C à la porte 2..." << std::endl;
    succes = labyrinthes[2].trouverCheminParalleleBifurcations(
        labyrinthes[2].getPositionCouronne(),
        labyrinthes[2].getPositionPorte2(),
        cheminC2
    );

    if (succes) {
        std::cout << "Chemin trouvé de C à la porte 2!" << std::endl;
        std::cout << "Longueur: " << cheminC2.size() << " pas" << std::endl;
        labyrinthes[2].afficherAvecChemin(cheminC2, true);
    } else {
        std::cout << "Impossible de trouver un chemin de C à la porte 2." << std::endl;
    }

    // Test de collecte du bouclier (2 → B)
    std::vector<std::pair<int, int>> cheminB;
    std::cout << "- Collecte du bouclier (2 → B)..." << std::endl;
    succes = labyrinthes[3].trouverCheminParalleleBifurcations(
        labyrinthes[3].getPositionPorte2(),
        labyrinthes[3].getPositionBouclier(),
        cheminB
    );

    if (succes) {
        std::cout << "Chemin trouvé pour la collecte du bouclier!" << std::endl;
        std::cout << "Longueur: " << cheminB.size() << " pas" << std::endl;
        labyrinthes[3].afficherAvecChemin(cheminB, true);
    } else {
        std::cout << "Impossible de collecter le bouclier." << std::endl;
    }
    
    // Calculer le temps total
    auto fin = std::chrono::high_resolution_clock::now();
    auto duree = std::chrono::duration_cast<std::chrono::milliseconds>(fin - debut).count();
    std::cout << "\nTemps d'exécution de la collecte des objets en parallèle: " << duree << " ms" << std::endl;
}

int main() {
    // Charger les labyrinthes depuis le fichier
    std::vector<Labyrinthe> labyrinthes = Labyrinthe::loadFile("labyrinthe.txt");
    
    if (labyrinthes.empty()) {
        std::cerr << "Erreur: Aucun labyrinthe n'a été chargé." << std::endl;
        return 1;
    }
    
    std::cout << "Nombre de labyrinthes chargés: " << labyrinthes.size() << std::endl;
    std::cout << "Nombre de threads disponibles: " << std::thread::hardware_concurrency() << std::endl;
    
    // Résolution parallèle
    resolutionParalleleParBifurcations(labyrinthes);
    
    // Collecte des objets en parallèle
    // collectionDesObjetsParallele(labyrinthes);
    
    return 0;
}
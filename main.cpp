#include <iostream>
#include <vector>
#include <string>
#include "Labyrinthe.hpp"

// Fonction pour résoudre la traversée complète à travers tous les labyrinthes
// Fonction pour résoudre la traversée complète à travers tous les labyrinthes
// Fonction pour résoudre la traversée complète à travers tous les labyrinthes
bool resoudreTraverseeComplete(
    std::vector<Labyrinthe>& labyrinthes, 
    std::vector<std::vector<std::pair<int, int>>>& cheminsParLabyrinthe) {
    
    // Vérifier qu'il y a au moins les quatre labyrinthes nécessaires
    if (labyrinthes.size() < 4) {
        std::cout << "Il manque des labyrinthes pour la traversée complète. Nous avons besoin de 4 labyrinthes, mais " 
                  << labyrinthes.size() << " ont été chargés." << std::endl;
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
    
    // --- Étape 2: Labyrinthe 2 (1 → T → B → 2) avec changement d'affichage à T ---
    std::vector<std::pair<int, int>>& chemin2 = cheminsParLabyrinthe[1]; // Pour le premier segment
    
    // Récupérer les positions des portes et objets importants
    auto porte1_lab2 = labyrinthes[1].getPositionPorte1();
    auto tnt = labyrinthes[1].getPositionTNT();
    
    // Vérifier que les positions essentielles existent
    if (porte1_lab2.first == -1 || tnt.first == -1) {
        std::cout << "Positions 1 ou T manquantes dans le labyrinthe 2." << std::endl;
        return false;
    }
    
    // Première partie: de 1 à T
    if (!labyrinthes[1].trouverChemin(porte1_lab2, tnt, chemin2)) {
        std::cout << "Aucun chemin trouvé de 1 à T dans le labyrinthe 2." << std::endl;
        return false;
    }
    
    std::cout << "Chemin trouvé de 1 à T (" << chemin2.size() << " cases)." << std::endl;
    std::cout << "\nLabyrinthe 2 avant explosion (1 → T):" << std::endl;
    labyrinthes[1].afficherAvecChemin(chemin2);
    
    // Seconde partie: de T à 2 via B (dans le labyrinthe modifié)
    std::cout << "\nExplosion de la TNT! Le labyrinthe se modifie..." << std::endl;
    
    // Nous continuons maintenant avec le labyrinthe modifié (indice 3)
    // mais en commençant à la position où était T dans le labyrinthe original
    auto positionT = tnt; // Récupérer la position de T dans le labyrinthe original
    auto bouclier = labyrinthes[3].getPositionBouclier();
    auto porte2 = labyrinthes[3].getPositionPorte2();
    
    if (bouclier.first == -1 || porte2.first == -1) {
        std::cout << "Positions B ou 2 manquantes dans le labyrinthe modifié." << std::endl;
        return false;
    }
    
    // On va maintenant trouver le chemin en deux étapes dans le labyrinthe modifié:
    // 1. De la position T (qui n'est plus marquée comme T) vers B
    // 2. De B vers 2
    
    std::vector<std::pair<int, int>> cheminTB;
    std::vector<std::pair<int, int>> cheminB2;
    
    if (!labyrinthes[3].trouverChemin(positionT, bouclier, cheminTB)) {
        std::cout << "Aucun chemin trouvé de la position T à B dans le labyrinthe modifié." << std::endl;
        return false;
    }
    
    if (!labyrinthes[3].trouverChemin(bouclier, porte2, cheminB2)) {
        std::cout << "Aucun chemin trouvé de B à 2 dans le labyrinthe modifié." << std::endl;
        return false;
    }
    
    // Construire le chemin complet dans le labyrinthe modifié
    std::vector<std::pair<int, int>> cheminCompletModifie = cheminTB;
    
    // Ajouter B → 2 (sans le premier point)
    if (!cheminB2.empty()) {
        cheminB2.erase(cheminB2.begin());
        cheminCompletModifie.insert(cheminCompletModifie.end(), cheminB2.begin(), cheminB2.end());
    }
    
    // Stocker ce chemin dans le vecteur des chemins
    cheminsParLabyrinthe[3] = cheminCompletModifie;
    
    std::cout << "Chemin trouvé après explosion de T à 2 via B (" << cheminCompletModifie.size() << " cases)." << std::endl;
    std::cout << "\nLabyrinthe 2 après explosion (position T → B → 2):" << std::endl;
    labyrinthes[3].afficherAvecChemin(cheminCompletModifie);
    
    std::cout << "Étape 2 réussie: Chemin complet trouvé de 1 à 2 via T et B." << std::endl;
    
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
    
    // Étape 1: 2 → C
    std::vector<std::pair<int, int>> chemin2C;
    if (!labyrinthes[2].trouverChemin(porte2_lab3, couronne, chemin2C)) {
        std::cout << "Aucun chemin trouvé de 2 à C dans le labyrinthe 3." << std::endl;
        return false;
    }
    
    // Étape 2: C → A
    std::vector<std::pair<int, int>> cheminCA;
    if (!labyrinthes[2].trouverChemin(couronne, arrivee, cheminCA)) {
        std::cout << "Aucun chemin trouvé de C à A dans le labyrinthe 3." << std::endl;
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
    
    if (labyrinthes.size() < 4) {
        std::cerr << "Nombre insuffisant de labyrinthes chargés. Nous avons besoin de 4 labyrinthes." << std::endl;
        return 1;
    }
    
    std::cout << "======== Résolution des labyrinthes ========" << std::endl;
    
    // Résolution complète à travers tous les labyrinthes
    std::cout << "====== Résolution complète à travers tous les labyrinthes ======" << std::endl;
    
    std::vector<std::vector<std::pair<int, int>>> cheminsParLabyrinthe;
    bool traverseeReussie = resoudreTraverseeComplete(labyrinthes, cheminsParLabyrinthe);
    
    if (traverseeReussie) {
        std::cout << "\n=== Résultat final ===" << std::endl;
        std::cout << "Traversée complète réussie!" << std::endl;
        
        // Calculer la longueur totale du chemin (tous les segments)
        int longueurTotale = cheminsParLabyrinthe[0].size() + // Labyrinthe 1
                            cheminsParLabyrinthe[1].size() + // Labyrinthe 2 (avant explosion)
                            cheminsParLabyrinthe[3].size() + // Labyrinthe 2 (après explosion)
                            cheminsParLabyrinthe[2].size();  // Labyrinthe 3
                            
        std::cout << "Longueur totale du parcours: " << longueurTotale << " cases." << std::endl;
        
        // Les labyrinthes ont déjà été affichés dans la fonction resoudreTraverseeComplete
        
        // Si on souhaite afficher également le labyrinthe 3 à la fin
        std::cout << "\nLabyrinthe 3 avec chemin:" << std::endl;
        labyrinthes[2].afficherAvecChemin(cheminsParLabyrinthe[2]);
    } else {
        std::cout << "\n=== Résultat final ===" << std::endl;
        std::cout << "Impossible de réaliser la traversée complète à travers tous les labyrinthes." << std::endl;
    }
    
    return 0;
}
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
    
    // --- NOUVELLE APPROCHE AVEC ORDRE DES OBJETS SELON LE SCHÉMA A→C→2, 2→B→1, 1→E→D ---
    
    // Essayer de résoudre le chemin principal d'abord
    std::cout << "Résolution du trajet principal (D → 1 → T → 2 → A)..." << std::endl;
    succes = labyrinthes[0].resoudreTrajetPrincipal(chemin);
    
    if (succes) {
        std::cout << "Trajet principal trouvé!" << std::endl;
        std::cout << "Longueur du chemin: " << chemin.size() << " pas" << std::endl;
        
        // Créer une version fusionnée des labyrinthes pour l'affichage
        Labyrinthe labyrintheFusionne = labyrinthes[0]; // Copie du premier labyrinthe
        
        // Afficher le chemin principal
        labyrintheFusionne.afficherAvecChemin(chemin);
        
        // Ensuite collecter les objets selon le schéma A→C→2, 2→B→1, 1→E→D
        std::cout << "\nRécupération des objets selon le schéma A→C→2, 2→B→1, 1→E→D..." << std::endl;
        std::vector<std::pair<int, int>> cheminCollecte;
        
        succes = labyrinthes[0].collecterObjets(cheminCollecte);
        
        if (succes) {
            std::cout << "Chemin de collecte des objets trouvé!" << std::endl;
            std::cout << "Longueur du chemin de collecte: " << cheminCollecte.size() << " pas" << std::endl;
            
            // Combiner les chemins
            std::vector<std::pair<int, int>> cheminComplet = chemin;
            cheminComplet.insert(cheminComplet.end(), cheminCollecte.begin(), cheminCollecte.end());
            
            std::cout << "\nAffichage du chemin complet avec collecte des objets:" << std::endl;
            labyrintheFusionne.afficherAvecChemin(cheminComplet);
            
            // Mesurer le temps total d'exécution
            auto fin = std::chrono::high_resolution_clock::now();
            auto duree = std::chrono::duration_cast<std::chrono::milliseconds>(fin - debut).count();
            
            std::cout << "\nChemin complet trouvé!" << std::endl;
            std::cout << "Longueur totale du chemin: " << cheminComplet.size() << " pas" << std::endl;
            std::cout << "Temps d'exécution: " << duree << " ms" << std::endl;
            
            // Afficher les étapes clés du chemin
            std::cout << "\nPoints clés du chemin:" << std::endl;
            for (size_t i = 0; i < cheminComplet.size(); ++i) {
                char cellule = labyrintheFusionne.getCase(cheminComplet[i].first, cheminComplet[i].second);
                if (cellule == 'D' || cellule == 'A' || cellule == 'E' || 
                    cellule == 'B' || cellule == 'C' || 
                    cellule == '1' || cellule == '2' || cellule == 'T') {
                    std::cout << "Position (" << cheminComplet[i].first << ", " 
                              << cheminComplet[i].second << ") [" << cellule << "]" << std::endl;
                }
            }
        } else {
            std::cout << "Impossible de collecter les objets selon le schéma spécifié." << std::endl;
        }
    } else {
        std::cout << "Aucun trajet principal trouvé." << std::endl;
    }
    
    // --- TESTS INDIVIDUELS DES LABYRINTHES ---
    
    // Pour déboguer ou tester des parties spécifiques
    std::cout << "\n--- TESTS INDIVIDUELS DES LABYRINTHES ---\n" << std::endl;
    
    // Test du labyrinthe 1 (D → 1)
    std::cout << "Test du labyrinthe 1 (D → 1)..." << std::endl;
    succes = labyrinthes[0].resoudreLabyrinthe1(chemin);
    
    if (succes) {
        std::cout << "Chemin trouvé pour le labyrinthe 1!" << std::endl;
        std::cout << "Longueur du chemin: " << chemin.size() << " pas" << std::endl;
        labyrinthes[0].afficherAvecChemin(chemin);
    } else {
        std::cout << "Aucun chemin trouvé pour le labyrinthe 1." << std::endl;
    }
    
    // Test du labyrinthe 2 (1 → T → 2)
    std::cout << "\nTest du labyrinthe 2 (1 → T → 2)..." << std::endl;
    succes = labyrinthes[1].resoudreLabyrinthe2(chemin);
    
    if (succes) {
        std::cout << "Chemin trouvé pour le labyrinthe 2!" << std::endl;
        std::cout << "Longueur du chemin: " << chemin.size() << " pas" << std::endl;
        labyrinthes[1].afficherAvecChemin(chemin);
    } else {
        std::cout << "Aucun chemin trouvé pour le labyrinthe 2." << std::endl;
    }
    
    // Test du labyrinthe 3 (2 → A)
    std::cout << "\nTest du labyrinthe 3 (2 → A)..." << std::endl;
    succes = labyrinthes[2].resoudreLabyrinthe3(chemin);
    
    if (succes) {
        std::cout << "Chemin trouvé pour le labyrinthe 3!" << std::endl;
        std::cout << "Longueur du chemin: " << chemin.size() << " pas" << std::endl;
        labyrinthes[2].afficherAvecChemin(chemin);
    } else {
        std::cout << "Aucun chemin trouvé pour le labyrinthe 3." << std::endl;
    }
    
    // Test de collecte des objets dans l'ordre C→B→E
    std::cout << "\nTest de collecte des objets (A→C→2, 2→B→1, 1→E→D)..." << std::endl;
    
    // Test de collecte de la couronne (A → C)
    std::vector<std::pair<int, int>> cheminC;
    std::cout << "- Collecte de la couronne (A → C)..." << std::endl;
    succes = labyrinthes[2].collecterCouronne(cheminC);
    
    if (succes) {
        std::cout << "Chemin trouvé pour la collecte de la couronne!" << std::endl;
        std::cout << "Longueur: " << cheminC.size() << " pas" << std::endl;
    } else {
        std::cout << "Impossible de collecter la couronne." << std::endl;
    }
    
    // Test du chemin C → 2
    std::vector<std::pair<int, int>> cheminC2;
    std::cout << "- Chemin de C à la porte 2..." << std::endl;
    succes = labyrinthes[2].trouverChemin(labyrinthes[2].getPositionCouronne(), labyrinthes[2].getPositionPorte2(), cheminC2);
    
    if (succes) {
        std::cout << "Chemin trouvé de C à la porte 2!" << std::endl;
        std::cout << "Longueur: " << cheminC2.size() << " pas" << std::endl;
    } else {
        std::cout << "Impossible de trouver un chemin de C à la porte 2." << std::endl;
    }
    
    // Test de collecte du bouclier (2 → B)
    std::vector<std::pair<int, int>> cheminB;
    std::cout << "- Collecte du bouclier (2 → B)..." << std::endl;
    succes = labyrinthes[1].collecterBouclier(cheminB);
    
    if (succes) {
        std::cout << "Chemin trouvé pour la collecte du bouclier!" << std::endl;
        std::cout << "Longueur: " << cheminB.size() << " pas" << std::endl;
    } else {
        std::cout << "Impossible de collecter le bouclier." << std::endl;
    }
    
    // Test du chemin B → 1
    std::vector<std::pair<int, int>> cheminB1;
    std::cout << "- Chemin de B à la porte 1..." << std::endl;
    succes = labyrinthes[1].trouverChemin(labyrinthes[1].getPositionBouclier(), labyrinthes[1].getPositionPorte1(), cheminB1);
    
    if (succes) {
        std::cout << "Chemin trouvé de B à la porte 1!" << std::endl;
        std::cout << "Longueur: " << cheminB1.size() << " pas" << std::endl;
    } else {
        std::cout << "Impossible de trouver un chemin de B à la porte 1." << std::endl;
    }
    
    // Test de collecte de l'épée (1 → E)
    std::vector<std::pair<int, int>> cheminE;
    std::cout << "- Collecte de l'épée (1 → E)..." << std::endl;
    succes = labyrinthes[0].collecterEpee(cheminE);
    
    if (succes) {
        std::cout << "Chemin trouvé pour la collecte de l'épée!" << std::endl;
        std::cout << "Longueur: " << cheminE.size() << " pas" << std::endl;
    } else {
        std::cout << "Impossible de collecter l'épée." << std::endl;
    }
    
    // Test du chemin E → D
    std::vector<std::pair<int, int>> cheminED;
    std::cout << "- Chemin de E au point de départ (D)..." << std::endl;
    succes = labyrinthes[0].trouverChemin(labyrinthes[0].getPositionEpee(), labyrinthes[0].getPositionDepart(), cheminED);
    
    if (succes) {
        std::cout << "Chemin trouvé de E à D!" << std::endl;
        std::cout << "Longueur: " << cheminED.size() << " pas" << std::endl;
    } else {
        std::cout << "Impossible de trouver un chemin de E à D." << std::endl;
    }
    
    return 0;
}
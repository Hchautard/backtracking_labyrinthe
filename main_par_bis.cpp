#include "Labyrinthe.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>

// Mutex pour l'affichage synchronisé
std::mutex coutMutex;

void resoudreLabyrinthe1Thread(Labyrinthe& lab) {
    std::vector<std::pair<int, int>> chemin;
    bool succes = lab.resoudreLabyrinthe1(chemin);
    
    // Sécuriser l'affichage avec un mutex
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "\nTest du labyrinthe 1 (D → 1)..." << std::endl;
    
    if (succes) {
        std::cout << "Longueur du chemin: " << chemin.size() << " pas" << std::endl;
        lab.afficherAvecChemin(chemin, false);
    } else {
        std::cout << "Aucun chemin trouvé pour le labyrinthe 1." << std::endl;
    }
}

void resoudreLabyrinthe2Thread(Labyrinthe& lab) {
    std::vector<std::pair<int, int>> chemin;
    bool succes = lab.trouverChemin(lab.getPositionPorte1(), lab.getPositionTNT(), chemin);
    
    // Sécuriser l'affichage avec un mutex
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "\nTest du labyrinthe 2 (1 → T)..." << std::endl;
    
    if (succes) {
        std::cout << "Longueur du chemin: " << chemin.size() << " pas" << std::endl;
        lab.afficherAvecChemin(chemin, false);
    } else {
        std::cout << "Aucun chemin trouvé de 1 à T dans le labyrinthe 2." << std::endl;
    }
}

void resoudreLabyrinthe2PrimeThread(Labyrinthe& lab, const std::pair<int, int>& positionTNT) {
    std::vector<std::pair<int, int>> chemin;
    bool succes = lab.trouverChemin(positionTNT, lab.getPositionPorte2(), chemin);
    
    // Sécuriser l'affichage avec un mutex
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "\nTest du labyrinthe 2 Prime (T → 2)..." << std::endl;
    
    if (succes) {
        std::cout << "Longueur du chemin: " << chemin.size() << " pas" << std::endl;
        lab.afficherAvecChemin(chemin, false);
    } else {
        std::cout << "Aucun chemin trouvé de T à 2 dans le labyrinthe 2 Prime." << std::endl;
    }
}

void resoudreLabyrinthe3Thread(Labyrinthe& lab) {
    std::vector<std::pair<int, int>> chemin;
    bool succes = lab.resoudreLabyrinthe3(chemin);
    
    // Sécuriser l'affichage avec un mutex
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "\nTest du labyrinthe 3 (2 → A)..." << std::endl;
    
    if (succes) {
        std::cout << "Longueur du chemin: " << chemin.size() << " pas" << std::endl;
        lab.afficherAvecChemin(chemin, false);
    } else {
        std::cout << "Aucun chemin trouvé pour le labyrinthe 3." << std::endl;
    }
}

void collecterCouronneThread(Labyrinthe& lab) {
    std::vector<std::pair<int, int>> chemin;
    bool succes = lab.collecterCouronne(chemin);
    
    // Sécuriser l'affichage avec un mutex
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "- Collecte de la couronne (A → C)..." << std::endl;
    
    if (succes) {
        std::cout << "Chemin trouvé pour la collecte de la couronne!" << std::endl;
        std::cout << "Longueur: " << chemin.size() << " pas" << std::endl;
        lab.afficherAvecChemin(chemin, true);
    } else {
        std::cout << "Impossible de collecter la couronne." << std::endl;
    }
}

// Fonction pour trouver le chemin de C à la porte 2 dans un thread
void cheminC2Thread(Labyrinthe& lab) {
    std::vector<std::pair<int, int>> chemin;
    bool succes = lab.trouverChemin(lab.getPositionCouronne(), lab.getPositionPorte2(), chemin);
    
    // Sécuriser l'affichage avec un mutex
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "- Chemin de C à la porte 2..." << std::endl;
    
    if (succes) {
        std::cout << "Chemin trouvé de C à la porte 2!" << std::endl;
        std::cout << "Longueur: " << chemin.size() << " pas" << std::endl;
        lab.afficherAvecChemin(chemin, true);
    } else {
        std::cout << "Impossible de trouver un chemin de C à la porte 2." << std::endl;
    }
}

void collecterBouclierThread(Labyrinthe& lab) {
    std::vector<std::pair<int, int>> chemin;
    bool succes = lab.trouverChemin(lab.getPositionPorte2(), lab.getPositionBouclier(), chemin);
    
    // Sécuriser l'affichage avec un mutex
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "- Collecte du bouclier (2 → B)..." << std::endl;
    
    if (succes) {
        std::cout << "Chemin trouvé pour la collecte du bouclier!" << std::endl;
        std::cout << "Longueur: " << chemin.size() << " pas" << std::endl;
        lab.afficherAvecChemin(chemin, true);
    } else {
        std::cout << "Impossible de collecter le bouclier." << std::endl;
    }
}

// Fonction pour trouver le chemin de B à la porte 1 dans un thread
void cheminB1Thread(Labyrinthe& lab) {
    std::vector<std::pair<int, int>> chemin;
    bool succes = lab.trouverChemin(lab.getPositionBouclier(), lab.getPositionPorte1(), chemin);
    
    // Sécuriser l'affichage avec un mutex
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "- Chemin de B à la porte 1..." << std::endl;
    
    if (succes) {
        std::cout << "Chemin trouvé de B à la porte 1!" << std::endl;
        std::cout << "Longueur: " << chemin.size() << " pas" << std::endl;
        lab.afficherAvecChemin(chemin, true);
    } else {
        std::cout << "Impossible de trouver un chemin de B à la porte 1." << std::endl;
    }
}

// Fonction pour collecter l'épée (1 -> E) dans un thread
void collecterEpeeThread(Labyrinthe& lab) {
    std::vector<std::pair<int, int>> chemin;
    bool succes = lab.collecterEpee(chemin);
    
    // Sécuriser l'affichage avec un mutex
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "- Collecte de l'épée (1 → E)..." << std::endl;
    
    if (succes) {
        std::cout << "Chemin trouvé pour la collecte de l'épée!" << std::endl;
        std::cout << "Longueur: " << chemin.size() << " pas" << std::endl;
        lab.afficherAvecChemin(chemin, true);
    } else {
        std::cout << "Impossible de collecter l'épée." << std::endl;
    }
}

// Fonction pour résoudre le retour du labyrinthe E -> 1 dans un thread
void retourE1Thread(Labyrinthe& lab) {
    std::vector<std::pair<int, int>> chemin;
    bool succes = lab.trouverChemin(lab.getPositionEpee(), lab.getPositionPorte1(), chemin);
    
    // Sécuriser l'affichage avec un mutex
    std::lock_guard<std::mutex> lock(coutMutex);
    
    if (succes) {
        std::cout << "Chemin trouvé pour le retour de E à 1!" << std::endl;
        lab.afficherAvecChemin(chemin, false);
    } else {
        std::cout << "Impossible de trouver un chemin pour le retour de E à 1." << std::endl;
    }
}

// Fonction pour résoudre le retour du labyrinthe 1 -> 2 dans un thread
void retour12Thread(Labyrinthe& lab) {
    std::vector<std::pair<int, int>> chemin;
    bool succes = lab.trouverChemin(lab.getPositionPorte1(), lab.getPositionPorte2(), chemin);
    
    // Sécuriser l'affichage avec un mutex
    std::lock_guard<std::mutex> lock(coutMutex);
    
    if (succes) {
        std::cout << "Chemin trouvé pour le retour de 1 à 2!" << std::endl;
        lab.afficherAvecChemin(chemin, false);
    } else {
        std::cout << "Impossible de trouver un chemin pour le retour de 1 à 2." << std::endl;
    }
}

// Fonction pour résoudre le retour du labyrinthe 2 -> A dans un thread
void retour2AThread(Labyrinthe& lab) {
    std::vector<std::pair<int, int>> chemin;
    bool succes = lab.trouverChemin(lab.getPositionPorte2(), lab.getPositionArrivee(), chemin);
    
    // Sécuriser l'affichage avec un mutex
    std::lock_guard<std::mutex> lock(coutMutex);
    
    if (succes) {
        std::cout << "Chemin trouvé pour le retour de 2 à A!" << std::endl;
        lab.afficherAvecChemin(chemin, false);
    } else {
        std::cout << "Impossible de trouver un chemin pour le retour de 2 à A." << std::endl;
    }
}

// Fonction pour résoudre tous les labyrinthes en parallèle
void resolutionDesLabyrinthesParallele(std::vector<Labyrinthe>& labyrinthes) {
    std::vector<std::thread> threads;
    
    // Affichage synchronisé
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "\n--- RESOLUTION PARALLELE BIS DES LABYRINTHES ---\n" << std::endl;
    }
    
    // Créer les threads pour résoudre chaque labyrinthe en parallèle
    threads.push_back(std::thread(resoudreLabyrinthe1Thread, std::ref(labyrinthes[0])));
    threads.push_back(std::thread(resoudreLabyrinthe2Thread, std::ref(labyrinthes[1])));
    threads.push_back(std::thread(resoudreLabyrinthe2PrimeThread, std::ref(labyrinthes[3]), labyrinthes[1].getPositionTNT()));
    threads.push_back(std::thread(resoudreLabyrinthe3Thread, std::ref(labyrinthes[2])));
    
    // Attendre que tous les threads terminent
    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

// Fonction pour collecter les objets en parallèle
void collectionDesObjetsParallele(std::vector<Labyrinthe>& labyrinthes) {
    std::vector<std::thread> threads;
    
    // Affichage synchronisé
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "\nCollecte des objets (A→C→2, 2→B→1, 1→E)..." << std::endl;
    }
    
    // Créer les threads pour collecter chaque objet en parallèle
    threads.push_back(std::thread(collecterCouronneThread, std::ref(labyrinthes[2])));
    threads.push_back(std::thread(cheminC2Thread, std::ref(labyrinthes[2])));
    threads.push_back(std::thread(collecterBouclierThread, std::ref(labyrinthes[3])));
    threads.push_back(std::thread(cheminB1Thread, std::ref(labyrinthes[3])));
    threads.push_back(std::thread(collecterEpeeThread, std::ref(labyrinthes[0])));
    
    // Attendre que tous les threads terminent
    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

// Fonction pour résoudre le retour en parallèle
void resoudreRetourLabyrinthesParallele(std::vector<Labyrinthe>& labyrinthes) {
    std::vector<std::thread> threads;
    
    // Affichage synchronisé
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "\nRetour à l'arrivée (E → 1 → 2 → A)..." << std::endl;
    }
    
    // Créer les threads pour résoudre chaque partie du retour en parallèle
    threads.push_back(std::thread(retourE1Thread, std::ref(labyrinthes[0])));
    threads.push_back(std::thread(retour12Thread, std::ref(labyrinthes[3])));
    threads.push_back(std::thread(retour2AThread, std::ref(labyrinthes[2])));
    
    // Attendre que tous les threads terminent
    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

int main() {
    // Charger les labyrinthes depuis le fichier
    std::vector<Labyrinthe> labyrinthes = Labyrinthe::loadFile("labyrinthe.txt");
    
    if (labyrinthes.empty()) {
        std::cerr << "Erreur: Aucun labyrinthe n'a été chargé." << std::endl;
        return 1;
    }
    
    std::cout << "Nombre de labyrinthes chargés: " << labyrinthes.size() << std::endl;
    
    // Mesurer le temps d'exécution
    auto debut = std::chrono::high_resolution_clock::now();
    
    // Résolution des labyrinthes en parallèle
    resolutionDesLabyrinthesParallele(labyrinthes);
    
    // Collecte des objets en parallèle
    collectionDesObjetsParallele(labyrinthes);
    
    // Retour à l'arrivée en parallèle
    resoudreRetourLabyrinthesParallele(labyrinthes);
    
    auto fin = std::chrono::high_resolution_clock::now();
    auto duree = std::chrono::duration_cast<std::chrono::milliseconds>(fin - debut).count();
    
    std::cout << "Temps d'exécution parallèle: " << duree << " ms" << std::endl;
    
    return 0;
}
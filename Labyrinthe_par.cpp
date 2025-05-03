#include "Labyrinthe.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <future>
#include <atomic>
#include <vector>

// Mutex pour protéger les ressources partagées
std::mutex mtx;

// Variable atomique pour arrêter tous les threads lorsqu'une solution est trouvée
std::atomic<bool> solutionTrouvee(false);

// Version parallèle du backtracking qui lance un thread pour chaque bifurcation
bool Labyrinthe::backtrackingParalleleBifurcations(int x, int y, int finX, int finY, 
                                       std::vector<std::vector<bool>>& visite, 
                                       std::vector<std::pair<int, int>>& chemin,
                                       int profondeur) {
    // Si une solution a déjà été trouvée par un autre thread, on s'arrête
    if (solutionTrouvee.load())
        return false;

    // Vérifier si la position est hors limites ou est un mur ou déjà visitée
    if (!estPositionValide(x, y) || visite[x][y]) {
        return false;
    }
    
    // Section critique: modification des structures partagées
    {
        std::lock_guard<std::mutex> lock(mtx);
        // Ajouter la position actuelle au chemin
        chemin.push_back({x, y});
        visite[x][y] = true;
    }
    
    // Si nous avons atteint la destination
    if (x == finX && y == finY) {
        solutionTrouvee.store(true);
        return true;
    }
    
    // Directions: haut, droite, bas, gauche
    const int dx[] = {-1, 0, 1, 0};
    const int dy[] = {0, 1, 0, -1};
    
    // Si nous sommes à une profondeur suffisante, on utilise le parallélisme
    if (profondeur <= 2) { // Limiter la profondeur pour éviter trop de threads
        std::vector<std::future<bool>> futures;
        bool resultat = false;
        
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            
            // Vérifier que la position est valide avant de lancer un thread
            if (estPositionValide(nx, ny) && !visite[nx][ny]) {
                // Créer une copie locale des structures pour chaque thread
                auto futureResult = std::async(std::launch::async, [this, nx, ny, finX, finY, &visite, &chemin, profondeur]() {
                    // Copie locale des structures partagées
                    std::vector<std::vector<bool>> visiteLocale = visite;
                    std::vector<std::pair<int, int>> cheminLocal = chemin;
                    
                    // Appel récursif avec profondeur+1
                    bool res = backtrackingParalleleBifurcations(nx, ny, finX, finY, visiteLocale, cheminLocal, profondeur + 1);
                    
                    // Si ce chemin a trouvé une solution, on met à jour le chemin global
                    if (res) {
                        std::lock_guard<std::mutex> lock(mtx);
                        chemin = cheminLocal; // Mettre à jour le chemin global
                    }
                    
                    return res;
                });
                
                futures.push_back(std::move(futureResult));
            }
        }
        
        // Attendre que tous les threads terminent ou qu'une solution soit trouvée
        for (auto& f : futures) {
            if (f.get()) {
                resultat = true;
                break;
            }
        }
        
        if (!resultat) {
            // Aucune direction ne mène à la solution, retirer cette position du chemin
            std::lock_guard<std::mutex> lock(mtx);
            chemin.pop_back();
            visite[x][y] = false; // Démarquer cette cellule
        }
        
        return resultat;
    } 
    else {
        // Exécution séquentielle à grande profondeur
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            
            if (backtrackingParalleleBifurcations(nx, ny, finX, finY, visite, chemin, profondeur + 1)) {
                return true;
            }
        }
        
        // Aucune direction ne mène à la solution, retirer cette position du chemin
        std::lock_guard<std::mutex> lock(mtx);
        chemin.pop_back();
        visite[x][y] = false; // Démarquer cette cellule
        return false;
    }
}

// Méthode pour trouver un chemin entre deux points en utilisant le backtracking parallèle
bool Labyrinthe::trouverCheminParalleleBifurcations(std::pair<int, int> debut, std::pair<int, int> fin, 
                                       std::vector<std::pair<int, int>>& chemin) {
    std::vector<std::vector<bool>> visite(hauteur, std::vector<bool>(largeur, false));
    chemin.clear();
    solutionTrouvee.store(false); // Réinitialiser l'indicateur de solution
    
    // Démarrer le backtracking parallèle depuis la position de départ
    return backtrackingParalleleBifurcations(debut.first, debut.second, fin.first, fin.second, visite, chemin, 0);
}
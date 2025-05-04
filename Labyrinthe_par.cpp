#include "Labyrinthe.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <condition_variable>

std::mutex mtx;

// Pour arrêter tous les threads lorsqu'une solution est trouvée
std::atomic<bool> solutionTrouvee(false);

// Condition variable pour synchroniser les threads
std::condition_variable cv;

bool Labyrinthe::backtrackingParalleleBifurcations(
    int x, 
    int y, 
    int finX, 
    int finY, 
    std::vector<std::vector<bool>>& visite, 
    std::vector<std::pair<int, int>>& chemin
) {
    // Si une solution a déjà été trouvée par un autre thread, on s'arrête
    if (solutionTrouvee.load()) {
        return false;
    }

    // Vérifier la position
    if (!estPositionValide(x, y) || visite[x][y]) {
        return false;
    }
    
    // Section critique: modification des structures partagées
    {
        std::lock_guard<std::mutex> lock(mtx);
        chemin.push_back({x, y});
        visite[x][y] = true;
    }
    
    // Si nous avons atteint la destination
    if (x == finX && y == finY) {
        solutionTrouvee.store(true);
        // On notifie les autres threads
        cv.notify_all();
        return true;
    }
    
    // Directions: haut, droite, bas, gauche
    const int dx[] = {-1, 0, 1, 0};
    const int dy[] = {0, 1, 0, -1};
    
    std::vector<std::thread> threads;
    std::atomic<bool> resultatLocal(false);
    
    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        // Vérifier que la position est valide
        // Et qu'elle n'a pas été visitée
        if (estPositionValide(nx, ny) && !visite[nx][ny]) {
            threads.emplace_back([this, nx, ny, finX, finY, &resultatLocal, &visite, &chemin]() {

                // Copie des structures sinon risque de casser
                // si passage par référence
                std::vector<std::vector<bool>> visiteLocale = visite;
                std::vector<std::pair<int, int>> cheminLocal = chemin;
                
                if (backtrackingParalleleBifurcations(nx, ny, finX, finY, visiteLocale, cheminLocal)) {
                    // Si ce chemin a trouvé une solution, on met à jour le chemin global
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        chemin = cheminLocal; 
                    }
                    resultatLocal.store(true);
                }
            });
        }
    }
    
    // Attendre que tous les threads terminent
    for (auto& t : threads) {
        t.join();
    }
    
    // Si aucune direction n'a trouvé de solution, retirer cette position du chemin
    if (!resultatLocal.load()) {
        std::lock_guard<std::mutex> lock(mtx);
        chemin.pop_back();
        visite[x][y] = false;
        return false;
    }
    
    return true;
}

// Méthode pour trouver un chemin entre deux points en utilisant le backtracking parallèle
bool Labyrinthe::trouverCheminParalleleBifurcations(std::pair<int, int> debut, std::pair<int, int> fin, 
                                       std::vector<std::pair<int, int>>& chemin) {
    std::vector<std::vector<bool>> visite(hauteur, std::vector<bool>(largeur, false));
    chemin.clear();
    solutionTrouvee.store(false); 
    
    // Démarrer le backtracking parallèle depuis la position de départ
    return backtrackingParalleleBifurcations(debut.first, debut.second, fin.first, fin.second, visite, chemin);
}
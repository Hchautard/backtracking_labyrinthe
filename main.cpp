#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include "Labyrinthe.hpp"

using namespace std;

int main() {
    // Chargement du labyrinthe depuis un fichier
    string nomFichier = "labyrinthe.txt";
    
    try {
        Labyrinthe labyrinthe = Labyrinthe::loadFile(nomFichier);
        
        cout << "\n======== Labyrinthe chargé ========" << endl;
        labyrinthe.afficherLabyrinthe();
        
        cout << "\nRecherche d'un chemin..." << endl;
        
        // Mesurer le temps d'exécution
        auto debut = chrono::high_resolution_clock::now();
        vector<tuple<int, int, int>> chemin = labyrinthe.resolve();
        auto fin = chrono::high_resolution_clock::now();
        auto duree = chrono::duration_cast<chrono::milliseconds>(fin - debut);
        
        cout << "Temps d'exécution: " << duree.count() << " ms" << endl;
        
        if (chemin.empty()) {
            cout << "Aucun chemin valide n'a été trouvé!" << endl;
        } else {
            cout << "Chemin trouvé! Longueur: " << chemin.size() << " pas" << endl;
            labyrinthe.afficherLabyrintheAvecChemin(chemin);
            
            // Afficher le chemin détaillé
            cout << "Détail du chemin:" << endl;
            for (size_t i = 0; i < chemin.size(); ++i) {
                int x = get<0>(chemin[i]);
                int y = get<1>(chemin[i]);
                int niveau = get<2>(chemin[i]);
                char c = labyrinthe.getCase(x, y, niveau);
                
                cout << i + 1 << ": (" << x << "," << y << ") niveau " << niveau << " - " << c << endl;
            }
        }
    } catch (const exception& e) {
        cerr << "Erreur: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
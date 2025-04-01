#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <tuple>
#include <algorithm>
#include "Labyrinthe.hpp"

using namespace std;

Labyrinthe::Labyrinthe() : largeur(0), hauteur(0), nbNiveaux(0) {}

Labyrinthe::Labyrinthe(const vector<vector<string>>& _niveaux) : niveaux(_niveaux) {
    if (!niveaux.empty() && !niveaux[0].empty()) {
        nbNiveaux = niveaux.size();
        hauteur = niveaux[0].size();
        largeur = niveaux[0][0].size();
        initPositions();
    } else {
        nbNiveaux = 0;
        hauteur = 0;
        largeur = 0;
    }
}

void Labyrinthe::afficherLabyrinthe() const {
    for (int n = 0; n < nbNiveaux; ++n) {
        cout << "=== Niveau " << n + 1 << " ===" << endl;
        for (const auto& ligne : niveaux[n]) {
            cout << ligne << endl;
        }
        cout << endl;
    }
}

void Labyrinthe::afficherLabyrintheAvecChemin(const vector<tuple<int, int, int>>& chemin) const {
    // Faire une copie des niveaux
    vector<vector<string>> niveauxAvecChemin = niveaux;
    
    // Marquer le chemin
    for (const auto& pos : chemin) {
        int x = get<0>(pos);
        int y = get<1>(pos);
        int n = get<2>(pos);
        
        // Ne pas remplacer les positions spéciales
        char cellule = niveauxAvecChemin[n][y][x];
        if (cellule != 'D' && cellule != 'A' && 
            cellule != 'C' && cellule != 'B' && cellule != 'E' &&
            cellule != '1' && cellule != '2' && cellule != 'T') {
            niveauxAvecChemin[n][y][x] = '*';
        }
    }
    
    // Afficher les niveaux avec le chemin
    for (int n = 0; n < nbNiveaux; ++n) {
        cout << "=== Niveau " << n + 1 << " avec chemin ===" << endl;
        for (const auto& ligne : niveauxAvecChemin[n]) {
            cout << ligne << endl;
        }
        cout << endl;
    }
}

// Chargement du labyrinthe depuis un fichier
Labyrinthe Labyrinthe::loadFile(const string& nomFichier) {
    ifstream fichier(nomFichier);
    if (!fichier.is_open()) {
        throw runtime_error("Impossible d'ouvrir le fichier: " + nomFichier);
    }

    vector<vector<string>> niveaux;
    vector<string> niveauActuel;
    string ligne;

    while (getline(fichier, ligne)) {
        // Si la ligne est vide et que le niveau actuel n'est pas vide,
        // On considère que c'est la fin d'un niveau
        if (ligne.empty() && !niveauActuel.empty()) {
            niveaux.push_back(niveauActuel);
            niveauActuel.clear();
            continue;
        }

        // Si la ligne est vide on l'ignore
        if (ligne.empty()) {
            continue;
        }

        // Gérer le cas où il y a plusieurs labyrinthes sur la même ligne (séparés par une tabulation)
        if (ligne.find('\t') != string::npos) {
            istringstream iss(ligne);
            string partie;
            vector<string> parties;
            
            while (getline(iss, partie, '\t')) {
                parties.push_back(partie);
            }
            
            // S'il n'y a pas encore de niveaux ou si niveauActuel est vide,
            // on ajoute toutes les parties comme des niveaux séparés
            if (niveaux.empty() && niveauActuel.empty()) {
                for (const auto& p : parties) {
                    vector<string> nouveauNiveau;
                    nouveauNiveau.push_back(p);
                    niveaux.push_back(nouveauNiveau);
                }
            } else {
                // Sinon, on ajoute chaque partie au niveau correspondant
                for (size_t i = 0; i < parties.size() && i < niveaux.size(); ++i) {
                    niveaux[i].push_back(parties[i]);
                }
            }
        } else {
            niveauActuel.push_back(ligne);
        }
    }

    if (!niveauActuel.empty()) {
        niveaux.push_back(niveauActuel);
    }

    fichier.close();
    return Labyrinthe(niveaux);
}

void Labyrinthe::initPositions() {
    positionsObjets.clear();
    positionsTeleporteurs1.clear();
    positionsTeleporteurs2.clear();
    positionsDynamite.clear();
    
    for (int n = 0; n < nbNiveaux; ++n) {
        for (int y = 0; y < hauteur; ++y) {
            for (int x = 0; x < largeur; ++x) {
                char c = getCase(x, y, n);
                
                if (c == 'D') {
                    positionDepart = make_pair(x, y);
                    niveauDepart = n;
                    cout << "Position de départ détectée: (" << x << "," << y << ") au niveau " << n << endl;
                } else if (c == 'A') {
                    positionArrivee = make_pair(x, y);
                    niveauArrivee = n;
                    cout << "Position d'arrivée détectée: (" << x << "," << y << ") au niveau " << n << endl;
                } else if (c == 'C' || c == 'B' || c == 'E') {
                    positionsObjets.push_back(make_tuple(x, y, n));
                    cout << "Objet " << c << " détecté: (" << x << "," << y << ") au niveau " << n << endl;
                } else if (c == '1') {
                    positionsTeleporteurs1.push_back(make_pair(x, y));
                    cout << "Téléporteur 1 détecté: (" << x << "," << y << ") au niveau " << n << endl;
                } else if (c == '2') {
                    positionsTeleporteurs2.push_back(make_pair(x, y));
                    cout << "Téléporteur 2 détecté: (" << x << "," << y << ") au niveau " << n << endl;
                } else if (c == 'T') {
                    positionsDynamite.push_back(make_pair(x, y));
                    cout << "Dynamite détectée: (" << x << "," << y << ") au niveau " << n << endl;
                }
            }
        }
    }
}

// Vérifie si la position est valide
bool Labyrinthe::isValidPosition(int x, int y, int niveau) const {
    return niveau >= 0 && niveau < nbNiveaux && 
           y >= 0 && y < (int)niveaux[niveau].size() && 
           x >= 0 && x < (int)niveaux[niveau][y].size();
}

// Récupère le caractère à une position
char Labyrinthe::getCase(int x, int y, int niveau) const {
    if (isValidPosition(x, y, niveau)) {
        return niveaux[niveau][y][x];
    }
    return ' ';
}

// Vérifie si c'est un mur
bool Labyrinthe::isWall(int x, int y, int niveau) const {
    if (!isValidPosition(x, y, niveau)) {
        return true;
    }
    char c = getCase(x, y, niveau);
    return c == '#' || c == 'M';  // 'M' est aussi un mur
}

// Vérifie si c'est un objet
bool Labyrinthe::isObject(int x, int y, int niveau) const {
    if (!isValidPosition(x, y, niveau)) {
        return false;
    }
    char c = getCase(x, y, niveau);
    return (c == 'C' || c == 'B' || c == 'E');
}

// Obtient les voisins accessibles d'une position
vector<pair<int, int>> Labyrinthe::getVoisinsAccessibles(int x, int y, int niveau, bool murExplose) const {
    vector<pair<int, int>> voisins;
    const int dx[] = {0, 1, 0, -1};
    const int dy[] = {-1, 0, 1, 0};
    
    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        if (isValidPosition(nx, ny, niveau) && !isWall(nx, ny, niveau)) {
            // Si le mur a explosé, on peut accéder aux cases 2 même si elles sont des téléporteurs
            if (murExplose || getCase(nx, ny, niveau) != '2') {
                voisins.push_back(make_pair(nx, ny));
            }
        }
    }
    
    return voisins;
}

// Vérifie si tous les objets ont été collectés
bool Labyrinthe::tousObjetsCollectes(const vector<pair<int, int>>& objetsCollectes) const {
    if (objetsCollectes.size() != positionsObjets.size()) {
        return false;
    }
    
    for (const auto& objet : positionsObjets) {
        int x = get<0>(objet);
        int y = get<1>(objet);
        
        if (find(objetsCollectes.begin(), objetsCollectes.end(), make_pair(x, y)) == objetsCollectes.end()) {
            return false;
        }
    }
    
    return true;
}

// Vérifie si une position est dans une liste de positions
bool Labyrinthe::contientPosition(const vector<pair<int, int>>& positions, int x, int y) const {
    return find(positions.begin(), positions.end(), make_pair(x, y)) != positions.end();
}

// Algorithme de backtracking pour explorer le labyrinthe multi-niveaux
bool Labyrinthe::backtrackingMultiNiveau(
    int x, int y, int niveau,
    vector<pair<int, int>>& objetsCollectes,
    vector<tuple<int, int, int>>& chemin,
    set<tuple<int, int, int>>& visite,
    bool murExplose
) {
    // Si on a atteint l'arrivée et collecté tous les objets, on a une solution
    if (x == positionArrivee.first && y == positionArrivee.second && niveau == niveauArrivee && 
        tousObjetsCollectes(objetsCollectes)) {
        return true;
    }
    
    // Marquer la position comme visitée
    visite.insert(make_tuple(x, y, niveau));
    
    // Vérifier si on a collecté un objet
    if (isObject(x, y, niveau)) {
        pair<int, int> pos = make_pair(x, y);
        if (find(objetsCollectes.begin(), objetsCollectes.end(), pos) == objetsCollectes.end()) {
            objetsCollectes.push_back(pos);
        }
    }
    
    // Vérifier si on est sur une position de téléporteur
    char currentCase = getCase(x, y, niveau);
    
    if (currentCase == '1') {
        // Chercher tous les téléporteurs 2
        for (const auto& teleporteur : positionsTeleporteurs2) {
            int nx = teleporteur.first;
            int ny = teleporteur.second;
            
            // Vérifier si on n'a pas déjà visité cette position
            if (visite.find(make_tuple(nx, ny, niveau)) == visite.end()) {
                // Ajouter la position au chemin
                chemin.push_back(make_tuple(nx, ny, niveau));
                
                // Essayer de continuer depuis le téléporteur 2
                if (backtrackingMultiNiveau(nx, ny, niveau, objetsCollectes, chemin, visite, murExplose)) {
                    return true;
                }
                
                // Si on ne trouve pas de solution, retirer la position du chemin
                chemin.pop_back();
            }
        }
    } else if (currentCase == '2') {
        // Chercher tous les téléporteurs 1
        for (const auto& teleporteur : positionsTeleporteurs1) {
            int nx = teleporteur.first;
            int ny = teleporteur.second;
            
            // Vérifier si on n'a pas déjà visité cette position
            if (visite.find(make_tuple(nx, ny, niveau)) == visite.end()) {
                // Ajouter la position au chemin
                chemin.push_back(make_tuple(nx, ny, niveau));
                
                // Essayer de continuer depuis le téléporteur 1
                if (backtrackingMultiNiveau(nx, ny, niveau, objetsCollectes, chemin, visite, murExplose)) {
                    return true;
                }
                
                // Si on ne trouve pas de solution, retirer la position du chemin
                chemin.pop_back();
            }
        }
    }
    
    // Vérifier si on est sur une dynamite
    if (currentCase == 'T') {
        // Activer l'explosion du mur pour les prochains mouvements
        murExplose = true;
    }
    
    // Explorer les voisins accessibles
    for (const auto& voisin : getVoisinsAccessibles(x, y, niveau, murExplose)) {
        int nx = voisin.first;
        int ny = voisin.second;
        
        // Vérifier si on n'a pas déjà visité cette position
        if (visite.find(make_tuple(nx, ny, niveau)) == visite.end()) {
            // Ajouter la position au chemin
            chemin.push_back(make_tuple(nx, ny, niveau));
            
            // Essayer de continuer depuis le voisin
            if (backtrackingMultiNiveau(nx, ny, niveau, objetsCollectes, chemin, visite, murExplose)) {
                return true;
            }
            
            // Si on ne trouve pas de solution, retirer la position du chemin
            chemin.pop_back();
        }
    }
    
    // Si on a collecté un objet à cette position, le retirer lors du backtracking
    if (isObject(x, y, niveau)) {
        pair<int, int> pos = make_pair(x, y);
        auto it = find(objetsCollectes.begin(), objetsCollectes.end(), pos);
        if (it != objetsCollectes.end()) {
            objetsCollectes.erase(it);
        }
    }
    
    return false;
}

// Résolution du labyrinthe
vector<tuple<int, int, int>> Labyrinthe::resolve() {
    vector<pair<int, int>> objetsCollectes;
    vector<tuple<int, int, int>> chemin;
    set<tuple<int, int, int>> visite;
    
    // Ajouter le point de départ au chemin
    chemin.push_back(make_tuple(positionDepart.first, positionDepart.second, niveauDepart));
    
    // Lancer le backtracking
    if (backtrackingMultiNiveau(positionDepart.first, positionDepart.second, niveauDepart, 
                               objetsCollectes, chemin, visite, false)) {
        return chemin;
    }
    
    // Aucune solution trouvée
    return vector<tuple<int, int, int>>();
}
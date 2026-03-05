#include <iostream>
#include <cmath>

// Definiamo i limiti massimi (invece di usare memoria dinamica)
const int RIGHE_SET = 5;
const int COLONNE_SET = 5;
const int MAX_PASSI_CONSENTITI = 25;

// Mappa del set cinematografico (0 = Libero, 1 = Ostacolo es. Luci)
int mappaStudio[RIGHE_SET][COLONNE_SET] = {
    {0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0}, // Ostacolo al centro
    {0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0}
};

struct Coordinate {
    int x, y;
};

// VETTORE STATICO: fondamentale per la sicurezza del Firmware
Coordinate percorsoCalcolato[MAX_PASSI_CONSENTITI];
int numeroPassi = 0;

// Funzione h(n): Calcolo dell'euristica (Distanza di Manhattan)
int calcolaDistanzaManhattan(Coordinate a, Coordinate b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

// Struttura base del nostro pianificatore di percorso
void calcolaPercorso(Coordinate start, Coordinate target) {
    std::cout << "[NAVIGAZIONE] Richiesta spostamento da (" 
              << start.x << "," << start.y << ") a (" 
              << target.x << "," << target.y << ")" << std::endl;

    // Controllo di sicurezza hardware: il target è dentro un ostacolo?
    if (mappaStudio[target.x][target.y] == 1) {
        std::cout << "ERRORE CRITICO: Il target si trova su un ostacolo!" << std::endl;
        return;
    }

    // Calcoliamo h(n)
    int costoStimato = calcolaDistanzaManhattan(start, target);
    std::cout << "Stima costo energetico h(n): " << costoStimato << " unita'" << std::endl;

    // Qui in futuro implementeremo il ciclo completo di A* o Dijkstra.
    // Per ora, salviamo il punto di inizio e fine nel nostro array sicuro.
    percorsoCalcolato[0] = start;
    percorsoCalcolato[1] = target;
    numeroPassi = 2;

    std::cout << "Percorso validato e salvato nella memoria statica del controller." << std::endl;
}

int main() {
    std::cout << "--- Avvio Modulo Pathfinding ---" << std::endl;
    
    Coordinate posAttuale = {0, 0}; // Posizione iniziale della telecamera
    Coordinate posAttore = {4, 4};  // Posizione da inquadrare
    
    calcolaPercorso(posAttuale, posAttore);
    
    std::cout << "\nTest 2: Proviamo ad andare contro un ostacolo:" << std::endl;
    Coordinate posOstacolo = {1, 1};
    calcolaPercorso(posAttuale, posOstacolo);

    return 0;
}
#include <iostream>
#include <cmath>
#include <queue>
#include <vector>
#include "../include/camera_core.h"

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

// Struttura Nodo per la coda di priorità
struct Nodo {
    Coordinate pos;
    int g_cost; // Costo energetico speso finora
    int f_cost; // Stima totale f(n) = g(n) + h(n)

    // Vogliamo che la coda metta in cima il nodo col costo f_cost MINORE.
    bool operator>(const Nodo& altro) const {
        return f_cost > altro.f_cost;
    }
};

// VETTORE STATICO: fondamentale per la sicurezza del Firmware
//Coordinate percorsoCalcolato[MAX_PASSI_CONSENTITI];
//int numeroPassi = 0;

// Funzione h(n): Calcolo dell'euristica (Distanza di Manhattan)
int calcolaDistanzaManhattan(Coordinate a, Coordinate b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

// Struttura base del nostro pianificatore di percorso
void calcolaPercorso(Coordinate start, Coordinate target) {
    std::cout << "\n[A* PATHFINDING] Avvio calcolo da (" << start.x << "," << start.y 
              << ") a (" << target.x << "," << target.y << ")" << std::endl;

    // Controllo di sicurezza hardware: il target è dentro un ostacolo?
    if (mappaStudio[target.x][target.y] == 1) {
        std::cout << "ERRORE CRITICO: Il target si trova su un ostacolo!" << std::endl;
        return;
    }

    /**
    // Calcoliamo h(n)
    int costoStimato = calcolaDistanzaManhattan(start, target);
    std::cout << "Stima costo energetico h(n): " << costoStimato << " unita'" << std::endl;

    // Qui in futuro implementeremo il ciclo completo di A* o Dijkstra.
    // Per ora, salviamo il punto di inizio e fine nel nostro array sicuro.
    percorsoCalcolato[0] = start;
    percorsoCalcolato[1] = target;
    numeroPassi = 2;

    std::cout << "Percorso validato e salvato nella memoria statica del controller." << std::endl;
    **/

    // Creiamo la nostra coda di priorità (Min-Heap)
    std::priority_queue<Nodo, std::vector<Nodo>, std::greater<Nodo>> coda;
    
    // Memoria statica per tenere traccia dei percorsi migliori trovati per ogni cella
    int g_score[RIGHE_SET][COLONNE_SET];
    for(int i = 0; i < RIGHE_SET; i++) {
        for(int j = 0; j < COLONNE_SET; j++) {
            g_score[i][j] = 9999; // Inizializziamo a "Infinito"
        }
    }

    // Partenza
    g_score[start.x][start.y] = 0;
    coda.push({start, 0, calcolaDistanzaManhattan(start, target)});

    // Direzioni: Su, Giù, Sinistra, Destra
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (!coda.empty()) {
        Nodo corrente = coda.top();
        coda.pop();

        // Target raggiunto!
        if (corrente.pos.x == target.x && corrente.pos.y == target.y) {
            std::cout << "[SUCCESS] Percorso ottimale trovato! Costo totale: " 
                      << corrente.g_cost << " passi." << std::endl;
            return; // Nel firmware reale qui salveremmo il percorso in un array
        }

        // Esploriamo le 4 piastrelle adiacenti
        for (int i = 0; i < 4; i++) {
            int nx = corrente.pos.x + dx[i];
            int ny = corrente.pos.y + dy[i];

            // Verifichiamo di non uscire dalla mappa e di non sbattere contro i muri
            if (nx >= 0 && nx < RIGHE_SET && ny >= 0 && ny < COLONNE_SET && mappaStudio[nx][ny] == 0) {
                int nuovo_g_cost = corrente.g_cost + 1;

                // Se questo percorso per arrivare al vicino è il migliore trovato finora
                if (nuovo_g_cost < g_score[nx][ny]) {
                    g_score[nx][ny] = nuovo_g_cost;
                    int f_cost = nuovo_g_cost + calcolaDistanzaManhattan({nx, ny}, target);
                    
                    coda.push({{nx, ny}, nuovo_g_cost, f_cost});
                    std::cout << "  -> Esploro nodo (" << nx << "," << ny 
                              << ") | Valore f(n) = " << f_cost << std::endl;
                }
            }
        }
    }
    std::cout << "[FAILED] Nessun percorso disponibile." << std::endl;
}

/** 
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
**/
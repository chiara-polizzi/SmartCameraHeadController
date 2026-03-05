#include <iostream>
#include <fstream>
#include <string>

// Simuliamo la lettura hardware leggendo dal nostro file CSV
int ottieniPosizioneEncoder() {
    // Usiamo la keyword 'static' in modo che il file rimanga aperto
    // e ricordi a che riga era arrivato tra una chiamata e l'altra della funzione.
    static std::ifstream fileSensore("data/sensor_log.csv");
    std::string linea;

    // Se il file è aperto e riusciamo a leggere una nuova riga...
    if (fileSensore.is_open() && std::getline(fileSensore, linea)) {
        // Convertiamo la stringa di testo (es. "45") in un numero intero
        return std::stoi(linea); 
    } else {
        // Se abbiamo finito le righe del file, avvisiamo il sistema
        std::cout << "[HARDWARE WARN] Nessun nuovo dato dall'encoder." << std::endl;
        return 180; // Restituiamo una posizione di sicurezza
    }
}

/** 
int main() {
    // Salviamo il valore in modo sicuro
    int posAttuale = ottieniPosizioneEncoder();
    
    std::cout << "Controllo Motore Avviato..." << std::endl;
    std::cout << "Posizione attuale: " << posAttuale << " gradi." << std::endl;
    
    if (posAttuale > 360) {
        std::cout << "Allarme: rotazione fuori limite! Necessario reset." << std::endl;
    } else {
        std::cout << "Stato: Regolare." << std::endl;
    }
    
    return 0;
}
**/
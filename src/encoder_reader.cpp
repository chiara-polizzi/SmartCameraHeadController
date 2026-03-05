#include <iostream>

// Simula la lettura fisica da un registro hardware del motore
int read_hardware_register(int address) {
    // In un firmware reale qui useremmo i puntatori per leggere la memoria fisica.
    // Per il nostro test, facciamo finta che il motore sia a 180 gradi.
    return 180; 
}

// Funzione sicura: restituisce un intero per VALORE, non per indirizzo!
int ottieniPosizioneEncoder() {
    int posizione = read_hardware_register(0x40001000); 
    return posizione; 
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
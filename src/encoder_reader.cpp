#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

// =====================================================================
// 1. LA NOSTRA "SCHEDA ELETTRONICA" SIMULATA (Hardware Level)
// =====================================================================
// Creiamo 256 byte di memoria RAM "grezza"
uint8_t FAKE_HARDWARE_MEMORY[256] = {0};

// Definiamo l'indirizzo esatto dove il sensore scriverà i suoi dati (offset 0x10)
#define ENCODER_REGISTER_ADDRESS (&FAKE_HARDWARE_MEMORY[0x10])

// Questa funzione simula la "fisica" del mondo. Legge il CSV e carica 
// la corrente elettrica (i dati) nel nostro registro di memoria.
// In un sistema vero, questo lo fa il cavo elettrico del sensore, non il codice!
void simulaImpulsoElettricoDalSensore() {
    static std::ifstream fileSensore("data/sensor_log.csv");
    std::string linea;

    if (!fileSensore.is_open()) {
        std::cout << "[ALLARME HARDWARE] Sensore scollegato! Impossibile leggere il CSV." << std::endl;
        return;
    }

    if (fileSensore.is_open() && std::getline(fileSensore, linea)) {
        int posizioneReale = std::stoi(linea);
        
        // Prendiamo il nostro indirizzo grezzo, lo trattiamo come spazio per un intero,
        // e ci scriviamo dentro il valore fisicamente.
        int* registroFisico = (int*)ENCODER_REGISTER_ADDRESS;
        *registroFisico = posizioneReale; 
    }
}

// =====================================================================
// 2. IL TUO FIRMWARE (Software Level)
// =====================================================================
// Questa è la funzione che scriveresti per il microcontrollore ARM/STM32
int ottieniPosizioneEncoder() {
    // 1. Diciamo all'universo di far girare la rotella del sensore di un "tick"
    simulaImpulsoElettricoDalSensore();

    // 2. IL VERO CODICE FIRMWARE: Leggiamo la memoria tramite puntatori.
    // Usiamo VOLATILE per dire al compilatore: "Attento, questo valore nella RAM 
    // potrebbe essere stato cambiato dall'hardware un secondo fa!"
    volatile int* ptrSensore = (volatile int*)ENCODER_REGISTER_ADDRESS;
    
    // Dereferenziamo il puntatore per estrarre il valore intero
    return *ptrSensore; 
}

/**
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
**/

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
#include <iostream>
// Includiamo il nostro "menu" e il file del PID
#include "camera_core.h" 
#include "pid_controller.cpp" // In un progetto enorme si separa anche questo, ma per ora lo includiamo direttamente

int main() {
    std::cout << "=======================================" << std::endl;
    std::cout << " SMART CAMERA HEAD CONTROLLER - BOOT" << std::endl;
    std::cout << "=======================================\n" << std::endl;

    // 1. Inizializziamo il sistema
    Coordinate posizioneAttuale = {0, 0};
    Coordinate attoreDaInquadrare = {4, 4};
    
    // 2. Calcoliamo il percorso sicuro (Pathfinding)
    calcolaPercorso(posizioneAttuale, attoreDaInquadrare);
    
    // 3. Avviamo il Controller PID per il movimento fluido
    PIDController telecameraPID(0.5, 0.01, 0.1);
    float targetGradi = 180.0;
    float tempoDelta = 1.0;
    
    std::cout << "\n--- Inizio Movimento Fisico ---" << std::endl;
    
    // Simuliamo 5 cicli del microcontrollore (il "while(true)" del firmware)
    for(int ciclo = 1; ciclo <= 5; ciclo++) {
        // A. Leggiamo il sensore hardware
        int gradiAttuali = ottieniPosizioneEncoder(); 
        
        // B. Calcoliamo la potenza necessaria col PID
        float potenzaCalcolata = telecameraPID.calcolaPotenza(targetGradi, gradiAttuali, tempoDelta);
        
        // C. Inviamo il comando fisico al motore DC/Brushless
        // Convertiamo la potenza (float) in un valore compatibile col registro hardware (uint32_t)
        uint32_t comandoMotore = static_cast<uint32_t>(std::abs(potenzaCalcolata));
        impostaVelocitaMotore(comandoMotore);
        
        std::cout << "Ciclo " << ciclo << " completato. Motore in funzione.\n" << std::endl;
    }

    std::cout << "Spegnimento sistema." << std::endl;
    return 0;
}
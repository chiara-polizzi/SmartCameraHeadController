#include <iostream>
#include <cstdint>

// In un vero microcontrollore avremmo un indirizzo fisso, es:
// #define MOTOR_CTRL_REGISTER 0x40002000
// Per non far crashare Windows (Segmentation Fault), simuliamo 
// il registro hardware con una variabile globale:
uint32_t registro_hardware_simulato = 0;

void impostaVelocitaMotore(uint32_t velocita) {
    // 1. Creiamo un puntatore al nostro registro.
    // Usiamo VOLATILE perché è un registro hardware!
    volatile uint32_t* ptrRegistro = &registro_hardware_simulato;
    
    // 2. Scriviamo il valore desiderato DENTRO l'indirizzo di memoria (Dereferenziazione)
    *ptrRegistro = velocita;
    
    std::cout << "[FIRMWARE] Inviato comando al motore: " << velocita << " RPM" << std::endl;
}

/** 
int main() {
    std::cout << "--- Avvio Modulo Attuatori (Motori DC/Brushless) ---" << std::endl;
    
    impostaVelocitaMotore(120); // Impostiamo a 120 RPM
    impostaVelocitaMotore(250); // Acceleriamo a 250 RPM
    impostaVelocitaMotore(0);   // Stop
    
    return 0;
}
**/
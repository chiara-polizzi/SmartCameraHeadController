#include <iostream>
#include <cmath>

class PIDController {
private:
    float Kp, Ki, Kd;
    float errorePrecedente;
    float sommaIntegrale;

public:
    // Costruttore: impostiamo i "pesi" di P, I e D
    PIDController(float p, float i, float d) {
        Kp = p;
        Ki = i;
        Kd = d;
        errorePrecedente = 0;
        sommaIntegrale = 0;
    }

    // Funzione che calcola la potenza del motore
    float calcolaPotenza(float target, float posizioneAttuale, float tempoDelta) {
        // Calcolo dell'errore
        float errore = target - posizioneAttuale;

        // Proporzionale
        float P = Kp * errore;

        // Integrale
        sommaIntegrale += errore * tempoDelta;
        float I = Ki * sommaIntegrale;

        // Derivativo
        float derivata = (errore - errorePrecedente) / tempoDelta;
        float D = Kd * derivata;

        // Salviamo l'errore per il prossimo ciclo
        errorePrecedente = errore;

        // Restituiamo la somma
        return P + I + D;
    }
};

int main() {
    std::cout << "--- Avvio Simulazione Movimento Telecamera ---" << std::endl;
    
    // Inizializziamo il nostro PID con dei valori di test
    PIDController telecameraPID(0.5, 0.01, 0.1);
    
    float target = 180.0; // Vogliamo girare la telecamera a 180 gradi
    float posizione = 0.0; // Partiamo da 0
    float tempoDelta = 1.0; // Simuliamo cicli di 1 secondo
    
    // Simuliamo 15 cicli del microcontrollore
    for(int ciclo = 1; ciclo <= 15; ciclo++) {
        // 1. Calcoliamo quanta potenza dare
        float potenzaMotore = telecameraPID.calcolaPotenza(target, posizione, tempoDelta);
        
        // 2. Muoviamo fisicamente la telecamera (simulazione)
        // La posizione cambia in base alla potenza applicata
        posizione += potenzaMotore; 
        
        std::cout << "Ciclo " << ciclo << " | "
                  << "Potenza Motore: " << potenzaMotore << " | "
                  << "Posizione Telecamera: " << posizione << " gradi" << std::endl;
    }
    
    return 0;
}
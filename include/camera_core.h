#ifndef CAMERA_CORE_H
#define CAMERA_CORE_H

#include <cstdint>

// --- Dichiariamo le funzioni di encoder_reader.cpp ---
int ottieniPosizioneEncoder();

// --- Dichiariamo le funzioni di motor_driver.cpp ---
void impostaVelocitaMotore(uint32_t velocita);

// --- Dichiariamo le strutture di pathfinder.cpp ---
struct Coordinate {
    int x, y;
};
void calcolaPercorso(Coordinate start, Coordinate target);

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

#endif
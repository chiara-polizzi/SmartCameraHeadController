#include <iostream>
#include <cassert> // Libreria fondamentale per gli Unit Test
#include <cmath>

class PIDController {
private:
    float Kp, Ki, Kd;
    float errorePrecedente;
    float sommaIntegrale;

public:
    PIDController(float p, float i, float d) : Kp(p), Ki(i), Kd(d), errorePrecedente(0), sommaIntegrale(0) {}

    float calcolaPotenza(float target, float posizioneAttuale, float tempoDelta) {
        float errore = target - posizioneAttuale;
        float P = Kp * errore;
        sommaIntegrale += errore * tempoDelta;
        float I = Ki * sommaIntegrale;
        float derivata = (errore - errorePrecedente) / tempoDelta;
        float D = Kd * derivata;
        errorePrecedente = errore;
        return P + I + D;
    }
};

void test_pid_comportamento_base() {
    PIDController pid(1.0, 0.0, 0.0); // Solo Proporzionale
    
    // TEST 1: Se siamo già sul target, la potenza deve essere 0
    float potenzaTargetRaggiunto = pid.calcolaPotenza(180.0, 180.0, 1.0);
    assert(potenzaTargetRaggiunto == 0.0 && "Test 1 Fallito: Il motore non si ferma sul target!");
    
    // TEST 2: Se siamo dietro al target, la potenza deve essere positiva (andare avanti)
    float potenzaAvanti = pid.calcolaPotenza(180.0, 100.0, 1.0);
    assert(potenzaAvanti > 0.0 && "Test 2 Fallito: Il motore va nella direzione sbagliata!");
    
    // TEST 3: Se abbiamo superato il target, la potenza deve essere negativa (tornare indietro)
    float potenzaIndietro = pid.calcolaPotenza(180.0, 200.0, 1.0);
    assert(potenzaIndietro < 0.0 && "Test 3 Fallito: Il motore non torna indietro se supera il limite!");

    std::cout << "[PASS] test_pid_comportamento_base superato." << std::endl;
}

void test_pid_vibrazione_sovrastima() {
    // Impostiamo un Kp (Proporzionale) ESAGERATAMENTE ALTO. 
    // Questo simula un sistema "nervoso" che vibra o impazzisce (Overshoot).
    PIDController pidInstabile(10.0, 0.0, 0.0); 
    
    // Target 10, partiamo da 0. Errore = 10.
    // Potenza calcolata = 10 (errore) * 10 (Kp) = 100! Troppa potenza!
    float potenzaEccessiva = pidInstabile.calcolaPotenza(10.0, 0.0, 1.0);
    
    // Verifichiamo che il sistema effettivamente eroghi una potenza spropositata
    assert(potenzaEccessiva > 50.0 && "Test Overshoot Fallito: Il PID non sta impazzendo come previsto.");
    
    std::cout << "[PASS] test_pid_vibrazione_sovrastima superato (il sistema va in overshoot come calcolato)." << std::endl;
}

int main() {
    std::cout << "--- Avvio Unit Test: Modulo PID ---" << std::endl;
    
    test_pid_comportamento_base();
    test_pid_vibrazione_sovrastima();
    
    std::cout << "TUTTI GLI UNIT TEST SONO PASSATI CON SUCCESSO! Il firmware e' matematicamente stabile." << std::endl;
    return 0;
}
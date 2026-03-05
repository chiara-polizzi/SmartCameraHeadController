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

#endif
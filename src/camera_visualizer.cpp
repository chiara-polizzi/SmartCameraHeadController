#include "../include/camera_visualizer.h"
#include <QBrush>
#include <QPen>
#include <iostream>

const int MAPPA_UI[5][5] = {
    {0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0},
    {0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0}
};

CameraVisualizer::CameraVisualizer(QWidget *parent) : QMainWindow(parent) {
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    setCentralWidget(view);
    setWindowTitle("Smart Camera - Live Telemetry");
    resize(600, 600);

    disegnaSetCinematografico();

    // Inizializziamo il pallino blu alla posizione zero
    telecameraUI = scene->addEllipse(0, 10, 80, 80, QPen(Qt::black), QBrush(Qt::blue));
    telecameraUI->setVisible(false);

    // INIZIALIZZIAMO IL NOSTRO PID! (P=0.5, I=0.01, D=0.1)
    telecameraPID = new PIDController(0.5, 0.01, 0.1);

    timerDiSistema = new QTimer(this);
    connect(timerDiSistema, &QTimer::timeout, this, &CameraVisualizer::aggiornaSensori);
    timerDiSistema->start(1000); 
}

void CameraVisualizer::disegnaSetCinematografico() {
    int dimensioneCella = 100;
    for(int riga = 0; riga < 5; riga++) {
        for(int col = 0; col < 5; col++) {
            int x = col * dimensioneCella;
            int y = riga * dimensioneCella;
            QBrush coloreSfondo = (MAPPA_UI[riga][col] == 1) ? QBrush(Qt::red) : QBrush(Qt::lightGray);
            scene->addRect(x, y, dimensioneCella, dimensioneCella, QPen(Qt::black), coloreSfondo);
        }
    }
}

void CameraVisualizer::aggiornaSensori() {
    // 1. LEGGIAMO L'HARDWARE (Legge dal nostro file CSV tramite i puntatori volatile!)
    int gradiAttuali = ottieniPosizioneEncoder(); 
    
    // 2. CALCOLIAMO LA POTENZA (Target = 180 gradi)
    float potenzaCalcolata = telecameraPID->calcolaPotenza(180.0, gradiAttuali, 1.0);
    uint32_t comandoMotore = static_cast<uint32_t>(std::abs(potenzaCalcolata));
    
    // 3. INVIAMO IL COMANDO AL MOTORE (Stampa a terminale)
    impostaVelocitaMotore(comandoMotore);

    // 4. AGGIORNIAMO LA GRAFICA!
    // Mappiamo i gradi fisici (0-180) in pixel sullo schermo (es. da 0 a 400 pixel)
    float posizionePixelX = (gradiAttuali / 180.0) * 400.0;
    
    // Spostiamo magicamente il pallino alle coordinate esatte calcolate dal firmware
    telecameraUI->setPos(posizionePixelX, 0); 
    telecameraUI->setVisible(true);
    
    std::cout << "[GUI] Telecamera aggiornata a: " << gradiAttuali << " gradi (Pixel X: " << posizionePixelX << ")" << std::endl;
}
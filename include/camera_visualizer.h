#ifndef CAMERA_VISUALIZER_H
#define CAMERA_VISUALIZER_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include "camera_core.h"

// Ereditiamo dalla classe base delle finestre di Qt
class CameraVisualizer : public QMainWindow {
    Q_OBJECT // Questa è la "parola magica" che fa funzionare i Signals/Slots di Qt

public:
    CameraVisualizer(QWidget *parent = nullptr);

private slots:
    void aggiornaSensori(); // Questa funzione verrà chiamata ogni secondo (Il nostro "Tick")

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsEllipseItem *telecameraUI; // Il pallino che rappresenta la telecamera
    QTimer *timerDiSistema;

    PIDController *telecameraPID;

    void disegnaSetCinematografico();
};

#endif
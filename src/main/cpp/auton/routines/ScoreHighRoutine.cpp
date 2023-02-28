#include "auton/routines/ScoreHighRoutine.h"

ScoreHighRoutine::ScoreHighRoutine() : COREAuton("High Score Routine") {}

void ScoreHighRoutine::AddNodes() {
    grabNode = new Node(2, new GrabberAction(GRABBER_GRAB));
    delayNode = new Node(2, new DelayAction());
    // scorehighNode = new Node(2, new ScoreHighAction(SCORE_HIGH));
    AddFirstNode(grabNode);
    grabNode -> AddNext(delayNode);
    // delayNode -> AddNext(scorehighNode);
}
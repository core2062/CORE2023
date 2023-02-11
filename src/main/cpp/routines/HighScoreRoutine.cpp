#include "routines/HighScoreRoutine.h"

HighScoreRoutine::HighScoreRoutine() : COREAuton("High Score Routine") {}

void HighScoreRoutine::AddNodes() {
    grab = new Node(2, new GrabberAction(GRABBER_GRAB));
    delay = new Node(2, new DelayAction());
    scorehigh = new Node(2, new ScoreHighAction(SCORE_HIGH));
    AddFirstNode(grab);
    grab -> AddNext(delay);
    delay -> AddNext(scorehigh);
}
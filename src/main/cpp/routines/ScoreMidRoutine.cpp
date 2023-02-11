#include "routines/ScoreMidRoutine.h"

ScoreMidRoutine::ScoreMidRoutine() : COREAuton("Middle Score Routine") {}

void ScoreMidRoutine::AddNodes() {
    grabNode = new Node(2, new GrabberAction(GRABBER_GRAB));
    delayNode = new Node(2, new DelayAction());
    scoremidNode = new Node(2, new ScoreMidAction(SCORE_MID));
    AddFirstNode(grabNode);
    grabNode -> AddNext(delayNode);
    delayNode -> AddNext(scoremidNode);
}
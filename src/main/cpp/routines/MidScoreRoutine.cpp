#include "routines/MidScoreRoutine.h"

MidScoreRoutine::MidScoreRoutine() : COREAuton("Middle Score Routine") {}

void MidScoreRoutine::AddNodes() {
    grab = new Node(2, new GrabberAction(GRABBER_GRAB));
    delay = new Node(2, new DelayAction());
    scoremid = new Node(2, new ScoreMidAction(SCORE_MID));
    AddFirstNode(grab);
    grab -> AddNext(delay);
    delay -> AddNext(scoremid);
}
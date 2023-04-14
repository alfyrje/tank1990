#ifndef scores_h
#define scores_h

#include "appstate.h"
#include "player.h"

class Scores : public AppState {
public:
    Scores();
    Scores(std::vector<Player*> t_players, int level, bool game_over);
    ~Scores();
    bool finished() const;
    void draw(Renderer* renderer);
    void update(int dt);
    void eventProcess(SDL_Event* event);
    AppState* nextState();

    std::vector<Player*> players;
    int currentLevel;
    bool gameOver;
    int showTime;
    int maxScore;
    int scoreCounter;
    bool scoreCounterChange;
};

#endif

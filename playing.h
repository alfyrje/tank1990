#ifndef playing_h
#define playing_h

#include<SDL.h>
#include "appstate.h"
#include "player.h"
#include "object.h"
#include "eagle.h"
#include "enemy.h"
#include "bonus.h"
#include "renderer.h"
#include<string>

class Playing : public AppState {
public:
    Playing();
    Playing(int players_count);
    Playing(std::vector<Player *> t_players, int previous_level);
    ~Playing();

    std::string level_name;
    int playersCount;
    int levelRows;
    int levelColumns;
    int currentLevel;
    void nextLevel();
    void loadLevel(std::string path);
    int enemyToKill;
    std::vector<Player*> players;
    std::vector<Enemy*> enemies;
    std::vector<Player*> killedPlayers;
    std::vector<std::vector<Object*> > level;
    std::vector<Object*> bushes;
    std::vector<Bonus*> bonuses;
    Eagle* eagle;

    bool finished() const;
    void draw(Renderer* renderer);
    void update(int dt);
    void eventProcess(SDL_Event* ev);
    AppState* nextState();
    bool playingFinished;
    bool gameOver;
    int gameOverTextPos;
    bool levelStartScreen;
    int levelStartTime;
    bool pause;

    SDL_Rect intersectRect(SDL_Rect *rect1, SDL_Rect *rect2);
    void checkCollisionTankWithLevel(Tank* tank, int dt);
    void checkCollisionTwoTanks(Tank* tank1, Tank* tank2, int dt);
    void checkCollisionBulletWithLevel(Bullet* bullet);
    void checkCollisionPlayerBulletsWithEnemy(Player* player, Enemy* enemy);
    void checkCollisionEnemyBulletsWithPlayer(Enemy* Enemy, Player* player);
    void checkCollisionTwoBullets(Bullet* bullet1, Bullet* bullet2);
    void checkCollisionBulletWithBush(Bullet *bullet);
    void checkCollisionPlayerWithBonus(Player* player, Bonus* bonus);

    int enemySpawnPos;
    int enemyReadyTime;
    int levelEndTime;
    void generateBonus();
    void generateEnemy();
    int dist;
    int min_dist;
    SDL_Point target;
    SDL_Point pos;
};

#endif

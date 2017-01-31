//Objects

#ifndef objects_h
#define objects_h


#endif /* objects_h */

//IDS

enum IDS {PLAYER, ENEMY, BULLET, BONUSSCORE, BONUSLIFE};

struct SpaceShip
{
    int ID;
    int x;
    int y;
    int lives;
    int speed;
    int boundx;
    int boundy;
    int score;
    int coins;
    int level;
    
    int maxFrame;
    int curFrame;
    int frameDelay;
    int frameCount;
    float frameWidth;
    int frameHeight;
    int animateDir;
    int animateCol;
    
    int animationRow;
    
    ALLEGRO_BITMAP * image;
};

struct Bullet
{
    int ID;
    int x;
    int y;
    int live;
    int speed;
};

struct Comet
{
    int ID;
    int x;
    int y;
    int live;
    int speed;
    int boundx;
    int boundy;
    
    int maxFrame;
    int curFrame;
    int frameCount;
    int frameDelay;
    int frameWidth;
    int frameHeight;
    int animateCol;
    int animateDir;
    
    ALLEGRO_BITMAP * image;
};

struct Explosion
{
    int x;
    int y;
    int live;
    
    int maxFrame;
    int curFrame;
    int frameCount;
    int frameDelay;
    int frameWidth;
    int frameHeight;
    int animateCol;
    int animateDir;
    
    ALLEGRO_BITMAP * image;
};

struct LifeBonus
{
    int ID;
    int x;
    int y;
    int live;
    int speed;
    int boundx;
    int boundy;
    
    int maxFrame;
    int curFrame;
    int frameDelay;
    int frameCount;
    float frameWidth;
    int frameHeight;
    
    ALLEGRO_BITMAP * image;
};

struct ScoreBonus
{
    int ID;
    int x;
    int y;
    int live;
    int speed;
    int boundx;
    int boundy;
    
    int maxFrame;
    int curFrame;
    int frameDelay;
    int frameCount;
    int frameWidth;
    float frameHeight;
    int animateDir;
    int animateCol;
    
    int animationRow;
    
    ALLEGRO_BITMAP * image;
};

















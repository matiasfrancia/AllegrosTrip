// My game 1

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdlib.h>
#include <time.h>
#include "objects.h"

//Constant variables
const int WIDTH = 400;
const int HEIGHT = 640;
const int FPS = 60;

const int numBullets = 4;
const int numComets = 6;
const int numExplosions = 4;
const int numLifeBonus = 1;
const int numScoreBonus = 1;

enum KEYS
{
    UP, DOWN, LEFT, RIGHT, SPACE
};

bool keys[5] = {false, false, false, false, false};

//Prototypes
void initShip(SpaceShip &ship, ALLEGRO_BITMAP * image);
void resetShipAnimation(SpaceShip &ship, int position);
void drawShip(SpaceShip &ship);
void shipMoveUp(SpaceShip &ship);
void shipMoveDown(SpaceShip &ship);
void shipMoveLeft(SpaceShip &ship);
void shipMoveRight(SpaceShip &ship);

void initBullet(Bullet bullet[], int size);
void drawBullet(Bullet bullet[], int size);
void fireBullet(Bullet bullet[], int size, SpaceShip &ship);
void updateBullet(Bullet bullet[], int size);
void bulletCollide(Bullet bullet[], int bSize, Comet comets[], int cSize, SpaceShip &ship, Explosion explosions[], int eSize);

void initComet(Comet comets[], int size, ALLEGRO_BITMAP * image);
void drawComet(Comet comets[], int size);
void startComet(Comet comets[], int size);
void updateComet(Comet comets[], int size);
void cometCollide(Comet comets[], int cSize, SpaceShip &ship, Explosion explosions[], int eSize);

void initExplosions(Explosion explosions[], int size, ALLEGRO_BITMAP * image);
void drawExplosions(Explosion explosions[], int size);
void startExplosions(Explosion explosions[], int size, int x, int y);
void updateExplosions(Explosion explosions[], int size);

void initLifeBonus(LifeBonus lbonus[], int size, ALLEGRO_BITMAP * image);
void drawLifeBonus(LifeBonus lbonus[], int size);
void startLifeBonus(LifeBonus lbonus[], int size);
void updateLifeBonus(LifeBonus lbonus[], int size);
void collideLifeBonus(LifeBonus lbonus[], int size, SpaceShip &ship);

void initScoreBonus(ScoreBonus sbonus[], int size, ALLEGRO_BITMAP * image);
void drawScoreBonus(ScoreBonus sbonus[], int size);
void startScoreBonus(ScoreBonus sbonus[], int size);
void updateScoreBonus(ScoreBonus sbonus[], int size);
void collideScoreBonus(ScoreBonus sbonus[], int size, SpaceShip &ship);

void updateLevel(SpaceShip &ship);

int main(int argc, char *argv[]){
    
    //Primitive variables
    bool done = false;
    bool redraw = true;
    bool isGameOver = false;
    
    //Allegro variables
    ALLEGRO_DISPLAY * display = NULL;
    ALLEGRO_EVENT_QUEUE * event_queue = NULL;
    ALLEGRO_TIMER * timer = NULL;
    ALLEGRO_BITMAP * playerImage;
    ALLEGRO_BITMAP * cometImage;
    ALLEGRO_BITMAP * expImage;
    ALLEGRO_BITMAP * lBonusImage;
    ALLEGRO_BITMAP * sBonusImage;
    ALLEGRO_FONT * arial18 = NULL;
    
    //Define objects
    SpaceShip ship;
    Bullet bullets[numBullets];
    Comet comets[numComets];
    Explosion explosions[numExplosions];
    LifeBonus lbonus[numLifeBonus];
    ScoreBonus sbonus[numScoreBonus];
    
    if(!al_init())
    {
        al_show_native_message_box(NULL, "Error", NULL, "Allegro couldn't initialize successfully", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    
    display = al_create_display(WIDTH, HEIGHT);
    
    al_set_window_title(display, "My first 2D game");
    al_clear_to_color(al_map_rgb(0, 0, 0));
    
    if(!display)
    {
        al_show_native_message_box(NULL, "Error", NULL, "Window wasn't created successfully", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    
    event_queue = al_create_event_queue();
    if(!event_queue)
    {
        al_show_native_message_box(NULL, "Error", NULL, "Event queue wasn't created successfully", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    
    timer = al_create_timer(1.0 / FPS);
    if(!timer)
    {
        al_show_native_message_box(NULL, "Error", NULL, "Timer wasn't created successfully", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    
    //Init addons
    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();
    al_install_audio();
    al_init_acodec_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    
    playerImage = al_load_bitmap("/Users/matias/Desktop/Matías/C++/Programas/AllegroFiles/Tutorial/GameMine1/GameMine1/resources/Images/SpaceShip/spaceship.png");
    cometImage = al_load_bitmap("/Users/matias/Desktop/Matías/C++/Programas/AllegroFiles/Tutorial/GameMine1/GameMine1/resources/Images/Asteroids/asteroid.png");
    expImage = al_load_bitmap("/Users/matias/Desktop/Matías/C++/Programas/AllegroFiles/Tutorial/GameMine1/GameMine1/resources/Images/Asteroids/Explosion/explosion.png");
    lBonusImage = al_load_bitmap("/Users/matias/Desktop/Matías/C++/Programas/AllegroFiles/Tutorial/GameMine1/GameMine1/resources/Images/Bonus/Lives/heart.png");
    sBonusImage = al_load_bitmap("/Users/matias/Desktop/Matías/C++/Programas/AllegroFiles/Tutorial/GameMine1/GameMine1/resources/Images/Bonus/Score/Coins.png");
    
    al_convert_mask_to_alpha(sBonusImage, al_map_rgb(255, 255, 255));
    
    arial18 = al_load_font("/Users/matias/Desktop/Matías/C++/Programas/AllegroFiles/Tutorial/GameMine1/GameMine1/resources/Fonts/arial.ttf", 18, 0);
    
    //Init functions
    srand((unsigned int) time(NULL));
    initShip(ship, playerImage);
    initBullet(bullets, numBullets);
    initComet(comets, numComets, cometImage);
    initExplosions(explosions, numExplosions, expImage);
    initLifeBonus(lbonus, numLifeBonus, lBonusImage);
    initScoreBonus(sbonus, numScoreBonus, sBonusImage);
    
    //Register event sources
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    
    al_hide_mouse_cursor(display);
    al_start_timer(timer);
    
    while(!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_ESCAPE:
                    done = true;
                    break;
                case ALLEGRO_KEY_UP:
                    keys[UP] = true;
                    break;
                case ALLEGRO_KEY_DOWN:
                    keys[DOWN] = true;
                    break;
                case ALLEGRO_KEY_LEFT:
                    keys[LEFT] = true;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    keys[RIGHT] = true;
                    break;
                case ALLEGRO_KEY_SPACE:
                    keys[SPACE] = true;
                    fireBullet(bullets, numBullets, ship);
                    break;
            }
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_ESCAPE:
                    done = true;
                    break;
                case ALLEGRO_KEY_UP:
                    keys[UP] = false;
                    break;
                case ALLEGRO_KEY_DOWN:
                    keys[DOWN] = false;
                    break;
                case ALLEGRO_KEY_LEFT:
                    keys[LEFT] = false;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    keys[RIGHT] = false;
                    break;
                case ALLEGRO_KEY_SPACE:
                    keys[SPACE] = false;
                    break;
            }
        }
        else if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
            
            if(keys[UP])
                shipMoveUp(ship);
            else if(keys[DOWN])
                shipMoveDown(ship);
            else
                resetShipAnimation(ship, 1);
            
            if(keys[LEFT])
                shipMoveLeft(ship);
            else if(keys[RIGHT])
                shipMoveRight(ship);
            else
                resetShipAnimation(ship, 2);
            
            if(!isGameOver)
            {
                updateExplosions(explosions, numExplosions);
                updateBullet(bullets, numBullets);
                startComet(comets, numComets);
                updateComet(comets, numComets);
                bulletCollide(bullets, numBullets, comets, numComets, ship, explosions, numExplosions);
                cometCollide(comets, numComets, ship, explosions, numExplosions);
                startLifeBonus(lbonus, numLifeBonus);
                updateLifeBonus(lbonus, numLifeBonus);
                collideLifeBonus(lbonus, numLifeBonus, ship);
                startScoreBonus(sbonus, numScoreBonus);
                updateScoreBonus(sbonus, numScoreBonus);
                collideScoreBonus(sbonus, numScoreBonus, ship);
                
                if(ship.lives <= 0)
                {
                    isGameOver = true;
                }
            }
        }
        
        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
            
            if(!isGameOver)
            {
                drawShip(ship);
                drawBullet(bullets, numBullets);
                drawComet(comets, numComets);
                drawExplosions(explosions, numExplosions);
                drawLifeBonus(lbonus, numLifeBonus);
                drawScoreBonus(sbonus, numScoreBonus);
                
                al_draw_textf(arial18, al_map_rgb(255, 255, 255), 5, 5, ALLEGRO_ALIGN_LEFT, "Lives: %i   Score: %i  Coins: %i", ship.lives, ship.score, ship.coins);
            }
            else
            {
                al_draw_textf(arial18, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "Game Over! Score: %i, Coins: %i", ship.score, ship.coins);
            }
            
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
        
    }
    
    al_destroy_timer(timer);
    al_destroy_bitmap(playerImage);
    al_destroy_bitmap(cometImage);
    al_destroy_bitmap(expImage);
    al_destroy_bitmap(lBonusImage);
    al_destroy_bitmap(sBonusImage);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    
    return 0;
}

//SpaceShip

void initShip(SpaceShip &ship, ALLEGRO_BITMAP * image)
{
    ship.x = WIDTH / 2;
    ship.y = HEIGHT - 20;
    ship.ID = PLAYER;
    ship.lives = 3;
    ship.speed = 6;
    ship.boundx = 12;
    ship.boundy = 10;
    ship.score = 0;
    ship.coins = 0;
    ship.level = 1;
    
    ship.maxFrame = 3;
    ship.curFrame = 1;
    ship.frameCount = 0;
    ship.frameDelay = 50;
    ship.frameWidth = 40.5;
    ship.frameHeight = 45;
    ship.animateCol = 3;
    ship.animateDir = 1;
    
    ship.animationRow = 2;
    
    ship.image = image;
}

void resetShipAnimation(SpaceShip &ship, int position)
{
    if(position == 1)
        ship.animationRow = 2;
    else
        ship.curFrame = 1;
}

void drawShip(SpaceShip &ship)
{
    int fx = (ship.curFrame % ship.animateCol) * ship.frameWidth;
    int fy = ship.animationRow * ship.frameHeight;
    
    al_draw_bitmap_region(ship.image, fx, fy, ship.frameWidth, ship.frameHeight, ship.x - ship.frameWidth / 2, ship.y - ship.frameHeight / 2, 0);
}

void shipMoveUp(SpaceShip &ship)
{
    ship.animationRow = 1;
    ship.y -= ship.speed;
    if(ship.y <= HEIGHT - 200)
        ship.y = HEIGHT - 200;
}

void shipMoveDown(SpaceShip &ship)
{
    ship.animationRow = 0;
    ship.y += ship.speed;
    if(ship.y >= HEIGHT - ship.frameHeight)
        ship.y = HEIGHT - ship.frameHeight;
}

void shipMoveLeft(SpaceShip &ship)
{
    ship.curFrame = 0;
    ship.x -= ship.speed;
    if(ship.x <= ship.frameWidth)
        ship.x = ship.frameWidth;
}

void shipMoveRight(SpaceShip &ship)
{
    ship.curFrame = 2;
    ship.x += ship.speed;
    if(ship.x >= WIDTH - ship.frameWidth)
        ship.x = WIDTH - ship.frameWidth;
}

//Bullet

void initBullet(Bullet bullet[], int size)
{
    
    for(int i = 0; i < size; i++)
    {
        bullet[i].ID = BULLET;
        bullet[i].speed = 10;
        bullet[i].live = false;
        
    }
    
}
void drawBullet(Bullet bullet[], int size)
{
    for(int i = 0; i < size; i++)
    {
        if(bullet[i].live)
        {
            al_draw_filled_circle(bullet[i].x, bullet[i].y, 2, al_map_rgb(255, 255, 255));
        }
    }
}
void fireBullet(Bullet bullet[], int size, SpaceShip &ship)
{
    for(int i = 0; i < size; i++)
    {
        if(!bullet[i].live)
        {
            bullet[i].x = ship.x;
            bullet[i].y = ship.y - 17;
            bullet[i].live = true;
            break;
        }
    }
}
void updateBullet(Bullet bullet[], int size)
{
    for(int i = 0; i < size; i++)
    {
        if(bullet[i].live)
        {
            bullet[i].y -= bullet[i].speed;
            if(bullet[i].y < 0)
                bullet[i].live = false;
        }
    }
}
void bulletCollide(Bullet bullet[], int bSize, Comet comets[], int cSize, SpaceShip &ship, Explosion explosions[], int eSize)
{
    for(int i = 0; i < bSize; i++)
    {
        if(bullet[i].live)
        {
            for(int j = 0; j < cSize; j++)
            {
                if(comets[j].live)
                {
                    if(bullet[i].x > (comets[j].x - comets[i].boundx) &&
                       bullet[i].x < (comets[j].x + comets[i].boundx) &&
                       bullet[i].y > (comets[j].y - comets[j].boundy) &&
                       bullet[i].y < (comets[j].y + comets[j].boundy))
                    {
                        bullet[i].live = false;
                        comets[j].live = false;
                        
                        ship.score++;
                        
                        startExplosions(explosions, eSize, bullet[i].x, bullet[i].y);
                    }
                }
            }
        }
    }
}


//Comets

void initComet(Comet comets[], int size, ALLEGRO_BITMAP * image)
{
    for(int i = 0; i < size; i++)
    {
        comets[i].ID = ENEMY;
        comets[i].live = false;
        comets[i].speed = rand() % 6 + 4;
        
        if(comets[i].speed >= 6)
            comets[i].speed = rand() % 4 + 7;
        
        comets[i].boundx = 35;
        comets[i].boundy = 35;
        
        comets[i].maxFrame = 143;
        comets[i].curFrame = 0;
        comets[i].frameCount = 0;
        comets[i].frameDelay = rand() % 7 + 2;
        comets[i].frameWidth = 96;
        comets[i].frameHeight = 96;
        comets[i].animateCol = 21;
        
        if(rand() % 2)
            comets[i].animateDir = 1;
        else
            comets[i].animateDir = -1;
        
        comets[i].image = image;
    }
}

void drawComet(Comet comets[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if(comets[i].live)
        {
            int fx = (comets[i].curFrame % comets[i].animateCol) * comets[i].frameWidth;
            int fy = (comets[i].curFrame / comets[i].animateCol) * comets[i].frameHeight;
            
            al_draw_bitmap_region(comets[i].image, fx, fy, comets[i].frameWidth, comets[i].frameHeight, comets[i].x - comets[i].frameWidth / 2, comets[i].y - comets[i].frameHeight / 2, 0);
        }
    }
}

void startComet(Comet comets[], int size)
{
    for(int i = 0; i < size; i++)
    {
        if(!comets[i].live)
        {
            if(rand() % 500 == 0)
            {
                comets[i].live = true;
                comets[i].x = 30 + rand() % (WIDTH - 60);
                comets[i].y = 0;
                
                break;
            }
        }
    }
}

void updateComet(Comet comets[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if(comets[i].live)
        {
            if (++comets[i].frameCount >= comets[i].frameDelay)
            {
                comets[i].curFrame += comets[i].animateDir;
                if(comets[i].curFrame >= comets[i].maxFrame)
                    comets[i].curFrame = 0;
                else if(comets[i].curFrame <= 0)
                    comets[i].curFrame = comets[i].maxFrame - 1;
                
                comets[i].frameCount = 0;
            }
            
            comets[i].y += comets[i].speed;
        }
    }
}

void cometCollide(Comet comets[], int cSize, SpaceShip &ship, Explosion explosions[], int eSize)
{
    for (int i = 0; i < cSize; i++)
    {
        if(comets[i].live)
        {
            if((comets[i].x - comets[i].boundx < ship.x + ship.boundx) &&
               (comets[i].x + comets[i].boundx > ship.x - ship.boundx) &&
               (comets[i].y - comets[i].boundy < ship.y + ship.boundy) &&
               (comets[i].y + comets[i].boundy > ship.y - ship.boundy))
            {
                ship.lives--;
                comets[i].live = false;
                
                startExplosions(explosions, numExplosions, comets[i].x, comets[i].y);
            }
            else if(comets[i].y > HEIGHT)
            {
                ship.lives--;
                comets[i].live = false;
            }
        }
    }
}

//Explosions

void initExplosions(Explosion explosions[], int size, ALLEGRO_BITMAP * image)
{
    for (int i = 0; i < size; i++)
    {
        explosions[i].live = false;
        
        explosions[i].maxFrame = 31;
        explosions[i].curFrame = 0;
        explosions[i].frameCount = 0;
        explosions[i].frameDelay = 1;
        explosions[i].frameWidth = 128;
        explosions[i].frameHeight = 128;
        explosions[i].animateCol = 8;
        explosions[i].animateDir = 1;
        
        explosions[i].image = image;
    }
}

void drawExplosions(Explosion explosions[], int size)
{
    for (int i = 0; i < size; i++){
        if(explosions[i].live)
        {
            int fx = (explosions[i].curFrame % explosions[i].animateCol) * explosions[i].frameWidth;
            int fy = (explosions[i].curFrame / explosions[i].animateCol) * explosions[i].frameHeight;
            
            al_draw_bitmap_region(explosions[i].image, fx, fy, explosions[i].frameWidth, explosions[i].frameHeight, explosions[i].x - explosions[i].frameWidth / 2, explosions[i].y - explosions[i].frameHeight / 2, 0);
        }
    }
}

void startExplosions(Explosion explosions[], int size, int x, int y)
{
    for (int i = 0; i < size; i++)
    {
        if(!explosions[i].live)
        {
            explosions[i].live = true;
            explosions[i].x = x;
            explosions[i].y = y;
            break;
        }
    }
}

void updateExplosions(Explosion explosions[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if(explosions[i].live)
        {
            if(++explosions[i].frameCount >= explosions[i].frameDelay)
            {
                explosions[i].curFrame += explosions[i].animateDir;
                if(explosions[i].curFrame >= explosions[i].maxFrame)
                {
                    explosions[i].curFrame = 0;
                    explosions[i].live = false;
                }
                
                explosions[i].frameCount = 0;
            }
        }
    }
}

//Life Bonus

void initLifeBonus(LifeBonus lbonus[], int size, ALLEGRO_BITMAP * image)
{
    for(int i = 0; i < size; i++)
    {
        lbonus[i].ID = BONUSLIFE;
        lbonus[i].live = false;
        lbonus[i].speed = 4;
        lbonus[i].boundx = 20;
        lbonus[i].boundy = 19;
        
        lbonus[i].maxFrame = 7;
        lbonus[i].curFrame = 0;
        lbonus[i].frameCount = 0;
        lbonus[i].frameDelay = 5;
        lbonus[i].frameWidth = 41;
        lbonus[i].frameHeight = 47;
        
        lbonus[i].image = image;
    }
}

void drawLifeBonus(LifeBonus lbonus[], int size)
{
    for(int i = 0; i < size; i++)
    {
        if(lbonus[i].live)
        {
            al_draw_bitmap_region(lbonus[i].image, lbonus[i].curFrame * lbonus[i].frameWidth, 0, lbonus[i].frameWidth, lbonus[i].frameHeight, lbonus[i].x - lbonus[i].frameWidth / 2, lbonus[i].y - lbonus[i].frameHeight / 2, 0);
            
            //al_draw_filled_rectangle(lbonus[i].x - lbonus[i].boundx, lbonus[i].y - lbonus[i].boundy, lbonus[i].x + lbonus[i].boundx, lbonus[i].y + lbonus[i].boundy, al_map_rgba(255, 0, 255, 100));
        }
    }
}

void startLifeBonus(LifeBonus lbonus[], int size)
{
    for(int i = 0; i < size; i++)
    {
        if(!lbonus[i].live)
        {
            if(rand() % 5000 == 0)
            {
                lbonus[i].live = true;
                lbonus[i].x = 30 + rand() % (WIDTH - 60);
                lbonus[i].y = 0;
                
                break;
            }
        }
    }
}

void updateLifeBonus(LifeBonus lbonus[], int size)
{
    for(int i = 0; i < size; i++)
    {
        if(lbonus[i].live)
        {
            if(++lbonus[i].frameCount >= lbonus[i].frameDelay)
            {
                if(++lbonus[i].curFrame >= lbonus[i].maxFrame)
                    lbonus[i].curFrame = 0;
                
                lbonus[i].frameCount = 0;
            }
            
            lbonus[i].y += lbonus[i].speed;
        }
    }
}

void collideLifeBonus(LifeBonus lbonus[], int size, SpaceShip &ship)
{
    for(int i = 0; i < size; i++)
    {
        if(lbonus[i].live)
        {
            if((lbonus[i].x - lbonus[i].boundx < ship.x + ship.boundx) &&
               (lbonus[i].x + lbonus[i].boundx > ship.x - ship.boundx) &&
               (lbonus[i].y - lbonus[i].boundy < ship.y + ship.boundy) &&
               (lbonus[i].y + lbonus[i].boundy > ship.y - ship.boundy))
            {
                ship.lives++;
                
                if(ship.lives >= 3)
                    ship.lives = 3;
                
                lbonus[i].live = false;
            }
            else if(lbonus[i].y >= HEIGHT)
            {
                lbonus[i].live = false;
            }
        }
    }
}

//Score Bonus

void initScoreBonus(ScoreBonus sbonus[], int size, ALLEGRO_BITMAP * image)
{
    for(int i = 0; i < size; i++)
    {
        sbonus[i].ID = BONUSLIFE;
        sbonus[i].live = false;
        sbonus[i].speed = 4;
        sbonus[i].boundx = 15;
        sbonus[i].boundy = 15;
        
        sbonus[i].maxFrame = 30;
        sbonus[i].curFrame = 0;
        sbonus[i].frameCount = 0;
        sbonus[i].frameDelay = 5;
        sbonus[i].frameWidth = 32;
        sbonus[i].frameHeight = 31.5;
        sbonus[i].animateCol = 8;
        sbonus[i].animateDir = 1;
        
        sbonus[i].image = image;
    }
}

void drawScoreBonus(ScoreBonus sbonus[], int size)
{
    for(int i = 0; i < size; i++)
    {
        if(sbonus[i].live)
        {
            int fx = (sbonus[i].curFrame % sbonus[i].animateCol) * sbonus[i].frameWidth;
            int fy = (sbonus[i].curFrame / sbonus[i].animateCol) * sbonus[i].frameHeight;
            
            al_draw_bitmap_region(sbonus[i].image, fx, fy, sbonus[i].frameWidth, sbonus[i].frameHeight, sbonus[i].x - sbonus[i].frameWidth / 2, sbonus[i].y - sbonus[i].frameHeight / 2, 0);
            
            //al_draw_filled_rectangle(sbonus[i].x - sbonus[i].boundx, sbonus[i].y - sbonus[i].boundy, sbonus[i].x + sbonus[i].boundx, sbonus[i].y + sbonus[i].boundy, al_map_rgba(255, 0, 255, 100));
        }
    }
}

void startScoreBonus(ScoreBonus sbonus[], int size)
{
    for(int i = 0; i < size; i++)
    {
        if(!sbonus[i].live)
        {
            if(rand() % 1000 == 0)
            {
                sbonus[i].live = true;
                sbonus[i].x = 30 + rand() % (WIDTH - 60);
                sbonus[i].y = 0;
                
                break;
            }
        }
    }
}

void updateScoreBonus(ScoreBonus sbonus[], int size)
{
    for(int i = 0; i < size; i++)
    {
        if(sbonus[i].live)
        {
            if(++sbonus[i].frameCount >= sbonus[i].frameDelay)
            {
                sbonus[i].curFrame += sbonus[i].animateDir;
                if(++sbonus[i].curFrame >= sbonus[i].maxFrame)
                    sbonus[i].curFrame = 0;
                else if(sbonus[i].curFrame <= 0)
                    sbonus[i].curFrame = sbonus[i].maxFrame - 1;
                
                sbonus[i].frameCount = 0;
            }
            
            sbonus[i].y += sbonus[i].speed;
        }
    }
}

void collideScoreBonus(ScoreBonus sbonus[], int size, SpaceShip &ship)
{
    for(int i = 0; i < size; i++)
    {
        if(sbonus[i].live)
        {
            if((sbonus[i].x - sbonus[i].boundx < ship.x + ship.boundx) &&
               (sbonus[i].x + sbonus[i].boundx > ship.x - ship.boundx) &&
               (sbonus[i].y - sbonus[i].boundy < ship.y + ship.boundy) &&
               (sbonus[i].y + sbonus[i].boundy > ship.y - ship.boundy))
            {
                ship.coins++;
                sbonus[i].live = false;
            }
            else if(sbonus[i].y >= HEIGHT)
            {
                sbonus[i].live = false;
            }
        }
    }
}

void updateLevel(SpaceShip &ship)
{
    if(ship.score )
    {
        
    }
}




































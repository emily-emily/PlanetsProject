/*Emily Yu
ICS3U P4
May 2018
Planets Project - ICS3U Summative*/

#include <allegro5/allegro.h>
#include <time.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "planets.h"

#include<stdio.h>

int main(int argc, char *argv[]){
    initializeAllegro();
    srand(time(0));

    //setup
    ALLEGRO_DISPLAY *display = nullptr;
    ALLEGRO_TIMER *timer = nullptr;
    ALLEGRO_EVENT_QUEUE *q = nullptr;
    ALLEGRO_BITMAP *sprite[8] = {nullptr};
    ALLEGRO_BITMAP *mImage = nullptr;
    ALLEGRO_BITMAP *background = nullptr;
    ALLEGRO_BITMAP *planet = nullptr;
    ALLEGRO_BITMAP *box = nullptr;
    ALLEGRO_KEYBOARD_STATE kState;
    ALLEGRO_FONT *font[iFonts] = {nullptr};

    //create and load
    display = al_create_display(SCREEN_W, SCREEN_H);
    timer = al_create_timer(1.0/FPS);
    q = al_create_event_queue();
    sprite[0] = al_load_bitmap("images/characters/character1.png");
    sprite[1] = al_load_bitmap("images/characters/character2.png");
    sprite[2] = al_load_bitmap("images/characters/character3.png");
    sprite[3] = al_load_bitmap("images/characters/character4.png");
    sprite[4] = al_load_bitmap("images/characters/character5.png");
    sprite[5] = al_load_bitmap("images/characters/character6.png");
    sprite[6] = al_load_bitmap("images/characters/character7.png");
    sprite[7] = al_load_bitmap("images/characters/character8.png");
    mImage = al_load_bitmap("images/meteorV3.png");
    background = al_load_bitmap("images/background.png");
    planet = al_load_bitmap("images/planet.png");
    box = al_load_bitmap("images/box.png");
    for (int i = 0; i < iFonts; i++)
        font[i] = al_load_ttf_font("font-Sansation/Sansation-Regular.ttf", 80 - 10 * i, 0);

    if (checkSetup(display, sprite, mImage, background, planet, box, timer, q, font) != 0)
        return -1;

    al_set_window_title(display, "Planet Game");

    //register event sources
	al_register_event_source(q, al_get_timer_event_source(timer));
	al_register_event_source(q, al_get_display_event_source(display));
	al_register_event_source(q, al_get_keyboard_event_source());
	al_register_event_source(q, al_get_mouse_event_source());

    //declare objects
    Planet a;
    a.r = 150;
    a.x = SCREEN_W / 2;
    a.y = SCREEN_H / 2;

    Sprite s;
    s.xPos = a.x + a.r;
    s.yPos = a.y - a.r;
    s.xVel = 0.0;
    s.yVel = 0.0;
    s.shiftX = 0.0;
    s.shiftY = 0.0;
    s.frame = 0;
    s.dir = 1;
    s.airborne = false;

    Meteor m[maxMeteors];
    for (int i = 0; i < maxMeteors; i++){
        m[i].xPos = 0;
        m[i].yPos = 0;
        m[i].xVel = 0;
        m[i].yVel = 0;
        m[i].w = al_get_bitmap_width(mImage);
        m[i].h = al_get_bitmap_height(mImage);
        m[i].available = true;
    }

    //buttons
    //GAMEOVER
    Button btnHighscores;
    strcpy(btnHighscores.text, "Highscores");
    findBtnXY(btnHighscores, font, btnHighscores.text, SCREEN_H - 300);
    Button btnToMain;
    strcpy(btnToMain.text, "Main Menu");
    findBtnXY(btnToMain, font, btnToMain.text, SCREEN_H - 230);
    //NEWHIGHSCORE
    Button btnEnter;
    strcpy(btnEnter.text, "Enter");
    findBtnXY(btnEnter, font, btnEnter.text, SCREEN_H - 250);
    Button btnNo;
    strcpy(btnNo.text, "No thanks");
    findBtnXY(btnNo, font, btnNo.text, SCREEN_H - 200);
    //HIGHSCORES
    Button btnBack;
    strcpy(btnBack.text, "Back");
    findBtnXY(btnBack, font, btnBack.text, SCREEN_H - 125);

	//additional setup
    al_start_timer(timer);
    bool paused = false;
    int counter = 0; //counts loops for meteors to spawn once a second
    int iFlash = 0; //for text on start screen
    float score = 0.0;
    bool running = true;
    //score
    char name[10][maxNameLength];
    int highscore[10] = {0};
    //screen
    Screen scr = START;

    //game loop
    while (running){
        ALLEGRO_EVENT ev;

        al_get_keyboard_state(&kState);
        al_wait_for_event(q, &ev);

        //start screen
        if (scr == START){
            if (ev.type == ALLEGRO_EVENT_TIMER){
                drawLayout(background, box, scr, font, score);
                drawStart(font[0], font[6], iFlash);
                al_flip_display();
                iFlash = (iFlash + 1) % FPS;
            }
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
                scr = GAME;
        }

        //gameplay
        if (scr == GAME){
            if (ev.type == ALLEGRO_EVENT_TIMER){
                //spawn a meteor once a second
                if (counter == 0)
                    createMeteor(m, mImage);
                //apply gravity
                gravity(s, m, a);

                //movement
                if (al_key_down(&kState, ALLEGRO_KEY_SPACE))
                    jump(s, a);

                if (al_key_down(&kState, ALLEGRO_KEY_LEFT))
                    shift(s, a, LEFT);
                else if (al_key_down(&kState, ALLEGRO_KEY_RIGHT))
                    shift(s, a, RIGHT);
                else shift(s, a, NONE);

                //check sprite-meteor collision
                for (int i = 0; i < maxMeteors; i++){
                    if (isCollision(s, al_get_bitmap_width(sprite[0]) * imageScale, al_get_bitmap_height(sprite[0]) * imageScale, m[i],
                                    al_get_bitmap_width(mImage) * imageScale, al_get_bitmap_height(mImage) * imageScale) && !m[i].available){
                        if (getHighscores(display, name, highscore) == 0 && score > highscore[9])
                            scr = NEWHIGHSCORE;
                        else scr = GAMEOVER;
                    }
                }

                //update new object locations and draw
                getNewCoordinates(s, m);
                drawLayout(background, box, scr, font, score);
                drawObjects(planet, a, s, sprite, m, mImage);
                al_flip_display();

                counter = (counter + 2) % FPS;
                score += 10.0 / FPS;
            }

            if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                if (ev.keyboard.keycode == ALLEGRO_KEY_P){
                    printf("Pause!");
                    togglePause(timer, paused);
                }
            }

            if (ev.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT){
                paused = false;
                togglePause(timer, paused);
            }

            /*if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                //for testing
                s.xPos = ev.mouse.x;
                s.xVel = 0;
                s.yPos = ev.mouse.y;
                s.yVel = 0;
            }*/
        }

        //game over screen
        if (scr == GAMEOVER){
            if (ev.type == ALLEGRO_EVENT_TIMER){
                drawLayout(background, box, scr, font, score);
                drawGameOver(font, score, btnHighscores, btnToMain);
                al_flip_display();
            }
            if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                if (btnIsClicked(btnHighscores, ev.mouse.x, ev.mouse.y))
                    scr = HIGHSCORES;
                if (btnIsClicked(btnToMain, ev.mouse.x, ev.mouse.y))
                    scr = START;
            }
        }

        if (scr == NEWHIGHSCORE){
            if (ev.type == ALLEGRO_EVENT_TIMER){
                drawLayout(background, box, scr, font, score);
                al_flip_display();
            }
        }

        if (scr == HIGHSCORES){
            if (ev.type == ALLEGRO_EVENT_TIMER){
                drawLayout(background, box, scr, font, score);
                if (drawHighscores(display, font, btnBack, name, highscore) != 0)
                    scr = GAMEOVER;
                else al_flip_display();
            }
            if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                if (btnIsClicked(btnBack, ev.mouse.x, ev.mouse.y))
                    scr = GAMEOVER;
            }
        }

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                running = false;
    }

    //destroy and release data
    for (int i = 0; i < 8; i++)
        al_destroy_bitmap(sprite[i]);
    al_destroy_display(display);
    al_destroy_timer(timer);
    for (int i = 0; i < iFonts; i++)
        al_destroy_font(font[i]);

    return 0;
}

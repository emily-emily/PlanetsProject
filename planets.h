//defined values
#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define MAGENTA al_map_rgb(240, 50, 230)

//constant values
const int SCREEN_W = 1200;
const int SCREEN_H = 700;

const int FPS = 60;
const int jumpVel = 600;
const int gVel = 40;
const int moveSpd = 20;

const int maxMeteors = 10;

//structures
struct Planet{
    int x;
    int y;
    int r;
};

struct Sprite{
    float xPos, yPos;
    float xVel, yVel;
};

struct Meteor{
    float xPos, yPos;
    float xVel, yVel;
    bool available;
};

enum Direction {LEFT, RIGHT};
enum Difficulty {BEGINNER = 8, EASY = 4, NORMAL = 3, HARD = 1};
//Difficulty gameDiff = BEGINNER;

//prototypes
void draw(Planet a, Sprite s, ALLEGRO_BITMAP *bitmap, Meteor m[], ALLEGRO_BITMAP *mImage);
//planetsAllegro
void initializeAllegro();
int checkSetup(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *sprite, ALLEGRO_BITMAP *mImage, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *q);

//planetsMovement
void getNewCoordinates(Sprite &s, Meteor m[]);
void jump(Sprite &s, Planet a);
void shift(Sprite &s, Planet a, Direction spriteD);
bool isGrounded(Sprite s, Planet a);
bool misGrounded(Meteor m[], int i, Planet a);
void gravity(Sprite &s, Meteor m[], Planet a);
float rotateAngle(Sprite s, Planet a);
bool isCollision(Sprite p, int sw, int sh, Meteor m, int mw, int mh);

void togglePause(ALLEGRO_TIMER *timer, bool &paused);
void createMeteor(Meteor m[], ALLEGRO_BITMAP *image);
void destroyMeteor(Meteor m[], int i);
void gameOver();

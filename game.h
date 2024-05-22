#include <SFML/Graphics.hpp>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <limits>
#include <cmath>
#include "AVLtree.h"

using namespace sf;
using namespace std;


const char title[] = "DATA ST-Project, Spring-2024";
using namespace sf;

const int MAX_ROWS = 62;
const int MAX_COLS = 36;
const int INF = numeric_limits<int>::max();

struct djBlock
{
    int Pos_x;
    int Pos_y;
    int data;
    int dist;
    bool visited;
    pair<int, int> parent;
};

void pause() {
    RenderWindow window(VideoMode(575, 350), "Pause");

    Texture obj4;
    obj4.loadFromFile("img/pawsEnd.jpg");
    Sprite Menu(obj4);
    Menu.setScale(575.0f / 1080.0f, 350.0f / 936.0f);

    Font font;
    font.loadFromFile("fonty/hiro.ttf");

    Text text4;
    text4.setFont(font);
    text4.setString("PAUSE");
    text4.setPosition(240, 120);
    text4.setCharacterSize(30);
    text4.setStyle(Text::Style::Bold);
    text4.setFillColor(Color::White);

    Text text5;
    text5.setFont(font);
    text5.setString("   PRESS R TO RESUME");
    text5.setPosition(99, 180);
    text5.setCharacterSize(30);
    text5.setStyle(Text::Style::Bold);
    text5.setFillColor(Color::White);

    while (window.isOpen())
    {
        Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed)
            {
                window.close();
            }
            if (ev.type == Event::KeyPressed)
            {
                if (ev.key.code == Keyboard::R)
                {
                    return;
                }

            }
        }

        window.clear();
        window.draw(Menu);
        window.draw(text4);
        window.draw(text5);
        window.display();
    }
}

void end_game() {
    RenderWindow window(VideoMode(575, 350), "Game Over");

    Texture obj4;
    obj4.loadFromFile("img/gamOvC.jpg");
    Sprite Menu(obj4);
    Menu.setScale(575.0f / 1080.0f, 350.0f / 937.0f);


    while (window.isOpen())
    {
        Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed)
            {
                window.close();
            }
            if (ev.type == Event::KeyPressed)
            {
                if (ev.key.code == Keyboard::Enter)
                {
                    exit(0);
                }

            }
        }

        window.clear();
        window.draw(Menu);
        window.display();
    }
}

void you_won() {
    RenderWindow window(VideoMode(475, 350), "YOU WON");

    Texture obj4;
    obj4.loadFromFile("img/winner.jpg");
    Sprite Menu(obj4);
    Menu.setScale(475.0f/1080.0f, 350.0f/1080.0f);

    Font font;
    font.loadFromFile("fonty/hiro.ttf");

    Text text4;
    text4.setFont(font);
    text4.setString("YOU WON");
    text4.setPosition(150, 95);
    text4.setCharacterSize(26);
    text4.setStyle(Text::Style::Bold);
    text4.setFillColor(Color::White);

    while (window.isOpen())
    {
        Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed)
            {
                window.close();
            }
            if (ev.type == Event::KeyPressed)
            {
                if (ev.key.code == Keyboard::Enter)
                {
                    exit(0);  // Exit the function and continue the game
                }

            }
        }

        window.clear();
        window.draw(Menu);
        window.draw(text4);
        window.display();
    }
}

class mazeBlock
{
public:
    RectangleShape block;
    int xx = 0;
    int yy = 0;

    // Default constructor
    mazeBlock() {}

    // Parameterized constructor
    mazeBlock(float x, float y, float width, float height)
    {
        xx = x;
        yy = y;
        block.setSize(Vector2f(width, height));
        block.setPosition(x, y);
        block.setFillColor(Color::White); // Adjust the color as needed
    }
};

class Enemy  // GAME HAS-A ENEMY
{
public:
    RectangleShape shape;
    float enX = 0;
    float enY = 0;
    int direction; // Direction of movement for the box

    // Default constructor
    Enemy()
        : direction(0) // Set a default direction (you can adjust as needed)
    {
        // Set default properties or leave them uninitialized as needed
    }

    // Parameterized constructor
    Enemy(float x, float y, float width, float height, int initialDirection)
        : direction(initialDirection)
    {
        enX = x;
        enY = y;
        shape.setSize(Vector2f(width, height));
        shape.setPosition(x, y);
        shape.setFillColor(Color::Transparent); // Adjust the color as needed
    }

    void move(float speed, float offsetY)
    {
        shape.move(speed * direction, offsetY);
        enX = shape.getPosition().x;
        enY = shape.getPosition().y;
    }

};

class Player  // GAME HAS-A PLAYER
{
public:
    RectangleShape shape;
    int playX = 0;
    int playY = 0;
    int direction; // Direction of movement for the box
    int lives = 3;

    // Default constructor
    Player()
        : direction(0) // Set a default direction (you can adjust as needed)
    {
        // Set default properties or leave them uninitialized as needed
    }

    // Parameterized constructor
    Player(float x, float y, float width, float height, int initialDirection)
        : direction(initialDirection)
    {
        playX = x;
        playY = y;
        shape.setSize(Vector2f(width, height));
        shape.setPosition(x, y);
        shape.setFillColor(Color::Transparent); // Adjust the color as needed
    }

    void move(float speed, float offsetY)
    {
        shape.move(speed * direction, offsetY);
        playX = shape.getPosition().x;
        playY = shape.getPosition().y;
    }




    //bool dec_lives()
    //{
    //    CHECK POSITION

    //    IF SAME POS, DEC LIVES
    //    lives--;

    //    CHECK IF LIVES ==0, CALL END GAME
    //    if (lives == 0)
    //    {
    //        return 1; //BECAUSE LIVES HAVE BECOME ZERO AND GAME ENDED
    //    }
    //    return 0;
    //}
};


class Game : public Player, public Enemy, public mazeBlock, public AVLTree
{
public:
    //-------Player SPRITE
    Sprite playerr;
    Texture playerr_texture;

    //------ENEMY1 SPRITE

    Texture dushman_text;
    Texture dushman_text2;
    Texture dushman_text3;
    Texture dushman_text4;
    Sprite dushman;
    Sprite dushman2;
    Sprite dushman3;
    Sprite dushman4;

    //------ARTIFACTS SPRITE

      // ->> swords
    Texture sword_text;
    Sprite talwar;
    Sprite talwar2;
    Sprite talwar3;
    Sprite talwar4;

    // ->> keys
    Texture chabi_text;
    Sprite chabi;
    Sprite chabi2;

    // ->> potions
    Texture potion_text2;
    Texture potion_text1;
    Sprite potion1;
    Sprite potion2;

    // ->> treasure
    Texture treasure_text;
    Sprite khazana;

    Texture op_treasure;
    Sprite khulaKhazana;

    // ->> gem
    Texture gem_text;    //bonus artifact
    Sprite heera;

    // ->> AVL Tree
    friend class AVLTree;
    AVLTree mazeInventree;
    AVLTree inventree;

    // ->> Doubly LL inventree
    friend class DoublyLinkedList;
    //Doublylinkedlist inventree;


    // ->> Background
    Sprite background;
    Texture bg_texture;

    //OBJECTSSSSSSSSSSSSSSSSSSSS
    // ->> Player
    Player P;

    // ->> Enemies
    Enemy E1;
    Enemy E2;
    Enemy E3;
    Enemy E4;

    //-----------SCORE VAR
    int score = 0;
    //-----------SWORD COUNT VAR
    int swordCount = 0;
    //---------- KEY COUNT VAR
    int keyCount = 0;

    //******************************** MAZEBLOCK DECLARATION **************************************

        ////>>>>>>>>>>>>>>>> BORDER <<<<<<<<<<<<<<<<<<<<< ---------------
        //tot = 196
    mazeBlock MZ_borderVU_left[12]; // --------------- vertical upper
    mazeBlock MZ_borderVU_right[17]; // --------------- vertical upper

    mazeBlock MZ_borderVL_left[17]; // --------------- vertical lower
    mazeBlock MZ_borderVL_right[12]; // --------------- vertical lower

    mazeBlock MZ_borderHU_left[32]; // --------------- horizontal upper
    mazeBlock MZ_borderHU_right[32]; // --------------- horizontal upper

    mazeBlock MZ_borderHL_left[32]; // --------------- horizontal lower
    mazeBlock MZ_borderHL_right[32]; // --------------- horizontal lower
    //--------------------------------------------------------------


    //VERTICAL CENTRE PIECES
    mazeBlock MZcentre_VU[9];
    mazeBlock MZcentre_VL[9];

    //HORIZONTAL CENTRE PIECES 
    mazeBlock MZcentre_H1[8];
    mazeBlock MZcentre_H2[8];
    mazeBlock MZcentre_H3[8];
    mazeBlock MZcentre_H4[8];


    //VERICAL UPPER LINES >>>>>>>>>> 

    //outer lines
    mazeBlock MZ_vu_outermostLeft_small[6];
    mazeBlock MZ_vu_outermostRight_small[6];

    //outer most long lines
    mazeBlock MZ_outerLeft_long[12];
    mazeBlock MZ_outerRight_long[12];

    //outer  upper small lines
    mazeBlock MZ_vu_outerLeft_small[6]; // new
    mazeBlock MZ_vu_outerRight_small[6]; // new


    //(inner middle)
    mazeBlock MZ_vu_innerLeft_small[6]; // new
    mazeBlock MZ_vu_innerRight_small[6]; // new
    mazeBlock MZ_vu_innerLeft_long[12]; // new
    mazeBlock MZ_vu_innerRight_long[12]; // new



    //VERICAL LOWER LINES >>>>>>>>>>

    //outer lines
    mazeBlock MZ_vL_outermostLeft_small[6];
    mazeBlock MZ_vL_outermostRight_small[6];


    //outer lower small lines
    mazeBlock MZ_vL_outerLeft_small[6]; // new
    mazeBlock MZ_vL_outerRight_small[6]; // new

    //(inner middle)
    mazeBlock MZ_vL_innerLeft[6];
    mazeBlock MZ_vL_innerRight[6];



    //HORIZONTAL UPPER LINES >>>>>>>>>>> 

    mazeBlock MZ_hu_inner[6];
    mazeBlock MZ_hL_inner[6];

    //horizontal uppers
    mazeBlock MZ_hu_outer1[6];
    mazeBlock MZ_hu_outer2[6];
    mazeBlock MZ_hu_outer3[6];
    mazeBlock MZ_hu_outer4[8];

    //HORIZONTAL LOWER LINES >>>>>>>>>>> 

    //horizontal downers
    mazeBlock MZ_hL_outer1[6];
    mazeBlock MZ_hL_outer2[6];
    mazeBlock MZ_hL_outer3[6];
    mazeBlock MZ_hL_outer4[8];

    //Arrays to store maze coordinates ----------------------------------------------------------------
    float mazeX[420] = {};
    float mazeY[420] = {};

    int counter = 0;
    int direction = 1; // Variable to store the direction of movement

    //FRIEND MEMBER FUNCTIONS
    friend void pause();
    friend void end_game();
    friend void you_won();
    
    //-------------------------------------INSTRUCTION WINDOW-----------------------------------------
    void instruction()
    {
        RenderWindow window(VideoMode(730, 550), "Instruction");
        Texture obj4;
        obj4.loadFromFile("img/inst.jpg");
        Sprite Menu(obj4);
        Menu.setScale(730.0f / 1080.0f, 550.0f / 934.0f);

        Font font;
        font.loadFromFile("fonty/hiro.ttf");

        Text text4;
        text4.setFont(font);
        text4.setString("KEY CONSTROLS IN THE GAME");
        text4.setPosition(230, 70);
        text4.setCharacterSize(22);
        text4.setStyle(Text::Bold);
        text4.setFillColor(Color::White);

        Text text6;
        text6.setFont(font);
        text6.setString("Press P to pause.");
        text6.setPosition(295, 120);
        text6.setCharacterSize(16);
        text6.setFillColor(Color::White);

        Text text7;
        text7.setFont(font);
        text7.setString("Press ESC to quit.");
        text7.setPosition(294, 160);
        text7.setCharacterSize(16);
        text7.setFillColor(Color::White);

        Text text8;
        text8.setFont(font);
        text8.setString("GAME PLAY");
        text8.setPosition(305, 210);
        text8.setStyle(Text::Bold);
        text8.setCharacterSize(22);
        text8.setFillColor(Color::White);

        Text text9;
        text9.setFont(font);
        text9.setString("Explore the labyrinths corridors and chambers to discover hidden treasures.");
        text9.setPosition(38, 260);
        text9.setCharacterSize(16);
        text9.setFillColor(Color::White);

        Text text10;
        text10.setFont(font);
        text10.setString("Navigate the labyrinth with speed and agility, aiming to find the fastest exit.");
        text10.setPosition(35, 300);
        text10.setCharacterSize(16);
        text10.setFillColor(Color::White);

        Text text11;
        text11.setFont(font);
        text11.setString("HAVE FUN");
        text11.setPosition(312, 360);
        text11.setStyle(Text::Bold);
        text11.setCharacterSize(22);
        text11.setFillColor(Color::White);

        Text texta;
        texta.setFont(font);
        texta.setString("Adventure awaits in the labyrinths depths. Are you ready to conquer its secrets?");
        texta.setPosition(19, 400);
        texta.setCharacterSize(16);
        texta.setFillColor(Color::White);

        Text text5;
        text5.setFont(font);
        text5.setString("PRESS ENTER TO GO BACK TO MENU");
        text5.setPosition(200, 465);
        text5.setStyle(Text::Bold);
        text5.setCharacterSize(22);
        text5.setFillColor(Color::White);

        while (window.isOpen())
        {
            Event ev;
            while (window.pollEvent(ev))
            {
                if (ev.type == Event::Closed)
                {
                    window.close(); // Close the game window
                }
                if (ev.type == Event::KeyPressed)
                {
                    if (ev.key.code == Keyboard::Enter)
                    {
                        return;  // Exit the function and continue the game
                    }
                }
            }

            window.clear();
            window.draw(Menu);
            window.draw(text4);
            window.draw(text6);
            window.draw(text7);
            window.draw(text8);
            window.draw(text9);
            window.draw(text10);
            window.draw(texta);
            window.draw(text11);
            window.draw(text5);

            window.display();
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////

    //GAME CONSTRUCTOR
    Game()
    {
        //Displaying Background
        bg_texture.loadFromFile("img/background.jpg");
        background.setTexture(bg_texture);
        background.setScale(1250.0f / 1080.0f, 780.0f / 942.0f);

        //RANDOM MAZE FUNCTIONALITY >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        //Calling inititialising stuff function
        srand(time(0));
        int num = (rand() % 2) + 1;
        if (num == 1)
            initializeStuff1();

        else
            initializeStuff2();

        // -- >> Player sprite
        if (!playerr_texture.loadFromFile("img/playerr.png")) {
            cerr << "Failed to load player!" << endl;
            return;
        }
        playerr.setTexture(playerr_texture);
        playerr.setPosition(150.f, 170.f);
        playerr.setScale(0.15f, 0.15f);


        // ->> dushman sprites
        dushman_text.loadFromFile("img/enemy1.png");
        dushman.setTexture(dushman_text);
        dushman.setPosition(313.f, 255.f);
        dushman.setScale(0.67f, 0.69f);

        dushman_text2.loadFromFile("img/enemy2.png");
        dushman2.setTexture(dushman_text2);
        dushman2.setPosition(860.f, 265.f);
        dushman2.setScale(0.88f, 0.88f);

        dushman_text3.loadFromFile("img/enemy3.png");
        dushman3.setTexture(dushman_text3);
        dushman3.setPosition(313.f, 515.f);
        dushman3.setScale(0.88f, 0.88f);

        dushman_text4.loadFromFile("img/enemy4.png");
        dushman4.setTexture(dushman_text4);
        dushman4.setPosition(860.f, 515.f);
        dushman4.setScale(0.71f, 0.74f);

    }

    //0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0

    //---------------------------------->>>>>>>>>>>>>>>> COLLISION FUNCTIONS <<<<<<<<<<<<<<<<-----------------------------------------

    float x_pos = 235.f, y_pos = 40.f;

    // Function to check collision between the player and potion
    bool checkCollisionPotion(Sprite& playah, Sprite& pot)
    {
        FloatRect playerBounds = playah.getGlobalBounds();
        FloatRect potBounds = pot.getGlobalBounds();

        // Check if the bounding boxes of the player and food intersect
        if (playerBounds.intersects(potBounds))
        {
            x_pos += 30;
            pot.setPosition(x_pos, y_pos); // Move the potion out of the visible area
            Node* remNode = nullptr;
            mazeInventree.root = mazeInventree.deleteNode(root, 30, remNode);
            if (remNode != nullptr)
            {
                remNode->pos_x = x_pos;
                remNode->pos_y = y_pos;
                /*remNode->sprite.setPosition(235.f, 40.f);
                remNode->sprite.setScale(0.36f, 0.36f);*/
                inventree.insert(remNode->obj_points, remNode->obj_initial, remNode->sprite, remNode->pos_x, remNode->pos_y);
                //window.draw(remNode->sprite);
                
            }
            x_pos += 30;

            // Return true to indicate that a collision was detected
            return true;
            
        }

        // Return false if no collision occurred
        return false;
    }

    //function to check collision btw player and sword
    bool checkCollisionTalwar(Sprite& banda, Sprite& talwar)
    {
        FloatRect playerBounds = banda.getGlobalBounds();
        FloatRect swordB = talwar.getGlobalBounds();

        // Check if the bounding boxes of the player and food intersect
        if (playerBounds.intersects(swordB))
        {
            talwar.setPosition(x_pos, y_pos); 
            Node* remNode = nullptr;
            mazeInventree.root = mazeInventree.deleteNode(root, 30, remNode);
            if (remNode != nullptr)
            {
                remNode->pos_x = x_pos;
                remNode->pos_y = y_pos;
                /*remNode->sprite.setPosition(235.f, 40.f);
                remNode->sprite.setScale(0.36f, 0.36f);*/
                inventree.insert(remNode->obj_points, remNode->obj_initial, remNode->sprite, remNode->pos_x, remNode->pos_y);
                //window.draw(remNode->sprite);
            }
            x_pos += 15;
            // Return true to indicate that a collision was detected
            return true;
            
        }

        // Return false if no collision occurred
        return false;
    }

    //function to check collision btw player and enemy
    bool checkCollisionDushman(Sprite& banda, Sprite& dushman)
    {
        FloatRect playerBounds = banda.getGlobalBounds();
        FloatRect enemBounds = dushman.getGlobalBounds();

        // Check if the bounding boxes of the player and enemy intersect

        if (playerBounds.intersects(enemBounds))
        {
            dushman.setPosition(-1000.f, -1000.f); // Move the enemy out of the visible area

            // Return true to indicate that a collision was detected
            return true;
        }

        // Return false if no collision occurred
        return false;
    }

    // Function to check collision between the player's player and a GEM
    bool checkCollisionGem(Sprite& player, Sprite& gem)
    {
        FloatRect playerBounds = player.getGlobalBounds();
        FloatRect gemBounds = gem.getGlobalBounds();

        // Check if the boundingboxes of the player and gem intersect
        if (playerBounds.intersects(gemBounds))
        {
            x_pos += 25;
            gem.setPosition(x_pos, y_pos);
            Node* remNode = nullptr;
            mazeInventree.root = mazeInventree.deleteNode(root, 20, remNode);
            if (remNode != nullptr)
            {
                remNode->pos_x = x_pos;
                remNode->pos_y = y_pos;
                /*remNode->sprite.setPosition(235.f, 40.f);
                remNode->sprite.setScale(0.36f, 0.36f);*/
                inventree.insert(remNode->obj_points, remNode->obj_initial, remNode->sprite, remNode->pos_x, remNode->pos_y);
                //window.draw(remNode->sprite);
            }
            x_pos += 15;
            // Return true to indicate that a collision was detected
            return true;
        }

        // Return false if no collision occurred
        return false;
    }

    // Function to check collision between the player's plyaer and a GEM
    bool checkCollisionTres(Sprite& banda, Sprite& treasure)
    {
        FloatRect playerBounds = banda.getGlobalBounds();
        FloatRect treasureB = treasure.getGlobalBounds();

        // Check if the bounding boxes of the player and enemy intersect

        if (playerBounds.intersects(treasureB))
        {
            treasure.setPosition(-1000.f, -1000.f); // Move the enemy out of the visible area

            // Return true to indicate that a collision was detected
            return true;
        }

        // Return false if no collision occurred
        return false;
    }

    // Function to check collision between the player's plyaer and a GEM
    bool checkCollisionKee(Sprite& banda, Sprite& kee)
    {
        FloatRect playerBounds = banda.getGlobalBounds();
        FloatRect keyBounds = kee.getGlobalBounds();

        // Check if the bounding boxes of the player and enemy intersect

        if (playerBounds.intersects(keyBounds))
        {
            kee.setPosition(x_pos, y_pos);
            Node* remNode = nullptr;
            mazeInventree.root = mazeInventree.deleteNode(root, 30, remNode);
            if (remNode != nullptr)
            {
                remNode->pos_x = x_pos;
                remNode->pos_y = y_pos;
                /*remNode->sprite.setPosition(235.f, 40.f);
                remNode->sprite.setScale(0.36f, 0.36f);*/
                inventree.insert(remNode->obj_points, remNode->obj_initial, remNode->sprite, remNode->pos_x, remNode->pos_y);
                //window.draw(remNode->sprite);
            }
            x_pos += 15;
            // Return true to indicate that a collision was detected
            return true;

        }

        // Return false if no collision occurred
        return false;
    }

    //Function to check collision between the player's player and enemy player
    bool checkCollisionEP(Sprite& player, Sprite& enem, Enemy& E1)
    {
        FloatRect playerBounds = player.getGlobalBounds();
        FloatRect enemBounds = enem.getGlobalBounds();

        //Check if the bounding boxes of the player and food intersect
        if (playerBounds.intersects(enemBounds))
        {
            //set the position od player and sprite to its initial pos
            enem.setPosition(-1000.f, -1000.f);
            E1.shape.setPosition(-1000.f, -1000.f);

            // Return true to indicate that a collision was detected
            return true;
        }

        // Return false if no collision occurred
        return false;
    }

    //---------------------------------------------FUNCTION TO DISPLAY SCORE ON THE GAME WINDOW-------------------------------------
    void displayScore(RenderWindow& window, int score)
    {
        Font fonty;
        fonty.loadFromFile("fonty/hiro.ttf");

        Text scoreDis;
        scoreDis.setFont(fonty);
        scoreDis.setString("Score: " + to_string(score));
        scoreDis.setPosition(820, 45);
        scoreDis.setCharacterSize(30);
        scoreDis.setStyle(Text::Style::Bold);
        scoreDis.setFillColor(Color::White);

        window.draw(scoreDis);
    }



    //0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0

    void level1();


    void initializeStuff1()
    {

        mazeInventree.insert(500, 'T', khazana, 1063, 415);   //treasure
        mazeInventree.insert(20, 'G', heera, 585, 380);   //gem

        mazeInventree.insert(20, 'K', chabi, 365, 175);   //key 1
        mazeInventree.insert(21, 'K', chabi2, 795, 178);   //key 2

        mazeInventree.insert(10, 'S', talwar, 240, 440);   //sword 1
        mazeInventree.insert(11, 'S', talwar2, 690, 490);   //sword 2
        mazeInventree.insert(12, 'S', talwar3, 1010, 250);   //sword 3
        mazeInventree.insert(13, 'S', talwar4, 980, 590);   //sword 4

        mazeInventree.insert(30, 'P', potion1, 465, 425);      //potion 1
        mazeInventree.insert(31, 'P', potion2, 710, 340);   //potion 2

        //Initialising Player and Enemy objects
        P = Player(150.f, 170.f, 15.f, 15.f, 1); // Initial direction is 1 (right)
        E1 = Enemy(313.f, 255.f, 15.f, 15.f, 1);
        E2 = Enemy(860.f, 265.f, 15.f, 15.f, 1);
        E3 = Enemy(313.f, 515.f, 15.f, 15.f, 1);
        E4 = Enemy(860.f, 515.f, 15.f, 15.f, 1);


        //--------------------------------------------- >>>>> LOAD SPRITES

        // ->> potion sprites
        if (!potion_text1.loadFromFile("img/potion.png")) {
            cerr << "Failed to load potion sprite!" << endl;
            return; // Return from the function or handle the error accordingly
        }



        potion1.setTexture(potion_text1);
        potion1.setPosition(465.f, 425.f);
        potion1.setScale(0.26f, 0.26f);

        potion_text2.loadFromFile("img/potion.png");
        potion2.setTexture(potion_text2);
        potion2.setPosition(710.f, 340.f);
        potion2.setScale(0.26f, 0.26f);


        // ->> talwaar sprite
        sword_text.loadFromFile("img/talwaar.png");
        talwar.setTexture(sword_text);
        talwar.setPosition(240.f, 440.f);
        talwar.setScale(0.17f, 0.14f);

        talwar2.setTexture(sword_text);
        talwar2.setPosition(690.f, 490.f);
        talwar2.setScale(0.17f, 0.14f);

        talwar3.setTexture(sword_text);
        talwar3.setPosition(795.f, 178.f); //795, 178
        talwar3.setScale(0.17f, 0.14f);

        talwar4.setTexture(sword_text);
        talwar4.setPosition(980.f, 590.f);
        talwar4.setScale(0.17f, 0.14f);


        // ->> key sprite
        chabi_text.loadFromFile("img/key.png");
        chabi.setTexture(chabi_text);
        chabi.setPosition(365.f, 175.f);
        chabi.setScale(0.12f, 0.1f);

        chabi2.setTexture(chabi_text);
        chabi2.setPosition(1010.f, 250.f); //1010,250
        chabi2.setScale(0.12f, 0.1f);

        // ->> treasure sprite
        treasure_text.loadFromFile("img/treasure.png");
        khazana.setTexture(treasure_text);
        khazana.setPosition(1063.f, 415.f);
        khazana.setScale(0.26f, 0.24f);

        // ->> gem sprite
        gem_text.loadFromFile("img/gem2.png");
        heera.setTexture(gem_text);
        heera.setPosition(585.f, 380.f);
        heera.setScale(0.12f, 0.12f);


        // ->> khula khazana
        op_treasure.loadFromFile("img/open_treasure.png");
        khulaKhazana.setTexture(op_treasure);
        khulaKhazana.setPosition(1063.f, 415.f);
        khulaKhazana.setScale(0.26f, 0.24f);


        //**************************** MAZE INITIALISED HERE ************************************
        {
            float tester = 150;


            //----------------------<<<<<<<<<< BORDER >>>>>>>>>>>>>---------------------------------
            {
                // --------- vertical upper -------------

                tester = 240;
                for (int i = 0; i < 12; i++, tester += 15)//left 2nd line
                {

                    MZ_borderVU_left[i] = mazeBlock(140.f, tester, 15.f, 15.f);
                    mazeX[counter] = 140.f;
                    mazeY[counter] = tester;
                    ++counter;
                }
                tester = 150;
                for (int i = 0; i < 17; i++, tester += 15)//right 2nd line
                {
                    MZ_borderVU_right[i] = mazeBlock(1090.f, tester, 15.f, 15.f);
                    mazeX[counter] = 1090.f;
                    mazeY[counter] = tester;
                    ++counter;
                }

                //----------------- vertical lower -------------------

                tester = 420;
                for (int i = 0; i < 17; i++, tester += 15)//left 2nd line
                {
                    MZ_borderVL_left[i] = mazeBlock(140.f, tester, 15.f, 15.f);
                    mazeX[counter] = 140.f;
                    mazeY[counter] = tester;
                    ++counter;
                }
                tester = 480;
                for (int i = 0; i < 12; i++, tester += 15)//right 2nd line
                {
                    MZ_borderVL_right[i] = mazeBlock(1090.f, tester, 15.f, 15.f);
                    mazeX[counter] = 1090.f;
                    mazeY[counter] = tester;
                    ++counter;
                }

                //----------------- horizontal upper --------------------------

                tester = 140; //x=140
                for (int i = 0; i < 32; i++, tester += 15)//left 2nd line
                {
                    MZ_borderHU_left[i] = mazeBlock(tester, 150.f, 15.f, 15.f);
                    mazeX[counter] = tester;
                    mazeY[counter] = 150;
                    ++counter;
                }
                tester = 615;
                for (int i = 0; i < 32; i++, tester += 15)
                {
                    MZ_borderHU_right[i] = mazeBlock(tester, 150.f, 15.f, 15.f);
                    mazeX[counter] = tester;
                    mazeY[counter] = 150;
                    ++counter;
                }


                //------------------ horizontal lower ------------------------------

                tester = 145;
                for (int i = 0; i < 32; i++, tester += 15)
                {
                    MZ_borderHL_left[i] = mazeBlock(tester, 660.f, 15.f, 15.f);
                    mazeX[counter] = tester;
                    mazeY[counter] = 660;
                    ++counter;
                }
                tester = 625;
                for (int i = 0; i < 32; i++, tester += 15)
                {
                    MZ_borderHL_right[i] = mazeBlock(tester, 660.f, 15.f, 15.f);
                    mazeX[counter] = tester;
                    mazeY[counter] = 660;
                    ++counter;
                }
            }
            //---------------------------------------------------------------------------------------------------

            //VERTICAL CENTRE PIECES
            tester = 200;
            for (int i = 0; i < 9; i++, tester += 15)
            {
                MZcentre_VU[i] = mazeBlock(610.f, tester, 15.f, 15.f);
                mazeX[counter] = 610.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 500;
            for (int i = 0; i < 9; i++, tester += 15)
            {
                MZcentre_VL[i] = mazeBlock(610.f, tester, 15.f, 15.f);
                mazeX[counter] = 610.f;
                mazeY[counter] = tester;
                ++counter;
            }

            //HORIZONTAL CENTRE PIECES
            tester = 230;
            for (int i = 0; i < 8; i++, tester += 15)
            {
                MZcentre_H1[i] = mazeBlock(tester, 410.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 410;
                ++counter;
            }
            tester = 430;
            for (int i = 0; i < 8; i++, tester += 15)
            {
                MZcentre_H2[i] = mazeBlock(tester, 410.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 410;
                ++counter;
            }
            tester = 690;
            for (int i = 0; i < 8; i++, tester += 15)
            {
                MZcentre_H3[i] = mazeBlock(tester, 410.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 410;
                ++counter;
            }
            tester = 880;
            for (int i = 0; i < 8; i++, tester += 15)
            {
                MZcentre_H4[i] = mazeBlock(tester, 410.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 410;
                ++counter;
            }


            //VERICAL UPPER LINES >>>>>>>>>>>>>>>>>>>>>

            //outermost upper small lines
            tester = 230;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vu_outermostLeft_small[i] = mazeBlock(240.f, tester, 15.f, 15.f);
                mazeX[counter] = 240.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 230;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vu_outermostRight_small[i] = mazeBlock(980.f, tester, 15.f, 15.f);
                mazeX[counter] = 980.f;
                mazeY[counter] = tester;
                ++counter;
            }

            //outermost upper long lines
            tester = 320;
            for (int i = 0; i < 12; i++, tester += 15)
            {
                MZ_outerLeft_long[i] = mazeBlock(340.f, tester, 15.f, 15.f);
                mazeX[counter] = 340.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 320;
            for (int i = 0; i < 12; i++, tester += 15)
            {
                MZ_outerRight_long[i] = mazeBlock(880.f, tester, 15.f, 15.f);
                mazeX[counter] = 880.f;
                mazeY[counter] = tester;
                ++counter;
            }


            // outer upper small lines
            tester = 165;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vu_outerLeft_small[i] = mazeBlock(340.f, tester, 15.f, 15.f);
                mazeX[counter] = 340.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 165;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vu_outerRight_small[i] = mazeBlock(880.f, tester, 15.f, 15.f);
                mazeX[counter] = 880.f;
                mazeY[counter] = tester;
                ++counter;
            }

            // inner middle lines              
            tester = 230;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vu_innerLeft_small[i] = mazeBlock(440.f, tester, 15.f, 15.f);
                mazeX[counter] = 440.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 260;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vu_innerRight_small[i] = mazeBlock(780.f, tester, 15.f, 15.f);
                mazeX[counter] = 780.f;
                mazeY[counter] = tester;
                ++counter;
            }

            tester = 320;
            for (int i = 0; i < 12; i++, tester += 15)
            {
                MZ_vu_innerLeft_long[i] = mazeBlock(540.f, tester, 15.f, 15.f);
                mazeX[counter] = 540.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 320;
            for (int i = 0; i < 12; i++, tester += 15)
            {
                MZ_vu_innerRight_long[i] = mazeBlock(680.f, tester, 15.f, 15.f);
                mazeX[counter] = 680.f;
                mazeY[counter] = tester;
                ++counter;
            }



            //VERTICAL LOWER LINES >>>>>>>>>>>>>>>>>>>>>
            //outermost small lines
            tester = 500;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vL_outermostLeft_small[i] = mazeBlock(240.f, tester, 15.f, 15.f);
                mazeX[counter] = 240.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 470;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vL_outermostRight_small[i] = mazeBlock(970.f, tester, 15.f, 15.f);
                mazeX[counter] = 970.f;
                mazeY[counter] = tester;
                ++counter;
            }


            //vertical outer 
            tester = 570;
            for (int i = 0; i < 6; i++, tester += 15)//left 3rd line
            {
                MZ_vL_outerLeft_small[i] = mazeBlock(340.f, tester, 15.f, 15.f);
                mazeX[counter] = 340.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 570;
            for (int i = 0; i < 6; i++, tester += 15)//right 3rd line
            {
                MZ_vL_outerRight_small[i] = mazeBlock(880.f, tester, 15.f, 15.f);
                mazeX[counter] = 880.f;
                mazeY[counter] = tester;
                ++counter;
            }

            // inner middle lines       
            tester = 465;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vL_innerLeft[i] = mazeBlock(440.f, tester, 15.f, 15.f);
                mazeX[counter] = 440.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 470;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vL_innerRight[i] = mazeBlock(780.f, tester, 15.f, 15.f);
                mazeX[counter] = 780.f;
                mazeY[counter] = tester;
                ++counter;
            }


            //HORIZONTAL UPPER LINES >>>>>>>>>>>>>>>>>>>>>>>>>


            //horizontal outer upper
            tester = 200;
            for (int i = 0; i < 6; i++, tester += 15)//left 4th line
            {
                MZ_hu_outer1[i] = mazeBlock(tester, 225.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 225;
                ++counter;
            }
            tester = 440;
            for (int i = 0; i < 6; i++, tester += 15)//right 4th line
            {
                MZ_hu_outer2[i] = mazeBlock(tester, 220.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 220;
                ++counter;
            }
            tester = 620;
            for (int i = 0; i < 6; i++, tester += 15)//left 4th line
            {
                MZ_hu_outer3[i] = mazeBlock(tester, 250.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 250;
                ++counter;
            }
            tester = 980;
            for (int i = 0; i < 8; i++, tester += 15)//right 4th line
            {
                MZ_hu_outer4[i] = mazeBlock(tester, 220.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 220;
                ++counter;
            }


            //horizontal inner
            tester = 400;
            for (int i = 0; i < 6; i++, tester += 15)//left 4th line
            {
                MZ_hu_inner[i] = mazeBlock(tester, 320.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 320;
                ++counter;
            }


            //HORIZONTAL LOWER LINES >>>>>>>>>>>>>>>>>>>>>>>>>

            //horizontal outer lower

            tester = 200;
            for (int i = 0; i < 6; i++, tester += 15)//left 4th line
            {
                MZ_hL_outer1[i] = mazeBlock(tester, 595.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 595;
                ++counter;
            }
            tester = 530;
            for (int i = 0; i < 6; i++, tester += 15)//right 4th line
            {
                MZ_hL_outer2[i] = mazeBlock(tester, 560.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 560;
                ++counter;
            }
            tester = 705;
            for (int i = 0; i < 6; i++, tester += 15)//left 4th line
            {
                MZ_hL_outer3[i] = mazeBlock(tester, 560.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 560;
                ++counter;
            }
            tester = 970;
            for (int i = 0; i < 8; i++, tester += 15)//right 4th line
            {
                MZ_hL_outer4[i] = mazeBlock(tester, 560.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 560;
                ++counter;
            }
            //horizontal inner
            tester = 730;
            for (int i = 0; i < 6; i++, tester += 15)//right 4th line
            {
                MZ_hL_inner[i] = mazeBlock(tester, 470.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 470;
                ++counter;
            }
        }
    }

    void initializeStuff2()
    {

        mazeInventree.insert(500, 'T', khazana, 1063, 415);   //treasure
        mazeInventree.insert(20, 'G', heera, 585, 380);   //gem

        mazeInventree.insert(20, 'K', chabi, 365, 175);   //key 1
        mazeInventree.insert(21, 'K', chabi2, 795, 178);   //key 2

        mazeInventree.insert(10, 'S', talwar, 240, 440);   //sword 1
        mazeInventree.insert(11, 'S', talwar2, 690, 490);   //sword 2
        mazeInventree.insert(12, 'S', talwar3, 1010, 250);   //sword 3
        mazeInventree.insert(13, 'S', talwar4, 980, 590);   //sword 4

        mazeInventree.insert(30, 'P', potion1, 465, 425);      //potion 1
        mazeInventree.insert(31, 'P', potion2, 710, 340);   //potion 2


        //Initialising Player and Enemy objects
        P = Player(150.f, 170.f, 15.f, 15.f, 1); // Initial direction is 1 (right)
        E1 = Enemy(313.f, 255.f, 15.f, 15.f, 1);
        E2 = Enemy(860.f, 265.f, 15.f, 15.f, 1);
        E3 = Enemy(313.f, 515.f, 15.f, 15.f, 1);
        E4 = Enemy(860.f, 515.f, 15.f, 15.f, 1);




        //--------------------------------------------- >>>>> LOAD SPRITES

        // ->> potion sprites
        if (!potion_text1.loadFromFile("img/potion.png")) {
            cerr << "Failed to load potion sprite!" << endl;
            return; // Return from the function or handle the error accordingly
        }



        potion1.setTexture(potion_text1);
        potion1.setPosition(465.f, 425.f);
        potion1.setScale(0.26f, 0.26f);

        potion_text2.loadFromFile("img/potion.png");
        potion2.setTexture(potion_text2);
        potion2.setPosition(710.f, 340.f);
        potion2.setScale(0.26f, 0.26f);


        // ->> talwaar sprite
        sword_text.loadFromFile("img/talwaar.png");
        talwar.setTexture(sword_text);
        talwar.setPosition(240.f, 440.f);
        talwar.setScale(0.17f, 0.14f);

        talwar2.setTexture(sword_text);
        talwar2.setPosition(690.f, 490.f);
        talwar2.setScale(0.17f, 0.14f);

        talwar3.setTexture(sword_text);
        talwar3.setPosition(1010.f, 250.f); //770, 178
        talwar3.setScale(0.17f, 0.14f);

        talwar4.setTexture(sword_text);
        talwar4.setPosition(980.f, 590.f);
        talwar4.setScale(0.17f, 0.14f);


        // ->> key sprite
        chabi_text.loadFromFile("img/key.png");
        chabi.setTexture(chabi_text);
        chabi.setPosition(365.f, 175.f);
        chabi.setScale(0.12f, 0.1f);

        chabi2.setTexture(chabi_text);
        chabi2.setPosition(795.f, 178.f); //1010,250
        chabi2.setScale(0.12f, 0.1f);

        // ->> treasure sprite
        treasure_text.loadFromFile("img/treasure.png");
        khazana.setTexture(treasure_text);
        khazana.setPosition(1063.f, 415.f);
        khazana.setScale(0.26f, 0.24f);

        // ->> gem sprite
        gem_text.loadFromFile("img/gem2.png");
        heera.setTexture(gem_text);
        heera.setPosition(585.f, 380.f);
        heera.setScale(0.12f, 0.12f);




        //**************************** MAZE INITIALISED HERE ************************************
        {
            float tester = 150;


            //----------------------<<<<<<<<<< BORDER >>>>>>>>>>>>>---------------------------------
            {
                // --------- vertical upper -------------

                tester = 240;
                for (int i = 0; i < 12; i++, tester += 15)//left 2nd line
                {

                    MZ_borderVU_left[i] = mazeBlock(140.f, tester, 15.f, 15.f);
                    mazeX[counter] = 140.f;
                    mazeY[counter] = tester;
                    ++counter;
                }
                tester = 150;
                for (int i = 0; i < 17; i++, tester += 15)//right 2nd line
                {
                    MZ_borderVU_right[i] = mazeBlock(1090.f, tester, 15.f, 15.f);
                    mazeX[counter] = 1090.f;
                    mazeY[counter] = tester;
                    ++counter;
                }

                //----------------- vertical lower -------------------

                tester = 420;
                for (int i = 0; i < 17; i++, tester += 15)//left 2nd line
                {
                    MZ_borderVL_left[i] = mazeBlock(140.f, tester, 15.f, 15.f);
                    mazeX[counter] = 140.f;
                    mazeY[counter] = tester;
                    ++counter;
                }
                tester = 480;
                for (int i = 0; i < 12; i++, tester += 15)//right 2nd line
                {
                    MZ_borderVL_right[i] = mazeBlock(1090.f, tester, 15.f, 15.f);
                    mazeX[counter] = 1090.f;
                    mazeY[counter] = tester;
                    ++counter;
                }

                //----------------- horizontal upper --------------------------

                tester = 140; //x=140
                for (int i = 0; i < 32; i++, tester += 15)//left 2nd line
                {
                    MZ_borderHU_left[i] = mazeBlock(tester, 150.f, 15.f, 15.f);
                    mazeX[counter] = tester;
                    mazeY[counter] = 150;
                    ++counter;
                }
                tester = 615;
                for (int i = 0; i < 32; i++, tester += 15)
                {
                    MZ_borderHU_right[i] = mazeBlock(tester, 150.f, 15.f, 15.f);
                    mazeX[counter] = tester;
                    mazeY[counter] = 150;
                    ++counter;
                }


                //------------------ horizontal lower ------------------------------

                tester = 145;
                for (int i = 0; i < 32; i++, tester += 15)
                {
                    MZ_borderHL_left[i] = mazeBlock(tester, 660.f, 15.f, 15.f);
                    mazeX[counter] = tester;
                    mazeY[counter] = 660;
                    ++counter;
                }
                tester = 625;
                for (int i = 0; i < 32; i++, tester += 15)
                {
                    MZ_borderHL_right[i] = mazeBlock(tester, 660.f, 15.f, 15.f);
                    mazeX[counter] = tester;
                    mazeY[counter] = 660;
                    ++counter;
                }
            }
            //---------------------------------------------------------------------------------------------------

            //VERTICAL CENTRE PIECES
            tester = 200;
            for (int i = 0; i < 9; i++, tester += 15)
            {
                MZcentre_VU[i] = mazeBlock(610.f, tester, 15.f, 15.f);
                mazeX[counter] = 610.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 470;
            for (int i = 0; i < 9; i++, tester += 15)
            {
                MZcentre_VL[i] = mazeBlock(610.f, tester, 15.f, 15.f);
                mazeX[counter] = 610.f;
                mazeY[counter] = tester;
                ++counter;
            }

            //HORIZONTAL CENTRE PIECES
            tester = 230;
            for (int i = 0; i < 8; i++, tester += 15)
            {
                MZcentre_H1[i] = mazeBlock(tester, 410.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 410;
                ++counter;
            }
            tester = 430;
            for (int i = 0; i < 8; i++, tester += 15)
            {
                MZcentre_H2[i] = mazeBlock(tester, 485.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 485;
                ++counter;
            }
            tester = 690;
            for (int i = 0; i < 8; i++, tester += 15)
            {
                MZcentre_H3[i] = mazeBlock(tester, 410.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 410;
                ++counter;
            }
            tester = 880;
            for (int i = 0; i < 8; i++, tester += 15)
            {
                MZcentre_H4[i] = mazeBlock(tester, 410.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 410;
                ++counter;
            }


            //VERTICAL UPPER LINES >>>>>>>>>>>>>>>>>>>>>

            //outermost upper small lines
            tester = 240;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vu_outermostLeft_small[i] = mazeBlock(240.f, tester, 15.f, 15.f);
                mazeX[counter] = 240.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 230;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vu_outermostRight_small[i] = mazeBlock(980.f, tester, 15.f, 15.f);
                mazeX[counter] = 980.f;
                mazeY[counter] = tester;
                ++counter;
            }

            //outermost upper long lines
            tester = 320;
            for (int i = 0; i < 12; i++, tester += 15)
            {
                MZ_outerLeft_long[i] = mazeBlock(340.f, tester, 15.f, 15.f);
                mazeX[counter] = 340.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 320;
            for (int i = 0; i < 12; i++, tester += 15)
            {
                MZ_outerRight_long[i] = mazeBlock(880.f, tester, 15.f, 15.f);
                mazeX[counter] = 880.f;
                mazeY[counter] = tester;
                ++counter;
            }


            // outer upper small lines
            tester = 165;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vu_outerLeft_small[i] = mazeBlock(340.f, tester, 15.f, 15.f);
                mazeX[counter] = 340.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 165;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vu_outerRight_small[i] = mazeBlock(880.f, tester, 15.f, 15.f);
                mazeX[counter] = 880.f;
                mazeY[counter] = tester;
                ++counter;
            }

            // inner middle lines              
            tester = 230;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vu_innerLeft_small[i] = mazeBlock(440.f, tester, 15.f, 15.f);
                mazeX[counter] = 440.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 320;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vu_innerRight_small[i] = mazeBlock(690.f, tester, 15.f, 15.f);
                mazeX[counter] = 690.f;
                mazeY[counter] = tester;
                ++counter;
            }

            tester = 320;
            for (int i = 0; i < 12; i++, tester += 15)
            {
                MZ_vu_innerLeft_long[i] = mazeBlock(540.f, tester, 15.f, 15.f);
                mazeX[counter] = 540.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 150;
            for (int i = 0; i < 12; i++, tester += 15)
            {
                MZ_vu_innerRight_long[i] = mazeBlock(780.f, tester, 15.f, 15.f);
                mazeX[counter] = 780.f;
                mazeY[counter] = tester;
                ++counter;
            }



            //VERTICAL LOWER LINES >>>>>>>>>>>>>>>>>>>>>
            //outermost small lines
            tester = 495;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vL_outermostLeft_small[i] = mazeBlock(240.f, tester, 15.f, 15.f);
                mazeX[counter] = 240.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 470;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vL_outermostRight_small[i] = mazeBlock(970.f, tester, 15.f, 15.f);
                mazeX[counter] = 970.f;
                mazeY[counter] = tester;
                ++counter;
            }


            //vertical outer 
            tester = 570;
            for (int i = 0; i < 6; i++, tester += 15)//left 3rd line
            {
                MZ_vL_outerLeft_small[i] = mazeBlock(340.f, tester, 15.f, 15.f);
                mazeX[counter] = 340.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 570;
            for (int i = 0; i < 6; i++, tester += 15)//right 3rd line
            {
                MZ_vL_outerRight_small[i] = mazeBlock(880.f, tester, 15.f, 15.f);
                mazeX[counter] = 880.f;
                mazeY[counter] = tester;
                ++counter;
            }

            // inner middle lines       
            tester = 485;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vL_innerLeft[i] = mazeBlock(430.f, tester, 15.f, 15.f);
                mazeX[counter] = 430.f;
                mazeY[counter] = tester;
                ++counter;
            }
            tester = 470;
            for (int i = 0; i < 6; i++, tester += 15)
            {
                MZ_vL_innerRight[i] = mazeBlock(780.f, tester, 15.f, 15.f);
                mazeX[counter] = 780.f;
                mazeY[counter] = tester;
                ++counter;
            }


            //HORIZONTAL UPPER LINES >>>>>>>>>>>>>>>>>>>>>>>>>


            //horizontal outer upper
            tester = 150;
            for (int i = 0; i < 6; i++, tester += 15)//left 4th line
            {
                MZ_hu_outer1[i] = mazeBlock(tester, 240.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 240;
                ++counter;
            }
            tester = 440;
            for (int i = 0; i < 6; i++, tester += 15)//right 4th line
            {
                MZ_hu_outer2[i] = mazeBlock(tester, 220.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 220;
                ++counter;
            }
            tester = 620;
            for (int i = 0; i < 6; i++, tester += 15)//left 4th line
            {
                MZ_hu_outer3[i] = mazeBlock(tester, 250.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 250;
                ++counter;
            }
            tester = 980;
            for (int i = 0; i < 8; i++, tester += 15)//right 4th line
            {
                MZ_hu_outer4[i] = mazeBlock(tester, 220.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 220;
                ++counter;
            }


            //horizontal inner
            tester = 400;
            for (int i = 0; i < 6; i++, tester += 15)//left 4th line
            {
                MZ_hu_inner[i] = mazeBlock(tester, 400.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 400;
                ++counter;
            }


            //HORIZONTAL LOWER LINES >>>>>>>>>>>>>>>>>>>>>>>>>

            //horizontal outer lower

            tester = 150;
            for (int i = 0; i < 6; i++, tester += 15)//left 4th line
            {
                MZ_hL_outer1[i] = mazeBlock(tester, 570.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 570;
                ++counter;
            }
            tester = 530;
            for (int i = 0; i < 6; i++, tester += 15)//right 4th line
            {
                MZ_hL_outer2[i] = mazeBlock(tester, 560.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 560;
                ++counter;
            }
            tester = 705;
            for (int i = 0; i < 6; i++, tester += 15)//left 4th line
            {
                MZ_hL_outer3[i] = mazeBlock(tester, 560.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 560;
                ++counter;
            }
            tester = 970;
            for (int i = 0; i < 8; i++, tester += 15)//right 4th line
            {
                MZ_hL_outer4[i] = mazeBlock(tester, 560.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 560;
                ++counter;
            }
            //horizontal inner
            tester = 795;
            for (int i = 0; i < 6; i++, tester += 15)//right 4th line
            {
                MZ_hL_inner[i] = mazeBlock(tester, 315.f, 15.f, 15.f);
                mazeX[counter] = tester;
                mazeY[counter] = 315;
                ++counter;
            }
        }
    }
    

    void start_game()//************************************************************* START GAME  ************************************************************************
    {

        level1();

    }

};


void Game::level1()
{

    srand(time(0));
    RenderWindow window(VideoMode(1250, 780), title);
    Clock clock;
    float timer = 0;
    bool gameover = false;

    //--------------------------------------------- >>>>> LOAD SPRITES

    // ->> inventory rectangle
    RectangleShape rectangle(Vector2f(310.f, 70.f));
    rectangle.setPosition(230.f, 30.f); // Set position
    rectangle.setOutlineThickness(5);    // Set outline thickness
    rectangle.setOutlineColor(Color::White); // Set outline color
    rectangle.setFillColor(Color::Transparent);

    // -> gem points text
    Font ff;
    ff.loadFromFile("fonty/hiro.ttf");
    Text pointsText;
    pointsText.setFont(ff);
    pointsText.setString("BONUS!");
    pointsText.setPosition(575.f, 340.f);
    pointsText.setCharacterSize(25);
   // pointsText.setStyle(pointsText.Bold);
    pointsText.setFillColor(Color(255, 215, 0));



    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Clock collisionTimer;
        bool collisionOccurred = false;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
            ////------------------------------------------PAUSE  CALL-----------------------------------
            if (e.key.code == sf::Keyboard::P)
            {
                pause();
            }
            if (e.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
        }

        //------------------ARTIFACT COLLISION CONDITIONS------------------------------

        // ->> potion collision
        if (checkCollisionPotion(playerr, potion1)) {
            //Increase the score
            score += 30;
        }if (checkCollisionPotion(playerr, potion2)) {
            //Increase the score
            score += 31;
        }

        // ->> sword collision
        if (checkCollisionTalwar(playerr, talwar)) {
            swordCount += 1;
            score += 10;
        }if (checkCollisionTalwar(playerr, talwar2)) {
            swordCount += 1;
            score += 11;
        }if (checkCollisionTalwar(playerr, talwar3)) {
            swordCount += 1;
            score += 12;
        }if (checkCollisionTalwar(playerr, talwar4)) {
            swordCount += 1;
            score += 13;
        }

        // ->> key collision
        if (checkCollisionKee(playerr, chabi)) {
            keyCount += 1;
            score += 20;
        }if (checkCollisionKee(playerr, chabi2)) {
            keyCount += 1;
            score += 21;
        }

        // ->> treasure collision
        if (checkCollisionTres(playerr, khazana)) {
            //if (keyCount == 1 ) {
            //    score += 500;
            //    //keyCount -= 1;
            //    chabi.setPosition(-1000.f, -1000.f);

            //}
            //else if (keyCount == 2) {
            //    
            //    chabi2.setPosition(-1000.f, -1000.f);
            //    chabi.setPosition(-1000.f, -1000.f);
            //    khazana.setPosition(-1000.f, -1000.f);
            //    gameover = true;
            //    
            //    //end_game();
            //}
            window.draw(khulaKhazana);
            you_won();
            //else {
            //    //gameover = true;
            //    chabi2.setPosition(-1000.f, -1000.f);
            //    chabi.setPosition(-1000.f, -1000.f);
            //    end_game();
            //}
            
            //end_game();
        }

        
        bool collisionHandled = false;

        if (checkCollisionEP(playerr, dushman, E1) && !collisionHandled) {
            if (swordCount != 0) {
                score += 5;
            }
            else {
                score -= 5;
                swordCount--;
            }
            collisionHandled = true;
        }
        collisionHandled = false;
        if (checkCollisionEP(playerr, dushman2, E2) && !collisionHandled) {
            if (swordCount != 0) {
                score -= 5;
            }
            else {
                score += 5;
                swordCount--;
            }
            collisionHandled = true;
        }
        collisionHandled = false;
        if (checkCollisionEP(playerr, dushman3, E3) && !collisionHandled) {
            if (swordCount != 0) {
                score -= 5;
            }
            else {
                score += 5;
                swordCount--;
            }
            collisionHandled = true;
        }
        collisionHandled = false;
        if (checkCollisionEP(playerr, dushman4, E4) && !collisionHandled) {
            if (swordCount != 0) {
                score -= 5;
            }
            else {
                score += 5;
                swordCount--;
            }
            collisionHandled = true;
        }

        // Inside the collision detection block
        if (checkCollisionGem(playerr, heera))
        {
            score += 20;
            pointsText.setPosition(-1000.f, -1000.f);
            

        }
        if (score == -15) {
            end_game();
        }

        ////////////////////////////////////////////////////////////////////////////

        window.clear(Color::Black);
        window.draw(background);

        // --------------------  Draw the sprites
        window.draw(dushman);
        window.draw(dushman2);
        window.draw(dushman3);
        window.draw(dushman4);

        window.draw(talwar);
        window.draw(talwar2);
        window.draw(talwar3);
        window.draw(talwar4);

        window.draw(chabi);
        window.draw(chabi2);
        window.draw(rectangle);
        window.draw(potion1);
        window.draw(potion2);
        window.draw(heera);
        window.draw(khazana);
        window.draw(pointsText);


        // MOVEMENT OF PLAYER (up,down,left,right)
        float speed = 1.4f;
        if (e.type == sf::Event::KeyPressed)
        {
            // enemyPlayerColllision();
            float speed = 2.5f;
            bool canMove = true;

            if (e.key.code == sf::Keyboard::Right)
            {
                for (int i = 0; i < 420; i++)
                {
                    if (P.shape.getPosition().x + P.shape.getSize().x + speed > mazeX[i] &&
                        P.shape.getPosition().x + P.shape.getSize().x < mazeX[i] + 15 &&
                        P.shape.getPosition().y < mazeY[i] + 15 &&
                        P.shape.getPosition().y + P.shape.getSize().y > mazeY[i])
                    {
                        canMove = false;
                        break;
                    }
                }

                if (canMove)
                {
                    if (P.shape.getPosition().x + P.shape.getSize().x <= 1075) {
                        // enemyPlayerColllision();
                        P.move(speed, 0.0f);
                    }
                }
            }
            else if (e.key.code == sf::Keyboard::Left)
            {
                for (int i = 0; i < 420; i++)
                {
                    if (P.shape.getPosition().x - speed < mazeX[i] + 15 &&
                        P.shape.getPosition().x > mazeX[i] &&
                        P.shape.getPosition().y < mazeY[i] + 15 &&
                        P.shape.getPosition().y + P.shape.getSize().y > mazeY[i])
                    {
                        canMove = false;
                        break;
                    }
                }

                if (canMove)
                {
                    if (P.shape.getPosition().x + P.shape.getSize().x >= 155)
                    {
                        // enemyPlayerColllision();
                        P.move(-speed, 0.0f);
                    }
                }
            }

            else if (e.key.code == sf::Keyboard::Up)
            {
                for (int i = 0; i < 420; i++)
                {
                    if (P.shape.getPosition().x < mazeX[i] + 15 &&
                        P.shape.getPosition().x + P.shape.getSize().x > mazeX[i] &&
                        P.shape.getPosition().y - speed < mazeY[i] + 15 &&
                        P.shape.getPosition().y + P.shape.getSize().y > mazeY[i])
                    {
                        canMove = false;
                        break;
                    }
                }

                if (canMove)
                {
                    if (P.shape.getPosition().y + P.shape.getSize().y >= 115)/////////////////
                    {
                        //enemyPlayerColllision();
                        P.move(0.0f, -speed);
                    }
                }
            }
            else if (e.key.code == sf::Keyboard::Down)
            {
                for (int i = 0; i < 420; i++)
                {
                    if (P.shape.getPosition().x < mazeX[i] + 15 &&
                        P.shape.getPosition().x + P.shape.getSize().x > mazeX[i] &&
                        P.shape.getPosition().y + P.shape.getSize().y + speed > mazeY[i] &&
                        P.shape.getPosition().y < mazeY[i] + 15)
                    {
                        canMove = false;
                        break;
                    }
                }

                if (canMove)
                {
                    if (P.shape.getPosition().y + P.shape.getSize().y <= 645)
                    {
                        // enemyPlayerColllision();
                        P.move(0.0f, speed);

                    }
                }

            }
            playerr.setPosition(P.shape.getPosition());
        }


        speed = 1.1f;
           E1.move(speed, 0.0f);
           if (E1.shape.getPosition().x + E1.shape.getSize().x > 380 || E1.shape.getPosition().x < 270)
           {
               E1.direction = -E1.direction;
           }
           dushman.setPosition(E1.shape.getPosition());
           E2.move(speed, 0.0f);
           if (E2.shape.getPosition().x + E2.shape.getSize().x > 910 || E2.shape.getPosition().x < 810)
           {
               E2.direction = -E2.direction;
           }
           dushman2.setPosition(E2.shape.getPosition());
           E3.move(speed, 0.0f);
           if (E3.shape.getPosition().x + E3.shape.getSize().x > 380 || E3.shape.getPosition().x < 270)
           {
               E3.direction = -E3.direction;
           }
           dushman3.setPosition(E3.shape.getPosition());
           E4.move(speed, 0.0f);
           if (E4.shape.getPosition().x + E4.shape.getSize().x > 910 || E4.shape.getPosition().x < 810)
           {
               E4.direction = -E4.direction;
           }
           dushman4.setPosition(E4.shape.getPosition());

        window.draw(P.shape);
        window.draw(E1.shape);


        window.draw(playerr);

        //window.draw(enem1);
        displayScore(window, score);


        //*******************************MAZE DRAWS HERE ***********************************************
        {
            //--------------<<<<<<<<<< BORDER >>>>>>>>>>>>>-------------------
            {
                // --------- vertical upper ---------------
                for (int i = 0; i < 12; i++)
                {

                    window.draw(MZ_borderVU_left[i].block);
                }
                for (int i = 0; i < 17; i++)
                {

                    window.draw(MZ_borderVU_right[i].block);
                }

                // --------- vertical lower -----------------
                for (int i = 0; i < 17; i++)
                {

                    window.draw(MZ_borderVL_left[i].block);
                }
                for (int i = 0; i < 12; i++)
                {

                    window.draw(MZ_borderVL_right[i].block);
                }
                // --------- horizontal upper ---------------
                for (int i = 0; i < 32; i++)
                {
                    window.draw(MZ_borderHU_left[i].block);
                }
                for (int i = 0; i < 32; i++)
                {
                    window.draw(MZ_borderHU_right[i].block);
                }
                // --------- horizontal lower ---------------
                for (int i = 0; i < 32; i++)
                {
                    window.draw(MZ_borderHL_left[i].block);
                }
                for (int i = 0; i < 32; i++)
                {
                    window.draw(MZ_borderHL_right[i].block);
                }
            }
            //---------------------------------------------------------------------CENTRE PIECES

            //VERTICAL CENTRE PIECES
            for (int i = 0; i < 9; i++)
            {
                window.draw(MZcentre_VU[i].block);
            }
            for (int i = 0; i < 9; i++)
            {
                window.draw(MZcentre_VL[i].block);
            }

            //HORIZONTAL CENTRE PIECES

            for (int i = 0; i < 8; i++)
            {
                window.draw(MZcentre_H1[i].block);
            }
            for (int i = 0; i < 8; i++)
            {
                window.draw(MZcentre_H2[i].block);
            }
            for (int i = 0; i < 8; i++)
            {
                window.draw(MZcentre_H3[i].block);
            }
            for (int i = 0; i < 8; i++)
            {
                window.draw(MZcentre_H4[i].block);
            }

            //--------------------------------------------------------------------- BAAQI PIECES ----------------------------------------------------------

            //VERICAL UPPER LINES >>>>>>>>>>>>>>>>>>>>>>>>>>

            //outermost small upper lines
            for (int i = 0; i < 6; i++)
            {
                window.draw(MZ_vu_outermostLeft_small[i].block);
            }
            for (int i = 0; i < 6; i++)
            {

                window.draw(MZ_vu_outermostRight_small[i].block);
            }

            //outer most long lines
            for (int i = 0; i < 12; i++)
            {
                window.draw(MZ_outerLeft_long[i].block);
            }
            for (int i = 0; i < 12; i++)
            {
                window.draw(MZ_outerRight_long[i].block);
            }


            //outer upper small lines
            for (int i = 0; i < 6; i++)
            {

                window.draw(MZ_vu_outerLeft_small[i].block);
            }
            for (int i = 0; i < 6; i++)
            {

                window.draw(MZ_vu_outerRight_small[i].block);
            }

            //inner middle lines

            for (int i = 0; i < 6; i++)
            {

                window.draw(MZ_vu_innerLeft_small[i].block);
            }
            for (int i = 0; i < 6; i++)
            {

                window.draw(MZ_vu_innerRight_small[i].block);
            }

            for (int i = 0; i < 12; i++)
            {
                window.draw(MZ_vu_innerLeft_long[i].block);
            }
            for (int i = 0; i < 12; i++)
            {
                window.draw(MZ_vu_innerRight_long[i].block);
            }



            //VERICAL LOWER LINES >>>>>>>>>>>>>>>>>>>>>>


            //outermost small lines
            for (int i = 0; i < 6; i++)
            {

                window.draw(MZ_vL_outermostLeft_small[i].block);
            }
            for (int i = 0; i < 6; i++)
            {
                window.draw(MZ_vL_outermostRight_small[i].block);
            }


            //outer most lower small lines

            for (int i = 0; i < 6; i++)
            {

                window.draw(MZ_vL_outerLeft_small[i].block);
            }
            for (int i = 0; i < 6; i++)
            {
                window.draw(MZ_vL_outerRight_small[i].block);
            }

            // Lower inner middle lines
            for (int i = 0; i < 6; i++)
            {

                window.draw(MZ_vL_innerLeft[i].block);
            }
            for (int i = 0; i < 6; i++)
            {
                window.draw(MZ_vL_innerRight[i].block);
            }



            //HORIZONTAL UPPER LINES  >>>>>>>>>>>>>>>>>>>>>>>>

            //horizontal outer most
            for (int i = 0; i < 6; i++)
            {
                window.draw(MZ_hu_outer1[i].block);
            }
            for (int i = 0; i < 6; i++)
            {
                window.draw(MZ_hu_outer2[i].block);
            }
            for (int i = 0; i < 6; i++)
            {
                window.draw(MZ_hu_outer3[i].block);
            }
            for (int i = 0; i < 8; i++)
            {
                window.draw(MZ_hu_outer4[i].block);
            }

            //horizontal mid upper
            for (int i = 0; i < 6; i++)
            {
                window.draw(MZ_hu_inner[i].block);
            }


            //HORIZONTAL LOWER LINES  >>>>>>>>>>>>>>>>>>>>>>>>

            //horizontal downers
            for (int i = 0; i < 6; i++)
            {
                window.draw(MZ_hL_outer1[i].block);
            }
            for (int i = 0; i < 6; i++)
            {
                window.draw(MZ_hL_outer2[i].block);
            }
            for (int i = 0; i < 6; i++)
            {
                window.draw(MZ_hL_outer3[i].block);
            }
            for (int i = 0; i < 8; i++)
            {
                window.draw(MZ_hL_outer4[i].block);
            }

            //horizontal mid lower
            for (int i = 0; i < 6; i++)
            {
                window.draw(MZ_hL_inner[i].block);
            }

        }

        

        //std::cout << "Elapsed Time: " << collisionTimer.getElapsedTime().asSeconds() << std::endl;
        //std::cout << "Collision Status: " << collisionOccurred << std::endl;


        //-------------------------------PLAYER ENEMY COLLISION-------------------------------------
        //gameover = checkCollisionEP(playerr, E1, P);
        if (gameover) {
            //here:
            end_game();
            break;
        }

        window.display();

    }
}



pair<int, int>* shortestPath(int& pathLength, int start_x, int start_y, int end_x, int end_y, int mazeX[], int mazeY[])
{
    djBlock array[MAX_ROWS][MAX_COLS];

    for (int a = 0; a < MAX_ROWS; a++)
    {
        for (int b = 0; b < MAX_COLS; b++)
        {
            array[a][b].Pos_x = 155 + a * 15;
            array[a][b].Pos_y = 115 + b * 15;
            array[a][b].data = 1;
            array[a][b].dist = INF;
            array[a][b].visited = false;
            array[a][b].parent = { -1, -1 };
        }
    }

    for (int i = 0; i < 420; i++)
    {
        //int mazeX = /* your code to get mazeX[i] */;
        //int mazeY = /* your code to get mazeY[i] */;
        array[mazeX[i]][mazeY[i]].data = 0;
    }

    pair<int, int> start = { start_x, start_y };
    pair<int, int> end = { end_x, end_y };

    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> pq;

    array[start.first][start.second].dist = 0;
    pq.push({ 0, start });

    while (!pq.empty()) {
        int dist = pq.top().first;
        int row = pq.top().second.first;
        int col = pq.top().second.second;
        pq.pop();

        array[row][col].visited = true;

        if (row == end.first && col == end.second)
            break;

        for (int i = 0; i < 4; ++i) {
            int newRow = row + (i == 0 ? -1 : (i == 1 ? 1 : 0));
            int newCol = col + (i == 2 ? -1 : (i == 3 ? 1 : 0));

            if (newRow >= 0 && newRow < MAX_ROWS && newCol >= 0 && newCol < MAX_COLS && array[newRow][newCol].data == 1) {
                int newDist = dist + 1;
                if (newDist < array[newRow][newCol].dist) {
                    array[newRow][newCol].dist = newDist;
                    array[newRow][newCol].parent = { row, col };
                    pq.push({ newDist, {newRow, newCol} });
                }
            }
        }
    }

    pair<int, int>* shortestPath = new pair<int, int>[MAX_ROWS * MAX_COLS];
    int pathIndex = 0;
    pair<int, int> curr = end;
    while (curr != start) {
        shortestPath[pathIndex++] = curr;
        curr = array[curr.first][curr.second].parent;
    }
    shortestPath[pathIndex++] = start;

    reverse(shortestPath, shortestPath + pathIndex);

    pathLength = pathIndex;

    return shortestPath;
}


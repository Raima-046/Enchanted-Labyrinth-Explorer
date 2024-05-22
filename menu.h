#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "game.h"

#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>

using namespace sf;
using namespace std;


class Menu {
public:
	//---------- TEXTURES AND SPRITES
	Texture menuBgT;
	Sprite menuBg;
    Texture carTexture;
    Sprite carSprite;
    string name;
    Texture ghostTexture;
    Sprite ghostSprite;
    Texture upLeftTexture;
    Sprite upLeftSprite;
    Texture f3Texture;
    Sprite f3Sprite;
    Texture f4Texture;
    Sprite f4Sprite;
    Enemy* pp = NULL;   //left firefly
    Enemy* ghost = NULL;     //right firefly
    Enemy* ul = NULL;   //up and left firely
    Enemy* f3 = NULL;
    Enemy* f4 = NULL;

	//--------- SOUND SYSTEM
	SoundBuffer menuSoundBuffer;
	Sound menuSound;
    SoundBuffer gameSoundBuffer;
    Sound gameSound;

	//------------------------- CONSTRUCTOR
	Menu() {

        pp = new Enemy(445.f, 170.f, 0.f, 0.f, -1); // Initial direction is 1 (left)
        ghost = new Enemy(445.f, 30.f, 0.f, 0.f, 1); // Ghost initial direction is 1 (right)
        ul = new Enemy(100.f, 320.f, 0.f, 0.f,-1);   
        f3 = new Enemy(300.f, 498.f, 0.f, 0.f, 1);   
        f4 = new Enemy(300.f, 615.f, 0.f, 0.f, -1);


		menuBgT.loadFromFile("img/menu.jpg");
		menuBg.setTexture(menuBgT);
		menuBg.setPosition(1.f, 0.f);
        menuBg.setScale(890.0f / 942.0f, 650.0f / 767.0f);

        carTexture.loadFromFile("img/left.png");    //197 x 261
        carSprite.setTexture(carTexture);
        carSprite.setPosition(445.f, 200.f);
        carSprite.setScale(0.9, 0.9);

        ghostTexture.loadFromFile("img/rite.png");     //202 x 261
        ghostSprite.setTexture(ghostTexture);
        ghostSprite.setPosition(445.f, 5.f);
        ghostSprite.setScale(0.9,0.9 );

        upLeftTexture.loadFromFile("img/left.png");     //202 x 261
        upLeftSprite.setTexture(upLeftTexture);
        upLeftSprite.setPosition(40.f, 275.f);
        upLeftSprite.setScale(0.9, 0.9);

        f3Texture.loadFromFile("img/rite.png");     //202 x 261
        f3Sprite.setTexture(f3Texture);
        f3Sprite.setPosition(40.f, 275.f);
        f3Sprite.setScale(0.9, 0.9);

        f4Texture.loadFromFile("img/left.png");     //202 x 261
        f4Sprite.setTexture(f4Texture);
        f4Sprite.setPosition(40.f, 275.f);
        f4Sprite.setScale(0.9, 0.9);

        menuSoundBuffer.loadFromFile("awaz/menuSound.wav");
        menuSound.setBuffer(menuSoundBuffer);
        gameSoundBuffer.loadFromFile("awaz/red.wav");
        gameSound.setBuffer(gameSoundBuffer);
	}

	//-------- MEMBER FUNCTIONS
	void disp_menu() {

		// Play the menu sound
		menuSound.play();
        
		//----------------------------------------------MENU WINDOW------------------------------------------
		RenderWindow window(VideoMode(890, 650), "Menu");

		Font font;
		if (!font.loadFromFile("fonty/hiro.ttf")) {
			cerr << "Failed to load font!" << endl;
			return;
		}

		Text startText;
		startText.setFont(font);
		startText.setString("Press Space to Start");
		startText.setCharacterSize(20);
		startText.setFillColor(Color(255, 215, 0));
		//startText.setStyle(Text::Bold);
		startText.setPosition(40.f, 205.f);

		Text inputName;
		inputName.setFont(font);
		inputName.setString("Press Q to Exit ");
		inputName.setCharacterSize(20);
		inputName.setFillColor(Color(255, 215, 0));
		//inputName.setStyle(Text::Bold);
		inputName.setPosition(36.f, 620.f);

        Text instruction;
        instruction.setFont(font);
        instruction.setString("Press I For instructions");
        instruction.setCharacterSize(20);
        instruction.setFillColor(Color(255, 215, 0));
        //instruction.setStyle(Text::Bold);
        instruction.setPosition(620.f, 205.f);

        // For Blinking Functionality
        Clock clock;
        Time blinkTime = seconds(0.3f);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                
                if (event.type == Event::Closed)
                    window.close();
                else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                    // Stop the menu sound
                    menuSound.stop();
                    Game g;
                    gameSound.play();
                    g.start_game();
                }
                
                else if (event.type == Event::KeyPressed && event.key.code == Keyboard::I) {
                    Game i;
                    i.instruction();
                }
                else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Q) {
                    window.close();
                }

            }

            

            //check if it's time to toggle visibility
            if (clock.getElapsedTime() >= blinkTime) {
                // Toggle visibility for each sprite
                carSprite.setColor(carSprite.getColor() == Color::Transparent ? Color::White : Color::Transparent);
                ghostSprite.setColor(ghostSprite.getColor() == Color::Transparent ? Color::White : Color::Transparent);
                upLeftSprite.setColor(upLeftSprite.getColor() == Color::Transparent ? Color::White : Color::Transparent);
                f3Sprite.setColor(f3Sprite.getColor() == Color::Transparent ? Color::White : Color::Transparent);
                f4Sprite.setColor(f4Sprite.getColor() == Color::Transparent ? Color::White : Color::Transparent);

                //restart the clock
                clock.restart();
            }


            // Move the car
            float carSpeed = 0.15f;
            pp->move(carSpeed, 0.0f);

            // If the car moves beyond the right edge of the window, reset its position to the left edge
            if (pp->shape.getPosition().x > window.getSize().x) {
                pp->shape.setPosition(-pp->shape.getSize().x, pp->shape.getPosition().y);
            }
            // If the car moves beyond the left edge of the window, reset its position to the right edge
            else if (pp->shape.getPosition().x < -pp->shape.getSize().x) {
                pp->shape.setPosition(window.getSize().x, pp->shape.getPosition().y);
            }

            // Move the ghost
            float ghostSpeed = 0.15f;
            ghost->move(ghostSpeed, 0.0f);

            // If the ghost moves beyond the right edge of the window, reset its position to the left edge
            if (ghost->shape.getPosition().x > window.getSize().x) {
                ghost->shape.setPosition(-ghost->shape.getSize().x, ghost->shape.getPosition().y);
            }
            // If the ghost moves beyond the left edge of the window, reset its position to the right edge
            else if (ghost->shape.getPosition().x < -ghost->shape.getSize().x) {
                ghost->shape.setPosition(window.getSize().x, ghost->shape.getPosition().y);
            }

            // Move the upleft firefly
            float ulSpeed = 0.15f;
            ul->move(ulSpeed, 0.0f);

            // If the ul moves beyond the right edge of the window, reset its position to the left edge
            if (ul->shape.getPosition().x > window.getSize().x) {
                ul->shape.setPosition(-ul->shape.getSize().x, ul->shape.getPosition().y);
            }
            // If the ul moves beyond the left edge of the window, reset its position to the right edge
            else if (ul->shape.getPosition().x < -ul->shape.getSize().x) {
                ul->shape.setPosition(window.getSize().x, ul->shape.getPosition().y);
            }

            // Move the f3 firefly
            float f3Speed = 0.15f;
            f3->move(f3Speed, 0.0f);

            // If the f3 moves beyond the right edge of the window, reset its position to the left edge
            if (f3->shape.getPosition().x > window.getSize().x) {
                f3->shape.setPosition(-f3->shape.getSize().x, f3->shape.getPosition().y);
            }
            // If the f3 moves beyond the left edge of the window, reset its position to the right edge
            else if (f3->shape.getPosition().x < -f3->shape.getSize().x) {
                f3->shape.setPosition(window.getSize().x, f3->shape.getPosition().y);
            }

            // Move the f4 firefly
            float f4Speed = 0.15f;
            f4->move(f4Speed, 0.0f);

            // If the f4 moves beyond the right edge of the window, reset its position to the left edge
            if (f4->shape.getPosition().x > window.getSize().x) {
                f4->shape.setPosition(-f4->shape.getSize().x, f4->shape.getPosition().y);
            }
            // If the f4 moves beyond the left edge of the window, reset its position to the right edge
            else if (f4->shape.getPosition().x < -f4->shape.getSize().x) {
                f4->shape.setPosition(window.getSize().x, f4->shape.getPosition().y);
            }

             
            ghostSprite.setPosition(ghost->shape.getPosition());   //Update ghostSprite position            
            carSprite.setPosition(pp->shape.getPosition());     // Update carSprite position           
            upLeftSprite.setPosition(ul->shape.getPosition());     // Update up left firely position            
            f3Sprite.setPosition(f3->shape.getPosition());      // Update f3firely position         
            f4Sprite.setPosition(f4->shape.getPosition());        // Update f4firely position

            window.clear(Color::Black);
            window.draw(menuBg);
            window.draw(carSprite);
            window.draw(ghostSprite);  // Draw the ghost after the car
            window.draw(upLeftSprite);
            window.draw(f3Sprite);
            window.draw(f4Sprite);
            window.draw(pp->shape);

            window.draw(startText);
            window.draw(inputName);
            window.draw(instruction);



            window.display();
        }
	}
};

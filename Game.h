//
// Created by bswenson3 on 11/9/16.
//
#pragma once

#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "RegularGameObject.h"
#include "AnimatedGameObject.h"

class Game {
public:
	//enum to resolve game state
	enum GameState {Starting, PlayerSelect, Fighting, NextStage, GameOver, GameWon};
	enum Meme {Harambe, Kangaroo, EvilKermit, Pepe, DatBoi, JoeBiden, Spongegar};
    //use default screen size
    Game();
    //run the game
    void run();

private:
    void processEvents();
	//handle input from the user
	void handlePlayerInput(sf::Event event, bool isDown);
	//update the game objects
	void update(sf::Time deltaT);
	//sets up various stages during the game
	void initializeState();
    //draw the scene
    void render();
    
	
	//test for going out of bounds
	bool isOutOfBounds(float x, float y);
	//test for winning game
	GameState isGameWon(float x, float y);

    sf::RenderWindow m_window;
	sf::Texture m_backgroundTexture;
	sf::Sprite m_background;
	sf::Font m_gameFont;

	sf::RectangleShape* m_characterSelectShape;
	sf::Text m_characterText;
	sf::Text m_gameWonText;
	
	
	

    GameObject* m_gameObject0;
	GameObject* m_gameObject1;
	//GameObject* m_fairy;
	//GameObject* m_lumberjacks;
	//GameObject* m_monster;
	//GameObject* m_oldMan;
	std::vector<GameObject*> m_gameObjects;
	GameState m_gameState = GameState::Starting;
	Meme m_meme;
	bool m_transition = false;

	bool m_highlightCharacter = false;
};



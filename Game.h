//
// Created by bswenson3 on 11/9/16.
//
#pragma once

#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "RegularGameObject.h"
#include "AnimatedGameObject.h"

class Game {
public:
	//enum to resolve game state
	enum GameState {Starting, Lore, Menu, Control, PlayerSelect, Fighting, GameWon, GameOver};
	enum Meme {Harambe, Kangaroo, EvilKermit, Pepe, DatBoi, JoeBiden, Spongegar};
	enum GameMode {Play, Controls};
    //use default screen size
    Game();
    //run the game
    void run();

private:
    void processEvents();
	//handle input from the user
	void handlePlayerInput(sf::Event event);
	//update the game objects
	void update(sf::Time deltaT);
	//sets up various stages during the game
	void initializeState();
    //draw the scene
    void render();

	//test for winning game
	GameState isGameWon(float x, float y);

    sf::RenderWindow m_window;
	sf::Texture m_backgroundTexture;
	sf::Sprite m_background;
	sf::Font m_gameFont;

	sf::RectangleShape* m_characterSelectShape;
	sf::Text m_characterText;
	sf::Text m_gameWonText;
	sf::Text m_highScoreText;
	sf::Text m_highestScoreText;
	
    GameObject* m_gameObject0;
	GameObject* m_gameObject1;
	std::vector<GameObject*> m_gameObjects;
	GameState m_gameState = GameState::Starting;
	Meme m_player, m_remote;
	GameMode m_gameMode;
	int m_highScore = 0;
	int m_highestScore = 0;
	bool m_transition = false;
	bool m_highlightCharacter = false;
};



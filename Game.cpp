//
// Created by bswenson3 on 11/9/16.
//

#include "Game.h"

Game::Game() :
	m_window(sf::VideoMode(800, 450), "Meme Kombat")
{
	m_window.setFramerateLimit(40);

	//load the background
	m_backgroundTexture.loadFromFile("content/memeCombat.jpg");
	m_background.setTexture(m_backgroundTexture, true);

	m_gameObject0 = new AnimatedGameObject(0.75f, false);
	m_gameObject0->load("content/pressEntertoBegin.png");
	m_gameObject0->useAnimations(GameObject::Direction::none);
	m_gameObject0->addTextureRect(sf::Rect<int>(0, 30, 290, 30));
	m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 290, 30));
	m_gameObject0->setPosition(255, 340);
	
	m_gameObjects = { m_gameObject0 };

	//make gameover text
	//m_gameFont.loadFromFile("content/comicSans.ttf");
	//m_gameWonText.setFont(m_gameFont);
	//m_gameWonText.setString("GAME OVER");
	//m_gameWonText.setCharacterSize(55);
	//m_gameWonText.setFillColor(sf::Color(196, 169, 86));
	
	//initialize the player
	//m_player = new AnimatedGameObject(0.15f, true);
    //load and size the player sprites    
    //m_player->load("content/linkSprites.png");
	//m_player->useAnimations(GameObject::Direction::down);
	//m_player->addTextureRect(sf::Rect<int>(250, 525, 95, 125));
	//m_player->addTextureRect(sf::Rect<int>(613, 528, 95, 125));
	//m_player->useAnimations(GameObject::Direction::left);
    //m_player->setScale(0.25f);
	//m_player->setPosition(245.0f, 350.0f);

	//strobing chicken object
	//m_chicken = new AnimatedGameObject(1.25f, false);
	//load texture and intRects
	//m_chicken->load("content/linkEnemies.png");
	//m_chicken->useAnimations(GameObject::Direction::none);
	//m_chicken->addTextureRect(sf::Rect<int>(279, 14, 18, 18));
	//m_chicken->addTextureRect(sf::Rect<int>(299, 14, 18, 18));
	//m_chicken->setStrobeInterval(100.0f);
	//m_chicken->setScale(1.2f);
	//m_chicken->setPosition(430.0f, 205.0f);

	//floating fairy object
	//m_fairy = new AnimatedGameObject(0.15f, false);
	//m_fairy->load("content/linkEnemies.png");
	//m_fairy->useAnimations(GameObject::Direction::none);
	//m_fairy->addTextureRect(sf::Rect<int>(370, 16, 24, 50));
	//m_fairy->addTextureRect(sf::Rect<int>(400, 16, 24, 50));
	//m_fairy->setScale(1.2f);
	//m_fairy->setPosition(385.0f, 15.0f);

	//seamonster object
	//m_monster = new RegularGameObject();
	//m_monster->load("content/linkEnemies.png");
	//m_monster->addTextureRect(sf::Rect<int>(180, 265, 35, 35));
	//m_monster->setScale(1.2f);
	//m_monster->setPosition(125.0f, 250.0f);

	//m_gameObjects = {m_player, m_chicken, m_fairy, m_monster};
}

void Game::run() {
	sf::Clock clock;

    while(m_window.isOpen()) {
		if (!m_transition) {
			sf::Time deltaT = clock.restart();
			processEvents();
			if (m_gameState != GameState::GameWon && m_gameState != GameState::GameOver) {
				update(deltaT);
				render();
			}
		}
		else {
			initializeState();
		}
    }
}

void Game::processEvents() {
    sf::Event event;
    while(m_window.pollEvent(event)) {
        switch(event.type) {
            case sf::Event::KeyPressed:
            //handle key down here
                handlePlayerInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handlePlayerInput(event.key.code, false);
                break;

            case sf::Event::Closed:
				for (GameObject* gameObject : m_gameObjects) {
					delete gameObject;
				}
				delete m_characterSelectShape;
				m_gameState = GameState::GameOver;
                m_window.close();
                break;
        }
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isDown) {
	if (m_gameState == GameState::Starting) {
		if (key == sf::Keyboard::Return) {
			m_transition = true;
		}
		m_gameObject0->useAnimations(GameObject::Direction::none);
	}
	if (m_gameState == GameState::Fighting) {
		if (!isDown) {
			//m_player->useAnimations(GameObject::Direction::none);
		}
		else {
			if (key == sf::Keyboard::Left) {
			}
			if (key == sf::Keyboard::Right) {
			}
			if (key == sf::Keyboard::Up) {
			}
			if (key == sf::Keyboard::Down) {
				//m_player->GameObject::m_down = isDown;
				//m_player->useAnimations(GameObject::Direction::down);
			}
		}
	}
}

//use time since last update to get smooth movement
void Game::update(sf::Time deltaT) {
	//if (m_gameState == GameState::Fighting) {
		//Look a vector class!
		//sf::Vector2f movement(0.0f, 0.0f);
		/*if (m_player->GameObject::m_up)
		movement.y -= m_player->getSpeed();
		if (m_player->GameObject::m_down)
		movement.y += m_player->getSpeed();
		if (m_player->GameObject::m_left)
		movement.x -= m_player->getSpeed();
		if (m_player->GameObject::m_right)
		movement.x += m_player->getSpeed();*/
		//sf::Vector2f playerPos = m_player->getPosition();
		//bool inValidArea = isOutOfBounds(playerPos.x, playerPos.y);
		//m_gameState = isGameWon(playerPos.x, playerPos.y);
		//if (inValidArea) {
		//	m_player->GameObject::m_isStuck = false;
		//	m_player->move(movement * deltaT.asSeconds());
		//}
		//else {
		//	m_player->GameObject::m_isStuck = true;
		//	switch (m_player->getLastDirection()) {
		//	case GameObject::Direction::left: {
		//		if (!m_player->GameObject::m_left)
		//			m_player->move(movement * deltaT.asSeconds());
		//		break;
		//	}
		//	case GameObject::Direction::right: {
		//		if (!m_player->GameObject::m_right)
		//			m_player->move(movement * deltaT.asSeconds());
		//		break;
		//	}
		//	};
	//	}
	//}
	
	//In any state, all GameObjects will be rendering
	for (GameObject* gameObject : m_gameObjects) {
		gameObject->update();
	}
	if (m_gameState == GameState::PlayerSelect) {
		sf::Vector2<int> mouseLoc = sf::Mouse::getPosition(m_window);
		if (mouseLoc.x > 65 && mouseLoc.x < 190 && mouseLoc.y > 78 && mouseLoc.y < 215) {
			m_characterSelectShape->setPosition(65, 78);
			m_showSelectShape = true;
		}
		if (mouseLoc.x > 200 && mouseLoc.x < 325 && mouseLoc.y > 78 && mouseLoc.y < 215) {
			m_characterSelectShape->setPosition(200, 78);
			m_showSelectShape = true;
		}
		if (mouseLoc.x > 200 && mouseLoc.x < 325 && mouseLoc.y > 225 && mouseLoc.y < 360) {
			m_characterSelectShape->setPosition(200, 225);
			m_showSelectShape = true;
		}
		if (mouseLoc.x > 335 && mouseLoc.x < 455 && mouseLoc.y > 225 && mouseLoc.y < 360) {
			m_characterSelectShape->setPosition(335, 225);
			m_showSelectShape = true;
		}
		if (mouseLoc.x > 470 && mouseLoc.x < 590 && mouseLoc.y > 225 && mouseLoc.y < 360) {
			m_characterSelectShape->setPosition(470, 225);
			m_showSelectShape = true;
		}
		if (mouseLoc.x > 470 && mouseLoc.x < 590 && mouseLoc.y > 78 && mouseLoc.y < 215) {
			m_characterSelectShape->setPosition(470, 78);
			m_showSelectShape = true;
		}
		if (mouseLoc.x > 600 && mouseLoc.x < 725 && mouseLoc.y > 78 && mouseLoc.y < 215) {
			m_characterSelectShape->setPosition(600, 78);
			m_showSelectShape = true;
		}
	}
}

void Game::render() {
    m_window.clear();
	m_window.draw(m_background);
	for (GameObject* gameObject : m_gameObjects) {
		gameObject->draw(m_window);
	}
	if (m_showSelectShape)
		m_window.draw(*m_characterSelectShape);
	if (m_gameState == GameState::GameWon)
		m_window.draw(m_gameWonText);
	m_window.display();
}

void Game::initializeState() {
	//Delete objects
	for (GameObject* gameObject : m_gameObjects) {
		delete gameObject;
	}
	switch (m_gameState) {
	case (GameState::Starting) : {
		//Set new background
		m_backgroundTexture.loadFromFile("content/chooseFighter.jpg");
		m_background.setTexture(m_backgroundTexture);
		//Make new gameObjects
		m_gameObjects = {};
		//Make new character select rectangle
		m_characterSelectShape = new sf::RectangleShape(sf::Vector2<float>(120, 134));
		m_characterSelectShape->setOutlineColor(sf::Color::Green);
		m_characterSelectShape->setOutlineThickness(3.5f);
		m_characterSelectShape->setFillColor(sf::Color::Transparent);
		//Change gameState
		m_gameState = GameState::PlayerSelect;
	}
	};
	//Make sure the transition ends
	m_transition = false;
}

bool Game::isOutOfBounds(float x, float y) {
	return ((y < 415 && y > 220 && x > 110 && x < 400) && !(x > 190 && x < 300 && y > 180 && y < 300)
		&& !(x > 108 && x < 185 && y > 325 && y < 370) && !(x > 200 && x < 255 && y > 350 && y < 415)
		&& !(x > 320 && x < 350 && y > 360 && y < 380)) || (x > 350 && x < 465 && y > 320 && y < 400);
}

Game::GameState Game::isGameWon(float x, float y) {
	if (false)
		return GameState::GameWon;
	else
		return m_gameState;
}
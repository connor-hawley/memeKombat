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

	//Initialize the press enter game object
	m_gameObject0 = new AnimatedGameObject(0.66f, GameObject::PlayerType::other);
	m_gameObject0->load("content/pressEntertoBegin.png");
	m_gameObject0->useAnimations(GameObject::Direction::none);
	m_gameObject0->addTextureRect(sf::Rect<int>(0, 30, 290, 30));
	m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 290, 30));
	m_gameObject0->setPosition(255, 340);
	
	m_gameObjects = { m_gameObject0 };

	//make game font
	m_gameFont.loadFromFile("content/comicSans.ttf");
	m_gameWonText.setFont(m_gameFont);
	m_gameWonText.setString("GAME OVER");
	m_gameWonText.setCharacterSize(55);
	m_gameWonText.setFillColor(sf::Color(196, 169, 86));
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
            case sf::Event::KeyPressed :
                handlePlayerInput(event);
                break;
            case sf::Event::KeyReleased :
                handlePlayerInput(event);
                break;
			case sf::Event::MouseButtonReleased :
				handlePlayerInput(event);
				break;
			case sf::Event::Closed : {
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
}

void Game::handlePlayerInput(sf::Event event) {
	switch (m_gameState) {
	case GameState::Starting : {
		if (event.type == sf::Event::EventType::KeyReleased) {
			if (event.key.code == sf::Keyboard::Return) {
				m_transition = true;
			}
		}
		break;
	}
	case GameState::PlayerSelect : {
		if (event.type == sf::Event::EventType::MouseButtonReleased) {
			sf::Event::MouseButtonEvent mouseEvent = event.mouseButton;
			if (mouseEvent.button == sf::Mouse::Button::Left) {
				sf::Vector2<float> shapeLoc = m_characterSelectShape->getPosition();
				if (shapeLoc.x == 65 && shapeLoc.y == 78) {
					m_player = Meme::Harambe;
					m_transition = true;
				}
				else if (shapeLoc.x == 200 && shapeLoc.y == 78) {
					m_player = Meme::Kangaroo;
					m_transition = true;
				}
				else if (shapeLoc.x == 200 && shapeLoc.y == 225) {
					m_player = Meme::EvilKermit;
					m_transition = true;
				}
				else if (shapeLoc.x == 335 && shapeLoc.y == 225) {
					m_player = Meme::Pepe;
					m_transition = true;
				}
				else if (shapeLoc.x == 470 && shapeLoc.y == 225) {
					m_player = Meme::DatBoi;
					m_transition = true;
				}
				else if (shapeLoc.x == 470 && shapeLoc.y == 78) {
					m_player = Meme::JoeBiden;
					m_transition = true;
				}
				else if (shapeLoc.x == 600 && shapeLoc.y == 78) {
					m_player = Meme::Spongegar;
					m_transition = true;
				}
			}
		}
		break;
	}
	case GameState::Fighting : {
		if (!m_gameObject0->m_animationTriggered) {
			if (event.type == sf::Event::EventType::KeyPressed) {
				//handle extra buttons. modern emulator controls are 
				//down - crouch
				//left - left
				//right - right
				//z - punch
				//x - block
				//c - kick
				//different attacks when crouching
				float x0 = m_gameObject0->getPosition().x;
				float x1 = m_gameObject1->getPosition().x;
				if (event.key.code == sf::Keyboard::Down) {
					m_gameObject0->m_down = true;
					m_gameObject0->useAnimations(GameObject::Direction::down);
				}
				else if (event.key.code == sf::Keyboard::Left) {
					m_gameObject0->m_left = true;
					m_gameObject0->useAnimations(GameObject::Direction::left);
				}
				else if (event.key.code == sf::Keyboard::Right) {
					m_gameObject0->m_right = true;
					m_gameObject0->useAnimations(GameObject::Direction::right);
				}

				if (event.key.code == sf::Keyboard::Z) {
					//local punch mechanics
					if (!m_gameObject0->m_down && m_gameObject0->m_canPunch) {
						m_gameObject0->useAnimations(GameObject::Direction::punch);
						m_gameObject0->m_canPunch = false;
						m_gameObject0->m_animationTriggered = true;
						if (((x1 > x0 && x1 - x0 < 95) || (x0 > x1 && x0 - x1 < 95))
							&& !(m_gameObject1->m_down || m_gameObject1->m_block)) {
							m_gameObject1->m_health -= 3;
						}
					}
					else if (m_gameObject0->m_down && m_gameObject0->m_canPunch) {
						m_gameObject0->useAnimations(GameObject::Direction::crouchPunch);
						m_gameObject0->m_canPunch = false;
						m_gameObject0->m_animationTriggered = true;
						if (((x1 > x0 && x1 - x0 < 95) || (x0 > x1 && x0 - x1 < 95))
							&& !(m_gameObject1->m_down || m_gameObject1->m_block)) {
							m_gameObject1->m_health -= 10;
						}
					}
				}
				else if (event.key.code == sf::Keyboard::X) {
					//local block mechanics
					if (!m_gameObject0->m_down) {
						m_gameObject0->useAnimations(GameObject::Direction::block);
						m_gameObject0->m_block = true;
					}
					else if (m_gameObject0->m_down) {
						m_gameObject0->useAnimations(GameObject::Direction::crouchBlock);
						m_gameObject0->m_block = true;
					}
				}
				else if (event.key.code == sf::Keyboard::C) {
					//local kick mechanics
					if (!m_gameObject0->m_down && m_gameObject0->m_canKick) {
						m_gameObject0->useAnimations(GameObject::Direction::kick);
						m_gameObject0->m_canKick = false;
						m_gameObject0->m_animationTriggered = true;
						if (((x1 > x0 && x1 - x0 < 95) || (x0 > x1 && x0 - x1 < 95))
							&& !(m_gameObject1->m_down || m_gameObject1->m_block)) {
							m_gameObject1->m_health -= 7;
						}
					}
					else if (m_gameObject0->m_down && m_gameObject0->m_canKick) {
						m_gameObject0->useAnimations(GameObject::Direction::crouchKick);
						m_gameObject0->m_canKick = false;
						m_gameObject0->m_animationTriggered = true;
						if (((x1 > x0 && x1 - x0 < 95) || (x0 > x1 && x0 - x1 < 95))
							&& !m_gameObject1->m_block) {
							m_gameObject1->m_health -= 5;
						}
					}
				}
			}
			else if (event.type == sf::Event::EventType::KeyReleased) {
				if (event.key.code == sf::Keyboard::Down) {
					m_gameObject0->m_down = false;
					m_gameObject0->useAnimations(GameObject::Direction::none);
				}
				else if (event.key.code == sf::Keyboard::Left) {
					m_gameObject0->m_left = false;
					m_gameObject0->useAnimations(GameObject::Direction::none);
				}
				else if (event.key.code == sf::Keyboard::Right) {
					m_gameObject0->m_right = false;
					m_gameObject0->useAnimations(GameObject::Direction::none);
				}
				if (event.key.code == sf::Keyboard::Z) {
					m_gameObject0->m_canPunch = true;
					m_gameObject0->useAnimations(GameObject::Direction::none);
				}
				else if (event.key.code = sf::Keyboard::X) {
					m_gameObject0->m_block = false;
					m_gameObject0->useAnimations(GameObject::Direction::none);
				}
				else if (event.key.code = sf::Keyboard::C) {
					m_gameObject0->m_canKick = true;
					m_gameObject0->useAnimations(GameObject::Direction::none);
				}
			}
			else {
				m_gameObject0->m_down = false;
				m_gameObject0->m_left = false;
				m_gameObject0->m_right = false;
				m_gameObject0->m_canPunch = true;
				m_gameObject0->m_canKick = true;
				m_gameObject0->m_animationTriggered = false;
				m_gameObject0->useAnimations(GameObject::Direction::none);
			}
		}
		break;
	}
	};
}

//use time since last update to get smooth movement
void Game::update(sf::Time deltaT) {
	//In any state, all GameObjects will be rendering
	for (GameObject* gameObject : m_gameObjects) {
		gameObject->update();
	}
	//highlights the character the player is hovering over
	switch (m_gameState) {
	case GameState::PlayerSelect : {
		sf::Vector2<int> mouseLoc = sf::Mouse::getPosition(m_window);
		if (mouseLoc.x > 65 && mouseLoc.x < 190 && mouseLoc.y > 78 && mouseLoc.y < 215) {
			m_characterSelectShape->setPosition(65, 78);
			m_characterText.setString("harambe");
			m_highlightCharacter = true;
		}
		else if (mouseLoc.x > 200 && mouseLoc.x < 325 && mouseLoc.y > 78 && mouseLoc.y < 215) {
			m_characterSelectShape->setPosition(200, 78);
			m_characterText.setString("kangaroo");
			m_highlightCharacter = true;
		}
		else if (mouseLoc.x > 200 && mouseLoc.x < 325 && mouseLoc.y > 225 && mouseLoc.y < 360) {
			m_characterSelectShape->setPosition(200, 225);
			m_characterText.setString("evil kermit");
			m_highlightCharacter = true;
		}
		else if (mouseLoc.x > 335 && mouseLoc.x < 455 && mouseLoc.y > 225 && mouseLoc.y < 360) {
			m_characterSelectShape->setPosition(335, 225);
			m_characterText.setString("pepe");
			m_highlightCharacter = true;
		}
		else if (mouseLoc.x > 470 && mouseLoc.x < 590 && mouseLoc.y > 225 && mouseLoc.y < 360) {
			m_characterSelectShape->setPosition(470, 225);
			m_characterText.setString("dat boi");
			m_highlightCharacter = true;
		}
		else if (mouseLoc.x > 470 && mouseLoc.x < 590 && mouseLoc.y > 78 && mouseLoc.y < 215) {
			m_characterSelectShape->setPosition(470, 78);
			m_characterText.setString("joe biden");
			m_highlightCharacter = true;
		}
		else if (mouseLoc.x > 600 && mouseLoc.x < 725 && mouseLoc.y > 78 && mouseLoc.y < 215) {
			m_characterSelectShape->setPosition(600, 78);
			m_characterText.setString("spongegar");
			m_highlightCharacter = true;
		}
		else {
			m_characterSelectShape->setPosition(0, 0);
			m_highlightCharacter = false;
		}
		break;
	}
	case GameState::Fighting: {
		sf::Vector2<float> movement(0.0f, 0.0f);
		if (m_gameObject0->m_left)
			movement.x -= m_gameObject0->getSpeed();
		if (m_gameObject0->m_right)
			movement.x += m_gameObject0->getSpeed();
		float x0 = m_gameObject0->getPosition().x;
		float x1 = m_gameObject1->getPosition().x;
		if (x0 > 0 && x0 < 650 && ((x0 < x1 && x1 - x0 > 55) || (x0 > x1 && x0 - x1 > 55))) {
			m_gameObject0->m_isStuck = false;
			m_gameObject0->move(movement * deltaT.asSeconds());
		}
		else {
			m_gameObject0->m_isStuck = true;
			switch (m_gameObject0->getLastDirection()) {
			case GameObject::Direction::left: {
				if (!m_gameObject0->GameObject::m_left)
					m_gameObject0->move(movement * deltaT.asSeconds());
				break;
			}
			case GameObject::Direction::right: {
				if (!m_gameObject0->GameObject::m_right)
					m_gameObject0->move(movement * deltaT.asSeconds());
				break;
			}
			};
		}
	}
	};
}

void Game::initializeState() {
	//Delete objects
	for (GameObject* gameObject : m_gameObjects) {
		delete gameObject;
	}
	switch (m_gameState) {
	case GameState::Starting: {
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
		//Set up text to describe characters
		m_characterText.setFont(m_gameFont);
		m_characterText.setCharacterSize(25);
		m_characterText.setFillColor(sf::Color(196, 169, 86));
		m_characterText.setPosition(340, 70);
		//Change gameState
		m_gameState = GameState::PlayerSelect;
		break;
	}
	case GameState::PlayerSelect: {
		//Stop displaying character select shape
		m_highlightCharacter = false;
		//Set new background
		m_backgroundTexture.loadFromFile("content/theCourtyard.jpg");
		m_background.setTexture(m_backgroundTexture);
		switch (m_player) {
		case Meme::Harambe: {
			//display harambe
			/*m_gameObject0 = new RegularGameObject();
			m_gameObject0->load("content/harambe64.png");
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->setScale(0.8f);
			m_gameObject0->setPosition(120, 230);*/
			m_gameObject0 = new AnimatedGameObject(1.0f, GameObject::PlayerType::local);
			m_gameObject0->load("content/harambe64.png");
			m_gameObject0->useAnimations(GameObject::Direction::up);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::down);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::left);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::right);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::none);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->setScale(0.8f);
			m_gameObject0->setPosition(120, 230);
			break;
		}
		case Meme::Kangaroo: {
			//display kangaroo
			m_gameObject0 = new AnimatedGameObject(1.0f, GameObject::PlayerType::local);
			m_gameObject0->load("content/kangaroo64.png");
			m_gameObject0->useAnimations(GameObject::Direction::up);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::down);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::left);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::right);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::none);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->setScale(0.8f);
			m_gameObject0->setPosition(120, 230);
			break;
		}
		case Meme::EvilKermit: {
			//display evil kermit
			m_gameObject0 = new AnimatedGameObject(1.0f, GameObject::PlayerType::local);
			m_gameObject0->load("content/evilKermit64.png");
			m_gameObject0->useAnimations(GameObject::Direction::up);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::down);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::left);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::right);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::none);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->setScale(0.8f);
			m_gameObject0->setPosition(120, 230);
			break;
		}
		case Meme::Pepe: {
			//display pepe
			m_gameObject0 = new AnimatedGameObject(1.0f, GameObject::PlayerType::local);
			m_gameObject0->load("content/pepe64.png");
			m_gameObject0->useAnimations(GameObject::Direction::up);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::down);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::left);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::right);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::none);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->setScale(0.8f);
			m_gameObject0->setPosition(120, 230);
			break;
		}
		case Meme::DatBoi: {
			//display dat boi
			m_gameObject0 = new AnimatedGameObject(1.0f, GameObject::PlayerType::local);
			m_gameObject0->load("content/datBoi64.png");
			m_gameObject0->useAnimations(GameObject::Direction::up);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::down);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::left);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::right);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::none);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->setScale(0.8f);
			m_gameObject0->setPosition(120, 230);
			break;
		}
		case Meme::JoeBiden: {
			//display joe biden
			m_gameObject0 = new AnimatedGameObject(0.20f, GameObject::PlayerType::local);
			m_gameObject0->load("content/joeBidenSprites.png");
			m_gameObject0->useAnimations(GameObject::Direction::down);
			m_gameObject0->addTextureRect(sf::Rect<int>(1290, 25, 65, 105));
			m_gameObject0->useAnimations(GameObject::Direction::left);
			m_gameObject0->addTextureRect(sf::Rect<int>(510, 135, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(390, 135, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(270, 135, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(150, 135, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(20, 135, 65, 105));
			m_gameObject0->useAnimations(GameObject::Direction::right);
			m_gameObject0->addTextureRect(sf::Rect<int>(20, 135, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(150, 135, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(270, 135, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(390, 135, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(510, 135, 65, 105));
			m_gameObject0->useAnimations(GameObject::Direction::punch);
			m_gameObject0->addTextureRect(sf::Rect<int>(20, 255, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(85, 255, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(150, 255, 65, 105));
			m_gameObject0->useAnimations(GameObject::Direction::crouchPunch);
			m_gameObject0->addTextureRect(sf::Rect<int>(20, 750, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(95, 745, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(170, 735, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(230, 720, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(286, 730, 65, 105));
			m_gameObject0->useAnimations(GameObject::Direction::block);
			m_gameObject0->addTextureRect(sf::Rect<int>(1090, 20, 65, 105));
			m_gameObject0->useAnimations(GameObject::Direction::crouchBlock);
			m_gameObject0->addTextureRect(sf::Rect<int>(1360, 25, 65, 105));
			m_gameObject0->useAnimations(GameObject::Direction::kick);
			m_gameObject0->addTextureRect(sf::Rect<int>(75, 485, 55, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(130, 485, 45, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(170, 485, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(240, 485, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(325, 285, 65, 105));
			m_gameObject0->useAnimations(GameObject::Direction::crouchKick);
			m_gameObject0->addTextureRect(sf::Rect<int>(395, 735, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(395, 735, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(470, 735, 75, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(395, 735, 65, 105));
			m_gameObject0->useAnimations(GameObject::Direction::none);
			m_gameObject0->addTextureRect(sf::Rect<int>(20, 20, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(115, 20, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(185, 20, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(320, 20, 65, 105));
			m_gameObject0->addTextureRect(sf::Rect<int>(450, 20, 65, 105));
			m_gameObject0->setScale(1.5f);
			m_gameObject0->setPosition(120, 206);
			break;
		}
		case Meme::Spongegar: {
			//display spongegar
			m_gameObject0 = new AnimatedGameObject(1.0f, GameObject::PlayerType::local);
			m_gameObject0->load("content/spongegar64.png");
			m_gameObject0->useAnimations(GameObject::Direction::up);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::down);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::left);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::right);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::none);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->setScale(0.8f);
			m_gameObject0->setPosition(120, 230);
			break;
		}
		};
		//create other remote or ai player
		m_ai = m_player;
		while (m_ai == m_player) {
			srand(time(NULL));
			int random = rand() % 6; //random number 0 to 6
			switch (random) {
			case Meme::Harambe: {
				//ai is harambe
				m_ai = Meme::Harambe;
				m_gameObject1 = new AnimatedGameObject(1.0f, GameObject::PlayerType::ai);
				m_gameObject1->load("content/harambe64.png");
				m_gameObject1->useAnimations(GameObject::Direction::up);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::down);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::left);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::right);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::none);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->setScale(0.8f);
				m_gameObject1->setPosition(560, 230);
				break;
			}
			case Meme::Kangaroo: {
				//ai is kangaroo
				m_ai = Meme::Kangaroo;
				m_gameObject1 = new AnimatedGameObject(1.0f, GameObject::PlayerType::ai);
				m_gameObject1->load("content/kangaroo64.png");
				m_gameObject1->useAnimations(GameObject::Direction::up);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::down);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::left);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::right);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::none);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->setScale(0.8f);
				m_gameObject1->setPosition(560, 230);
				break;
			}
			case Meme::EvilKermit: {
				//ai is evil kermit
				m_ai = Meme::EvilKermit;
				m_gameObject1 = new AnimatedGameObject(1.0f, GameObject::PlayerType::ai);
				m_gameObject1->load("content/evilKermit64.png");
				m_gameObject1->useAnimations(GameObject::Direction::up);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::down);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::left);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::right);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::none);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->setScale(0.8f);
				m_gameObject1->setPosition(560, 230);
				break;
			}
			case Meme::Pepe: {
				//ai is pepe
				m_ai = Meme::Pepe;
				m_gameObject1 = new AnimatedGameObject(1.0f, GameObject::PlayerType::ai);
				m_gameObject1->load("content/pepe64.png");
				m_gameObject1->useAnimations(GameObject::Direction::up);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::down);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::left);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::right);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::none);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->setScale(0.8f);
				m_gameObject1->setPosition(560, 230);
				break;
			}
			case Meme::DatBoi: {
				//ai is dat boi
				m_ai = Meme::DatBoi;
				m_gameObject1 = new AnimatedGameObject(1.0f, GameObject::PlayerType::ai);
				m_gameObject1->load("content/datBoi64.png");
				m_gameObject1->useAnimations(GameObject::Direction::up);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::down);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::left);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::right);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::none);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->setScale(0.8f);
				m_gameObject1->setPosition(560, 230);
				break;
			}
			case Meme::JoeBiden: {
				//ai is joe biden
				m_ai = Meme::JoeBiden;
				m_gameObject1 = new AnimatedGameObject(1.0f, GameObject::PlayerType::ai);
				m_gameObject1->load("content/joeBiden64.png");
				m_gameObject1->useAnimations(GameObject::Direction::up);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::down);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::left);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::right);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::none);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->setScale(0.8f);
				m_gameObject1->setPosition(560, 230);
				break;
			}
			case Meme::Spongegar: {
				//ai is spongegar
				m_ai = Meme::Spongegar;
				m_gameObject1 = new AnimatedGameObject(1.0f, GameObject::PlayerType::ai);
				m_gameObject1->load("content/spongegar64.png");
				m_gameObject1->useAnimations(GameObject::Direction::up);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::down);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::left);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::right);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::none);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->setScale(0.8f);
				m_gameObject1->setPosition(560, 230);
				break;
			}
			};
		}
		

		m_gameObjects = { m_gameObject0, m_gameObject1 };
		//Change gamestate
		m_gameState = GameState::Fighting;
		m_transition = false;
		break;
	}
	};
	//Make sure the transition ends
	m_transition = false;
}

void Game::render() {
    m_window.clear();
	m_window.draw(m_background);
	for (GameObject* gameObject : m_gameObjects) {
		gameObject->draw(m_window);
	}
	if (m_highlightCharacter) {
		m_window.draw(*m_characterSelectShape);
		m_window.draw(m_characterText);
	}
	if (m_gameState == GameState::GameWon)
		m_window.draw(m_gameWonText);
	m_window.display();
}

Game::GameState Game::isGameWon(float x, float y) {
	if (false)
		return GameState::GameWon;
	else
		return m_gameState;
}
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
            //handle key down here
                handlePlayerInput(event); //event.key.code
                break;
            case sf::Event::KeyReleased :
                handlePlayerInput(event);
                break;
			//case sf::Event::MouseButtonPressed :
			//	handlePlayerInput(event); //event.mousePressed
			//	break;
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
			//m_gameObject0->useAnimations(GameObject::Direction::none);
		}
		break;
	}
	case GameState::PlayerSelect : {
		if (event.type == sf::Event::EventType::MouseButtonReleased) {
			sf::Event::MouseButtonEvent mouseEvent = event.mouseButton;
			if (mouseEvent.button == sf::Mouse::Button::Left) {
				sf::Vector2<float> shapeLoc = m_characterSelectShape->getPosition();
				if (shapeLoc.x == 65 && shapeLoc.y == 78) {
					m_meme = Meme::Harambe;
					m_transition = true;
				}
				else if (shapeLoc.x == 200 && shapeLoc.y == 78) {
					m_meme = Meme::Kangaroo;
					m_transition = true;
				}
				else if (shapeLoc.x == 200 && shapeLoc.y == 225) {
					m_meme = Meme::EvilKermit;
					m_transition = true;
				}
				else if (shapeLoc.x == 335 && shapeLoc.y == 225) {
					m_meme = Meme::Pepe;
					m_transition = true;
				}
				else if (shapeLoc.x == 470 && shapeLoc.y == 225) {
					m_meme = Meme::DatBoi;
					m_transition = true;
				}
				else if (shapeLoc.x == 470 && shapeLoc.y == 78) {
					m_meme = Meme::JoeBiden;
					m_transition = true;
				}
				else if (shapeLoc.x == 600 && shapeLoc.y == 78) {
					m_meme = Meme::Spongegar;
					m_transition = true;
				}
			}
		}
		break;
	}
	case GameState::Fighting : {
		if (event.type == sf::Event::EventType::KeyPressed) {
			if (event.key.code == sf::Keyboard::Up) {
				m_gameObject0->m_up = true;
				m_gameObject0->useAnimations(GameObject::Direction::up);
			}
			if (event.key.code == sf::Keyboard::Down) {
				m_gameObject0->m_down = true;
				m_gameObject0->useAnimations(GameObject::Direction::down);
			}
			if (event.key.code == sf::Keyboard::Left) {
				m_gameObject0->m_left = true;
				m_gameObject0->useAnimations(GameObject::Direction::left);
			}
			if (event.key.code == sf::Keyboard::Right) {
				m_gameObject0->m_right = true;
				m_gameObject0->useAnimations(GameObject::Direction::right);
			}
		} 
		else {
			m_gameObject0->m_up = false;
			m_gameObject0->m_down = false;
			m_gameObject0->m_left = false;
			m_gameObject0->m_right = false;
			m_gameObject0->useAnimations(GameObject::Direction::none);
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
		float x = m_gameObject0->getPosition().x;
		if (x > 0 && x < 650) {
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
		switch (m_meme) {
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
			m_gameObject0 = new AnimatedGameObject(1.0f, GameObject::PlayerType::local);
			m_gameObject0->load("content/joeBiden64.png");
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
		m_gameObjects = { m_gameObject0 };
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
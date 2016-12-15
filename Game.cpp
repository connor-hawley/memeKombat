//
// Created by bswenson3 on 11/9/16.
//

#include "Game.h"
#include <iostream>
#include <string>

using namespace std;

Game::Game() :
	m_window(sf::VideoMode(800, 450), "Meme Kombat")
{
	m_window.setFramerateLimit(40);

	//load the background
	m_backgroundTexture.loadFromFile("memeCombat.jpg");
	m_background.setTexture(m_backgroundTexture, true);

	//Initialize the press enter game object
	m_gameObject0 = new AnimatedGameObject(0.66f, GameObject::PlayerType::other);
	m_gameObject0->load("pressEntertoBegin.png");
	m_gameObject0->useAnimations(GameObject::Direction::none);
	m_gameObject0->addTextureRect(sf::Rect<int>(0, 30, 290, 30));
	m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 290, 30));
	m_gameObject0->setPosition(255, 340);
	
	m_gameObjects = { m_gameObject0 };

	//make game font
	m_gameFont.loadFromFile("comicSans.ttf");
	
}

void Game::run() {
	sf::Clock clock;
    while(m_window.isOpen()) {
		if (!m_transition) {
			sf::Time deltaT = clock.restart();
			processEvents();
			if (m_gameState != GameOver) {
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
                handlePlayerInput(event);
                break;
			case sf::Event::KeyReleased:
                handlePlayerInput(event);
                break;
			case sf::Event::MouseButtonReleased:
				handlePlayerInput(event);
				break;
			case sf::Event::Closed: {
				for (GameObject* gameObject : m_gameObjects) {
					delete gameObject;
				}
				delete m_characterSelectShape;
				m_gameState = GameOver;
				m_window.close();
				break;
			}
				
        }
    }
}

void Game::handlePlayerInput(sf::Event event) {
	switch (m_gameState) {
	case Starting : {
		if (event.type == sf::Event::EventType::KeyReleased) {
			if (event.key.code == sf::Keyboard::Return) {
				m_transition = true;
			}
		}
		break;
	}
	case Lore: {
		if (event.type == sf::Event::EventType::KeyReleased) {
			if (event.key.code == sf::Keyboard::Return) {
				m_transition = true;
			}
		}
		break;
	}
	case Menu: {
		if (event.type = sf::Event::EventType::MouseButtonReleased) {
			sf::Event::MouseButtonEvent mouseEvent = event.mouseButton;
			if (mouseEvent.button == sf::Mouse::Button::Left) {
				sf::Vector2<float> shapeLoc = m_characterSelectShape->getPosition();
				if (shapeLoc.x == 290 && shapeLoc.y == 90) {
					m_gameMode = Play;
					m_transition = true;
				}
				else if (shapeLoc.x == 290 && shapeLoc.y == 190) {
					m_gameMode = Controls;
					m_transition = true;
				}
			}
		}
		break;
	}
	case Control: {
		if (event.type == sf::Event::EventType::KeyReleased) {
			if (event.key.code == sf::Keyboard::Return) {
				m_transition = true;
			}
		}
		break;
	}
	case PlayerSelect : {
		if (event.type == sf::Event::EventType::MouseButtonReleased) {
			sf::Event::MouseButtonEvent mouseEvent = event.mouseButton;
			if (mouseEvent.button == sf::Mouse::Button::Left) {
				sf::Vector2<float> shapeLoc = m_characterSelectShape->getPosition();
				if (shapeLoc.x == 65 && shapeLoc.y == 78) {
					m_player = Harambe;
					m_transition = true;
				}
				else if (shapeLoc.x == 200 && shapeLoc.y == 78) {
					m_player = Kangaroo;
					m_transition = true;
				}
				else if (shapeLoc.x == 200 && shapeLoc.y == 225) {
					m_player = EvilKermit;
					m_transition = true;
				}
				else if (shapeLoc.x == 335 && shapeLoc.y == 225) {
					m_player = Pepe;
					m_transition = true;
				}
				else if (shapeLoc.x == 470 && shapeLoc.y == 225) {
					m_player = DatBoi;
					m_transition = true;
				}
				else if (shapeLoc.x == 470 && shapeLoc.y == 78) {
					m_player = JoeBiden;
					m_transition = true;
				}
				else if (shapeLoc.x == 600 && shapeLoc.y == 78) {
					m_player = Spongegar;
					m_transition = true;
				}
			}
		}
		break;
	}
	case Fighting : {
        //GameObject0
		if (!m_gameObject0->m_animationTriggered) {
			if (event.type == sf::Event::EventType::KeyPressed) {
				//handle extra buttons. modern emulator controls are 
				//down - crouch
				//left - left
				//right - right
				//i - punch
				//o - block
				//p - kick
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

				if (event.key.code == sf::Keyboard::I) {
					//local punch mechanics
					if (!m_gameObject0->m_down && m_gameObject0->m_canPunch) {
						m_gameObject0->useAnimations(GameObject::Direction::punch);
						m_gameObject0->m_canPunch = false;
						m_gameObject0->m_animationTriggered = true;
						if (((x1 > x0 && x1 - x0 < 95) || (x0 > x1 && x0 - x1 < 95))
							&& !(m_gameObject1->m_down || m_gameObject1->m_block)) {
							m_highScore += 1;
							if (m_gameObject1->m_health - 3 > 0)
								m_gameObject1->m_health -= 3;
							else {
								m_gameObject1->m_health = 0;
								m_transition = true;
							}
						}
					}
					else if (m_gameObject0->m_down && m_gameObject0->m_canPunch) {
						m_gameObject0->useAnimations(GameObject::Direction::crouchPunch);
						m_gameObject0->m_canPunch = false;
						m_gameObject0->m_animationTriggered = true;
						if (((x1 > x0 && x1 - x0 < 95) || (x0 > x1 && x0 - x1 < 95))
							&& !(m_gameObject1->m_down || m_gameObject1->m_block)) {
							m_highScore += 30;
							if (m_gameObject1->m_health - 10 > 0)
								m_gameObject1->m_health -= 10;
							else {
								m_gameObject1->m_health = 0;
								m_transition = true;
							}
						}
					}
				}
				else if (event.key.code == sf::Keyboard::O) {
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
				else if (event.key.code == sf::Keyboard::P) {
					//local kick mechanics
					if (!m_gameObject0->m_down && m_gameObject0->m_canKick) {
						m_gameObject0->useAnimations(GameObject::Direction::kick);
						m_gameObject0->m_canKick = false;
						m_gameObject0->m_animationTriggered = true;
						if (((x1 > x0 && x1 - x0 < 95) || (x0 > x1 && x0 - x1 < 95))
							&& !(m_gameObject1->m_down || m_gameObject1->m_block)) {
							m_highScore += 7;
							if (m_gameObject1->m_health - 7 > 0)
								m_gameObject1->m_health -= 7;
							else {
								m_gameObject1->m_health = 0;
								m_transition = true;
							}
						}
					}
					else if (m_gameObject0->m_down && m_gameObject0->m_canKick) {
						m_gameObject0->useAnimations(GameObject::Direction::crouchKick);
						m_gameObject0->m_canKick = false;
						m_gameObject0->m_animationTriggered = true;
						if (((x1 > x0 && x1 - x0 < 95) || (x0 > x1 && x0 - x1 < 95))
							&& !m_gameObject1->m_block) {
							m_highScore += 2;
							if (m_gameObject1->m_health - 5 > 0)
								m_gameObject1->m_health -= 5;
							else {
								m_gameObject1->m_health = 0;
								m_transition = true;
							}
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
                    cout << "Right Ended" << endl;
					m_gameObject0->m_right = false;
					m_gameObject0->useAnimations(GameObject::Direction::none);
				}
				if (event.key.code == sf::Keyboard::I) {
					m_gameObject0->m_canPunch = true;
					m_gameObject0->useAnimations(GameObject::Direction::none);
				}
				else if (event.key.code = sf::Keyboard::O) {
					m_gameObject0->m_block = false;
					m_gameObject0->useAnimations(GameObject::Direction::none);
				}
				else if (event.key.code = sf::Keyboard::P) {
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

        //GameObject1
        if (!m_gameObject1->m_animationTriggered) {
            if (event.type == sf::Event::EventType::KeyPressed) {
                //handle extra buttons. modern emulator controls are
                //down - crouch
                //left - left
                //right - right
                //i - punch
                //o - block
                //p - kick
                //different attacks when crouching
                float x0 = m_gameObject1->getPosition().x;
                float x1 = m_gameObject0->getPosition().x;
                if (event.key.code == sf::Keyboard::S) {
                    m_gameObject1->m_down = true;
                    m_gameObject1->useAnimations(GameObject::Direction::down);
                }
                else if (event.key.code == sf::Keyboard::A) {
                    m_gameObject1->m_left = true;
                    m_gameObject1->useAnimations(GameObject::Direction::left);
                }
                else if (event.key.code == sf::Keyboard::D) {
                    m_gameObject1->m_right = true;
                    m_gameObject1->useAnimations(GameObject::Direction::right);
                }

                if (event.key.code == sf::Keyboard::C) {
                    //local punch mechanics
                    if (!m_gameObject1->m_down && m_gameObject1->m_canPunch) {
                        m_gameObject1->useAnimations(GameObject::Direction::punch);
                        m_gameObject1->m_canPunch = false;
                        m_gameObject1->m_animationTriggered = true;
                        if (((x1 > x0 && x1 - x0 < 95) || (x0 > x1 && x0 - x1 < 95))
                            && !(m_gameObject0->m_down || m_gameObject0->m_block)) {
                            m_highScore += 1;
                            if (m_gameObject0->m_health - 3 > 0)
                                m_gameObject0->m_health -= 3;
                            else {
                                m_gameObject0->m_health = 0;
                                m_transition = true;
                            }
                        }
                    }
                    else if (m_gameObject1->m_down && m_gameObject1->m_canPunch) {
                        m_gameObject1->useAnimations(GameObject::Direction::crouchPunch);
                        m_gameObject1->m_canPunch = false;
                        m_gameObject1->m_animationTriggered = true;
                        if (((x1 > x0 && x1 - x0 < 95) || (x0 > x1 && x0 - x1 < 95))
                            && !(m_gameObject0->m_down || m_gameObject0->m_block)) {
                            m_highScore += 30;
                            if (m_gameObject0->m_health - 10 > 0)
                                m_gameObject0->m_health -= 10;
                            else {
                                m_gameObject0->m_health = 0;
                                m_transition = true;
                            }
                        }
                    }
                }
                else if (event.key.code == sf::Keyboard::V) {
                    //local block mechanics
                    if (!m_gameObject1->m_down) {
                        m_gameObject1->useAnimations(GameObject::Direction::block);
                        m_gameObject1->m_block = true;
                    }
                    else if (m_gameObject1->m_down) {
                        m_gameObject1->useAnimations(GameObject::Direction::crouchBlock);
                        m_gameObject1->m_block = true;
                    }
                }
                else if (event.key.code == sf::Keyboard::B) {
                    //local kick mechanics
                    if (!m_gameObject1->m_down && m_gameObject1->m_canKick) {
                        m_gameObject1->useAnimations(GameObject::Direction::kick);
                        m_gameObject1->m_canKick = false;
                        m_gameObject1->m_animationTriggered = true;
                        if (((x1 > x0 && x1 - x0 < 95) || (x0 > x1 && x0 - x1 < 95))
                            && !(m_gameObject0->m_down || m_gameObject0->m_block)) {
                            m_highScore += 7;
                            if (m_gameObject0->m_health - 7 > 0)
                                m_gameObject0->m_health -= 7;
                            else {
                                m_gameObject0->m_health = 0;
                                m_transition = true;
                            }
                        }
                    }
                    else if (m_gameObject1->m_down && m_gameObject1->m_canKick) {
                        m_gameObject1->useAnimations(GameObject::Direction::crouchKick);
                        m_gameObject1->m_canKick = false;
                        m_gameObject1->m_animationTriggered = true;
                        if (((x1 > x0 && x1 - x0 < 95) || (x0 > x1 && x0 - x1 < 95))
                            && !m_gameObject0->m_block) {
                            m_highScore += 2;
                            if (m_gameObject0->m_health - 5 > 0)
                                m_gameObject0->m_health -= 5;
                            else {
                                m_gameObject0->m_health = 0;
                                m_transition = true;
                            }
                        }
                    }
                }
            }
            else if (event.type == sf::Event::EventType::KeyReleased) {
                if (event.key.code == sf::Keyboard::S) {
                    m_gameObject1->m_down = false;
                    m_gameObject1->useAnimations(GameObject::Direction::none);
                }
                else if (event.key.code == sf::Keyboard::A) {
                    m_gameObject1->m_left = false;
                    m_gameObject1->useAnimations(GameObject::Direction::none);
                }
                else if (event.key.code == sf::Keyboard::D) {
                    cout << "Key D released" << endl;
                    m_gameObject1->m_right = false;
                    m_gameObject1->useAnimations(GameObject::Direction::none);
                }
                if (event.key.code == sf::Keyboard::C) {
                    m_gameObject1->m_canPunch = true;
                    m_gameObject1->useAnimations(GameObject::Direction::none);
                }
                else if (event.key.code = sf::Keyboard::V) {
                    m_gameObject1->m_block = false;
                    m_gameObject1->useAnimations(GameObject::Direction::none);
                }
                else if (event.key.code = sf::Keyboard::B) {
                    m_gameObject1->m_canKick = true;
                    m_gameObject1->useAnimations(GameObject::Direction::none);
                }
            }
            else {
                m_gameObject1->m_down = false;
                m_gameObject1->m_left = false;
                m_gameObject1->m_right = false;
                m_gameObject1->m_canPunch = true;
                m_gameObject1->m_canKick = true;
                m_gameObject1->m_animationTriggered = false;
                m_gameObject1->useAnimations(GameObject::Direction::none);
            }
        }
		break;
	}
	case GameWon: {
		if (event.type == sf::Event::EventType::KeyReleased) {
			if (event.key.code == sf::Keyboard::Return) {
				m_transition = true;
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
	switch (m_gameState) {
	case Menu: {
		//highlights the menu option the player selected

		sf::Vector2<int> mouseLoc = sf::Mouse::getPosition(m_window);
		if (mouseLoc.x > 290 && mouseLoc.x < 520 && mouseLoc.y > 90 && mouseLoc.y < 157) {
			m_characterSelectShape->setPosition(290, 90);
			m_highlightCharacter = true;
		}
		else if (mouseLoc.x > 290 && mouseLoc.x < 520 && mouseLoc.y > 190 && mouseLoc.y < 257) {
			m_characterSelectShape->setPosition(290, 190);
			m_highlightCharacter = true;
		}
		else {
			m_characterSelectShape->setPosition(0, 0);
			m_highlightCharacter = false;
		}
		break;
	}
	case PlayerSelect : {
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
	case Fighting: {
        //GameObject0
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

        //GameObject1
        sf::Vector2<float> movement2(0.0f, 0.0f);
        if (m_gameObject1->m_left)
            movement2.x -= m_gameObject1->getSpeed();
        if (m_gameObject1->m_right)
            movement2.x += m_gameObject1->getSpeed();
        if (x0 > 0 && x0 < 650 && ((x0 < x1 && x1 - x0 > 55) || (x0 > x1 && x0 - x1 > 55))) {
            m_gameObject1->m_isStuck = false;
            m_gameObject1->move(movement2 * deltaT.asSeconds());
        }
        else {
            m_gameObject1->m_isStuck = true;
            switch (m_gameObject1->getLastDirection()) {
                case GameObject::Direction::left: {
                    if (!m_gameObject1->GameObject::m_left)
                        m_gameObject1->move(movement2 * deltaT.asSeconds());
                    break;
                }
                case GameObject::Direction::right: {
                    if (!m_gameObject1->GameObject::m_right)
                        m_gameObject1->move(movement2 * deltaT.asSeconds());
                    break;
                }
            };
        }

		//update high score
		m_highScoreText.setString(std::to_string(m_highScore));
	}
	};
}

void Game::initializeState() {
	//Delete objects
	for (GameObject* gameObject : m_gameObjects) {
		delete gameObject;
	}
	switch (m_gameState) {
	case Starting: {
		//set background
		m_characterSelectShape = new sf::RectangleShape(sf::Vector2<float>(800, 450));
		m_characterSelectShape->setOutlineColor(sf::Color::Transparent);
		m_characterSelectShape->setFillColor(sf::Color::Black);
		//m_backgroundTexture.loadFromFile("blackScreen.png");
		//m_background.setTexture(m_backgroundTexture);
		//spicy memes
		m_gameObjects = {};
		//repurposing character text for lore screen
		m_characterText.setFont(m_gameFont);
		m_characterText.setCharacterSize(16);
		m_characterText.setFillColor(sf::Color(0, 245, 61));
		m_characterText.setPosition(75, 150);
		m_characterText.setString("Once upon a time, in a meme world far, far away, all the memes lived happily in peace.\nThen, one day, the shit-posters attacked, turning the memes against each other.\nNow, the memes battle to determine the spiciest meme of all.\n\n\nPRESS ENTER TO CONTINUE");
		m_gameState = Lore;
		m_transition = false;
		break;
	}
	case Control:
	case Lore: {
		//delete character select shape
		delete m_characterSelectShape;
		//create selection shape
		m_characterSelectShape = new sf::RectangleShape(sf::Vector2<float>(230, 67));
		m_characterSelectShape->setOutlineColor(sf::Color::Blue);
		m_characterSelectShape->setOutlineThickness(3.5f);
		m_characterSelectShape->setFillColor(sf::Color::Transparent);
		m_characterSelectShape->setPosition(290, 90);
		//load background
		m_backgroundTexture.loadFromFile("blackScreen.png");
		m_background.setTexture(m_backgroundTexture);
		//spicy memes
		m_gameObjects = {};
		//repurposing character text for menu
		m_characterText.setCharacterSize(35);
		m_characterText.setFillColor(sf::Color(196, 169, 86));
		m_characterText.setPosition(300, 100);
		m_characterText.setString("Play\n\nControls");
		m_gameState = Menu;
		m_transition = false;
		break;
	}
	case Menu: {
		switch (m_gameMode) {
		case Play: {
			//Set new background
			m_backgroundTexture.loadFromFile("chooseFighter.jpg");
			m_background.setTexture(m_backgroundTexture);
			//Make new gameObjects
			m_gameObjects = {};
			//Make new character select rectangle
			delete m_characterSelectShape;
			m_characterSelectShape = new sf::RectangleShape(sf::Vector2<float>(120, 134));
			m_characterSelectShape->setOutlineColor(sf::Color::Green);
			m_characterSelectShape->setOutlineThickness(3.5f);
			m_characterSelectShape->setFillColor(sf::Color::Transparent);
			//Set up text to describe characters
			m_characterText.setCharacterSize(25);
			m_characterText.setFillColor(sf::Color(196, 169, 86));
			m_characterText.setPosition(340, 70);
			m_characterText.setOutlineColor(sf::Color::Black);
			m_characterText.setOutlineThickness(2);
			//Change gameState
			m_gameState = PlayerSelect;
			break;
		}
		case Controls: {
			//Set new background
			m_backgroundTexture.loadFromFile("blackScreen.png");
			m_background.setTexture(m_backgroundTexture);
			//Make new gameObjects
			m_gameObjects = {};
			//Make new character select rectangle
			m_highlightCharacter = false;
			//Write out controls
			m_characterText.setCharacterSize(25);
			m_characterText.setFillColor(sf::Color(196, 169, 86));
			m_characterText.setPosition(150, 30);
			m_characterText.setString("                    PLAYER 1                    PLAYER 2 \n\n-Move          Left/Right                  A/D\n-Crouch        Down                           S\n-Punch          I                                  C\n-Block          O                                 V\n-Kick            P                                  B\n\nPress enter to go back");
			m_gameState = Control;
			break;
		}
		};
		m_transition = false;
		break;
	}
	case PlayerSelect: {
		//Stop displaying character select shape
		m_highlightCharacter = false;
		//Set new background
		m_backgroundTexture.loadFromFile("theCourtyard.jpg");
		m_background.setTexture(m_backgroundTexture);
		//Set up high score text
		m_highScoreText.setFont(m_gameFont);
		m_highScoreText.setCharacterSize(50);
		m_highScoreText.setString(std::to_string(m_highScore));
		m_highScoreText.setFillColor(sf::Color(196, 169, 86));
		m_highScoreText.setPosition(350, 10);
		m_highScoreText.setOutlineColor(sf::Color::Black);
		m_highScoreText.setOutlineThickness(2);
		switch (m_player) {
		case Harambe: {
			//display harambe
			/*m_gameObject0 = new RegularGameObject();
			m_gameObject0->load("harambe64.png");
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->setScale(0.8f);
			m_gameObject0->setPosition(120, 230);*/
			m_gameObject0 = new AnimatedGameObject(1.0f, GameObject::PlayerType::local);
			m_gameObject0->load("harambe64.png");
			m_gameObject0->useAnimations(GameObject::Direction::up);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::down);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::left);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::right);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::punch);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchPunch);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::block);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchBlock);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::kick);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchKick);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::none);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->setScale(0.8f);
			m_gameObject0->setPosition(560, 230);
			break;
		}
		case Kangaroo: {
			//display kangaroo
			m_gameObject0 = new AnimatedGameObject(1.0f, GameObject::PlayerType::local);
			m_gameObject0->load("kangaroo64.png");
			m_gameObject0->useAnimations(GameObject::Direction::up);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::down);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::left);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::right);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::punch);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchPunch);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::block);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchBlock);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::kick);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchKick);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::none);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->setScale(0.8f);
			m_gameObject0->setPosition(560, 230);
			break;
		}
		case EvilKermit: {
			//display evil kermit
			m_gameObject0 = new AnimatedGameObject(1.0f, GameObject::PlayerType::local);
			m_gameObject0->load("evilKermit64.png");
			m_gameObject0->useAnimations(GameObject::Direction::up);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::down);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::left);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::right);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::punch);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchPunch);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::block);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchBlock);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::kick);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchKick);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::none);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->setScale(0.8f);
			m_gameObject0->setPosition(560, 230);
			break;
		}
		case Pepe: {
			//display pepe
			m_gameObject0 = new AnimatedGameObject(1.0f, GameObject::PlayerType::local);
			m_gameObject0->load("pepe64.png");
			m_gameObject0->useAnimations(GameObject::Direction::up);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::down);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::left);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::right);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::punch);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchPunch);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::block);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchBlock);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::kick);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchKick);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::none);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->setScale(0.8f);
			m_gameObject0->setPosition(560, 230);
			break;
		}
		case DatBoi: {
			//display dat boi
			m_gameObject0 = new AnimatedGameObject(1.0f, GameObject::PlayerType::local);
			m_gameObject0->load("datBoi64.png");
			m_gameObject0->useAnimations(GameObject::Direction::up);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::down);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::left);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::right);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::punch);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchPunch);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::block);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchBlock);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::kick);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchKick);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::none);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->setScale(0.8f);
			m_gameObject0->setPosition(560, 230);
			break;
		}
		case JoeBiden: {
			//display joe biden
			m_gameObject0 = new AnimatedGameObject(0.20f, GameObject::PlayerType::local);
			m_gameObject0->load("joeBidenSprites.png");
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
			m_gameObject0->setPosition(560, 206);
			break;
		}
		case Spongegar: {
			//display spongegar
			m_gameObject0 = new AnimatedGameObject(1.0f, GameObject::PlayerType::local);
			m_gameObject0->load("spongegar64.png");
			m_gameObject0->useAnimations(GameObject::Direction::up);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::down);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::left);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::right);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::punch);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchPunch);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::block);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchBlock);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::kick);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::crouchKick);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->useAnimations(GameObject::Direction::none);
			m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
			m_gameObject0->setScale(0.8f);
			m_gameObject0->setPosition(560, 230);
			break;
		}
		};
		//create other remote or remote player
		m_remote = m_player;
		while (m_remote == m_player) {
			srand(time(NULL));
			int random = rand() % 6; //random number 0 to 6
			switch (random) {
			case Harambe: {
				//remote is harambe
                m_remote = Harambe;
				m_gameObject1 = new AnimatedGameObject(1.0f, GameObject::PlayerType::remote);
				m_gameObject1->load("harambe64.png");
				m_gameObject1->useAnimations(GameObject::Direction::up);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::down);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::left);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::right);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::punch);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchPunch);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::block);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchBlock);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::kick);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchKick);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::none);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->setScale(0.8f);
				m_gameObject1->setPosition(120, 230);
				break;
			}
			case Kangaroo: {
				//remote is kangaroo
                m_remote = Kangaroo;
				m_gameObject1 = new AnimatedGameObject(1.0f, GameObject::PlayerType::remote);
				m_gameObject1->load("kangaroo64.png");
				m_gameObject1->useAnimations(GameObject::Direction::up);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::down);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::left);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::right);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::punch);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchPunch);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::block);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchBlock);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::kick);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchKick);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::none);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->setScale(0.8f);
				m_gameObject1->setPosition(120, 230);
				break;
			}
			case EvilKermit: {
				//remote is evil kermit
                m_remote = EvilKermit;
				m_gameObject1 = new AnimatedGameObject(1.0f, GameObject::PlayerType::remote);
				m_gameObject1->load("evilKermit64.png");
				m_gameObject1->useAnimations(GameObject::Direction::up);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::down);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::left);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::right);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::punch);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchPunch);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::block);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchBlock);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::kick);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchKick);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::none);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->setScale(0.8f);
				m_gameObject1->setPosition(120, 230);
				break;
			}
			case Pepe: {
				//remote is pepe
                m_remote = Pepe;
				m_gameObject1 = new AnimatedGameObject(1.0f, GameObject::PlayerType::remote);
				m_gameObject1->load("pepe64.png");
				m_gameObject1->useAnimations(GameObject::Direction::up);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::down);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::left);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::right);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::punch);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchPunch);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::block);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchBlock);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::kick);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchKick);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::none);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->setScale(0.8f);
				m_gameObject1->setPosition(120, 230);
				break;
			}
			case DatBoi: {
				//remote is dat boi
                m_remote = DatBoi;
				m_gameObject1 = new AnimatedGameObject(1.0f, GameObject::PlayerType::remote);
				m_gameObject1->load("datBoi64.png");
				m_gameObject1->useAnimations(GameObject::Direction::up);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::down);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::left);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::right);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::punch);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchPunch);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::block);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchBlock);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::kick);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchKick);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::none);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->setScale(0.8f);
				m_gameObject1->setPosition(120, 230);
				break;
			}
			case JoeBiden: {
				//remote is joe biden
				m_remote = JoeBiden;
				m_gameObject1 = new AnimatedGameObject(1.0f, GameObject::PlayerType::remote);
				m_gameObject1->load("joeBiden64.png");
				m_gameObject1->useAnimations(GameObject::Direction::up);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::down);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::left);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::right);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::punch);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchPunch);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::block);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchBlock);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::kick);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchKick);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::none);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->setScale(0.8f);
				m_gameObject1->setPosition(120, 230);
				break;
			}
			case Spongegar: {
				//remote is spongegar
                m_remote = Spongegar;
				m_gameObject1 = new AnimatedGameObject(1.0f, GameObject::PlayerType::remote);
				m_gameObject1->load("spongegar64.png");
				m_gameObject1->useAnimations(GameObject::Direction::up);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::down);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::left);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::right);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::punch);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchPunch);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::block);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchBlock);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::kick);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::crouchKick);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->useAnimations(GameObject::Direction::none);
				m_gameObject1->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
				m_gameObject1->setScale(0.8f);
				m_gameObject1->setPosition(120, 230);
				break;
			}
			};
		}
		m_gameObjects = { m_gameObject0, m_gameObject1 };
		//Change gamestate
		m_gameState = Fighting;
		m_transition = false;
		break;
	}
	case Fighting: {
		//reset background
		//m_backgroundTexture.loadFromFile("theCourtyard.jpg");
		//m_background.setTexture(m_backgroundTexture);
		//game over text
		if (m_highScore > m_highestScore) {
			m_highestScore = m_highScore;
		}
		//set gameWonText
		m_gameWonText.setFont(m_gameFont);
		m_gameWonText.setString("GAME OVER\npress enter to play again\nhigh score: \nyour score: ");
		m_gameWonText.setCharacterSize(55);
		m_gameWonText.setFillColor(sf::Color(196, 169, 86));
		m_gameWonText.setPosition(100, 10);
		m_gameWonText.setOutlineColor(sf::Color::Black);
		m_gameWonText.setOutlineThickness(2);
		//set up highScore and highestScore text boxes
		m_highestScoreText.setFont(m_gameFont);
		m_highestScoreText.setString(std::to_string(m_highScore));
		m_highestScoreText.setCharacterSize(55);
		m_highestScoreText.setFillColor(sf::Color(196, 169, 86));
		m_highestScoreText.setPosition(400, 170);
		m_highestScoreText.setOutlineColor(sf::Color::Black);
		m_highestScoreText.setOutlineThickness(2);
		m_highScoreText.setPosition(400, 250);
		//creat gameObjects
		m_gameObjects = {};
		m_gameState = GameWon;
		m_transition = false;
		break;
	}
	case GameWon: {
		//load the background
		m_backgroundTexture.loadFromFile("memeCombat.jpg");
		m_background.setTexture(m_backgroundTexture, true);
		//reset high score
		m_highScore = 0;
		//Initialize the press enter game object
		m_gameObject0 = new AnimatedGameObject(0.66f, GameObject::PlayerType::other);
		m_gameObject0->load("pressEntertoBegin.png");
		m_gameObject0->useAnimations(GameObject::Direction::none);
		m_gameObject0->addTextureRect(sf::Rect<int>(0, 30, 290, 30));
		m_gameObject0->addTextureRect(sf::Rect<int>(0, 0, 290, 30));
		m_gameObject0->setPosition(255, 340);

		m_gameObjects = { m_gameObject0 };
		m_gameState = Starting;
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
	if (m_highlightCharacter || m_gameState == Lore) {
		m_window.draw(*m_characterSelectShape);
		m_window.draw(m_characterText);
	}
	if (m_gameState == Menu || m_gameState == Control) {
		m_window.draw(m_characterText);
	}
	if (m_gameState == Fighting || m_gameState == GameWon)
		m_window.draw(m_highScoreText);
	if (m_gameState == GameWon) {
		m_window.draw(m_gameWonText);
		m_window.draw(m_highestScoreText);
	}
		
	m_window.display();
}

Game::GameState Game::isGameWon(float x, float y) {
	if (false)
		return GameWon;
	else
		return m_gameState;
}
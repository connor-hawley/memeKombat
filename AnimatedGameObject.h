//
// Created by Connor Hawley on 11/20/16.
//
#pragma once

#include <SFML/Graphics.hpp>
#include "GameObject.h"

class AnimatedGameObject : public GameObject {
public:
    AnimatedGameObject(float spriteInterval, bool isPlayer);

	~AnimatedGameObject() {}
	
    bool load(const std::string &filename);

    void draw(sf::RenderWindow &window);

	void update();

    void move(sf::Vector2f);

	void setPosition(float x, float y);

    sf::Vector2f getPosition() const;

	float getHeight() const { return 0.0f; }

	float getWidth() const { return 0.0f; }

    void setScale(float scale);

	void addTextureRect(const sf::Rect<int> rectangle);

	void nextTextureRect();

	void setTextureRect(int ind);

	void setSpriteInterval(float spriteInterval);

	void setStrobeInterval(float strobeInterval);

	void useAnimations(GameObject::Direction dir);

	float getSpeed();

	GameObject::Direction getLastDirection();

private:
    sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::Clock m_clock;
	sf::Clock m_strobeClock;
	std::string m_filename;

	std::vector<sf::Rect<int>> m_intRectsDown;
	std::vector<sf::Rect<int>> m_intRectsRight;
	std::vector<sf::Rect<int>> m_intRectsUp;
	std::vector<sf::Rect<int>> m_intRectsLeft;
	std::vector<sf::Rect<int>> m_intRectsNone;
	std::vector<sf::Rect<int>> *m_intRectRef;
	GameObject::Direction m_lastDirection = none;

	bool m_valid = false;
	bool m_strobe = false;
	bool m_isPlayer = false;
	int m_currentTextureRect;
	float m_spriteInterval;
	float m_spriteIntervalIdle;
	float m_spriteIntervalMove;
	float m_strobeInitial;
	float m_strobeDestination;

	float m_speed = 120.0f;
};



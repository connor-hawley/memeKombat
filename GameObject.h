//
// Created by bswenson3 on 11/9/16.
//
#pragma once

#include <SFML/Graphics.hpp>

class GameObject {
public:
	bool m_isStuck = false;
	bool m_left = false;
	bool m_right = false;
	bool m_up = false;
	bool m_down = false;
	float m_health = 100.0f;
	enum Direction { up, down, left, right, none };
	enum PlayerType { local, remote, ai, strobe, other };

    virtual bool load(const std::string& filename) = 0;

    virtual void draw(sf::RenderWindow& window) = 0;

	virtual void update() = 0;

    virtual void move(sf::Vector2f) = 0;

	virtual void setPosition(float x, float y) = 0;

    virtual sf::Vector2f getPosition() const = 0;

    virtual float getHeight() const = 0;

    virtual float getWidth() const = 0;

    virtual void setScale(float scale) = 0;

	virtual void addTextureRect(const sf::Rect<int> rectangle) = 0;

	virtual void nextTextureRect() = 0;

	virtual void setTextureRect(int ind) = 0;

	virtual void setSpriteInterval(float spriteInterval) = 0;

	virtual void setStrobeInterval(float strobeInterval) = 0;

	virtual void useAnimations(Direction dir) = 0;

	virtual float getSpeed() = 0;

	virtual Direction getLastDirection() = 0;	
};



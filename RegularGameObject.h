//
// Created by Connor Hawley on 11/20/16
//
#pragma once

#include <SFML/Graphics.hpp>
#include "GameObject.h"

class RegularGameObject : public GameObject {
public:
	
	RegularGameObject();

	~RegularGameObject() {}
	
	bool load(const std::string& filename);

    void draw(sf::RenderWindow& window);

	void update() {}

	void move(sf::Vector2f) {}

	void setPosition(float x, float y);

	sf::Vector2f getPosition() const { return sf::Vector2f(0.0f, 0.0f); }

	float getHeight() const { return 0.0f; }

	float getWidth() const { return 0.0f; }

    void setScale(float scale);

	void addTextureRect(const sf::Rect<int> rectangle);

	void nextTextureRect() {}

	void setTextureRect(int ind) {};

	void setSpriteInterval(float spriteInterval) {}

	void setStrobeInterval(float strobeInterval) {}

	void useAnimations(Direction dir) {}

	float getSpeed() { return 0.0f; }

	Direction getLastDirection() { return none; }

private:
	sf::Sprite m_sprite;
    sf::Texture m_texture;
	sf::Rect<int> m_textureRect;
    std::string m_filename;
    bool m_valid = false;

};
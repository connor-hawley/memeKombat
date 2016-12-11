//
// Created by Connor Hawley on 11/20/16.
//

#include "RegularGameObject.h"

RegularGameObject::RegularGameObject() {

}

bool RegularGameObject::load(const std::string& filename) {
	if (m_texture.loadFromFile(filename)) {
		m_filename = filename;
		m_sprite.setTexture(m_texture);
		m_valid = true;
		return true;
	}
	return false;
}


void RegularGameObject::draw(sf::RenderWindow& window) {
	if (m_valid)
		window.draw(m_sprite);
}

void RegularGameObject::setPosition(float x, float y) {
	if (m_valid)
		m_sprite.setPosition(x, y);
}


void RegularGameObject::setScale(float scale) {
	m_sprite.setScale(scale, scale);
}

void RegularGameObject::addTextureRect(const sf::Rect<int> rectangle) {
	m_textureRect = rectangle;
	m_sprite.setTextureRect(m_textureRect);
}


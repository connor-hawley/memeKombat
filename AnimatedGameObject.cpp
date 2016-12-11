//
// Created by Connor Hawley on 11/20/16.
//

#include "AnimatedGameObject.h"

AnimatedGameObject::AnimatedGameObject(float spriteInterval, bool isPlayer) {
	m_spriteInterval = spriteInterval;
	m_spriteIntervalMove = spriteInterval;
	m_isPlayer = isPlayer;
	if (m_isPlayer)
		m_spriteIntervalIdle = m_spriteIntervalMove + 1.85f;
	m_currentTextureRect = 0;
}

bool AnimatedGameObject::load(const std::string& filename) {
	if (m_texture.loadFromFile(filename)) {
		m_texture.setSmooth(true);
		m_sprite.setTexture(m_texture);
		m_filename = filename;
		m_valid = true;
		return true;
	}
	return false;
}


void AnimatedGameObject::draw(sf::RenderWindow& window) {
	if (m_valid)
		window.draw(m_sprite);
}

void AnimatedGameObject::update() {
	if (m_clock.getElapsedTime().asSeconds() > m_spriteInterval) {
		nextTextureRect();
		m_clock.restart();
	} 
	if (m_strobe) {
		sf::Vector2<float> currentPos = this->getPosition();
		if (m_strobeDestination - m_strobeInitial > 0
			&& m_strobeDestination - currentPos.x > 0)
			move(sf::Vector2<float>(40.0f, 0) * m_strobeClock.getElapsedTime().asSeconds());
		else if (m_strobeDestination - m_strobeInitial < 0 
			&& m_strobeDestination - currentPos.x < 0) 
			move(sf::Vector2<float>(-40.0f, 0) * m_strobeClock.getElapsedTime().asSeconds());
		else 
			setStrobeInterval(m_strobeInitial - m_strobeDestination);
		m_strobeClock.restart();
	}
}

void AnimatedGameObject::move(sf::Vector2f loc) {
	if (m_valid)
		m_sprite.move(loc);
}

void AnimatedGameObject::setPosition(float x, float y) {
	if (m_valid)
		m_sprite.setPosition(x, y);
}

sf::Vector2f AnimatedGameObject::getPosition() const {
	if (m_valid)
		return m_sprite.getPosition();
	else
		return sf::Vector2f(0, 0);
}


void AnimatedGameObject::setScale(float scale) {
	if (m_valid)
		m_sprite.setScale(scale, scale);
}

void AnimatedGameObject::addTextureRect(const sf::Rect<int> rectangle) {
	m_intRectRef->push_back(rectangle);
	this->setTextureRect(m_intRectRef->size() - 1);
}

void AnimatedGameObject::nextTextureRect() {
	if (m_currentTextureRect < (int) m_intRectRef->size() - 1 && !m_intRectRef->empty()) {
		std::vector<sf::Rect<int>> &rectVec = *m_intRectRef;
		sf::Rect<int> &intRect = rectVec[++m_currentTextureRect];
		m_sprite.setTextureRect(intRect);
	}
	else if (!m_intRectRef->empty()) {
		m_currentTextureRect = 0;
		std::vector<sf::Rect<int>> &rectVec = *m_intRectRef;
		sf::Rect<int> &intRect = rectVec[m_currentTextureRect];
		m_sprite.setTextureRect(intRect);
	}
	else {
		throw "TextureRect vector is empty";
	}
}

void AnimatedGameObject::setTextureRect(int ind) {
	std::vector<sf::Rect<int>> &rectVec = *m_intRectRef;
	if (ind < (int) m_intRectRef->size() && !rectVec.empty()) {
		sf::Rect<int>&intRect = rectVec[ind];
		m_sprite.setTextureRect(intRect);
	}
}


void AnimatedGameObject::setSpriteInterval(float spriteInterval) { m_spriteInterval = spriteInterval; }

void AnimatedGameObject::setStrobeInterval(float strobeInterval) {
	m_strobe = true;
	m_strobeInitial = this->getPosition().x;
	m_strobeDestination = m_strobeInitial + strobeInterval;
}

void AnimatedGameObject::useAnimations(GameObject::Direction dir) {
	switch (dir) {
	case up: {
		m_intRectRef = &m_intRectsUp;
		this->setSpriteInterval(m_spriteIntervalMove);
		if (!m_isStuck)
			m_lastDirection = up;
		break;
	}
	case down: {
		m_intRectRef = &m_intRectsDown;
		this->setSpriteInterval(m_spriteIntervalMove);
		if (!m_isStuck)
			m_lastDirection = down;
		break;
	}
	case left: {
		m_intRectRef = &m_intRectsLeft;
		this->setSpriteInterval(m_spriteIntervalMove);
		if (!m_isStuck)
			m_lastDirection = left;
		break;
	}
	case right: {
		m_intRectRef = &m_intRectsRight;
		this->setSpriteInterval(m_spriteIntervalMove);
		if (!m_isStuck)
			m_lastDirection = right;
		break;
	}
	case none: {
		m_intRectRef = &m_intRectsNone;
		if (m_isPlayer)
			this->setSpriteInterval(m_spriteIntervalIdle);
		else
			this->setSpriteInterval(m_spriteIntervalMove);

		if (!m_isStuck)
			m_lastDirection = none;
		break;
	}
	};
}

float AnimatedGameObject::getSpeed() { return m_speed; }

GameObject::Direction AnimatedGameObject::getLastDirection() { return m_lastDirection; }

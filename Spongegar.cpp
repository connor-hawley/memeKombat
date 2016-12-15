//
// Created by Nitish Katta on 12/15/16.
//

#include "Spongegar.h"

Spongegar::Spongegar(float spriteInterval, PlayerType playerType) {
    // Call to superclass constructor
    AnimatedGameObject(spriteInterval, playerType);
    this->load("spongegar64.png");
    this->useAnimations(GameObject::Direction::up);
    this->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
    this->useAnimations(GameObject::Direction::down);
    this->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
    this->useAnimations(GameObject::Direction::left);
    this->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
    this->useAnimations(GameObject::Direction::right);
    this->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
    this->useAnimations(GameObject::Direction::punch);
    this->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
    this->useAnimations(GameObject::Direction::crouchPunch);
    this->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
    this->useAnimations(GameObject::Direction::block);
    this->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
    this->useAnimations(GameObject::Direction::crouchBlock);
    this->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
    this->useAnimations(GameObject::Direction::kick);
    this->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
    this->useAnimations(GameObject::Direction::crouchKick);
    this->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
    this->useAnimations(GameObject::Direction::none);
    this->addTextureRect(sf::Rect<int>(0, 0, 120, 134));
    this->setScale(0.8f);
    this->setPosition(560, 230);

}
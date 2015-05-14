#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <memory>

namespace Textures {
	enum ID
	{
		Landscape, Player, Fog, Lamp, BackgroundGrass, Monster, UI, HealthBar,
		Inventory, Sword, Gold, Flash, PortalForest, Fireball
	};
}

class TextureHolder
{
public:
	TextureHolder(void);
	~TextureHolder(void);



	void load( Textures::ID id, const std::string& filename );
	sf::Texture& get( Textures::ID id );
	const sf::Texture& get( Textures::ID id ) const;

private:
	std::map<Textures::ID, std::unique_ptr<sf::Texture>> mTextureMap;
};


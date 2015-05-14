#pragma once
#include <SFML/Graphics.hpp>
#include "TextureHolder.h"

class BoxedObj
{
public:
	
	BoxedObj(void);
	~BoxedObj(void);
	
	void			setBox( int left, int top, int width, int height );
	sf::IntRect&	getBox();
	//void			setTextureHolder( TextureHolder & textures );
	//void			draw();
	//virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:

	sf::IntRect		boundingBox;
	//TextureHolder   m_textures;

};


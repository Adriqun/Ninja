/**
    play_button.h
    Purpose: class Play_button - for button 'play' to start game, contain state.

    @author Adrian Michalek
    @version 2016.10.03
	@email adrmic98@gmail.com
*/

#include "play_button.h"


Play_button::Play_button()
{
	free();
}

Play_button::~Play_button()
{
	free();
}

void Play_button::free()
{
	state = 0;
	
	mySprite.free();
	myText.free();
	
	play = true;
	click.free();
}



void Play_button::load( int screen_w, int bot )
{
	mySprite.setName( "play_button-mySprite" );
	mySprite.load( "data/02_menu/blankbutton.png", 4 );
	mySprite.setPosition( screen_w/2 - mySprite.getWidth()/2, bot );
	
	myText.setName( "play_button-myText" );
	myText.setFont( "data/02_menu/BADABB__.TTF", 130, 0x82, 0xae, 0x20 );
	myText.setText( "play" );
	myText.setPosition( screen_w/2 - myText.getWidth()/2 +10, bot -5 );
	
	click.setID( "play_button-click" );
	click.load( "data/02_menu/click.wav", 50 );
}

void Play_button::draw( sf::RenderWindow* &window )
{
    window->draw( mySprite.get() );
	window->draw( myText.get() );
}

void Play_button::handle( sf::Event &event )
{
	if( state != 2 )
	{
		if( mySprite.getAlpha() == 0xFF )
		{
			int x, y;
			mySprite.setOffset( 0 );
			
			if( event.type == sf::Event::MouseMoved )
			{
				x = event.mouseMove.x;
				y = event.mouseMove.y;
					
				if( mySprite.checkCollision( x, y ) )
				{
					if( state != 2 )
					{
						state = 1;
						mySprite.setOffset( 1 );
					}
				}
			}

			if( event.type == sf::Event::MouseButtonPressed )
			{
				x = event.mouseButton.x;
				y = event.mouseButton.y;
					
				if( mySprite.checkCollision( x, y ) )
				{
					state = 2;
					mySprite.setOffset( 2 );
					
					if( play )
					{
						click.play();
					}
				}
			}
		}
	}
}



void Play_button::fadein( int i, int max )
{
	mySprite.fadein( i, max );
	myText.fadein( i, max );
}

void Play_button::fadeout( int i, int min )
{
	mySprite.fadeout( i, min );
	myText.fadeout( i, min );
}



void Play_button::setState( int state )
{
	mySprite.setOffset( 0 );
	this->state = state;
}

const int Play_button::getBot() const
{
	return mySprite.getBot();
}

const int Play_button::getX() const
{
	return mySprite.getX();
}

const sf::Uint8& Play_button::getState() const
{
	return state;
}

bool Play_button::nextState() const
{
	if( state == 2 && mySprite.getAlpha() == 0 )
	{
		return true;
	}
	
	return false;
}
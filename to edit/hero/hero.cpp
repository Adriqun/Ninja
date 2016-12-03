#include "hero/hero.h"
#include <fstream>

Activity::Activity()
{
	line = 0;
	counter = 0;
	active = false;
}

Activity::~Activity()
{
	free();
}

void Activity::free()
{
	line = 0;
	counter = 0;
	active = false;
}

void Activity::summarize()
{
	if( counter >= line )	counter = 0;
	else if( counter > 0 )	counter++;
}




void Hero::fadein( int v, int max )
{
	for( int i = 0; i < nr; i++ )
		sprite[ i ].fadein( v, max );
}

void Hero::fadeout( int v, int min )
{
	for( int i = 0; i < nr; i++ )
		sprite[ i ].fadeout( v, min );
}




int Hero::strToInt( string s )
{
    bool m = false;
    int tmp = 0;
    unsigned i = 0;
	
    if (s[ 0 ] == '-' )
    {
          i++;
          m = true;
    }
	
    while( i < s.size() )
    {
      tmp = 10*tmp+s[ i ] -48;
      i++;
    }
	
    return m ? -tmp : tmp;   
}

bool Hero::checkKey( int a )
{
	if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key( a ) ) )
		return true;
	
	return false;
}

bool Hero::checkKeys( int a, int b )
{
	if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key( a ) ) && b == -1 )
		return true;
	else if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key( a ) ) )
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key( b ) ) )
			return true;
	return false;
}




Hero::Hero()
{
	WIDTH_IDLE = 0;
	WIDTH_RUN = 0;
	WIDTH_JUMP = 0;
	WIDTH_ATTACK = 0;
	SCALE = 0;
	
	
	nr = 0;
	sprite = NULL;
	
	
	vel = 0;
	vel_value = 0;
	gravity = 0;
	grav_value = 0;
	
	
	which = 0;
	offset = 0;
	delay = 0;
	
	
	right = true;
	moving = false;
}

Hero::~Hero()
{
	free();
}

void Hero::free()
{
	WIDTH_IDLE = 0;
	WIDTH_RUN = 0;
	WIDTH_JUMP = 0;
	WIDTH_ATTACK = 0;
	SCALE = 0;
	
	
	if( sprite != NULL )
	{
		for( int i = 0; i < nr; i ++ )
		{
			sprite[ i ].free();
		}
		
		delete [] sprite;
		sprite = NULL;
		nr = 0;
	}
	
	
	vel = 0;
	vel_value = 0;
	gravity = 0;
	grav_value = 0;
	
	
	which = 0;
	offset = 0;
	delay = 0;
	
	
	right = true;
	moving = false;
	
	
	for( unsigned i = 0; i < keys.size(); i++ )
	{
		if( keys[ i ] != NULL )
		{
			delete [] keys[ i ];
			keys[ i ] = NULL;
		}
	}
	
	
	keys.clear();
	
	
	
	j.free();
	a.free();
	c.free();
}



void Hero::load( int& screen_w, int& y, string path )
{
	free();
	
	// 	Set sprites.
	SCALE = 0.25;
	
	int startX = 10;
	int startY = y -sprite[ i ].getHeight() -300;
	
	nr = STRENGTH +1;
	sprite = new MySprite [ nr ];
	
	for( int i = 0; i < nr; i++ )
	{
		sprite[ i ].setName( "hero-sprite[" + to_string( i ) + "]" );
		sprite[ i ].load( path + to_string( i ) + ".png", STRENGTH );
		sprite[ i ].setScale( SCALE, SCALE );
		sprite[ i ].setPosition( startX, startY );
	}
	sprite[ ATTACK ].setPosition( sprite[ ATTACK ].getX(), sprite[ ATTACK ].getY() + 10 );
	
	WIDTH_IDLE = sprite[ IDLE ].getWidth();
	WIDTH_RUN = sprite[ RUN ].getWidth();
	WIDTH_JUMP = sprite[ JUMP ].getWidth();
	WIDTH_ATTACK = sprite[ ATTACK ].getWidth();
	
	
	
	
	// Set keys.
	fstream file;
	file.open( "data/txt/menu/keyboard_temporary.txt" );
	if( file.bad() )
	{
		printf( "Cannot open file! (hero)\n" );
	}
	else
	{
		string line;
		for( int i = 0; i < nr; i ++ )
		{
			int* arrow;
			keys.push_back( arrow );
			keys[ i ] = new int[ 2 ];
			
			file >> line;	keys[ i ][ 0 ] = strToInt( line );
			file >> line;	keys[ i ][ 1 ] = strToInt( line );
		}
	}
	file.close();
	
	
	
	
	
	// Set other valuables
	vel_value = 1;
	vel = vel_value;
	
	vel_grav = 1;
	grav = vel_grav;
	
	
	
	
	
	// Start sprites
	which = IDLE;
	offset = 0;
	delay = 6;
	
	right = true;
	moving = false;
	
	
	// How much will it last
	j.line = STRENGTH*delay + 10*delay;
	a.line = STRENGTH*delay + 2*delay;
	c.line = STRENGTH*delay + 2*delay;
}

void Hero::draw( sf::RenderWindow* &window )
{
	window->draw( sprite[ which ].get() );
	
	sprite[ which ].setOffset( offset /delay );
	
	offset ++;
	if( offset == STRENGTH *delay )
	{
		offset = 0;
		
		j.active = false;
		a.active = false;
	}
}




void Hero::idle()
{
	which = IDLE;
}

bool Hero::move()
{
	if( checkKeys( keys[ 0 ][ 0 ], keys[ 0 ][ 1 ] ) )
	{
		which = RUN;
		
		for( int i = 0; i < nr; i++ )
		{
			sprite[ i ].setScale( -SCALE, SCALE );
			sprite[ i ].setPosition( sprite[ i ].getX() -vel, sprite[ i ].getY() );
		}
		
		if( right )
		{
			for( int i = 0; i < nr; i++ )
				sprite[ i ].setPosition( sprite[ i ].getX() + (sprite[ i ].getWidth()*-1), sprite[ i ].getY() );
				
			right = false;
		}
		
		moving = true;
		return true;
	}
	else if( checkKeys( keys[ 1 ][ 0 ], keys[ 1 ][ 1 ] ) )
	{
		which = RUN;
		
		for( int i = 0; i < nr; i++ )
		{
			sprite[ i ].setScale( SCALE, SCALE );
			sprite[ i ].setPosition( sprite[ i ].getX() +vel, sprite[ i ].getY() );
		}
		
		if( !right )
		{
			for( int i = 0; i < nr; i++ )
				sprite[ i ].setPosition( sprite[ i ].getX() - sprite[ i ].getWidth(), sprite[ i ].getY() );
				
			right = true;
		}
		
		moving = true;
		return true;
	}
	else
		moving = false;
	
	return false;
}

bool Hero::jump()
{
	if( j.counter == 0 && checkKeys( keys[ 2 ][ 0 ], keys[ 2 ][ 1 ] ) )
	{
		offset = 0;
		j.counter = 1;
		j.active = true;
	}
	else if( j.active )
	{
		which = JUMP;
		
		if( right )
		{
			for( int i = 0; i < nr; i++ )
				sprite[ i ].setPosition( sprite[ i ].getX() +vel*2, sprite[ i ].getY() );
		}
		else
		{
			for( int i = 0; i < nr; i++ )
				sprite[ i ].setPosition( sprite[ i ].getX() -vel*2, sprite[ i ].getY() );
		}
	}
	
	j.summarize();
	
	return j.active;	
}

bool Hero::attack()
{
	if( a.counter == 0 && checkKeys( keys[ 7 ][ 0 ], keys[ 7 ][ 1 ] ) )
	{
		offset = 0;
		a.counter = 1;
		a.active = true;
		
		if( !right )
			sprite[ ATTACK ].setPosition( sprite[ RUN ].getX(), sprite[ ATTACK ].getY() );
	}
	else if( a.active )
	{
		which = ATTACK;
	}
	else
	{
		if( !right )
			sprite[ ATTACK ].setPosition( sprite[ RUN ].getX() + 44, sprite[ ATTACK ].getY() );
	}
	
	a.summarize();
	
	return a.active;
}


void Hero::reverse()
{
	if( moving )
	{
		if( right )
		{
			for( int i = 0; i < nr; i++ )
			{
				sprite[ i ].setPosition( sprite[ i ].getX() -vel, sprite[ i ].getY() );
			}
		}
		else
		{
			for( int i = 0; i < nr; i++ )
			{
				sprite[ i ].setPosition( sprite[ i ].getX() +vel, sprite[ i ].getY() );
			}
		}
	}
	else if( j.active )
	{
		if( right )
		{
			for( int i = 0; i < nr; i++ )
				sprite[ i ].setPosition( sprite[ i ].getX() -vel*2, sprite[ i ].getY() );
		}
		else
		{
			for( int i = 0; i < nr; i++ )
				sprite[ i ].setPosition( sprite[ i ].getX() +vel*2, sprite[ i ].getY() );
		}
	}
}




void Hero::gravitation()
{
	if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key( 0 ) ) )
	for( int i = 0; i < nr; i++ )
	{
		sprite[ i ].setPosition( sprite[ i ].getX(), sprite[ i ].getY() +gravity );
	}
	
	if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key( 1 ) ) )
	for( int i = 0; i < nr; i++ )
	{
		sprite[ i ].setPosition( sprite[ i ].getX(), sprite[ i ].getY() -gravity );
	}
}

void Hero::weightlessness()
{
	for( int i = 0; i < nr; i++ )
	{
		sprite[ i ].setPosition( sprite[ i ].getX(), sprite[ i ].getY() -gravity );
	}
}




const int Hero::getX()
{
	int x;
	
	if( moving )
	{
		x = sprite[ RUN ].getX();
	}
	else if( j.active )
	{
		x = sprite[ JUMP ].getX();
	}
	else if( a.active )
	{
		x = sprite[ ATTACK ].getX();
	}
	else
	{
		x = sprite[ IDLE ].getX();
	}
	
	if( !right )
	{
		x += sprite[ IDLE ].getWidth();
	}
	
	return x;
}

const int Hero::getY()
{
	int y;
	
	if( moving )
	{
		y = sprite[ RUN ].getY();
	}
	else if( j.active )
	{
		y = sprite[ JUMP ].getY();
	}
	else if( a.active )
	{
		y = sprite[ ATTACK ].getY();
	}
	else
	{
		y = sprite[ IDLE ].getY();
	}

	return y;
}

const int Hero::getW()
{
	int w;
	
	if( moving )
	{
		w = sprite[ RUN ].getWidth();
	}
	else if( j.active )
	{
		w = sprite[ JUMP ].getWidth();
	}
	else if( a.active )
	{
		w = sprite[ ATTACK ].getWidth();
	}
	else
	{
		w = sprite[ IDLE ].getWidth();
	}
	
	if( w < 0 )
		w = -w;

	return w;
}

const int Hero::getH()
{
	int h;
	
	if( moving )
	{
		h = sprite[ RUN ].getHeight();
	}
	else if( j.active )
	{
		h = sprite[ JUMP ].getHeight();
	}
	else if( a.active )
	{
		h = sprite[ ATTACK ].getHeight();
	}
	else
	{
		h = sprite[ IDLE ].getHeight();
	}

	return h;
}
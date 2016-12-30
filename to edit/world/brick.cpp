#include "world/brick.h"
#include "world/rules.h"
#include <cstdlib>	// rand

void Brick::addBlock( int chosen, int x_width, int floor )
{
	// add block.
	blocks.push_back( new Block() );
	
	// set chosen.
	blocks[ blocks.size()-1 ]->nr = chosen;
	
	// set x.
	blocks[ blocks.size()-1 ]->x = x_width;
	
	// set y.
	blocks[ blocks.size()-1 ]->y = floor;
}

void Brick::addLadder( int x, int y )
{
	// add ladder.
	ladders.push_back( new Ladder() );
	
	// set x.
	ladders[ ladders.size()-1 ]->x = x;
	
	// set y.
	ladders[ ladders.size()-1 ]->y = y;
}


bool Brick::randFloor( bool &top, sf::Uint8 floor, sf::Uint8 &new_floor )
{
	bool flag = false;
	sf::Uint8 max = 4;
	sf::Uint8 min = 1;
	
	// Draw lots for true / false to change floor.
	if( rand()%2 == 1 )	// 50%
	{
		// Getting new floor.
		for(;;)
		{
			new_floor = rand()%max + min;
			
			if( new_floor != floor )
			{
				if( new_floor >= floor-1 && new_floor <= floor+1 )
				{
					// printf( "old %d, new %d\n", floor, new_floor );
					break;
				}
			}
		}
		
		if( floor < new_floor )	// means new floor is higher
		{
			flag = true;
			top = true;
		}
		else
		{
			flag = true;
			top = false;
		}
	}
	
	return flag;
}

void Brick::fill( int a, int n )	// fill till n
{
	for( unsigned i = 0; i < blocks.size(); i++ )
	{
		if( blocks[ i ]->nr == a || blocks[ i ]->nr == n )
		{
			// we check if we have free place
			bool free_place = true;
			for( unsigned j = 0; j < blocks.size(); j++ )
			{
				if( blocks[ i ]->y == blocks[ j ]->y && blocks[ i ]->x +width == blocks[ j ]->x  )
				{
					free_place = false;
				}
			}
			
			if( free_place &&  blocks[ i ]->x + width <= right )
			{
				addBlock( n, blocks[ i ]->x + width, blocks[ i ]->y );
			}
		}
	}
}

void Brick::positioning()
{
	// Create rules.
	Rules* rules = new Rules;
	rules->ruleRightSide();
	rules->ruleLeftSide();
	rules->ruleBotSide();
	
	
	
	// Random stuff
	sf::Uint8 scope;
	int8_t lastNr, chosen;
	
	
	
	// Floor stuff.
	sf::Uint8 floor = 1;
	sf::Uint8 new_floor = 1;
	
	// Floor flags
	bool top = false;
	bool flag = false;
	
	// how many blocks in line
	int c = 100;
	while( c-- )
	{
		// add block to the right
		lastNr = blocks[ blocks.size()-1 ]->nr;
		scope = rules->getRightRules( lastNr ).size();
		chosen = rules->getRightRules( lastNr )[ rand()%scope ]->nr;
		addBlock( chosen, blocks[ blocks.size()-1 ]->x + width, screen_h -width*floor );
		
		// RAND FLOOR
		top = false;
		flag = randFloor( top, floor, new_floor );
		
		// add top block
		if( flag )
		{
			if( top )
			{
				int need;
				if( rand()%2 == 1 )	need = -1;
				else				need = 14;
					
				while( true )
				{
					lastNr = blocks[ blocks.size()-1 ]->nr;
					if( lastNr == need )
					{
						break;
					}
					
					lastNr = blocks[ blocks.size()-1 ]->nr;
					addBlock( rules->fillForTop( lastNr, need ), blocks[ blocks.size()-1 ]->x + width, screen_h -width*floor );
				}
				
				
				floor = new_floor;
				lastNr = blocks[ blocks.size()-1 ]->nr;
				addBlock( rules->getTopBlockFor( lastNr ), blocks[ blocks.size()-1 ]->x, screen_h -width*floor );
					
				
				// add ladder
				int ladder_x = blocks[ blocks.size()-1 ]->x -ladder.getWidth() +10;
				int ladder_y = blocks[ blocks.size()-1 ]->y +width -ladder.getHeight();
				addLadder( ladder_x, ladder_y );
			}
			else
			{
				int need;
				if( rand()%2 == 1 )	need = 2;
				else				need = 7;
				
				while( true )
				{
					lastNr = blocks[ blocks.size()-1 ]->nr;
					addBlock( rules->fillForBot( lastNr ), blocks[ blocks.size()-1 ]->x + width, screen_h -width*floor );
					
					lastNr = blocks[ blocks.size()-1 ]->nr;
					if( lastNr == need )
					{
						break;
					}
				}
				
				floor = new_floor;
				lastNr = blocks[ blocks.size()-1 ]->nr;
				addBlock( rules->getBotBlockFor( lastNr ), blocks[ blocks.size()-1 ]->x, screen_h -width*floor );
				
				// add block to the right
				lastNr = blocks[ blocks.size()-1 ]->nr;
				scope = rules->getRightRules( lastNr ).size();
				chosen = rules->getRightRules( lastNr )[ rand()%scope ]->nr;
				addBlock( chosen, blocks[ blocks.size()-1 ]->x + width, screen_h -width*floor );
				
				// add ladder
				int ladder_x = blocks[ blocks.size()-1 ]->x -10;
				int ladder_y = blocks[ blocks.size()-1 ]->y -ladder.getHeight();
				addLadder( ladder_x, ladder_y );
			}
		}
	}
	

	
	// fill left bottom
	for( unsigned i = 0; i < blocks.size(); i++ )
	{
		if( blocks[ i ]->y < screen_h -width && ( blocks[ i ]->nr == 10 || blocks[ i ]->nr == 0 ) )
		{
			// we check if we have free place
			bool free_place = true;
			for( unsigned j = 0; j < blocks.size(); j++ )
			{
				if( blocks[ i ]->y +width == blocks[ j ]->y && blocks[ i ]->x == blocks[ j ]->x )
				{
					free_place = false;
				}
			}
			
			if( free_place )
			{
				if( blocks[ i ]->nr == 0 )
				{
					// we are looking for block nr 13
					
					bool put = false;
					int x = blocks[ i ]->x +width;
					int y = blocks[ i ]->y +width;
					
					for( unsigned j = 0; j < blocks.size(); j++ )
					{
						if( x == blocks[ j ]->x && y == blocks[ j ]->y )
						{
							// new x
							x += width;

							if( blocks[ j ]->nr == 13 )
							{
								put = true;
								// printf( "ok\n" );
								break;
							}
						}
						else
						{
							put = false;
						}
					}
					
					if( rand()%2 == 1 || put )
					{
						addBlock( 10, blocks[ i ]->x, blocks[ i ]->y +width );
					}
					else
					{
						addBlock( 8, blocks[ i ]->x, blocks[ i ]->y +width );
					}
				}
				else if( blocks[ i ]->nr == 10 )
				{
					addBlock( 10, blocks[ i ]->x, blocks[ i ]->y +width );
				}
			}
		}
	}
	
	// fill right bottom
	for( unsigned i = 0; i < blocks.size(); i++ )
	{
		if( blocks[ i ]->y < screen_h -width && ( blocks[ i ]->nr == 12 || blocks[ i ]->nr == 2 ) )
		{
			// we check if we have free place
			bool free_place = true;
			for( unsigned j = 0; j < blocks.size(); j++ )
			{
				if( blocks[ i ]->y +width == blocks[ j ]->y && blocks[ i ]->x == blocks[ j ]->x )
				{
					free_place = false;
				}
			}
			
			if( free_place )
			{
				// we have to check what kind of block is on the left 'further'
				int type = -1;
				int distance = -1;
				
				for( unsigned j = 0; j < blocks.size(); j++ )
				{
					if( blocks[ j ]->y == blocks[ i ]->y +width && blocks[ i ]->x -blocks[ j ]->x > 0 )
					{
						if( blocks[ j ]->nr == 8 || blocks[ j ]->nr == 10 ||
							blocks[ j ]->nr == 0 || blocks[ j ]->nr == 14 )
						{
							if( distance >= blocks[ i ]->x - blocks[ j ]->x || distance == -1 )
							{
								distance = blocks[ i ]->x - blocks[ j ]->x;

								// we found block
								type = blocks[ j ]->nr;
							}
						}
					}
				}
				
				// printf( "type = %d\n", type );
				
				if( type == 8 )			type = 9;
				else 					type = 12;
				
				addBlock( type, blocks[ i ]->x, blocks[ i ]->y +width );
			}
		}
	}
	
	
	// Delete rules
	delete rules;
	
	// printf( "\n\n\n\n" );
}



bool Brick::checkCollision( Rect* rect )
{
	if( rect != NULL )
	{
		for( unsigned i = 0; i < blocks.size(); i++ )
		{
			if( blocks[ i ]->nr != -1 )
			{
				if( blocks[ i ]->x > -width && blocks[ i ]->x < screen_w )
				{
					block[ blocks[ i ]->nr ].setPosition( blocks[ i ]->x, blocks[ i ]->y );
					if( block[ blocks[ i ]->nr ].checkCollision( rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight() ) )
					{
						return true;
					}
				}
			}
		}
	}
	
	return false;
}

bool Brick::checkBlockByPixel( Rect* rect )
{
	if( rect != NULL )
	{
		int l = rect->getX();		// left
		int r = rect->getRight();	// right
		int t = rect->getY();		// top
		int b = rect->getBot(); 		// bot
		
		for( unsigned i = 0; i < blocks.size(); i++ )
		{
			if( blocks[ i ]->nr != -1 )
			{
				if( blocks[ i ]->x > -width && blocks[ i ]->x < screen_w )
				{
					block[ blocks[ i ]->nr ].setPosition( blocks[ i ]->x, blocks[ i ]->y );
				
					for( int j = l; j <= r; j++ )
					{
						if( block[ blocks[ i ]->nr ].checkPixelCollision( j, t ) )		return true;
						else if( block[ blocks[ i ]->nr ].checkPixelCollision( j, b ) )	return true;
					}
					
					for( int j = t; j <= b; j++ )
					{
						if( block[ blocks[ i ]->nr ].checkPixelCollision( l, j ) )		return true;
						else if( block[ blocks[ i ]->nr ].checkPixelCollision( r, j ) )	return true;
					}
				}
			}
		}
	}
	
	return false;
}

bool Brick::checkLadder( Rect* rect )
{
	if( rect != NULL )
	{
		for( unsigned i = 0; i < ladders.size(); i++ )
		{
			if( ladders[ i ]->x > -width && ladders[ i ]->x < screen_w )
			{
				ladder.setPosition( ladders[ i ]->x, ladders[ i ]->y );
				if( ladder.checkCollision( rect->getX() +rect->getWidth() /4, rect->getY() -50, rect->getWidth() /2, rect->getHeight() ) )
				{
					return true;
				}
			}
		}
	}
	
	return false;
}



sf::Uint8 Brick::moveX( sf::Uint8 direction, float vel )
{
	if( direction == 1 )
	{
		if( left > -10 )
		{
			return 1;
		}
		
		for( unsigned i = 0; i < blocks.size(); i++ )
		{
			blocks[ i ]->x += vel;
		}
		
		for( unsigned i = 0; i < ladders.size(); i++ )
		{
			ladders[ i ]->x += vel;
		}
		
		left += vel;
		right += vel;
	}
	else if( direction == 2 )
	{
		if( right < screen_w -width +10 )
		{
			return 2;
		}
		
		for( unsigned i = 0; i < blocks.size(); i++ )
		{
			blocks[ i ]->x -= vel;
		}
		
		for( unsigned i = 0; i < ladders.size(); i++ )
		{
			ladders[ i ]->x -= vel;
		}
		
		left -= vel;
		right -= vel;
	}
	
	return 0;
}

sf::Uint8 Brick::getWidth()
{
	return width;
}







Brick::Brick()
{
	nr = 0;
	block = NULL;
	
	width = 0;
	screen_w = 0;
	screen_h = 0;
	
	left = 0;
	right = 0;
}

Brick::~Brick()
{
	free();
}

void Brick::free()
{
	if( block != NULL )
	{
		for( int i = 0; i < nr; i++ )
		{
			block[ i ].free();
		}
		
		delete [] block;
		block = NULL;
		nr = 0;
	}
	
	ladder.free();
	
	width = 0;
	screen_w = 0;
	screen_h = 0;
	
	blocks.clear();
	ladders.clear();
	
	left = 0;
	right = 0;
}




void Brick::load( int screen_w, int screen_h, int nr, int type )
{
	free();
	
	width = 128;
	this->screen_w = screen_w;
	this->screen_h = screen_h;

	this->nr = nr;
	block = new MySprite[ nr ];
	
	for( int i = 0; i < nr; i++ )
	{
		block[ i ].setName( "brick-block[" +to_string( i ) +"]" );
		block[ i ].loadByImage( "data/sprites/play/" +to_string( type ) +"/" +to_string( i ) +".png" );
	}
	
	ladder.setName( "brick-ladder" );
	ladder.loadByImage( "data/sprites/play/0/ladder.png" );
	
	// Set first block.
	blocks.push_back( new Block() );
	blocks[ blocks.size()-1 ]->nr = 1;
	blocks[ blocks.size()-1 ]->x = 0;
	blocks[ blocks.size()-1 ]->y = screen_h -width;
	
	// start
	positioning();
	
	// Searching for left
	for( unsigned i = 0; i < blocks.size(); i++ )
	{
		if( blocks[ i ]->x < left )
		{
			left = blocks[ i ]->x;
		}
	}
	
	// Searching right
	for( unsigned i = 0; i < blocks.size(); i++ )
	{
		if( blocks[ i ]->x > right )
		{
			right = blocks[ i ]->x;
		}
	}
	
	fill( 8, 15 );
	fill( 10, 11 );
	fill( 14, 11 );
}

void Brick::draw( sf::RenderWindow* &window )
{
	for( unsigned i = 0; i < blocks.size(); i++ )
	{
		if( blocks[ i ]->nr != -1 )
		{
			if( blocks[ i ]->x > -width && blocks[ i ]->x < screen_w )
			{
				block[ blocks[ i ]->nr ].setPosition( blocks[ i ]->x, blocks[ i ]->y );
				window->draw( block[ blocks[ i ]->nr ].get() );
			}
		}
	}
}

void Brick::drawLadders( sf::RenderWindow* &window )
{
	for( unsigned i = 0; i < ladders.size(); i++ )
	{
		if( ladders[ i ]->x > -width && ladders[ i ]->x < screen_w )
		{
			ladder.setPosition( ladders[ i ]->x, ladders[ i ]->y );
			ladder.setColor( sf::Color( ladders[ i ]->red, ladders[ i ]->green, ladders[ i ]->blue ) );
			window->draw( ladder.get() );
		}
	}
}





int Brick::getScreenWidth()
{
	return screen_w;
}

void Brick::fadein( int v, int max )
{
	for( int i = 0; i < nr; i++ )
	{
		block[ i ].fadein( v, max );
	}
	
	ladder.fadein( v, max );
}

void Brick::fadeout( int v, int min )
{
	for( int i = 0; i < nr; i++ )
	{
		block[ i ].fadeout( v, min );
	}
	
	ladder.fadeout( v, min );
}
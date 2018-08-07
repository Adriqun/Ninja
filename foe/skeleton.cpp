#include "skeleton.h"

Skeleton::Skeleton()
{
	free();
}

Skeleton::~Skeleton()
{
	free();
}

void Skeleton::free()
{
	armour = 0;
	damage = 0;
	velocity = 0;
	heartPoints = hp = 0;

	x = y = 0;
	width = 0;
	centerX = 0;
	left = right = 0;
	scale = 0;

	state = -1;
	offset = 0;
	if (!lines.empty())
	{
		lines.clear();
		lines.shrink_to_fit();
	}
		
	setText();
	textFrequency = 8;
	if (!texts.empty())
	{
		texts.clear();
		texts.shrink_to_fit();
	}

	appeared = false;
	attackNum = 0;
	attackLine = 1;
	attackCounter = attackLine;
	walkFrequency = 4;
	inactionX = -1;
	inactionLine = rand() % static_cast <int>(walkFrequency) + walkFrequency;
	inactionCounter = -1;
}


void Skeleton::setArmour(float value)
{
	armour = value;
}

void Skeleton::setDamage(float value)
{
	damage = value;
}

void Skeleton::setVelocity(float value)
{
	velocity = value;
}

void Skeleton::setHeartPoints(float value)
{
	heartPoints = hp = value;
}

void Skeleton::setWalkFrequency(float seconds)
{
	walkFrequency = seconds;
}

void Skeleton::setAttackFrequency(float seconds)
{
	attackLine = seconds;
}

const float& Skeleton::getArmour() const
{
	return armour;
}

const float& Skeleton::getDamage() const
{
	return damage;
}

const float& Skeleton::getVelocity() const
{
	return velocity;
}

const float& Skeleton::getHeartPoints() const
{
	return heartPoints;
}

void Skeleton::harm(float value)
{
	heartPoints -= (value - (value * armour / 1000));

	if (heartPoints <= 0)
		setDie();
}

float Skeleton::getHPScale()
{
	return heartPoints / hp < 0 ? 0 : heartPoints / hp;
}




void Skeleton::moveX(double elapsedTime)
{
	if (attackCounter > attackLine / 3)
	{
		this->x += (elapsedTime * velocity);
	}
}

void Skeleton::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Skeleton::setWidth(float width)
{
	this->width = width;
}

void Skeleton::setCenterX(float centerX)
{
	this->centerX = centerX;
}

void Skeleton::setBorders(float left, float right)
{
	this->left = left;
	this->right = right;
}

void Skeleton::setScale(float scale)
{
	this->scale = scale;
}

void Skeleton::turnLeft()
{
	if (scale < 0 && isAttackDone())
	{
		scale = -scale;
	}
}

void Skeleton::turnRight()
{
	if (scale > 0 && isAttackDone())
	{
		scale = -scale;
	}
}

float Skeleton::getX()
{
	float xOffset = 0;

	if (scale < 0)
	{
		if (state == APPEAR)		xOffset = width * 0.70;
		else if (state == IDLE)	xOffset = width * 0.59;
		else if (state == WALK)	xOffset = width * 0.59;
		else if (state == ATTACK)	xOffset = width * 1.01;
		else if (state == DIE)		xOffset = width * 1.01;
	}
	else
	{
		if (state == ATTACK)	xOffset = -width * 0.42;
		else if (state == DIE)	xOffset = -width * 0.42;
	}

	return x + xOffset;
}

float Skeleton::getY()
{
	float yOffset = width * 0.01;
	if (state == ATTACK)
	{
		yOffset += width * 0.073;
	}


	return y + yOffset;
}

float Skeleton::getRealX()
{
	return x + (width *0.05);
}

float Skeleton::getRealY()
{
	return y - getRealHeight();
}

float Skeleton::getRealWidth()
{
	return width / 2;
}

float Skeleton::getRealHeight()
{
	return width * 1.26;
}

float Skeleton::getAttackX()
{
	float myx = getRealX() - getAttackWidth();

	if (scale < 0)
	{
		myx += getAttackWidth() * 2;
	}

	return myx;
}

float Skeleton::getAttackY()
{
	return getRealY() + width * 0.5;
}

float Skeleton::getAttackWidth()
{
	return width * 0.43;
}

float Skeleton::getAttackHeight()
{
	return width * 0.6;
}

float Skeleton::getMouthX()
{
	return getRealX() + getRealWidth() / 1.5;
}

float Skeleton::getMouthY()
{
	return getRealY() + width * 0.45;
}

float Skeleton::getCenterX()
{
	return centerX;
}

const float& Skeleton::getLeft() const
{
	return left;
}

const float& Skeleton::getRight() const
{
	return right;
}

const float& Skeleton::getScaleX() const
{
	return scale;
}

float Skeleton::getScaleY()
{
	return scale < 0 ? -scale : scale;
}




void Skeleton::setState(int value)
{
	this->state = value;
}

void Skeleton::setOffset(float value)
{
	this->offset = value;
}

void Skeleton::setLines(std::vector<int> lines)
{
	this->lines = lines;
}

const int& Skeleton::getState() const
{
	return state;
}

const float& Skeleton::getOffset() const
{
	return offset;
}

const std::vector<int>& Skeleton::getLines() const
{
	return lines;
}




void Skeleton::setAppear()
{
	state = APPEAR;
	offset = 0;
	inactionX = -1;
	inactionCounter = 0;
}

void Skeleton::setIdle()
{
	if (appeared && state != ATTACK)
	{
		state = IDLE;
	}
}

void Skeleton::setWalk()
{
	if (isAttackDone())
	{
		state = WALK;
		inactionX = -1;
		inactionCounter = 0;
	}
}

void Skeleton::setAttack()
{
	state = ATTACK;
	offset = 0;
	attackCounter = 0;
	inactionX = -1;
	inactionCounter = 0;
}

void Skeleton::setDie()
{
	state = DIE;
	offset = 0;
	inactionX = -1;
	inactionCounter = 0;
}

bool Skeleton::isAlive()
{
	return state > -2 && state != DIE;
}

bool Skeleton::isAbleToAppear()
{
	return state == -1;
}

bool Skeleton::isAbleToWalk()
{
	return state == IDLE || state == WALK;
}

bool Skeleton::isAbleToAttack()
{
	return (state == IDLE || state == WALK) && attackCounter > attackLine;
}

bool Skeleton::isAttackDone()
{
	return attackCounter > attackLine / 3;
}

bool Skeleton::isAttacking(bool hide)
{
	if (state == ATTACK)
	{
		if (static_cast <int> (offset) == 3)
		{
			if (hide)
				return true;
			else if (attackNum == 0)
			{
				attackNum++;
				return true;
			}
		}
		else if (static_cast <int> (offset) == 6)
		{
			if (hide)
				return true;
			else if (attackNum == 1)
			{
				attackNum++;
				return true;
			}
		}
	}

	return false;
}

void Skeleton::mechanics(double elapsedTime)
{
	if (state > -1)
	{
		offset += elapsedTime * 20;	// 20 offsets per second.

		if (state == APPEAR)
		{
			if (offset >= lines[APPEAR])
			{
				offset = 0;
				state = IDLE;
				appeared = true;
			}
		}

		if (inactionX != -1)
		{
			if (getRealX() + getRealWidth() < inactionX)
			{
				state = WALK;
				turnRight();
				moveX(elapsedTime);
				if (getRealX() + getRealWidth() > inactionX)
				{
					offset = 0;
					state = IDLE;
					inactionX = -1;
					inactionCounter = 0;
				}
			}
			else if (getRealX() > inactionX)
			{
				state = WALK;
				turnLeft();
				moveX(-elapsedTime);
				if (getRealX() < inactionX)
				{
					offset = 0;
					state = IDLE;
					inactionX = -1;
					inactionCounter = 0;
				}
			}
		}

		if (state == IDLE)
		{
			if (offset >= lines[IDLE])
			{
				offset = 0;
			}

			inactionCounter += elapsedTime;
			if (inactionCounter > inactionLine)
			{
				offset = 0;
				inactionX = rand() % static_cast <int> (right - left) + left;
				inactionLine = rand() % static_cast <int> (walkFrequency) + walkFrequency;
			}
		}


		if (state == WALK && offset >= lines[WALK])
		{
			offset = 0;
		}

		if (state == ATTACK)
		{
			if (offset >= lines[ATTACK])
			{
				attackNum = 0;
				offset = 0;
				state = IDLE;
			}
		}
		else
		{
			attackCounter += elapsedTime;
		}

		if (state == DIE)
		{
			if (offset >= lines[DIE])
			{
				offset = 0;
				state = -2;
			}
		}
		else if (!texts.empty())
		{
			textCounter += elapsedTime;
			if (textCounter > textLine * 1.6)
			{
				setText();
			}
		}
	}
}




void Skeleton::setTextFrequency(float seconds)
{
	textFrequency = seconds;
}

void Skeleton::setText()
{
	textCounter = 0;
	textLine = (rand() % static_cast<int>(textFrequency) + textFrequency);
	chosenText = rand() % texts.size();
}

void Skeleton::addText(std::string line)
{
	this->texts.push_back(line);
	chosenText = rand() % texts.size();	// rand again
}

bool Skeleton::showText()
{
	return textCounter > textLine && state != DIE;
}

const std::string& Skeleton::getText() const
{
	return texts[chosenText];
}

bool Skeleton::isLeftText()
{
	return scale > 0;
}
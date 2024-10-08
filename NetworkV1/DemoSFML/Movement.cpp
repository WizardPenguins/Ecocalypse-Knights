
#include "Main.hpp"
#include "Movement.hpp"

//Permet de changer la position d'une entité sur une case de la grille vers une autre case
void MoveInGrid(char** _grid, int* _move, int* _pos)
{

	char temp = _grid[_pos[1]][_pos[0]];
	_grid[_pos[1]][_pos[0]] = ' ';
	_grid[_move[1]][_move[0]] = temp;

}

//Permet de changer la position d'une entité avec un vecteur int* _move
void MoveInPixel(int* _move, int* _posX, int* _posY)
{

	_posX += _move[0];
	_posY += _move[1];

}

//Permet de changer la position d'une entité pixel par pixel avec une direction _dir
void MoveWithDir(int* _x, int* _y, Direction _dir)
{

	switch (_dir)
	{

	case UP:
	{

		_y--;
		break;

	}
	case DOWN:
	{

		_y++;
		break;

	}
	case LEFT:
	{

		_x--;
		break;

	}
	case RIGHT:
	{

		_x++;
		break;

	}
	default:
	{

		break;

	}

	}

}

//Permet d'animer un Sprite
void SpriteAnim(sf::Sprite* _sprite, sf::IntRect* _rect)
{

	int spriteWidth = _sprite->getLocalBounds().width;
	int nbFrame = spriteWidth / _rect->width;

	if (_rect->left + _rect->width == spriteWidth)
	{

		_rect->left = 0;

	}
	else
	{

		_rect->left += _rect->width;

	}

	_sprite->setTextureRect(*_rect);

}
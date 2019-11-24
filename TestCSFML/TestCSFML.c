#include "stdafx.h"
#include <stdlib.h>
#include <math.h>

#define WIDTH 1025
#define HEIGHT 765
#define CELLSIZE 5


sfRenderWindow* window = 0;
sfEvent event;

#pragma region Sprite
sfSprite* LoadSprite(char* _sNom, char _isCentered)
{
	sfTexture* tempTex;
	sfSprite* tempSprite;
	tempTex = sfTexture_createFromFile(_sNom, NULL);

	tempSprite = sfSprite_create();
	sfSprite_setTexture(tempSprite, tempTex, 0);

	if (_isCentered)
	{
		sfVector2u tailleImage = sfTexture_getSize(tempTex);
		sfVector2f centre = { tailleImage.x / 2, tailleImage.y / 2 };
		sfSprite_setOrigin(tempSprite, centre);
	}
	return tempSprite;
}

void BlitSprite(sfSprite* _sprite, sfVector2f pos/*float _fPosX, float _fPosY*/)
{
	//sfVector2f tempPos = { _fPosX, _fPosY };
	sfSprite_setPosition(_sprite, pos/*tempPos*/);
	sfRenderWindow_drawSprite(window, _sprite, NULL);
}
#pragma endregion

#pragma region Text
typedef struct
{
	sfFont* police;
	sfText* texte;
	sfColor couleur;
}TextInfo;

void CreateText(TextInfo* message, char* police, sfColor couleur)
{
	message->police = sfFont_createFromFile(police);
	message->texte = sfText_create();

	sfText_setFont(message->texte, message->police);
	sfText_setColor(message->texte, couleur);
}

void BlitText(TextInfo* message, sfVector2f pos, int taille, char* texte, sfRenderWindow* window)
{
	sfText_setString(message->texte, texte);
	sfText_setCharacterSize(message->texte, taille);

	sfFloatRect rect = sfText_getLocalBounds(message->texte);
	sfText_setPosition(message->texte, (sfVector2f) { pos.x - rect.width / 2, pos.y - rect.height });

	sfRenderWindow_drawText(window, message->texte, NULL);
}
#pragma endregion

typedef struct
{
	sfImage* image;
	sfTexture* texture;

	sfColor myColor;

	sfVector2f screenSize;
	sfVector2f pos;
	sfVector2f vit;

	int cellSize;
	int direction;
	int cellHalf;

}Data;

void Update(Data* _data)
{

	_data->pos.x += _data->vit.x * _data->cellSize;
	_data->pos.y += _data->vit.y * _data->cellSize;
	_data->vit.x = 0;
	_data->vit.y = 0;



	if (_data->pos.x > 0 && _data->pos.x < _data->screenSize.x && _data->pos.y > 0 && _data->pos.y < _data->screenSize.y)
	{
		_data->myColor = sfImage_getPixel(_data->image, _data->pos.x, _data->pos.y);
	}

	if (_data->myColor.r == 0)
	{
		switch (_data->direction)
		{
		case 0:
			_data->vit.x = -1;
			_data->direction = 3;
			break;
		case 1:
			_data->vit.y = -1;
			_data->direction = 0;
			break;
		case 2:
			_data->vit.x = 1;
			_data->direction = 1;
			break;
		case 3:
			_data->vit.y = 1;
			_data->direction = 2;
			break;
		}
	}
	else
	{
		switch (_data->direction)
		{
		case 0:
			_data->vit.x = 1;
			_data->direction = 1;
			break;
		case 1:
			_data->vit.y = 1;
			_data->direction = 2;
			break;
		case 2:
			_data->vit.x = -1;
			_data->direction = 3;
			break;
		case 3:
			_data->vit.y = -1;
			_data->direction = 0;
			break;
		}
	}

	if (_data->myColor.r == 0)
	{
		for (float i = _data->pos.x - _data->cellHalf; i <= _data->pos.x + _data->cellHalf; i++)
		{
			for (float j = _data->pos.y - _data->cellHalf; j <= _data->pos.y + _data->cellHalf; j++)
			{
				if (i > 0 && i < _data->screenSize.x && j > 0 && j < _data->screenSize.y)
				{
					sfImage_setPixel(_data->image, i, j, sfWhite);
				}
			}
		}
	}
	else
	{
		for (float i = _data->pos.x - _data->cellHalf; i <= _data->pos.x + _data->cellHalf; i++)
		{
			for (float j = _data->pos.y - _data->cellHalf; j <= _data->pos.y + _data->cellHalf; j++)
			{
				if (i > 0 && i < _data->screenSize.x && j > 0 && j < _data->screenSize.y)
				{
					sfImage_setPixel(_data->image, i, j, sfBlack);
				}
			}
		}
	}
}

void DisplayKeys()
{
	printf("Keys:\n");
	printf("\tPlay / pause: Space\n");
	printf("\tReset background image: R\n");
	printf("\n\n");
	printf("\tChange speed:\n");
	printf("\t\tNumpad +\n");
	printf("\t\tNumpad -\n");
	printf("\t\tThe speed is in iterations / frames\n");
	printf("To change values open the project...");
}

int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(NULL));
	Data myData;

	char params;
	printf("Custom parameters? (y/n)\t");
	scanf("%c", &params);
	fflush(stdin); while ((getchar()) != '\n');

	if (params == 'n')
	{
		myData.screenSize.x = WIDTH;
		myData.screenSize.y = HEIGHT;
		myData.cellSize = CELLSIZE;
		myData.direction = rand() % 4;
	}
	else if (params == 'y')
	{
		printf("Custom screen size? (y/n)\t");
		scanf("%c", &params);
		fflush(stdin); while ((getchar()) != '\n');
		if (params == 'n')
		{
			myData.screenSize.x = WIDTH;
			myData.screenSize.y = HEIGHT;
		}
		else if (params == 'y')
		{
			int temp;
			int temp2;

			printf("Width: \t\t");
			scanf("%d", &temp);
			fflush(stdin); while ((getchar()) != '\n');

			printf("Height: \t");
			scanf("%d", &temp2);
			fflush(stdin); while ((getchar()) != '\n');

			myData.screenSize.x = temp;
			myData.screenSize.y = temp2;
		}

		printf("Custom cell size? (y/n)\t\t");
		scanf("%c", &params);
		fflush(stdin); while ((getchar()) != '\n');
		if (params == 'n')
		{
			myData.cellSize = CELLSIZE;
		}
		else if (params == 'y')
		{
			int temp;
			printf("Cell size: \t");
			scanf("%d", &temp);
			fflush(stdin); while ((getchar()) != '\n');

			myData.cellSize = temp;
		}

		printf("Custom base direction? (y/n)\t");
		scanf("%c", &params);
		fflush(stdin); while ((getchar()) != '\n');
		if (params == 'n')
		{
			myData.direction = rand() % 4;
		}
		else if (params == 'y')
		{
			int temp;
			printf("Cell dir:(0: left, 1:down, 2:right, 3:up) \t");
			scanf("%d", &temp);
			fflush(stdin); while ((getchar()) != '\n');

			myData.direction = temp;
		}
	}
	DisplayKeys();

	myData.cellHalf = (int)(myData.cellSize / 2.0f);
	myData.pos = (sfVector2f) { myData.screenSize.x / 2, myData.screenSize.y / 2 };
	myData.vit = (sfVector2f) { 0, 0 };

	myData.myColor;

	sfClock* myClock = sfClock_create();
	float Dt = 0;
	sfVideoMode mode = { myData.screenSize.x, myData.screenSize.y, 32 };
	window = sfRenderWindow_create(mode, "SFML window", sfResize | sfClose, NULL);

	myData.image = sfImage_create(myData.screenSize.x, myData.screenSize.y);
	myData.texture = sfTexture_createFromImage(myData.image, NULL);
	sfSprite* sprite = sfSprite_create();
	sfSprite_setTexture(sprite, myData.texture, sfTrue);

	sfImage* image = sfImage_createFromColor(myData.cellSize, myData.cellSize, sfRed);
	sfImage* texture = sfTexture_createFromImage(image, NULL);
	sfSprite* block = sfSprite_create();
	sfSprite_setTexture(block, texture, sfTrue);
	sfVector2f centre = { myData.cellSize / 2, myData.cellSize / 2 + 1 };
	sfSprite_setOrigin(block, centre);

	TextInfo infos;
	CreateText(&infos, "arial.ttf", sfRed);

	sfBool play = sfFalse;
	int multiplier = 1;
	sfBool isMulti = sfTrue;
	int frameCount = 0;

	int actionCount = 0;
	char* basedir = malloc(sizeof(char) * 20);
	switch (myData.direction)
	{
	case 0:
		//left
		sprintf(basedir, "%s", "Base Dir: Left");
		break;
	case 1:
		//down
		sprintf(basedir, "%s", "Base Dir: Down");
		break;
	case 2:
		//right
		sprintf(basedir, "%s", "Base Dir: Right");
		break;
	case 3:
		//up
		sprintf(basedir, "%s", "Base Dir: Up");
		break;
	}

	sfRenderWindow_setFramerateLimit(window, 100);
	while (sfRenderWindow_isOpen(window))
	{
		Dt = sfTime_asSeconds(sfClock_restart(myClock));
		while (sfRenderWindow_pollEvent(window, &event))
		{
			if (event.type == sfEvtClosed) exit(0);
			if (event.type == sfEvtKeyPressed)
			{
				if (event.key.code == sfKeyR)
				{
					sfImage_destroy(myData.image);
					myData.image = sfImage_create(myData.screenSize.x, myData.screenSize.y);
					myData.texture = sfTexture_createFromImage(myData.image, NULL);
					sfSprite_setTexture(sprite, myData.texture, sfTrue);

					actionCount = 0;
				}

				if (event.key.code == sfKeySpace)
				{
					printf("ok");
					play = !play;
				}
			}

			if (event.key.code == sfKeySubtract)
			{
				if (isMulti)
				{
					if (multiplier > 1)
					{
						multiplier--;
					}
					else
					{
						isMulti = sfFalse;
					}
				}
				else
				{
					multiplier++;
				}
			}
			if (event.key.code == sfKeyAdd)
			{
				if (isMulti)
				{
					multiplier++;
				}
				else
				{
					if (multiplier > 1)
					{
						multiplier--;
					}
					else
					{
						isMulti = sfTrue;
					}
				}
			}
		}
		if (play)
		{
			if (isMulti)
			{
				for (int i = 0; i <= multiplier; i++)
				{
					Update(&myData);
					actionCount++;
				}
			}
			else
			{
				if (frameCount >= multiplier)
				{
					frameCount = 0;
					Update(&myData);
					actionCount++;
				}
				else
				{
					frameCount++;
				}
			}
		}

		sfTexture_updateFromImage(myData.texture, myData.image, 0, 0);

		sfRenderWindow_clear(window, sfBlack);

		BlitSprite(sprite, (sfVector2f) { 0, 0 });
		BlitSprite(block, myData.pos);

		char temp[20] = { 0 };
		sprintf(temp, "%d", actionCount);
		BlitText(&infos, (sfVector2f) { myData.screenSize.x / 2, 15 }, 15, temp, window);
		if (isMulti)
		{
			sprintf(temp, "mult:%d", multiplier);
		}
		else
		{
			sprintf(temp, "mult: 1/%d", multiplier);
		}
		BlitText(&infos, (sfVector2f) { myData.screenSize.x / 2, 32 }, 15, temp, window);
		BlitText(&infos, (sfVector2f) { myData.screenSize.x / 2, 49 }, 15, basedir, window);

		

		sfRenderWindow_display(window);

	}
	return 0;
}
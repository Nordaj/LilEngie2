#include <LilEngie.h>
#include "ComponentList.h"

Game* game;

void Init()
{
	//Setup component creation factory ptr for the engine to use for serialization
	game->sceneManager.gameComponentFactory = CreateComponentFromString;
}

void Start()
{
	LIL_LOG("Hello, welcome to Lil Engie 2.");
}

void Update()
{

}

LIL_ENGIE_SETUP(Start, Update, Init)

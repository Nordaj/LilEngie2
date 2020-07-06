#include <LilEngie.h>
#include "GameComponentFactory.h"

Game* game;

void Init()
{
	GameComponentFactory* factory = new GameComponentFactory();
	factory->InitComponentList();
	ComponentFactory::core = factory;
}

void Start()
{
	LIL_LOG("Hello, welcome to Lil Engie 2.");
}

void Update()
{

}

LIL_ENGIE_SETUP(Start, Update, Init)

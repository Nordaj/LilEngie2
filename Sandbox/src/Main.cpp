#include <iostream>
#include <LilEngie.h>

void Start()
{
	std::cout << "Hello World" << std::endl;
}

void Update()
{

}

/*
int main() 
{ 
	LilEngie::Game game(Start, Update); 
	return 0; 
}
*/

LIL_ENGIE_SETUP(Start, Update)

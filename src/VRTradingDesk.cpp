#include <iostream>
#include "OVR.h"

#include "Display.h"

/* Start the program */
int main(){
	Init(true);
	InitBuffers();
	RunDisplay();
}
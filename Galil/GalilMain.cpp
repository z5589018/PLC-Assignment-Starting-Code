
#include "Galil.h"
using namespace System;

int main(void) {
	EmbeddedFunctions funcs(true);
	Galil myGalil(&funcs, "192.168.0.120 -d");
	
	return 0;
}
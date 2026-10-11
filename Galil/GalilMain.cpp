
#include "Galil.h"
using namespace System;

int main(void) {
	EmbeddedFunctions funcs(true);
	Galil myGalil(&funcs, "192.168.0.120 -d");
	
	myGalil.DigitalOutput(260);

	Console::ReadKey();
	myGalil.DigitalOutput(0);
	myGalil.DigitalByteOutput(0, 5);

	Console::WriteLine (myGalil.DigitalBitInput(0));
	Console::WriteLine (myGalil.DigitalBitInput(16));
	Console::WriteLine (myGalil.DigitalBitInput(3));

	Console::WriteLine(myGalil.AnalogInput(4));
	
	Console::ReadKey();
	myGalil.DigitalOutput(0);
	myGalil.DigitalByteOutput(1, 5);

	Console::ReadKey();
	myGalil.DigitalOutput(0);
	myGalil.DigitalBitOutput(1, 10);

	Console::ReadKey();
	myGalil.DigitalBitOutput(1, 12);

	Console::ReadKey();
	myGalil.DigitalBitOutput(0, 10);

	Console::ReadKey();
	myGalil.DigitalOutput(0);
	

	Console::ReadKey();
	myGalil.DigitalOutput(0);


	Console::ReadKey();
	myGalil.AnalogOutput(5,1.00);
	Console::WriteLine(myGalil.AnalogInput(5));

	Console::ReadKey();

	Console::WriteLine("Galil opened successfully yayayyya");
	return 0;
}
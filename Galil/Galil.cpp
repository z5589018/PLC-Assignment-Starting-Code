#include "Galil.h"

// NEED TO ADD A LOT MORE COMMENTS IN THIS SHI

Galil::Galil() {
	Functions = new EmbeddedFunctions(true);
	g = 0;
	for (int i = 0; i < 2; i++) ControlParameters[i] = 0;
	setPoint = 0;
	Functions->GOpen("192.168.0.120 -d", &g);
	check = true;
	
}

Galil::Galil(EmbeddedFunctions* Funcs, GCStringIn address) {
	Functions = Funcs;
	g = 0;
	for (int i = 0; i < 2; i++) ControlParameters[i] = 0;
	setPoint = 0;
	Functions->GOpen(address, &g);
	check = true;
}

Galil::Galil(const Galil& other) {								
	Functions = new EmbeddedFunctions(true);
	g = 0;
	for (int i = 0; i < 2; i++) {
		ControlParameters[i] = other.ControlParameters[i];
	}
	setPoint = other.setPoint;

	Functions->GClose(g);
	check = true;
}

Galil::~Galil() {
	if (g) Functions->GClose(g);
	g = 0;

	delete Functions;
	Functions = nullptr;
}


// Digital Outputs:

void Galil::DigitalOutput(uint16_t value) {						// Write to all 16 bits of digital output, 1 command to the Galil
	// 0 - 7 -> 8 bit number
	// 8 - 15 -> 8 bit number
	char commd[64];
	char buff[256];

	uint8_t low = value & 0xFF;
	uint8_t high = value >> 8;

	sprintf_s(commd, sizeof(commd), "OP %d, %d;", low, high);
	try {
		Functions->GCommand(g, commd, buff, sizeof(buff), 0); 
	}
	catch (Exception^ error){
		check = false;
	}

}

void Galil::DigitalByteOutput(bool bank, uint8_t value) {		// Write to one byte, either high or low byte, as specified by user in 'bank' 0 = low, 1 = high
	char commd[64];
	char buff[256];
	//Eg. [0,5] -> pins 0,2 would be on
	// [1,5] -> pins 8, 10 would be on
	// 
	// Low
	if (bank == 0) {
		DigitalOutput(value);
	}
	// High
	if (bank == 1) {
		sprintf_s(commd, sizeof(commd), "OP %d, %d;", 0, value);
		try {
			Functions->GCommand(g, commd, buff, sizeof(buff), 0);
		}
		catch (Exception^ error) {
			check = false;
		}
	}

}


void Galil::DigitalBitOutput(bool val, uint8_t bit) {			// Write single bit to digital outputs. 'bit' specifies which bit
	char commd[64];
	char buff[256];
	if (val == 0) {
		sprintf_s(commd, sizeof(commd), "CB %d;", bit);
		try {
			Functions->GCommand(g, commd, buff, sizeof(buff), 0);
		}
		catch (Exception^ error) {
			check = false;
		}
	}
	if (val == 1) {
		sprintf_s(commd, sizeof(commd), "SB %d;", bit);
		try {
			Functions->GCommand(g, commd, buff, sizeof(buff), 0);
		}
		catch (Exception^ error) {
			check = false;
		}
	}
	else {
		Console::WriteLine("Invalid Val > 1");
	}
}



// Digital Input

uint16_t Galil::DigitalInput() {								// Return the 16 bits of input data
	// Query the digital inputs of the GALIL, See Galil command library @IN
	std::vector<int> galilInput;
	uint16_t result = 0;
	for (int i = 0; i < 16; i++) {
		galilInput.push_back(DigitalBitInput(i));
	}
	check = true;
	return result;
}

uint8_t Galil::DigitalByteInput(bool bank) {
	uint8_t result = 0;
	std::vector<int> galilInput;
	if (bank == 0) {
		for (int i = 0; i < 8; i++) {
			galilInput.push_back(DigitalBitInput(i));
		}
	}

	else {
		for (int i = 0; i < 8; i++) {
			galilInput[i] = DigitalBitInput(i+8);
		}
	}
	for (int i = 0; i < 8; i++) {
		result = result * 10 + galilInput[i];
	}
	check = true;
	return result;

}


bool Galil::DigitalBitInput(uint8_t bit) {						// Read single bit from current digital inputs. Above functions may use this function
	char commd[64];
	char buff[256];

	sprintf_s(commd, sizeof(commd), "MG @IN[%d];", bit);
	GReturn stuff;
	try {
		Functions->GCommand(g, commd, buff, sizeof(buff), 0);
	}
	catch (Exception^ error) {
		check = false;
	}
	return atof(buff) > 0.5;
}

// NEED TO COMPLETE FUNCTION:
bool Galil::CheckSuccessfulWrite() {
	return check;
}

// Analog Functions:

float Galil::AnalogInput(uint8_t channel) {
	char commd[64];
	char buff[256] = { 0 };

	sprintf_s(commd, sizeof(commd), "MG @AN[%d];", channel);
	try {
		Functions->GCommand(g, commd, buff, sizeof(buff), 0);
	}
	catch (Exception^ error) {
		check = false;
	}
	return (float)atof(buff);
}

void Galil::AnalogOutput(uint8_t channel, double voltage) {
	char commd[64];
	char buff[256] = { 0 };

	sprintf_s(commd, sizeof(commd), "AO %d,%f;", channel, voltage);
	try {
		Functions->GCommand(g, commd, buff, sizeof(buff), 0);
	}
	catch (Exception^ error) {
		check = false;
	}
}

void Galil::AnalogInputRange(uint8_t channel, uint8_t range) {
	char commd[64];
	char buff[256] = { 0 };

	sprintf_s(commd, sizeof(commd), "AQ %d,%d;", channel, range);
	try {
		Functions->GCommand(g, commd, buff, sizeof(buff), 0);
	}
	catch (Exception^ error) {
		check = false;
	}
}

// Encoder:

void Galil::WriteEncoder() {
	char commd[64];
	char buff[256] = { 0 };

	sprintf_s(commd, sizeof(commd), "WE 0,0;");
	try {
		Functions->GCommand(g, commd, buff, sizeof(buff), 0);
	}
	catch (Exception^ error) {
		check = false;
	}
}

int Galil::ReadEncoder() {
	char commd[64];
	char buff[256] = { 0 };

	sprintf_s(commd, sizeof(commd), "QE 0;");
	try {
		Functions->GCommand(g, commd, buff, sizeof(buff), 0);
	}
	catch (Exception^ error) {
		check = false;
	}

	return (int)atof(buff);
}


// Control Functions
void Galil::setSetPoint(int s) {
	setPoint = s;
}

double Galil::getSetPoint() {
	return setPoint;
}

void Galil::setKp(double gain) {
	ControlParameters[0] = gain;
}

double Galil::getKp() {
	return ControlParameters[0];
}

void Galil::setKi(double gain) {
	ControlParameters[1] = gain;
}

double Galil::getKi() {
	return ControlParameters[1];
}

void Galil::setKd(double gain) {
	ControlParameters[2] = gain;
}

double Galil::getKd() {
	return ControlParameters[2];
}

// Overloading << operator
// No Galil:: prefix because its a free function
std::ostream& operator<<(std::ostream& output, Galil& galil) {
	char info[256] = { 0 };
	char version[256] = { 0 };

	galil.Functions->GInfo(galil.g, info, sizeof(info));
	galil.Functions->GVersion(version, sizeof(version));

	output << "Info: " << info << std::endl << std::endl;
	output << "Version: " << version << std::endl << std::endl;

	return output;
}

// Overloaging = operator
Galil& Galil::operator=(const Galil& other) {
	if (this != &other) {
		// Copied from Copy constructor:
		Functions = new EmbeddedFunctions(true);
		g = 0;
		for (int i = 0; i < 2; i++) {
			ControlParameters[i] = other.ControlParameters[i];
		}
		setPoint = other.setPoint;

	}
	return *this;	
}
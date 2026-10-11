#include "EmbeddedFunctions.h"
#include <cstdint>

using namespace System;
using namespace System::Net::Sockets;
using namespace System::Text;

EmbeddedFunctions::EmbeddedFunctions() {
	String^ address = "192.168.0.120";
	int port = 23;
	GOpen(address, port);

}

EmbeddedFunctions::~EmbeddedFunctions() {
	GClose();
}

void EmbeddedFunctions::GOpen(String^ address, const int port) {
	try {
		// Initialising the Client
		GalilMngHndl = gcnew TcpClient(address, port);
		GalilMngHndl->NoDelay = true;
		GalilMngHndl->SendTimeout = 500;
		GalilMngHndl->ReceiveTimeout = 500;
		GalilMngHndl->NoDelay = true;

		GalilStream = GalilMngHndl->GetStream();
	}
	catch(Exception^ error) {
		throw gcnew Exception("Could not connect to " + address + ": " + error);
	}
}

void EmbeddedFunctions::GClose() {
	delete GalilMngHndl;
	delete GalilStream;
	GalilMngHndl = nullptr;
	GalilStream = nullptr;
}

String^ EmbeddedFunctions::GCommand(String^ command) {
	// Altered code from Week 3 Lecture:

	GalilMngHndl; //Declares a managed handle to a .NET TCP Client
	String^ IPaddress = "192.168.0.120";
	int Port = 23;
	array<uint8_t>^ SendData;
	array<uint8_t>^ RecvData;
	String^ Response;

	GalilMngHndl = gcnew TcpClient(IPaddress, Port);
	GalilMngHndl->SendBufferSize = 64;
	GalilMngHndl->ReceiveBufferSize = 2048;
	GalilMngHndl->SendTimeout = 500;
	GalilMngHndl->ReceiveTimeout = 500;
	GalilMngHndl->NoDelay = true;

	GalilStream = GalilMngHndl->GetStream();
	SendData = gcnew array<uint8_t>(64);
	RecvData = gcnew array<uint8_t>(2048);

	SendData = Encoding::ASCII->GetBytes(command);
	GalilStream->Write(SendData, 0, SendData->Length);
	Threading::Thread::Sleep(10);

	GalilStream->Read(RecvData, 0, RecvData->Length);
	Threading::Thread::Sleep(25);
	Response = Encoding::ASCII->GetString(RecvData);

	return Response;

}
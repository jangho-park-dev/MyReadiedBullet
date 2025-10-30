#include <iostream>
#include "IOCPServer.h"

IOCPServer g_IOCPServer;
int main()
{

	g_IOCPServer.mainLoop();

	return 0;
}
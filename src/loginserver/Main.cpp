#include "Server.h"

int main()
{
    Server server{ "127.0.0.1", 18046 };

    std::cout << "Initializing Opcodes...\n";
    opcodeTable.Initialize();

    std::cout << "Server started on Port: " << 18046 << "\n";
    server.Run();
}
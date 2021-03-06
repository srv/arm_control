
#include <iostream>

#include "melfa/melfa.h"
#include "melfa/exceptions.h"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Sends raw commands to slot 1" << std::endl;
        std::cerr << "Usage: " << argv[0] 
            << " <device name> <raw command>" << std::endl;
        std::cerr << "Use cuotes for the command." << std::endl;
        return -1;
    }

    std::string device_name(argv[1]);
    std::string command(argv[2]);

    melfa::Melfa::ConfigParams params;
    params.device = std::string(argv[1]);

    melfa::Melfa melfa(params);
    try
    {
        melfa.connect();
        melfa.sendCommand(command);
    }
    catch (melfa::SerialConnectionError& err)
    {
        std::cerr << "Serial Connection error: " << err.what() << std::endl;
    }
    catch (melfa::RobotError& err)
    {
        std::cerr << "Robot error: " << err.what() << std::endl;
    }

    return 0;
}


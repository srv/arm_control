#include <fstream>
#include <iostream>

#include "melfa.h"
#include "exceptions.h"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] 
            << " <device name> <program file name>" << std::endl;
        return -1;
    }

    std::string device_name(argv[1]);
    std::string program_file_name(argv[2]);

    std::ifstream in(program_file_name.c_str());
    if (!in.is_open())
    {
        std::cerr << "cannot open file " << program_file_name << "." << std::endl;
        return -3;
    }
 
    arm_control::Melfa::ConfigParams params;
    params.device = std::string(argv[1]);

    arm_control::Melfa melfa(params);
    
    try
    {
        melfa.connect();
        std::string command;
        while (getline(in, command))
        {
            while (melfa.isBusy()) usleep(1000);
            std::cout << "executing: " << command << std::endl;
            melfa.execute(command);
            double x, y, z, roll, pitch, yaw;
            melfa.getPose(x, y, z, roll, pitch, yaw);
            std::cout << "current pose: (" << x << ", " << y << ", " << z << ") "
                "(" << roll << ", " << pitch << ", " << yaw << ")" << std::endl;
        }
    }
    catch (arm_control::MelfaSerialConnectionError& err)
    {
        std::cerr << "Serial Connection error: " << err.what() << std::endl;
    }
    catch (arm_control::MelfaRobotError& err)
    {
        std::cerr << "Robot error: " << err.what() << std::endl;
    }

    return 0;
}


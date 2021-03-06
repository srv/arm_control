#include <fstream>
#include <iostream>

#include "melfa/melfa.h"
#include "melfa/tool_pose.h"
#include "melfa/joint_state.h"
#include "melfa/exceptions.h"

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
 
    melfa::Melfa::ConfigParams params;
    params.device = std::string(argv[1]);

    melfa::Melfa melfa(params);
    
    try
    {
        melfa.connect();
        std::string command;
        while (getline(in, command))
        {
            while (melfa.isBusy())
            {
                std::cout << "Melfa is busy, waiting" << std::endl;
                sleep(1);
            }
            std::cout << "executing: " << command << std::endl;
            melfa.execute(command);
            melfa::ToolPose pose = melfa.getToolPose();
            melfa::JointState joint_state = melfa.getJointState();
            std::cout << "current tool pose: " << pose << std::endl;
            std::cout << "current joint state: " << joint_state << std::endl;
        }
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


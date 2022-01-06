#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include "yaml-cpp/yaml.h" 

using namespace std;

/**
 * @brief Contains everything to deal with devices
 * 
 */
namespace Device{

    /**
     * @brief The description of a device
     * 
     */
    struct Device{

        std::string lastIP;
        std::string realName;
        std::string displayName;
        int subDevicesCount;
        std::vector<string> availableServices;

    };

    /**
     * @brief Overlaod the operator<< to display a device in a fancy manner
     * 
     * @param os The desired output stream
     * @param arg The node to print
     * @return std::ostream& The generated output stream
     */
    inline std::ostream& operator<<(std::ostream& os, Device const& arg)
    {
        os << "-> Config for device: " << arg.realName << endl;
        os << "	* Display Name: " << arg.displayName << endl;
        os << "	* Last known IP: " << arg.lastIP << endl;
        os << "	* Number of sub-devices: " << arg.subDevicesCount << endl;
        os << "	* Services: " <<endl;
        for(int i = 0;i<arg.availableServices.size();i++)
            os << "		- " << arg.availableServices.at(i) << endl; 

        return os;
    }

    /**
     * @brief Load devices from a config file
     * 
     * @param configPath The path of the config file
     * @param devices The destination Vector of loaded device
     * @return true The loading was successfull
     * @return false The loading failed
     */
    inline bool LoadConfigFile(char *configPath, std::vector<Device>& devices){

        //Load config file
        YAML::Node config;
        try{
            config = YAML::LoadFile(configPath);

            if(config.IsNull()){
                cout << "Unable to load config file" << endl;
                exit(EXIT_FAILURE);
            }

        }catch(YAML::BadFile e){
            cout << "Unable to load config file: " << e.msg << endl;
            exit(EXIT_FAILURE);
        }

        //Load the nodes
        for(YAML::const_iterator it = config["devices"].begin(); it != config["devices"].end();++it)
            devices.push_back(it->second.as<Device>());
        
        return true;
    }

    enum REQUEST_RESPONSE{
        OK,
        TIMED_OUT,
        DEVICE_UNKNOWN,
        INTERNAL_ERROR
    };
}

namespace YAML {
template<>
struct convert<Device::Device> {

  static bool decode(const Node& node, Device::Device& rhs) {
    if(node.IsNull()) {
      return false;
    }

	rhs.realName = node["realName"].as<string>();
	rhs.displayName = node["displayName"].as<string>();
	rhs.lastIP = node["lastIP"].as<string>();
	rhs.subDevicesCount = node["subDevicesCount"].as<int>();
	
	for(int i = 0;i<node["availableServices"].size();i++)
		rhs.availableServices.push_back(node["availableServices"][i].as<string>());
	

    return true;
  }
};
}
#endif
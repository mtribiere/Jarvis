#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

namespace Device{
    typedef struct Device{

        std::string lastIP;
        std::string realName;
        std::string displayName;
        int subDevicesCount;
        std::vector<string> availableServices;

    } Device;

    std::ostream& operator<<(std::ostream& os, Device const& arg)
    {
        cout << "-> Config for device: " << arg.realName << endl;
        cout << "	* Display Name: " << arg.displayName << endl;
        cout << "	* Last known IP: " << arg.lastIP << endl;
        cout << "	* Number of sub-devices: " << arg.subDevicesCount << endl;
        cout << "	* Services: " <<endl;
        for(int i = 0;i<arg.availableServices.size();i++)
            cout << "		- " << arg.availableServices.at(i) << endl; 

        return os;
    }

    bool LoadConfigFile(char *configPath, std::vector<Device>& devices){

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
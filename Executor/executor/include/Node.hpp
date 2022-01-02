#ifndef NODE_HPP
#define NODE_HPP

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

class Node{
    public:
        Node(std::string _deviceName, std::string _lastIP, std::vector<int> _devicesID) : deviceName(_deviceName), 
                                                             lastIP(_lastIP),
                                                             devicesID(_devicesID) {};
    
    private:
        std::string lastIP;
        std::string deviceName;
        std::vector<int> devicesID;
};

#endif
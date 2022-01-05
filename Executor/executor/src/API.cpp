#include "../include/API.hpp"

const std::shared_ptr<http_response> GetApiHandler::render_POST(const http_request& req) {
    //Get the URI
    string uri = req.get_path();
    uri = uri.substr(1,uri.size()-1);
    
    //Get the target device
    string device = uri.substr(0,uri.find('/'));
    std::cout << "Get request for " << device;

    string rqResponse = client->getInfoFromDevice(device,"POWER","1");

    //Send response
    std::shared_ptr<http_response> *rep;

    if(rqResponse == ""){
        rep = new std::shared_ptr<http_response>(new string_response("ERROR",200));
        (*rep)->with_header("Access-Control-Allow-Origin","*");
    }else{
        rep = new std::shared_ptr<http_response>(new string_response(rqResponse,200));
        (*rep)->with_header("Access-Control-Allow-Origin","*");
    }

    
    return (*rep);
}

const std::shared_ptr<http_response> SetApiHandler::render_POST(const http_request& req) {
    //Get the URI
    string uri = req.get_path();
    uri = uri.substr(1,uri.size()-1);
    
    //Get the target device
    string device = uri.substr(0,uri.find('/'));
    std::cout << "Set request for " << device << " -> " << req.get_arg("state") <<std::endl;

    client->publishMessage(req.get_arg("state"),"cmnd/"+device+"/POWER");

    //Send response
    std::shared_ptr<http_response> rep(new string_response("OK",200));
    rep->with_header("Access-Control-Allow-Origin","*");
    
    return rep;
}
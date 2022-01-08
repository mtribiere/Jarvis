#include "../include/API.hpp"

const std::shared_ptr<http_response> GetApiHandler::render_POST(const http_request& req) {
    //Get the URI
    string uri = req.get_path();
    uri = uri.substr(1,uri.size()-1);
    
    //Get the target device
    string device = uri.substr(0,uri.find('/'));
    std::cout << "Get request for " << device;

    string rqResponse = ""; 
    Device::REQUEST_RESPONSE rc = client->getInfoFromDevice(device,"POWER","1",rqResponse);

    //Send response
    std::shared_ptr<http_response> *rep;

    if(rc != Device::REQUEST_RESPONSE::OK){
        
        if(rc == Device::REQUEST_RESPONSE::TIMED_OUT){
            rep = new std::shared_ptr<http_response>(new string_response("DEVICE TIMEOUT",408));

        }else  if(rc == Device::REQUEST_RESPONSE::DEVICE_UNKNOWN){
            rep = new std::shared_ptr<http_response>(new string_response("DEVICE UNKNOWN",404));
        }else{
            rep = new std::shared_ptr<http_response>(new string_response("INTERNAL ERROR",500));
        }
        
    }else{
        rep = new std::shared_ptr<http_response>(new string_response(rqResponse,200));
    }
    
    (*rep)->with_header("Access-Control-Allow-Origin","*");
    
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


const std::shared_ptr<http_response> GPApiHandler::render(const http_request& req) {

    //Get the URI
    string uri = req.get_path();
    string reponse = "";

    if(uri == "/list"){
        reponse = "{DEVICE_LIST}";
    }

    //Send response
    std::shared_ptr<http_response> rep(new string_response(reponse,200));
    rep->with_header("Access-Control-Allow-Origin","*");
    
    return rep;

}
#pragma once

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"

#include <iostream>
#include <iostream>
#include <fstream>
#include <Poco/Token.h>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "../../database/conference.h"
#include "../../helper.h"

std::vector<long> parseTalks(const std::string& talksStr) {
    std::vector<long> talks;
    std::istringstream iss(talksStr);
    std::string token;
    while (std::getline(iss, token, ',')) {
        talks.push_back(std::stol(token));
    }
    return talks;
}

class ConferenceHandler : public HTTPRequestHandler
{

public:
    ConferenceHandler(const std::string &format) : _format(format)
    {
    }

    void handleRequest(HTTPServerRequest &request,
                       HTTPServerResponse &response)
    {
        HTMLForm form(request, request.stream());
        try
        {
            std::string scheme;
            std::string info;
            long id {-1};
            std::string login;
            request.getCredentials(scheme, info);
            std::cout << "scheme: " << scheme << " identity: " << info << std::endl;
            if(scheme == "Bearer") {
                if(!extract_payload(info,id,login)) {
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_FORBIDDEN);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                    root->set("type", "/errors/not_authorized");
                    root->set("title", "Internal exception");
                    root->set("status", "403");
                    root->set("detail", "user not authorized");
                    root->set("instance", "/pizza_order");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(root, ostr);
                    return;                   
                }
            }
            std::cout << "id:" << id << " login:" << login << std::endl;
            if (hasSubstr(request.getURI(), "/conferences") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET))
            {
                long id = atol(form.get("user_id").c_str());
                auto results = database::ConferenceInfo::read_by_user_id(id);
                Poco::JSON::Array arr;
                for (auto s : results)
                    arr.add(s.toJSON());
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(arr, ostr);

                return;
            }
            
            else if (hasSubstr(request.getURI(), "/reporttoconf") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET))
            {
                std::cout<< "входит в conferences_by_report1 "<<std::endl;
                long id = atol(form.get("id").c_str());
                std::cout<< "входит в conferences_by_report2 "<<std::endl;
                std::cout<< "id"<<id<<std::endl;
                std::optional<database::ConferenceInfo> result = database::ConferenceInfo::read_by_reports(id);

                if (result)
                {   Poco::JSON::Object::Ptr resultJSON = result->testtoJSON3();
                    std::vector<std::string> elements;
                    Poco::JSON::Array::Ptr talksArray = resultJSON->getArray("talks");

                    Poco::JSON::Array arr;

                    for (size_t i = 0; i < talksArray->size(); ++i)
                        {
                          
                            Poco::Dynamic::Var talkVar = talksArray->get(i);
                            Poco::JSON::Object::Ptr talkObj = talkVar.extract<Poco::JSON::Object::Ptr>();

                           
                            std::string talkId = talkObj->getValue<std::string>("id");
                            long talkIdLong = std::stol(talkId);
                            
                            std::optional<database::ReportInfo> result2 = database::ReportInfo::read_by_id(talkIdLong);
                                std::cout<<"ТуТ добавляемс "<<std::endl;
                                
        
                            if (result2)
                            {
                                arr.add(result2->toJSON());   
                            }
                            
                            
                            
                        }
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(arr, ostr);

                return;
                }

                else
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                    root->set("type", "/errors/not_foundsss");
                    root->set("title", "Internal exception");
                    root->set("status", "404");
                    root->set("detail", "not found by conference id");
                    root->set("instance", "/conference");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(root, ostr);
                    return;
                }
            }
            else if (hasSubstr(request.getURI(), "/conferences") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_PUT))
            {
                                database::ConferenceInfo confer;
                confer.id() = atol(form.get("id").c_str());
                long id_conf = atol(form.get("talks").c_str());
                std::cout<<id_conf;
                confer.update_rep(id_conf);
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                ostr << confer.get_id();
                return;


    
            }
            
            else if (hasSubstr(request.getURI(), "/conference") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET))
            {
                long id = atol(form.get("id").c_str());
                std::cout<< "входит в HTTP_GET 1 "<<std::endl;
                std::cout<< "id"<<id<<std::endl;
                std::optional<database::ConferenceInfo> result = database::ConferenceInfo::read_by_id(id);

                if (result)
                {   
                    std::cout<< "входит в HTTP_GET 2 ";
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(result->toJSON(), ostr);
                    return;
                }
                else
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                    root->set("type", "/errors/not_foundsss");
                    root->set("title", "Internal exception");
                    root->set("status", "404");
                    root->set("detail", "not found by conference id");
                    root->set("instance", "/conference");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(root, ostr);
                    return;
                }
            }
            

            else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
            {   std::cout<< "входит в HTTP_POST 10 ";
                database::ConferenceInfo confer;
                confer.id() = atol(form.get("id").c_str());
                confer.user_id() = atol(form.get("user_id").c_str());
                confer.type() = form.get("type");
                confer.title() = form.get("title");
                confer.location() = form.get("location");
                confer.date() = form.get("date");

            

                confer.description().topic = form.get("topic");
                confer.description().descriptions = form.get("description");

                
             
                std::vector<long> talks = parseTalks(form.get("talks"));


                for (const auto& talk : talks) {
                database::ConferenceInfo::Talk t;
                t.id = talk;
                std::cout << "talk^"<<talk << std::endl;
                confer.talks().push_back(t);
            }



                confer.add();
                
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                ostr << confer.get_id();
                return;
            }

            else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_DELETE)
            {
                database::ConferenceInfo order;
                order.id() = atol(form.get("id").c_str());
                order.deletes();
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                ostr << order.get_id();
                return;
            }
            
            else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_PUT)
            {
               std::cout<< "входит в HTTP_POST 10 ";
                database::ConferenceInfo confer;
                confer.id() = atol(form.get("id").c_str());
                confer.user_id() = atol(form.get("user_id").c_str());
                confer.type() = form.get("type");
                confer.title() = form.get("title");
                confer.location() = form.get("location");
                confer.date() = form.get("date");
                confer.description().topic = form.get("topic");
                confer.description().descriptions = form.get("description");
             
                std::vector<long> talks = parseTalks(form.get("talks"));
                for (const auto& talk : talks) 
                {
                database::ConferenceInfo::Talk t;
                t.id = talk;
                std::cout << "talk^"<<talk << std::endl;
                confer.talks().push_back(t);
                }

                confer.update();

                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                ostr << confer.get_id();
                return;
            }
        }
        catch (std::exception &ex)
        {
            std::cout << "exception:" << ex.what() << std::endl;
        }

        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("type", "/errors/not_found");
        root->set("title", "Internal exception");
        root->set("status", Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        root->set("detail", "request ot found");
        root->set("instance", "/conference");
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    }

private:
    std::string _format;
};
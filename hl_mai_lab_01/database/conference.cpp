#include "conference.h"
#include "database.h"
#include "report.h"
#include <sstream>

#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

namespace database
{


    std::vector<database::ConferenceInfo::Talk> extractTalks(const Poco::JSON::Array::Ptr& talksArray) {
    std::vector<database::ConferenceInfo::Talk> talks;

    for (size_t i = 0; i < talksArray->size(); ++i) {
        Poco::Dynamic::Var talkVar = talksArray->get(i);
        Poco::JSON::Object::Ptr talkObject = talkVar.extract<Poco::JSON::Object::Ptr>();

       
        long talkId = talkObject->getValue<long>("id");

       
        database::ConferenceInfo::Talk talk;
        talk.id = talkId;
        talks.push_back(talk);
    }

    return talks;
}

    ConferenceInfo ConferenceInfo::fromJSON(const std::string &str)
    {
        int start = str.find("_id");
        int end = str.find(",",start);

        std::string s1 = str.substr(0,start-1);
        std::string s2 = str.substr(end+1);

        std::cout << s1 << s2 << std::endl;
        std::cout << "from json:" << str << std::endl;
        ConferenceInfo conference;

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(s1+s2);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        conference.id() = object->getValue<long>("id");
        conference.user_id() = object->getValue<long>("user_id");
        conference.title() = object->getValue<std::string>("title");
        conference.type() = object->getValue<std::string>("type");
        conference.location() = object->getValue<std::string>("location");
        conference.date() = object->getValue<std::string>("date");
   
        std::cout<<conference.id()<<std::endl;
        std::cout<<conference.user_id()<<std::endl;
        std::cout<<conference.title()<<std::endl;
        std::cout<<conference.type()<<std::endl;
        std::cout<<conference.location()<<std::endl;
        std::cout<<conference.date()<<std::endl;
    


        Poco::JSON::Object::Ptr object_array = object->getObject("description");
        conference._description.topic = object_array->getValue<std::string>("topic");
        conference._description.descriptions = object_array->getValue<std::string>("descriptions");
    
        std::string talksStr = object->getValue<std::string>("talks");
        Poco::JSON::Parser parser2;
        Poco::Dynamic::Var talksResult = parser2.parse(talksStr);
        Poco::JSON::Array::Ptr talksArray = talksResult.extract<Poco::JSON::Array::Ptr>();
        std::vector<database::ConferenceInfo::Talk> talks = extractTalks(talksArray);
      
        for (const auto& talk : talks) {
        std::cout << "Talk id: " << talk.id << std::endl;
        }
      
        conference.talks() = talks;

        return conference;
    }
ConferenceInfo ConferenceInfo::testfromJSON(const std::string &str)
    {
        int start = str.find("_id");
        int end = str.find(",",start);

        std::string s1 = str.substr(0,start-1);
        std::string s2 = str.substr(end+1);

        std::cout << s1 << s2 << std::endl;
        std::cout << "from json:" << str << std::endl;
        ConferenceInfo conference;

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(s1+s2);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

 
        std::string talksStr = object->getValue<std::string>("talks");
        Poco::JSON::Parser parser2;
        Poco::Dynamic::Var talksResult = parser2.parse(talksStr);
        Poco::JSON::Array::Ptr talksArray = talksResult.extract<Poco::JSON::Array::Ptr>();
        std::vector<database::ConferenceInfo::Talk> talks = extractTalks(talksArray);
      
        for (const auto& talk : talks) {
        std::cout << "Talk id: " << talk.id << std::endl;
        }
      
        conference.talks() = talks;

        return conference;
    }

    ConferenceInfo ConferenceInfo::reportsFromJSON(const std::string &str)
    {
        int start = str.find("_id");
        int end = str.find(",",start);

        std::string s1 = str.substr(0,start-1);
        std::string s2 = str.substr(end+1);

        std::cout << s1 << s2 << std::endl;
        std::cout << "from json:" << str << std::endl;
        ConferenceInfo conference;

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(s1+s2);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        
std::string talksStr = object->getValue<std::string>("talks");
        Poco::JSON::Parser parser2;
Poco::Dynamic::Var talksResult = parser2.parse(talksStr);
Poco::JSON::Array::Ptr talksArray = talksResult.extract<Poco::JSON::Array::Ptr>();

std::vector<database::ConferenceInfo::Talk> talks = extractTalks(talksArray);

for (const auto& talk : talks) {
    std::cout << "Talk id: " << talk.id << std::endl;
}
conference.talks() = talks;
for (const auto& talk : conference.talks()) {
    std::cout << "Talk idteststt: " << talk.id << std::endl;
}


        return conference;
    }


    
    long ConferenceInfo::get_id() const
    {
        return _id;
    }

    long ConferenceInfo::get_user_id() const
    {
        return _user_id;
    }
    const std::string &ConferenceInfo::get_title() const
    {
        return _title;
    }
    const std::string &ConferenceInfo::get_type() const
    {
        return _type;
    }
    const std::string &ConferenceInfo::get_date() const
    {
        return _date;
    }
    const std::string &ConferenceInfo::get_location() const
    {
        return _location;
    }
    const std::string ConferenceInfo::get_description_as_string() const
    {
        return _description.to_string();
    }
    const std::string ConferenceInfo::Description::to_string() const
    {
        std::stringstream ss;
        ss << topic << ", " << descriptions;
        return ss.str();
    }



    long &ConferenceInfo::id()
    {
        return _id;
    }
    long &ConferenceInfo::user_id()
    {
        return _user_id;
    }
    std::string &ConferenceInfo::title()
    {
        return _title;
    }
    std::string &ConferenceInfo::type()
    {
        return _type;
    }
    std::string &ConferenceInfo::location()
    {
        return _location;
    }
    std::string &ConferenceInfo::date()
    {
        return _date;
    }

    ConferenceInfo::Description &ConferenceInfo::description()
    {
        return _description;
    }
    


    std::optional<ConferenceInfo> ConferenceInfo::read_by_id(long id)
    {
        std::optional<ConferenceInfo> result;
        std::map<std::string,long> params;
        params["id"] = id;
        std::vector<std::string> results = database::Database::get().get_from_mongo2("conference" ,params);
        
        if(!results.empty())
        {   
            std::cout<<"тут получаем результат";
            result = fromJSON(results[0]);
        }
        return result;
    }

    std::optional<ConferenceInfo> ConferenceInfo::delete_by_id(long id)
    {
        std::optional<ConferenceInfo> result;
        std::map<std::string,long> params;
        params["id"] = id;
        std::vector<std::string> results = database::Database::get().get_from_mongo2("conference" ,params);

        if(!results.empty())
            result = fromJSON(results[0]);
            
        
        return result;
    }

    std::vector<ConferenceInfo> ConferenceInfo::read_by_user_id(long user_id)
    {
        std::vector<ConferenceInfo> result;
        std::map<std::string,long> params;
        params["user_id"] = user_id;

        std::vector<std::string> results = database::Database::get().get_from_mongo2("conference",params);

        for(std::string& s : results) 
            {
                std::cout<<s<<std::endl;
                result.push_back(fromJSON(s));
            }

        return result;
    }


       std::optional<ConferenceInfo> ConferenceInfo::read_by_reports(long id)
    {
        std::optional<ConferenceInfo> result;
        std::map<std::string,long> params;
        params["id"] = id;
        std::vector<std::string> results = database::Database::get().get_from_mongo2("conference" ,params);


        if(!results.empty())
        {   
            std::cout<<"тут получаем результат";
            result = fromJSON(results[0]);

        }
        return result;
    }



    

    void ConferenceInfo::add()
    {
        std::cout<< "входит в add";
        database::Database::get().send_to_mongo("conference",toJSON());
    }
    void ConferenceInfo::deletes()
    {
        std::map<std::string,long> params;
        params["id"] = _id;       
        database::Database::get().remove_from_mongo("conference",params);
    }
        

    void ConferenceInfo::update()
    {
        std::map<std::string,long> params;
        params["id"] = _id;       
        database::Database::get().update_mongo("conference",params,toJSON());
    }
    


    void ConferenceInfo::update_rep(long id_conf)
{
    std::map<std::string,long> params;
    params["id"] = _id;
    std::vector<ConferenceInfo> currentResults;
    std::optional<ConferenceInfo> result;
    std::vector<std::string> results = database::Database::get().get_from_mongo2("conference" ,params);
    if (results.empty()) {
        std::cout << "Запись с указанным ID не найдена" << std::endl;
        }
    
    
    if (!results.empty()) {
            for (const auto& result : results) {
            std::cout<<"tetsts"<<std::endl;
            std::cout << result << std::endl;
            database::Database::get().update_mongo("conference",params,testtoJSON(results[0], id_conf));
     }
    }

}


Poco::JSON::Object::Ptr ConferenceInfo::toJSON() const
{
    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

    root->set("id", static_cast<Poco::Int64>(_id));
    root->set("user_id", static_cast<Poco::Int64>(_user_id));
    root->set("title", _title);
    root->set("type", _type);
    root->set("location", _location);
    root->set("date", _date);
    std::cout<< "входит в toJSON";


        Poco::JSON::Object::Ptr des = new Poco::JSON::Object();

        des->set("topic",_description.topic);
        des->set("descriptions",_description.descriptions);

        root->set("description", des);
    Poco::JSON::Array::Ptr talksArray = new Poco::JSON::Array();
    for (const auto& talk : _talks) {
        Poco::JSON::Object::Ptr talkObj = new Poco::JSON::Object();
        talkObj->set("id",talk.id);
        std::cout<< talkObj<<std::endl;;
        talksArray->add(talkObj);
    }
    
    root->set("talks", talksArray);

    return root;
}
Poco::JSON::Object::Ptr ConferenceInfo::testtoJSON3() const
{
    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

    
    std::cout<< "входит в toJSON";

    Poco::JSON::Array::Ptr talksArray = new Poco::JSON::Array();
    for (const auto& talk : _talks) {
        Poco::JSON::Object::Ptr talkObj = new Poco::JSON::Object();
        talkObj->set("id",talk.id);
        std::cout<< talkObj<<std::endl;;
        talksArray->add(talkObj);
    }
    
    root->set("talks", talksArray);

    return root;
}
Poco::JSON::Object::Ptr ConferenceInfo::testtoJSON(const std::string &str, long id_conf) const
{   
        int start = str.find("_id");
        int end = str.find(",",start);

        std::string s1 = str.substr(0,start-1);
        std::string s2 = str.substr(end+1);

        std::cout << s1 << s2 << std::endl;
        std::cout << "from json:" << str << std::endl;
        ConferenceInfo conference;

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(s1+s2);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        conference.id() = object->getValue<long>("id");
        conference.user_id() = object->getValue<long>("user_id");
        conference.title() = object->getValue<std::string>("title");
        conference.type() = object->getValue<std::string>("type");
        conference.location() = object->getValue<std::string>("location");
        conference.date() = object->getValue<std::string>("date");
        std::cout<<conference.id()<<std::endl;
        std::cout<<conference.user_id()<<std::endl;
        std::cout<<conference.title()<<std::endl;
        std::cout<<conference.type()<<std::endl;
        std::cout<<conference.location()<<std::endl;
        std::cout<<conference.date()<<std::endl;
    


        Poco::JSON::Object::Ptr object_array = object->getObject("description");
        conference._description.topic = object_array->getValue<std::string>("topic");
        conference._description.descriptions = object_array->getValue<std::string>("descriptions");
    
        std::string talksStr = object->getValue<std::string>("talks");
        Poco::JSON::Parser parser2;
        Poco::Dynamic::Var talksResult = parser2.parse(talksStr);
        Poco::JSON::Array::Ptr talksArray = talksResult.extract<Poco::JSON::Array::Ptr>();
        std::vector<database::ConferenceInfo::Talk> talks = extractTalks(talksArray);

        database::ConferenceInfo::Talk newTalk;
        newTalk.id = id_conf; 
        talks.push_back(newTalk);


        conference.talks() = talks;
        for (const auto& talk : conference.talks()) {
        std::cout << "Talk id: " << talk.id << std::endl;
        }

    
    
    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

    root->set("id", conference._id);
    root->set("user_id", conference._user_id);
    root->set("title", conference._title);
    root->set("type", conference._type);
    root->set("location", conference._location);
    root->set("date", conference._date);

    std::cout<< conference._id<<std::endl;
    std::cout<< conference._user_id<<std::endl;
    std::cout<< conference._title<<std::endl;
    std::cout<< conference._type<<std::endl;
    std::cout<< conference._location<<std::endl;
    std::cout<< conference._date<<std::endl;
    



        Poco::JSON::Object::Ptr des = new Poco::JSON::Object();

        des->set("topic",conference._description.topic);
        des->set("descriptions",conference._description.descriptions);

        root->set("description", des);
    Poco::JSON::Array::Ptr talksArray2 = new Poco::JSON::Array();
    for (const auto& talk : conference._talks) {
        Poco::JSON::Object::Ptr talkObj = new Poco::JSON::Object();
        talkObj->set("id",talk.id);
        std::cout<< talkObj<<std::endl;;
        talksArray2->add(talkObj);
    }
    
    root->set("talks", talksArray2);

    return root;
}


Poco::JSON::Object::Ptr ConferenceInfo::testtoJSON2(const std::string &str, long id_conf) const
{   
        int start = str.find("_id");
        int end = str.find(",",start);

        std::string s1 = str.substr(0,start-1);
        std::string s2 = str.substr(end+1);

        std::cout << s1 << s2 << std::endl;
        std::cout << "from json:" << str << std::endl;
        ConferenceInfo conference;

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(s1+s2);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

    
    
        std::string talksStr = object->getValue<std::string>("talks");
        Poco::JSON::Parser parser2;
        Poco::Dynamic::Var talksResult = parser2.parse(talksStr);
        Poco::JSON::Array::Ptr talksArray = talksResult.extract<Poco::JSON::Array::Ptr>();
        std::vector<database::ConferenceInfo::Talk> talks = extractTalks(talksArray);

        database::ConferenceInfo::Talk newTalk;
        newTalk.id = id_conf; 
        talks.push_back(newTalk);


        conference.talks() = talks;
        for (const auto& talk : conference.talks()) {
        std::cout << "Talk id: " << talk.id << std::endl;
        }

    
    
    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

    root->set("id", conference._id);
    root->set("user_id", conference._user_id);
    root->set("title", conference._title);
    root->set("type", conference._type);
    root->set("location", conference._location);
    root->set("date", conference._date);

    std::cout<< conference._id<<std::endl;
    std::cout<< conference._user_id<<std::endl;
    std::cout<< conference._title<<std::endl;
    std::cout<< conference._type<<std::endl;
    std::cout<< conference._location<<std::endl;
    std::cout<< conference._date<<std::endl;
    



        Poco::JSON::Object::Ptr des = new Poco::JSON::Object();

        des->set("topic",conference._description.topic);
        des->set("descriptions",conference._description.descriptions);

        root->set("description", des);


    Poco::JSON::Array::Ptr talksArray2 = new Poco::JSON::Array();
    for (const auto& talk : conference._talks) {
        Poco::JSON::Object::Ptr talkObj = new Poco::JSON::Object();
        talkObj->set("id",talk.id);
        std::cout<< talkObj<<std::endl;;
        talksArray2->add(talkObj);
    }
    
    root->set("talks", talksArray2);

    return root;
}

}
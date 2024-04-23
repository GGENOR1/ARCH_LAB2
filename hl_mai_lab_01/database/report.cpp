#include "report.h"
#include "database.h"

#include <sstream>

#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

namespace database
{
    ReportInfo ReportInfo::fromJSON(const std::string &str)
    {
        int start = str.find("_id");
        int end = str.find(",",start);

        std::string s1 = str.substr(0, start-1);
        std::string s2 = str.substr(end+1);

        std::cout << s1 << s2 << std::endl;
        std::cout << "from json:" << str << std::endl;
        ReportInfo report;

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(s1+s2);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        report.id() = object->getValue<long>("id");
        report.user_id() = object->getValue<long>("user_id");
        report.type() = object->getValue<std::string>("type");
        report.rate() = object->getValue<long>("rate");
        report.name() = object->getValue<std::string>("name");
        Poco::JSON::Object::Ptr object_array = object->getObject("description");
        report._description.topic = object_array->getValue<std::string>("topic");
        report._description.descriptions = object_array->getValue<std::string>("descriptions");


        return report;
    }

    long ReportInfo::get_id() const
    {
        return _id;
    }

    long ReportInfo::get_user_id() const
    {
        return _user_id;
    }
    const std::string &ReportInfo::get_name() const
    {
        return _name;
    }
    long ReportInfo::get_rate() const
    {
        return _rate;
    }
    const std::string &ReportInfo::get_type() const
    {
        return _type;
    }
    const std::string ReportInfo::get_description_as_string() const
    {
        return _description.to_string();
    }
    const std::string ReportInfo::Description::to_string() const
    {
        std::stringstream ss;
        ss << topic << ", " << descriptions;
        return ss.str();
    }
    long &ReportInfo::id()
    {
        return _id;
    }
    long &ReportInfo::user_id()
    {
        return _user_id;
    }
    std::string &ReportInfo::name()
    {
        return _name;
    }
    long &ReportInfo::rate()
    {
        return _rate;
    }
    std::string &ReportInfo::type()
    {
        return _type;
    }
    ReportInfo::Description &ReportInfo::description()
    {
        return _description;
    }

    std::optional<ReportInfo> ReportInfo::read_by_id(long id)
    {
        std::optional<ReportInfo> result;
        std::map<std::string,long> params;
        params["id"] = id;
        std::vector<std::string> results = database::Database::get().get_from_mongo("reports" ,params);

        if(!results.empty())
            result = fromJSON(results[0]);
        
        return result;
    }

    std::optional<ReportInfo> ReportInfo::delete_by_id(long id)
    {
        std::optional<ReportInfo> result;
        std::map<std::string,long> params;
        params["id"] = id;
        std::vector<std::string> results = database::Database::get().get_from_mongo("reports" ,params);

        if(!results.empty())
            result = fromJSON(results[0]);
        
        return result;
    }

    std::vector<ReportInfo> ReportInfo::read_by_user_id(long user_id)
    {
        std::vector<ReportInfo> result;
        std::map<std::string,long> params;
        params["user_id"] = user_id;

        std::vector<std::string> results = database::Database::get().get_from_mongo("reports",params);

        for(std::string& s : results) 
            result.push_back(fromJSON(s));
        

        return result;
    }

    std::vector<ReportInfo> ReportInfo::read_all_reports()
    {   
    std::vector<ReportInfo> results;
    std::vector<std::string> allResults = database::Database::get().get_all_from_mongo();

    for (const auto& result : allResults) {
        std::optional<ReportInfo> report = fromJSON(result);
        if (report) {
            results.push_back(*report);
        }
    }

    return results;
    }

    void ReportInfo::add()
    {
        database::Database::get().send_to_mongo("reports",toJSON());
    }
    void ReportInfo::deletes()
    {
        std::map<std::string,long> params;
        params["id"] = _id;       
        database::Database::get().remove_from_mongo("reports",params);
    }

    void ReportInfo::update()
    {
        std::map<std::string,long> params;
        params["id"] = _id;       
        database::Database::get().update_mongo("reports",params,toJSON());
    }
    Poco::JSON::Object::Ptr ReportInfo::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("user_id", _user_id);
        root->set("type", _type);
        root->set("name", _name);
        root->set("rate", _rate);

        Poco::JSON::Object::Ptr des = new Poco::JSON::Object();

        des->set("topic",_description.topic);
        des->set("descriptions",_description.descriptions);

        root->set("description", des);


        return root;
    }
}
#pragma once

#include <string>
#include <vector>
#include <optional>

#include <Poco/JSON/Object.h>

namespace database
{
    class ConferenceInfo{
        private:
            long _id;                   //id конференции
            long _user_id;              //id создателя
            std::string _title;         //название конференции 
            std::string _type;          //формат проведение (например онлайн, оффлайн, совмещенно)
            std::string _location;      //место проведения (например ссылка или адрес или совместно)
            //std::chrono::system_clock::time_point _date; // дата проведения конференции
            std::string _date; // дата проведения конференции
            //std::vector<long long> _talks; // массив с идентификаторами докладов
            
            //std::vector<long> _talks;// массив с идентификаторами докладов

            struct Description {
                std::string topic; //тэги или тематика
                std::string descriptions; //краткое описание конференции
                const std::string to_string() const;
            };

            Description _description;
            



        public:
                    struct Talk {
                long id;
            };
            std::vector<Talk> _talks;
            static ConferenceInfo  fromJSON(const std::string & str);
            static ConferenceInfo  testfromJSON(const std::string & str);
            static ConferenceInfo  reportsFromJSON(const std::string & str);
            long               get_id() const;
            long               get_user_id() const;
            const std::string  &get_title() const;
            const std::string  &get_type() const;
            const std::string  &get_location() const;
            const std::string  get_description_as_string() const;
            const std::string  &get_date() const;
            //static std::vector<Talk> extractTalks(const Poco::JSON::Array::Ptr& talksArray);

         
           //const std::vector<long long>& get_talks() const;
            //const std::vector<long>& get_talks() const;
            long&        id();
            long&        user_id();
            std::string& title();
            std::string& type();
            std::string& location();
            Description& description();
            std::string& date();
            
            
            //std::vector<long>& talks();
        
            static std::optional<ConferenceInfo> read_by_id(long id);
            static std::optional<ConferenceInfo> delete_by_id(long id);
            static std::vector<ConferenceInfo> read_by_user_id(long user_id);
            static std::optional<ConferenceInfo> read_by_reports(long id);
            void   add();
            void   update();
            void   deletes();
            void update_rep(long id_conf);

            Poco::JSON::Object::Ptr toJSON() const;
            Poco::JSON::Object::Ptr testtoJSON3() const;
            Poco::JSON::Object::Ptr testtoJSON(const std::string &str, long id) const;
            Poco::JSON::Object::Ptr testtoJSON2(const std::string &str, long id) const;
            Poco::JSON::Object::Ptr talksToJSON() const;
             const std::vector<Talk>& get_talks() const {
            return _talks;
            }

            std::vector<Talk>& talks() {
            return _talks;
            }
        

    };
}
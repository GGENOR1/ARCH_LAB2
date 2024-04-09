#pragma once

#include <string>
#include <vector>
#include <optional>

#include <Poco/JSON/Object.h>

namespace database
{
    class ReportInfo{
        private:
            long _id;           //id доклада
            long _user_id;      //id создателя
            std::string _type; // тип доклада (как пример, если имеется только электронная версия, бумажная, все)
            long _rate;       //оценка доклада
            std::string _name; // название доклада

            struct Description {
                std::string topic; //тэги или тематика
                std::string descriptions; //краткое описание доклада
                const std::string to_string() const;
            };
            Description _description;

        public:

            static ReportInfo  fromJSON(const std::string & str);

            long               get_id() const;
            long               get_user_id() const;
            const std::string  &get_name() const;
            long               get_rate() const;
            const std::string &get_type() const;
            const std::string  get_description_as_string() const;

            long&        id();
            long&        user_id();
            std::string& name();
            long&        rate();
            std::string& type();
            Description&     description();

            static std::optional<ReportInfo> read_by_id(long id);
            static std::optional<ReportInfo> delete_by_id(long id);
            static std::vector<ReportInfo> read_by_user_id(long user_id);
            static std::vector<ReportInfo> read_all_reports();

            //static std::vector<ReportInfo>read_by_reports(long id);
            
            void   add();
            void   update();
            void   deletes();
            Poco::JSON::Object::Ptr toJSON() const;
            //Poco::JSON::Object::Ptr testtoJSON2(const std::string &str, long id) const;

    };
}
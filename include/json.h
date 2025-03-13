#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <initializer_list>
#include <iostream>

namespace badge881::json
{
    enum type
    {
        nullType,
        booleanType,
        numberType,
        stringType,
        objectType,
        collectionType
    };

    class json;

    typedef std::initializer_list<std::pair<std::string, json>> object;
    typedef std::initializer_list<json> collection;

    json parse(std::istream&);
    
    json parseFile(std::string);

    json parse(std::string);

    std::istream &operator>>(std::istream &, json &);
    
    std::string print(const json&);
    
    void printFile(const json&, const std::string&);
    
    std::ostream &operator<<(std::ostream &, const json &);
    
    class json
    {
        
        type typeName = nullType;

        std::nullptr_t dataForNull = nullptr;
        bool dataForBoolean = false;
        double dataForNum = 0.0;
        std::string dataForString = "";
        std::vector<json> dataForCollection = {};
        std::unordered_map<std::string, json> dataForObject = {};
        
        public:
        json();
        json(const json &);
        json(json &&);
        json(const type &);
        json(const std::string &);
        json(const char*);
        json(bool);
        json(double);
        json(int);
        json(unsigned int);
        json(long long);
        json(unsigned long long);
        json(collection);
        json(object);
        json(std::vector<json>);
        json(std::unordered_map<std::string, json>);
        
        json &operator=(const type &);
        json &operator=(const json &);
        json &operator=(json &&);
        json &operator=(const std::string &);
        json &operator=(bool);
        json &operator=(double);
        json &operator=(int);
        json &operator=(unsigned int);
        json &operator=(long long);
        json &operator=(unsigned long long);
        json &operator=(collection);
        json &operator=(object);
        json &operator=(std::vector<json>);
        json &operator=(std::unordered_map<std::string, json>);
        
        ~json();
        void clear();
        
        class type_error : public std::exception
        {
            std::string problem;
            
            public:
            type_error(std::string p) : problem(p) {}
            const char *what() const noexcept override { return problem.c_str(); }
        };
        class read_error : public std::exception
        {
            std::string problem;
            
            public:
            read_error(std::string p) : problem(p) {}
            const char *what() const noexcept override { return problem.c_str(); }
        };

        bool isNull() const;
        bool isBoolean() const;
        bool isNumber() const;
        bool isString() const;
        bool isObject() const;
        bool isCollection() const;
        
        type getType() const;
        std::string getTypeString() const;
        
        template <typename typeT>
        typeT &get();
        
        json &operator[](const std::string &);
        json &operator[](const int &);
        
        bool operator==(const json &) const;
        bool operator!=(const json &) const;
    };
    json parse(std::string);
    json parse(std::istream &);
};

template <>
struct std::hash<badge881::json::json>
{
    size_t operator()(const badge881::json::json &) const noexcept;

private:
    void combine(size_t &, const size_t &) const noexcept;
};
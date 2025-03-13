#include "../include/json.h"
#include <sstream>
#include <fstream>

void std::hash<badge881::json::json>::combine(size_t &hash, const size_t &other) const noexcept
{
    hash ^= other + 0x9e3779b9 + (hash << 6) + (hash >> 2);
}

size_t std::hash<badge881::json::json>::operator()(const badge881::json::json &s) const noexcept
{
    size_t hash = 0;
    if (s.isBoolean())
        hash = std::hash<bool>{}(s.get<bool>());
    else if (s.isNumber())
        hash = std::hash<double>{}(s.get<double>());
    else if (s.isString())
        hash = std::hash<std::string>{}(s.get<std::string>());
    else if (s.isObject())
        for (const auto &[key, value] : s.get<std::unordered_map<std::string, badge881::json::json>>())
        {
            combine(hash, std::hash<std::string>{}(key));
            combine(hash, std::hash<badge881::json::json>{}(value));
        }
    else if (s.isCollection())
        for (const auto &value : s.get<std::vector<badge881::json::json>>())
            combine(hash, std::hash<badge881::json::json>{}(value));
    return hash;
}

namespace badge881::json
{

    json::json() : typeName(nullType), dataForNull(nullptr) {}

    json::json(const json &other) : typeName(other.typeName)
    {
        switch (typeName)
        {
        case nullType:
            dataForNull = nullptr;
            break;
        case booleanType:
            dataForBoolean = other.dataForBoolean;
            break;
        case numberType:
            dataForNum = other.dataForNum;
            break;
        case stringType:
            dataForString = other.dataForString;
            break;
        case objectType:
            dataForObject = other.dataForObject;
            break;
        case collectionType:
            dataForCollection = other.dataForCollection;
            break;
        }
    }

    json::json(json &&other) : typeName(other.typeName)
    {
        switch (typeName)
        {
        case nullType:
            dataForNull = nullptr;
            break;
        case booleanType:
            dataForBoolean = other.dataForBoolean;
            break;
        case numberType:
            dataForNum = other.dataForNum;
            break;
        case stringType:
            dataForString = std::move(other.dataForString);
            break;
        case objectType:
            dataForObject = std::move(other.dataForObject);
            break;
        case collectionType:
            dataForCollection = std::move(other.dataForCollection);
            break;
        }
        other.typeName = nullType;
        other.dataForNull = nullptr;
    }

    json::json(const type &t) : typeName(t)
    {
        clear();
    }

    json::json(const std::string &s) : typeName(stringType), dataForString(s) {}

    json::json(const char *s) : typeName(stringType), dataForString(s) {}

    json::json(bool b) : typeName(booleanType), dataForBoolean(b) {}

    json::json(double num) : typeName(numberType), dataForNum(num) {}

    json::json(int num) : typeName(numberType), dataForNum(static_cast<double>(num)) {}

    json::json(unsigned int num) : typeName(numberType), dataForNum(static_cast<double>(num)) {}

    json::json(long long num) : typeName(numberType), dataForNum(static_cast<double>(num)) {}

    json::json(unsigned long long num) : typeName(numberType), dataForNum(static_cast<double>(num)) {}

    json::json(collection col) : typeName(collectionType), dataForCollection(col) {}

    json::json(std::vector<json> col) : typeName(collectionType), dataForCollection(col) {}

    json::json(object obj) : typeName(objectType)
    {
        for (const auto &[key, value] : obj)
        {
            dataForObject[key] = value;
        }
    }

    json::json(std::unordered_map<std::string, json> obj) : typeName(objectType), dataForObject(obj) {}

    json &json::operator=(const type &t)
    {
        typeName = t;
        clear();
        switch (typeName)
        {
        case nullType:
            dataForNull = nullptr;
            break;
        case booleanType:
            dataForBoolean = false;
            break;
        case numberType:
            dataForNum = 0.0;
            break;
        case stringType:
            dataForString = "";
            break;
        case objectType:
            dataForObject = {};
            break;
        case collectionType:
            dataForCollection = {};
            break;
        }

        return *this;
    }

    json &json::operator=(const json &other)
    {
        if (this != &other)
        {
            typeName = other.typeName;
            clear();
            switch (typeName)
            {
            case nullType:
                dataForNull = nullptr;
                break;
            case booleanType:
                dataForBoolean = other.dataForBoolean;
                break;
            case numberType:
                dataForNum = other.dataForNum;
                break;
            case stringType:
                dataForString = other.dataForString;
                break;
            case objectType:
                dataForObject = other.dataForObject;
                break;
            case collectionType:
                dataForCollection = other.dataForCollection;
                break;
            }
        }
        return *this;
    }

    json &json::operator=(json &&other)
    {
        if (this != &other)
        {
            typeName = other.typeName;
            clear();
            switch (typeName)
            {
            case nullType:
                dataForNull = nullptr;
                break;
            case booleanType:
                dataForBoolean = other.dataForBoolean;
                break;
            case numberType:
                dataForNum = other.dataForNum;
                break;
            case stringType:
                dataForString = std::move(other.dataForString);
                break;
            case objectType:
                dataForObject = std::move(other.dataForObject);
                break;
            case collectionType:
                dataForCollection = std::move(other.dataForCollection);
                break;
            }
            other.typeName = nullType;
            other.dataForNull = nullptr;
        }
        return *this;
    }

    json &json::operator=(const std::string &s)
    {
        typeName = stringType;
        clear();
        dataForString = s;
        return *this;
    }

    json &json::operator=(bool b)
    {
        typeName = booleanType;
        clear();
        dataForBoolean = b;
        return *this;
    }

    json &json::operator=(double num)
    {
        typeName = numberType;
        clear();
        dataForNum = num;
        return *this;
    }

    json &json::operator=(int num)
    {
        typeName = numberType;
        clear();
        dataForNum = static_cast<double>(num);
        return *this;
    }

    json &json::operator=(unsigned int num)
    {
        typeName = numberType;
        clear();
        dataForNum = static_cast<double>(num);
        return *this;
    }

    json &json::operator=(long long num)
    {
        typeName = numberType;
        clear();
        dataForNum = static_cast<double>(num);
        return *this;
    }

    json &json::operator=(unsigned long long num)
    {
        typeName = numberType;
        clear();
        dataForNum = static_cast<double>(num);
        return *this;
    }

    json &json::operator=(collection col)
    {
        typeName = collectionType;
        clear();
        dataForCollection = col;
        return *this;
    }

    json &json::operator=(std::vector<json> col)
    {
        typeName = collectionType;
        clear();
        dataForCollection = col;
        return *this;
    }

    json &json::operator=(object obj)
    {
        typeName = objectType;
        clear();
        for (const auto &[key, value] : obj)
            dataForObject[key] = value;
        return *this;
    }

    json &json::operator=(std::unordered_map<std::string, json> obj)
    {
        typeName = objectType;
        clear();
        for (const auto &[key, value] : obj)
            dataForObject[key] = value;
        return *this;
    }

    json::~json()
    {
        clear();
    }

    void json::clear()
    {
        dataForNull = nullptr;
        dataForBoolean = false;
        dataForNum = 0.0;
        dataForString = "";
        dataForObject.clear();
        dataForCollection.clear();
    }

    bool json::isNull() const 
    {
        return typeName == nullType;
    }

    bool json::isBoolean() const 
    {
        return typeName == booleanType;
    }

    bool json::isNumber() const 
    {
        return typeName == numberType;
    }

    bool json::isString() const
    {
        return typeName == stringType;
    }

    bool json::isObject() const
    {
        return typeName == objectType;
    }

    bool json::isCollection() const
    {
        return typeName == collectionType;
    }

    type json::getType() const
    {
        return typeName;
    }

    std::string json::getTypeString() const
    {
        switch (typeName)
        {
        case nullType:
            return "null";
        case booleanType:
            return "boolean";
        case numberType:
            return "number";
        case stringType:
            return "string";
        case objectType:
            return "object";
        case collectionType:
            return "collection";
        }
        return "null";
    }

    template <typename T>
    T &json::get()
    {
        throw type_error("given type is not supported");
    }

    template <>
    std::nullptr_t &json::get<std::nullptr_t>() 
    {
        if (!isNull())
            throw type_error("json value is not null, type is : \'" + getTypeString() + "\'");
        return dataForNull;
    }

    template <>
    bool &json::get<bool>()
    {
        if (!isBoolean())
            throw type_error("json value is not a boolean, type is : \'" + getTypeString() + "\'");
        return dataForBoolean;
    }

    template <>
    double &json::get<double>()
    {
        if (!isNumber())
            throw type_error("json value is not a number, type is : \'" + getTypeString() + "\'");
        return dataForNum;
    }

    template <>
    std::string &json::get<std::string>()
    {
        if (!isString())
            throw type_error("json value is not a string, type is : \'" + getTypeString() + "\'");
        return dataForString;
    }

    template <>
    std::vector<json> &json::get<std::vector<json>>()
    {
        if (!isCollection())
            throw type_error("json value is not a collection, type is : \'" + getTypeString() + "\'");
        return dataForCollection;
    }

    template <>
    std::unordered_map<std::string, json> &json::get<std::unordered_map<std::string, json>>()
    {
        if (!isObject())
            throw type_error("json value is not a object, type is : \'" + getTypeString() + "\'");
        return dataForObject;
    }


    json &json::operator[](const std::string &key)
    {
        if (!isObject())
            throw type_error("json value is not a object, type is : \'" + getTypeString() + "\'");
        return dataForObject[key];
    }

    json &json::operator[](const int &index)
    {
        if (!isCollection())
            throw type_error("json value is not a collection, type is : \'" + getTypeString() + "\'");
        if (index < 0)
            throw std::out_of_range("index is out of range");
        if (index >= dataForCollection.size())
            dataForCollection.resize(index + 1);
        return dataForCollection[index];
    }

    bool json::operator==(const json &other) const
    {
        if (typeName != other.typeName)
            return false;
        switch (typeName)
        {
        case nullType:
            return true;
        case booleanType:
            return dataForBoolean == other.dataForBoolean;
        case numberType:
            return dataForNum == other.dataForNum;
        case stringType:
            return dataForString == other.dataForString;
        case objectType:
            return dataForObject == other.dataForObject;
        case collectionType:
            return dataForCollection == other.dataForCollection;
        }
        return false;
    }

    bool json::operator!=(const json &other) const
    {
        return !operator==(other);
    }

    std::string print(const json &j)
    {
        std::stringstream os;

        switch (j.getType())
        {
        case nullType:
            os << "null";
            break;
        case booleanType:
            os << (j.get<bool>() ? "true" : "false");
            break;
        case numberType:
            os << j.get<double>();
            break;
        case stringType:
            os << "\"" << j.get<std::string>() << "\"";
            break;
        case objectType:
            os << "{";
            const std::unordered_map<std::string, json>& obj = j.get<std::unordered_map<std::string, json>>();
            for (auto it = obj.begin(); it != obj.end(); ++it)
            {
                if (it != obj.begin())
                    os << ", ";
                os << "\"" << it->first << "\": " << print(it->second);
            }
            os << "}";
            break;
        case collectionType:
            os << "[";
            const std::vector<json>& array = j.get<std::vector<json>>();
            for (size_t i = 0; i < array.size(); ++i)
            {
                if (i > 0)
                    os << ", ";
                os << print(array[i]);
            }
            os << "]";
            break;
        }
        return os.str();
    }

    void printFile(const json &j, std::string filePath)
    {
        std::ofstream os(filePath);
        os << print(j);
    }

    std::ostream &operator<<(std::ostream &os, const json &j)
    {
        os << print(j);
        return os;
    }

    void skipWhitespace(std::istream &is)
    {
        while (is.peek() == ' ' || is.peek() == '\n' || is.peek() == '\t' || is.peek() == '\r')
            is.get();
    }

    json parse(std::istream &is)
    {
        json j;
        skipWhitespace(is);
        char start = is.peek();

        if (start == 'n') // should be null
        {
            std::string shouldbeNULL;
            shouldbeNULL.resize(4);
            is.read(&shouldbeNULL[0], 4);
            if (shouldbeNULL != "null")
                throw json::read_error("invalid json input : null error");
        }
        else if (start == 't') // should be true
        {
            std::string shouldbeTRUE;
            shouldbeTRUE.resize(4);
            is.read(&shouldbeTRUE[0], 4);
            if (shouldbeTRUE != "true")
                throw json::read_error("invalid json input : true error");
            j = json(true);
        }
        else if (start == 'f') // should be false
        {
            std::string shouldbeFALSE;
            shouldbeFALSE.resize(5);
            is.read(&shouldbeFALSE[0], 5);
            if (shouldbeFALSE != "false")
                throw json::read_error("invalid json input : false error");
            j = json(false);
        }
        else if (auto num = [](char a)
                 { return isdigit(a) || a == '-' || a == '.'; };
                 num(start))
        {
            std::stringstream current;
            while (num(is.peek()))
                current << char(is.get());
            double value;
            current >> value;
            return json(value);
        }
        else if (start == '"')
        {
            std::string value;
            char last = is.get(), current = is.get();
            while (current != '"' || last == '\\')
            {
                value += current;
                last = current;
                current = is.get();
            }
            return json(value);
        }
        else if (start == '{')
        {
            std::unordered_map<std::string, json> obj;

            skipWhitespace(is);
            while (is.get() != '}')
            {
                std::string key;
                json value;
                skipWhitespace(is);
                char last = is.get(), current = is.get();
                while (current != '"' || last == '\\')
                {
                    key += current;
                    last = current;
                    current = is.get();
                }
                skipWhitespace(is);
                if (is.get() != ':')
                    throw json::read_error("invalid json input : object error");
                obj[key] = parse(is);
                skipWhitespace(is);
            }
            j = json(obj);
        }
        else if (start == '[')
        {
            std::vector<json> col;

            while (is.get() != ']')
            {
                col.push_back(parse(is));
                skipWhitespace(is);
            }
            j = json(col);
        }
        else
            throw json::read_error("invalid json input : type not found");
        return j;
    }

    json parse(std::string input)
    {
        std::istringstream is(input);
        return parse(is);
    }

    json parseFile(std::string filePath)
    {
        std::ifstream is(filePath);
        return parse(is);
    }

    std::istream &operator>>(std::istream &is, json &j)
    {
        j = parse(is);
        return is;
    }
};
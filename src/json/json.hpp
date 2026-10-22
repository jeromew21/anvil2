#pragma once

#include "nlohmann/json.hpp"

// Add this template specialization for std::optional support
namespace nlohmann {
    template<typename T>
    struct adl_serializer<std::optional<T>> {
        static void to_json(json &j, const std::optional<T> &opt) {
            if (opt) {
                j = *opt;
            } else {
                j = nullptr;
            }
        }

        static void from_json(const json &j, std::optional<T> &opt) {
            if (j.is_null()) {
                opt = std::nullopt;
            } else {
                opt = j.get<T>();
            }
        }
    };
}

// Serialization / Deserialization helpers
template<typename T>
std::string json_serialize(const T &obj) {
    using namespace nlohmann;
    json j = obj;
    return j.dump();
}

template<typename T>
T json_deserialize(const std::string &json_string) {
    using namespace nlohmann;
    json j = json::parse(json_string);
    return j.get<T>();
}

// Templated function that throws on error
template<typename T>
T json_deserialize_or_throw(const std::string &str) {
    using namespace nlohmann;
    try {
        json j = json::parse(str);
        return j.get<T>();
    } catch (const json::parse_error &e) {
        throw std::runtime_error(std::string("JSON parse error: ") + e.what());
    } catch (const json::type_error &e) {
        throw std::runtime_error(std::string("JSON type error: ") + e.what());
    }
}

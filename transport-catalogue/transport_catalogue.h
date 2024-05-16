#pragma once

#include <string>
#include <deque>
#include <iostream>
#include <set>
#include <algorithm>
#include <unordered_set>
#include <vector>
#include <string_view>
#include "geo.h"

using Geo::Coordinates;

namespace Data
{
class TransportCatalogue 
{ 
    struct Bus; 
    struct Stop; 
    struct BusHash; 

public: 
    void Addbus(std::string_view name, std::vector<std::string_view> stops) const; 
    void Addstop(std::string_view name, Coordinates &&coordinates) const; 
    const Bus *get_bus(std::string_view bus) const; 

private: 
    const std::unordered_set<Bus, Bus_Hash> &get_buses() const; 
    size_t GetStopCount(std::string_view bus) const; 
    size_t GetUniqueStopCount(std::string_view bus) const; 
    double GetRouteLength(std::string_view bus) const; 

    struct Stop 
    { 
        std::string name; 
        Coordinates coordinates; 

        bool operator==(const Stop &stop) const 
        { 
            return name == stop.name && coordinates == stop.coordinates; 
        } 
    };
};
        struct Bus 
        { 
            std::string name; 
            std::vector<const Stop *> stops; 

            bool operator==(const Bus &bus) const 
            { 
                return name == bus.name; 
            } 
        };

        struct BusHash
        {
            size_t operator()(const Bus &bus) const
            {
                return std::hash<std::string>{}(bus.name);
            }
        };

        struct StopHash
        {
            size_t operator()(const Stop &stop) const
            {
                return static_cast<size_t>(static_cast<int>((stop.coordinates.lat * stop.coordinates.lng * 100000.0)));
            }
        };

        std::unordered_set<Bus, BusHash> buses_;
        std::unordered_set<Stop, StopHash> stops_;
    };
}

#include "stat_reader.h"
#include <iomanip>
#include <set>
#include <math.h>

using namespace std::string_view_literals;

Request::Req Request::ParseRequest(const std::string_view request)
{
    Req result;
    auto not_space = request.find_first_not_of(' ');
    if (not_space == std::string_view::npos)
        return {};
    auto pos = request.find(' ', not_space);
    result.request = request.substr(not_space, pos - not_space);
    pos = request.find_first_not_of(' ', pos);
    auto last_pos = request.find_last_not_of(' ');
    result.what = request.substr(pos, last_pos - pos + 1);
    return result;
}
void Request::ParseAndPrintStat(const Data::TransportCatalogue &transport_catalogue, std::string_view request,
                       std::ostream &output)
{
    auto parsed = Request::ParseRequest(request);
    output << parsed.request << " "sv << parsed.what << ": "sv;
    if (parsed.request == "Bus"sv)
    {
        if (transport_catalogue.get_bus(parsed.what) == nullptr)
        {
            output << "not found\n"sv;
        }
        else
        {
            PrintBusData(transport_catalogue, parsed.what, output);
        }
        return;
    }
    if (parsed.request == "Stop"sv)
    {
        auto stop = transport_catalogue.get_stop(parsed.what);
        if (stop == nullptr)
        {
            output << "not found\n"sv;
        }
        else
        {
            PrintBusStopData(transport_catalogue, stop, output);
        }
    }
}

void Request::PrintBusData(const Data::TransportCatalogue &transport_catalogue, std::string_view bus_name, std::ostream &output)
{
    double route_length = transport_catalogue.get_route_length(bus_name);
    output << stop_on_route << " stops on route, "sv;
    output << unique_stop_on_route << " unique stops, "sv;
    PutRouteToOutput(route_length, output);
}

void Request::PrintBusStopData(const Data::TransportCatalogue &transport_catalogue, const Data::Stop* stop, std::ostream &output)
{
    std::set<std::string_view> bus_list;
    for (const auto &bus : transport_catalogue.get_buses())
    {
        if (std::find(bus.stops.cbegin(), bus.stops.cend(), stop) != bus.stops.cend())
            bus_list.insert(bus.name);
    }
    if (bus_list.empty())
    {
        output << "no buses\n";
        return;
    }
    output << "buses"sv;
    for (const auto &bus : bus_list)
    {
        output << " "sv << bus;
    }
    output << "\n";
}
void PutRouteToOutput(double value, std::ostream &output)
{
    int precision = 1;
    while (true)
    {
        ++precision;
        if (value < std::pow(10, precision))
            break;
    }
    output << std::fixed << std::setprecision(6 - precision) << value << " route length\n"sv;
}

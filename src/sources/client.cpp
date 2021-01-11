// client.cpp

#include <client.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/nowide/iostream.hpp>
#include <iostream>
#include <string>

namespace pt = boost::property_tree;

ClientLayer::Client::Client(const NetworkLayer::ContextPtr& contextPtr,
                            const std::string& host, const std::string& port)
        : mConnection(contextPtr, {host, port}) {
    mConnectionSuccessful = mConnection.start();
}

void ClientLayer::Client::process() {
    if (!mConnectionSuccessful) {
        std::cout << "Connection Was Unsuccessful";
        return;
    }

    mConnection.send(mCity, mToken);
    std::string result = mConnection.receive();

    if (result[0] != '{')
        std::cout << result << std::endl;
    else
        processJson(result);

    mConnection.close();
}

void ClientLayer::Client::setCity(const std::string& city) {
    mCity = city;
}

void ClientLayer::Client::setToken(const std::string& token) {
    mToken = token;
}

void ClientLayer::Client::processJson(const std::string& json) {
    using namespace std;

    boost::property_tree::ptree propertyTree;
    stringstream strStream(json);
    boost::property_tree::read_json(strStream, propertyTree);

    auto city = propertyTree.get<string>("name");
    auto temperature = propertyTree.get<double>("main.temp");
    auto windSpeed = propertyTree.get<double>("wind.speed");
    auto windDirection = propertyTree.get<double>("wind.deg");

    boost::nowide::cout << "City: " << city << endl;
    boost::nowide::cout << "Temperature: " << temperature << "°C" << endl;
    boost::nowide::cout << "Wind's speed: " << windSpeed << " meter/sec" << endl;
    boost::nowide::cout << "Wind's direction: " << windDirection << "°" << endl;
}

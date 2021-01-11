// application.cpp

#include <application.hpp>
#include <client.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>

namespace po = boost::program_options;
using namespace std;

namespace {
    constexpr const char* const kConfigFile = "./config.txt";
    constexpr const char* const kDefaultCity = "Kyiv";
    constexpr const char* const kHost = "api.openweathermap.org";
    constexpr const char* const kPort = "80";
}

Application::Application(int argc, char** argv) :
        mArgCount(argc), mArgs(argv) {}

int Application::exec() {
    try {
        string city;
        string token;

        po::variables_map vm;

        // Parse Command Line Arguments
        po::options_description mainOptions("Main options");
        mainOptions.add_options()
                ("help,h", "Show help")
                ("city,c", po::value<string>(&city), "Enter city (If the city name is divided into "
                                                     "several words - use _ instead of space)")
                ("token,t", po::value<string>(&token), "Enter token");

        po::store(po::parse_command_line(mArgCount, mArgs, mainOptions), vm);
        po::notify(vm);

        // Parse Configuration File
        ifstream fileStream(kConfigFile);
        if (fileStream) {
            po::options_description configOptions;
            configOptions.add_options()
                    ("token,t", po::value<string>(&token), "Enter token");

            store(parse_config_file(fileStream, configOptions), vm);
            notify(vm);
        }

        if (vm.empty() || vm.count("help")) {
            cout << mainOptions << endl;
            return 0;
        }

        if (token.empty()) {
            cout << "Token was not defined" << endl;
            return 0;
        }

        if (city.empty()) {
            cout << "City was not defined. Using default: " << kDefaultCity << endl;
            city = kDefaultCity;
        } else {
            for (char& ch : city)
                if (ch == '_')
                    ch = ' ';
        }

        NetworkLayer::ContextPtr contextPointer = make_shared<boost::asio::io_context>();
        ClientLayer::Client client(contextPointer, kHost, kPort);
        client.setCity(city);
        client.setToken(token);
        client.process();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Exception of unknown type" << endl;
        return 1;
    }

    return 0;
}


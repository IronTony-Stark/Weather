#pragma once

#include <network.hpp>

namespace ClientLayer {
    using tcp = NetworkLayer::tcp;

    class Client {
    public:
        /**
         * @brief Ctor
         * @param contextPtr - smart pointer to io_context
         * @param host - server address
         * @param port - server port
         * */
        Client(const NetworkLayer::ContextPtr& contextPtr, const std::string& host, const std::string& port);

        /**
         * @brief Main function that connections to server, sends request and shows result
         * */
        void process();

        void setCity(const std::string& city);
        void setToken(const std::string& token);

    private:
        static void processJson(const std::string& json);

        NetworkLayer::Network mConnection;
        bool mConnectionSuccessful;
        std::string mCity;
        std::string mToken;
    };
}
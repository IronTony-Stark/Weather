#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace NetworkLayer {
    namespace beast = boost::beast;
    namespace http = beast::http;
    namespace ip = boost::asio::ip;
    using tcp = ip::tcp;
    using ContextPtr = std::shared_ptr<boost::asio::io_context>;

    typedef struct IPAddress {
        std::string host;
        std::string port;
    } IPAddress;

    class Network {
    public:
        /**
         * @brief Ctor
         * @param contextPtr - smart pointer to io_context
         * @param connectionIP - structure that contains host and port for connection
         * */
        Network(const ContextPtr& contextPtr, IPAddress connectionIP);

        /**
         * @brief Dtor
         * Shutdowns and closes stream's socket
         * */
        ~Network();

        /**
         * @brief Connection to server
         * @returns successful connection
         * */
        bool start();

        /**
         * @brief Close connection to server
         * */
        void close();

        /**
         * @brief Prepare request and call send
         * */
        void send(const std::string& city, const std::string& token);

        /**
         * @brief Convert response to string
         * @returns Server response
         * */
        std::string receive();

    private:
        /**
         * @brief Send request to connectionIP
         * @param request - ready for send request
         * */
        void send(const http::request<http::string_body>& request);

        /**
         * @brief Receive server response
         * @returns server response
         * */
        http::response<http::dynamic_body> receiveResponse();

        tcp::resolver mResolver;
        beast::tcp_stream mStream;
        IPAddress mIpAddress;
        bool mIsConnected;
    };
}
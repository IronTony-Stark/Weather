// networklayer.cpp

#include <network.hpp>
#include <iostream>
#include <boost/beast/version.hpp>

namespace {
    constexpr const int kConnectTimeOutSec = 10;
    constexpr const int kSendTimeOutSec = 10;
    constexpr const int kReceiveTimeOutSec = 10;
}

NetworkLayer::Network::Network(const ContextPtr& contextPtr, IPAddress connectionIP)
        : mResolver(*contextPtr), mStream(*contextPtr),
          mIpAddress(std::move(connectionIP)), mIsConnected(false) {}

NetworkLayer::Network::~Network() {
    close();
}

bool NetworkLayer::Network::start() {
    beast::error_code ec;

    mStream.expires_after(std::chrono::seconds(kConnectTimeOutSec));
    auto const results = mResolver.resolve(mIpAddress.host, mIpAddress.port);
    mStream.connect(results, ec);
    mStream.expires_never();

    mIsConnected = !ec;
    return mIsConnected;
}

void NetworkLayer::Network::close() {
    if (!mIsConnected)
        return;
    mIsConnected = false;

    beast::error_code ec;

    mStream.socket().shutdown(tcp::socket::shutdown_both, ec);
    if (ec && ec != beast::errc::not_connected)
        throw beast::system_error{ec};

    mStream.socket().close(ec);
    if (ec)
        throw beast::system_error{ec};
}

void NetworkLayer::Network::send(const std::string& city, const std::string& token) {
    std::string target = "/data/2.5/weather?q=" + city + "&appid=" + token + "&units=metric";
    http::request<http::string_body> request;

    request.version(11);
    request.method(http::verb::get);
    request.target(target);
    request.set(http::field::host, mIpAddress.host);
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    send(request);
}

std::string NetworkLayer::Network::receive() {
    http::response<http::dynamic_body> res = receiveResponse();
    if (res.result() != http::status::ok) {
        std::string code = std::to_string(res.result_int());
        std::string reason = res.reason().to_string();
        return code + "\n" + reason;
    } else {
        return beast::buffers_to_string(res.body().data());
    }
}

void NetworkLayer::Network::send(const boost::beast::http::request<boost::beast::http::string_body>& request) {
    beast::error_code ec;

    mStream.expires_after(std::chrono::seconds(kSendTimeOutSec));
    http::write(mStream, request, ec);
    mStream.expires_never();

    if (ec)
        throw beast::system_error{ec};
}

boost::beast::http::response<boost::beast::http::dynamic_body> NetworkLayer::Network::receiveResponse() {
    beast::error_code ec;
    beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;

    mStream.expires_after(std::chrono::seconds(kReceiveTimeOutSec));
    http::read(mStream, buffer, res, ec);
    mStream.expires_never();

    if (ec)
        throw beast::system_error{ec};

    return res;
}

#pragma once

#include <string>
#include <Sockets/IPAddress.hpp>

namespace Socket
{
    class Auth
    {
    public:
        Auth(std::string const& user, std::string const& host, uint16_t port);
        Auth();
        std::string const& GetUser() const { return user_;}
        std::string const& GetHost() const { return host_;}
        uint16_t GetPort()           const { return port_;}
        IPAddress GetIPAddress() const;
    private:
        std::string user_;
        std::string host_;
        uint16_t port_;
    };

    class Uri
    {
    public:
        Uri(const char* value);
        Uri(std::string const& value);
        Uri();

        std::string const& GetScheme() const { return scheme_;}
        Auth const& GetAuthority() const { return authority_;}
        std::string const& GetPath() const { return path_;}
        std::string const& GetHost() const { return authority_.GetHost();}
        uint16_t GetPort() const { return authority_.GetPort();}

        void SchemeIs(std::string const& scheme);
        void AuthorityIs(std::string const& authority);
    private:
        std::string scheme_;
        Auth authority_;
        std::string path_;
    };
}
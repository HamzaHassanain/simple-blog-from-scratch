#include <bits/stdc++.h>

#include "library/web-lib.hpp"

int main()
{
    try
    {

        auto server = std::make_unique<hamza_web::web_server<>>(8080);

        server->listen([]
                       { std::cout << "Server is listening..." << std::endl; });
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
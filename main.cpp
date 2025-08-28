
#include "definentions.hpp"
#include <bits/stdc++.h>
#include "library/web-lib.hpp"
#include "views/views.hpp"
#include "routes/routes.hpp"

int main()
{
    try
    {
        int port = 8080;
        std::string host = "0.0.0.0";
        hh_web::logger::absolute_path_to_logs = "/home/hamza/Documents/Learnings/Projects/simple-blog-from-scratch/logs/";
        hh_web::logger::enabled_logging = true;
        hh_http::config::MAX_BODY_SIZE = 1024 * 64;
        hh_http::config::MAX_HEADER_SIZE = 1024 * 4;
        hh_http::config::MAX_IDLE_TIME_SECONDS = std::chrono::seconds(5);

        auto server = std::make_unique<hh_web::web_server<>>(port);

        // Set up the router
        auto router = make_views_router();
        auto api = make_apis_router();

        server->use_router(router);
        server->use_router(api);

        server->use_static("static");

        server->listen([port]
                       { std::cout << "Server is listening at: http://localhost:" + std::to_string(port) << std::endl; });
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
#pragma once

#include "../models/models.hpp"
#include "../views/views.hpp"
#include "../library/web-lib.hpp"
#include "../library/libs/json/json-parser.hpp"

#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <algorithm>

// Utility function to parse form data from request body
std::map<std::string, std::string> parse_form_data(const std::string &body)
{
    std::map<std::string, std::string> form_data;

    std::stringstream ss(body);
    std::string pair;

    while (std::getline(ss, pair, '&'))
    {
        size_t pos = pair.find('=');
        if (pos != std::string::npos)
        {
            std::string key = pair.substr(0, pos);
            std::string value = pair.substr(pos + 1);

            // Basic URL decoding (replace + with space and decode %XX sequences)
            std::replace(value.begin(), value.end(), '+', ' ');
            // TODO: Implement full URL decoding if needed

            form_data[key] = value;
        }
    }

    return form_data;
}

// Utility function to get current timestamp
std::string get_current_timestamp()
{
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%B %d, %Y");
    return oss.str();
}

// Utility function to get blog by ID
Blog get_blog_by_id(int id)
{
    auto blogs = Blog::get_blogs_from_file(CPP_PROJECT_SOURCE_DIR + std::string("/blogs.db"));
    for (const auto &blog : blogs)
    {
        if (blog.get_id() == id)
        {
            return blog;
        }
    }
    throw std::runtime_error("Blog not found");
}

// Utility function to save blogs to file
void save_blogs_to_file(const std::vector<Blog> &blogs)
{
    std::ofstream file(CPP_PROJECT_SOURCE_DIR + std::string("/blogs.db"));
    for (const auto &blog : blogs)
    {
        file << blog.to_string_for_file() << std::endl;
    }
    file.close();
}

using namespace hh_json;

// Utility function to convert Blog to JSON object
std::shared_ptr<JsonObject> blog_to_json(const Blog &blog)
{
    auto json_blog = std::make_shared<JsonObject>();
    json_blog->insert("id", maker::make_number(blog.get_id()));
    json_blog->insert("title", maker::make_string(blog.get_title()));
    json_blog->insert("content", maker::make_string(blog.get_content()));
    json_blog->insert("preview_content", maker::make_string(blog.get_preview_content()));
    json_blog->insert("created_at", maker::make_string(blog.get_created_at()));
    return json_blog;
}

// Utility function to create JSON response
void send_json_response(std::shared_ptr<hh_web::web_response> res, std::shared_ptr<JsonObject> json_obj, int status_code = 200, const std::string &status_message = "OK")
{
    res->set_status(status_code, status_message);
    res->set_header("Content-Type", "application/json");
    res->set_body(json_obj->stringify());
}

// Utility function to send JSON error response
void send_json_error(std::shared_ptr<hh_web::web_response> res, const std::string &error_message, int status_code = 500)
{
    auto error_json = std::make_shared<JsonObject>();
    error_json->insert("error", maker::make_string(error_message));
    send_json_response(res, error_json, status_code, "Error");
}

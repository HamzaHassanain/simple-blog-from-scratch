#pragma once

#include "../utils/utils.hpp"
#include "../models/models.hpp"
#include "../library/web-lib.hpp"
#include "../library/libs/json/json-parser.hpp"
#include <fstream>
#include <sstream>

using namespace hh_json;

// Middleware for API admin authentication
hh_web::exit_code api_auth_admin(std::shared_ptr<hh_web::web_request> req, std::shared_ptr<hh_web::web_response> res)
{
    auto auth_headers = req->get_header("Authorization");

    bool authenticated = false;
    for (const auto &auth_header : auth_headers)
    {
        // Simple Bearer token authentication (in production, use proper JWT or similar)
        if (auth_header.find("Bearer admin-token-123") != std::string::npos)
        {
            authenticated = true;
            break;
        }
    }

    // Also check for cookie-based auth as fallback
    if (!authenticated)
    {
        auto cookies = req->get_header("Cookie");
        for (const auto &cookie : cookies)
        {
            if (cookie.find("admin_session=authenticated") != std::string::npos)
            {
                authenticated = true;
                break;
            }
        }
    }

    if (!authenticated)
    {
        send_json_error(res, "Unauthorized", 401);
        return hh_web::exit_code::EXIT;
    }

    return hh_web::exit_code::CONTINUE;
}

// GET /api/blogs - Get all blogs
hh_web::exit_code api_get_all_blogs_controller(std::shared_ptr<hh_web::web_request> req, std::shared_ptr<hh_web::web_response> res)
{
    try
    {
        auto blogs = Blog::get_blogs_from_file(CPP_PROJECT_SOURCE_DIR + std::string("/blogs.db"));

        auto response_json = std::make_shared<JsonObject>();
        auto blogs_array = std::make_shared<JsonArray>();

        for (const auto &blog : blogs)
        {
            blogs_array->insert(blog_to_json(blog));
        }

        response_json->insert("blogs", blogs_array);
        response_json->insert("count", maker::make_number(blogs.size()));

        send_json_response(res, response_json);
        return hh_web::exit_code::EXIT;
    }
    catch (const std::exception &e)
    {
        send_json_error(res, "Error loading blogs");
        return hh_web::exit_code::EXIT;
    }
}

// GET /api/blogs/:id - Get single blog
hh_web::exit_code api_get_single_blog_controller(std::shared_ptr<hh_web::web_request> req, std::shared_ptr<hh_web::web_response> res)
{
    try
    {
        auto params = req->get_path_params();
        int blog_id = -1;

        for (const auto &[key, value] : params)
        {
            if (key == "id")
            {
                blog_id = std::stoi(value);
                break;
            }
        }

        if (blog_id == -1)
        {
            send_json_error(res, "Invalid blog ID", 400);
            return hh_web::exit_code::EXIT;
        }

        Blog blog = get_blog_by_id(blog_id);
        auto blog_json = blog_to_json(blog);

        send_json_response(res, blog_json);
        return hh_web::exit_code::EXIT;
    }
    catch (const std::exception &e)
    {
        send_json_error(res, "Blog not found", 404);
        return hh_web::exit_code::EXIT;
    }
}

// POST /api/blogs - Create new blog
hh_web::exit_code api_create_blog_controller(std::shared_ptr<hh_web::web_request> req, std::shared_ptr<hh_web::web_response> res)
{
    try
    {
        // Parse JSON body
        auto parsed_json = parse(req->get_body());

        // Extract required fields
        std::string title, content;

        if (parsed_json.find("title") != parsed_json.end())
        {
            title = getter::get_string(parsed_json["title"]);
        }

        if (parsed_json.find("content") != parsed_json.end())
        {
            content = getter::get_string(parsed_json["content"]);
        }

        if (title.empty() || content.empty())
        {
            send_json_error(res, "Title and content are required", 400);
            return hh_web::exit_code::EXIT;
        }

        // Create preview content (first 150 characters)
        std::string preview = content.length() > 150 ? content.substr(0, 150) + "..." : content;
        std::string created_at = get_current_timestamp();

        // Load existing blogs
        auto blogs = Blog::get_blogs_from_file(CPP_PROJECT_SOURCE_DIR + std::string("/blogs.db"));

        // Create new blog
        Blog new_blog(title, content, preview, created_at);
        blogs.push_back(new_blog);

        // Save blogs
        save_blogs_to_file(blogs);

        // Return created blog
        auto response_json = std::make_shared<JsonObject>();
        response_json->insert("message", maker::make_string("Blog created successfully"));
        response_json->insert("blog", blog_to_json(new_blog));

        send_json_response(res, response_json, 201, "Created");
        return hh_web::exit_code::EXIT;
    }
    catch (const std::exception &e)
    {
        send_json_error(res, "Error creating blog");
        return hh_web::exit_code::EXIT;
    }
}

// PUT /api/blogs/:id - Update existing blog
hh_web::exit_code api_update_blog_controller(std::shared_ptr<hh_web::web_request> req, std::shared_ptr<hh_web::web_response> res)
{
    try
    {
        auto params = req->get_path_params();
        int blog_id = -1;

        for (const auto &[key, value] : params)
        {
            if (key == "id")
            {
                blog_id = std::stoi(value);
                break;
            }
        }

        if (blog_id == -1)
        {
            send_json_error(res, "Invalid blog ID", 400);
            return hh_web::exit_code::EXIT;
        }

        // Parse JSON body
        auto parsed_json = parse(req->get_body());

        // Extract fields to update
        std::string title, content;

        if (parsed_json.find("title") != parsed_json.end())
        {
            title = getter::get_string(parsed_json["title"]);
        }

        if (parsed_json.find("content") != parsed_json.end())
        {
            content = getter::get_string(parsed_json["content"]);
        }

        if (title.empty() || content.empty())
        {
            send_json_error(res, "Title and content are required", 400);
            return hh_web::exit_code::EXIT;
        }

        // Load existing blogs
        auto blogs = Blog::get_blogs_from_file(CPP_PROJECT_SOURCE_DIR + std::string("/blogs.db"));

        // Find and update the blog
        bool found = false;
        Blog updated_blog(title, content, "", ""); // Temporary blog for response

        for (auto &blog : blogs)
        {
            if (blog.get_id() == blog_id)
            {
                blog.set_title(title);
                blog.set_content(content);
                // Update preview
                std::string preview = content.length() > 150 ? content.substr(0, 150) + "..." : content;
                blog.set_preview_content(preview);
                updated_blog = blog; // Copy for response
                found = true;
                break;
            }
        }

        if (!found)
        {
            send_json_error(res, "Blog not found", 404);
            return hh_web::exit_code::EXIT;
        }

        // Save blogs
        save_blogs_to_file(blogs);

        // Return updated blog
        auto response_json = std::make_shared<JsonObject>();
        response_json->insert("message", maker::make_string("Blog updated successfully"));
        response_json->insert("blog", blog_to_json(updated_blog));

        send_json_response(res, response_json);
        return hh_web::exit_code::EXIT;
    }
    catch (const std::exception &e)
    {
        send_json_error(res, "Error updating blog");
        return hh_web::exit_code::EXIT;
    }
}

// DELETE /api/blogs/:id - Delete blog
hh_web::exit_code api_delete_blog_controller(std::shared_ptr<hh_web::web_request> req, std::shared_ptr<hh_web::web_response> res)
{
    try
    {
        auto params = req->get_path_params();
        int blog_id = -1;

        for (const auto &[key, value] : params)
        {
            if (key == "id")
            {
                blog_id = std::stoi(value);
                break;
            }
        }

        if (blog_id == -1)
        {
            send_json_error(res, "Invalid blog ID", 400);
            return hh_web::exit_code::EXIT;
        }

        // Load existing blogs
        auto blogs = Blog::get_blogs_from_file(CPP_PROJECT_SOURCE_DIR + std::string("/blogs.db"));

        // Find and remove the blog
        auto it = std::remove_if(blogs.begin(), blogs.end(),
                                 [blog_id](const Blog &blog)
                                 { return blog.get_id() == blog_id; });

        if (it == blogs.end())
        {
            send_json_error(res, "Blog not found", 404);
            return hh_web::exit_code::EXIT;
        }

        blogs.erase(it, blogs.end());

        // Save blogs
        save_blogs_to_file(blogs);

        // Return success response
        auto response_json = std::make_shared<JsonObject>();
        response_json->insert("message", maker::make_string("Blog deleted successfully"));

        send_json_response(res, response_json);
        return hh_web::exit_code::EXIT;
    }
    catch (const std::exception &e)
    {
        send_json_error(res, "Error deleting blog");
        return hh_web::exit_code::EXIT;
    }
}

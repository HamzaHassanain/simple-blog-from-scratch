#pragma once

#include "../utils/utils.hpp"
#include "../models/models.hpp"
#include "../views/views.hpp"
#include "../library/web-lib.hpp"

// GET /
hh_web::exit_code index_controller(std::shared_ptr<hh_web::web_request> req, std::shared_ptr<hh_web::web_response> res)
{
    try
    {
        auto blogs = Blog::get_blogs_from_file(CPP_PROJECT_SOURCE_DIR + std::string("/blogs.db"));
        std::string html = index_view(blogs);
        res->set_header("Content-Type", "text/html");
        res->set_body(html);
        return hh_web::exit_code::EXIT;
    }
    catch (const std::exception &e)
    {
        res->set_status(500, "Internal Server Error");
        res->set_body("Error loading blogs");
        return hh_web::exit_code::EXIT;
    }
}

// GET /blogs/:id
hh_web::exit_code get_single_blog_controller(std::shared_ptr<hh_web::web_request> req, std::shared_ptr<hh_web::web_response> res)
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
            res->set_status(400, "Bad Request");
            res->set_body("Invalid blog ID");
            return hh_web::exit_code::EXIT;
        }

        Blog blog = get_blog_by_id(blog_id);
        std::string html = get_single_blog_view(blog);
        res->set_header("Content-Type", "text/html");
        res->set_body(html);
        return hh_web::exit_code::EXIT;
    }
    catch (const std::exception &e)
    {
        res->set_status(404, "Not Found");
        res->set_body("Blog not found");
        return hh_web::exit_code::EXIT;
    }
}

// GET /admin/login
hh_web::exit_code admin_login_controller(std::shared_ptr<hh_web::web_request> req, std::shared_ptr<hh_web::web_response> res)
{
    std::string html = admin_login_view();
    res->set_header("Content-Type", "text/html");
    res->set_body(html);
    return hh_web::exit_code::EXIT;
}

// POST /admin/login
hh_web::exit_code admin_login_post_controller(std::shared_ptr<hh_web::web_request> req, std::shared_ptr<hh_web::web_response> res)
{
    try
    {
        auto form_data = parse_form_data(req->get_body());

        std::string username = form_data["username"];
        std::string password = form_data["password"];

        // Simple authentication (in production, use proper password hashing)
        if (username == "admin" && password == "password")
        {
            // Set session cookie (simplified)
            res->add_header("Set-Cookie", "admin_session=authenticated; Path=/; HttpOnly");
            res->set_status(302, "Found");
            res->add_header("Location", "/admin/dashboard");
            return hh_web::exit_code::EXIT;
        }
        else
        {
            res->set_status(302, "Found");
            res->add_header("Location", "/admin/login?error=1");
            return hh_web::exit_code::EXIT;
        }
    }
    catch (const std::exception &e)
    {
        res->set_status(500, "Internal Server Error");
        res->set_body("Login error");
        return hh_web::exit_code::EXIT;
    }
}

// GET /admin/dashboard
hh_web::exit_code admin_dashboard_controller(std::shared_ptr<hh_web::web_request> req, std::shared_ptr<hh_web::web_response> res)
{
    try
    {
        auto blogs = Blog::get_blogs_from_file(CPP_PROJECT_SOURCE_DIR + std::string("/blogs.db"));
        std::string html = admin_dashboard_view(blogs);
        res->set_header("Content-Type", "text/html");
        res->set_body(html);
        return hh_web::exit_code::EXIT;
    }
    catch (const std::exception &e)
    {
        res->set_status(500, "Internal Server Error");
        res->set_body("Error loading dashboard");
        return hh_web::exit_code::EXIT;
    }
}

// GET /admin/blogs/:id/edit
hh_web::exit_code admin_edit_blog_controller(std::shared_ptr<hh_web::web_request> req, std::shared_ptr<hh_web::web_response> res)
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
            res->set_status(400, "Bad Request");
            res->set_body("Invalid blog ID");
            return hh_web::exit_code::EXIT;
        }

        Blog blog = get_blog_by_id(blog_id);
        std::string html = admin_edit_blog_view(blog);
        res->set_header("Content-Type", "text/html");
        res->set_body(html);
        return hh_web::exit_code::EXIT;
    }
    catch (const std::exception &e)
    {
        res->set_status(404, "Not Found");
        res->set_body("Blog not found");
        return hh_web::exit_code::EXIT;
    }
}

// POST /admin/blogs/create
hh_web::exit_code create_blog_controller(std::shared_ptr<hh_web::web_request> req, std::shared_ptr<hh_web::web_response> res)
{
    try
    {
        auto form_data = parse_form_data(req->get_body());

        std::string title = form_data["title"];
        std::string content = form_data["content"];

        if (title.empty() || content.empty())
        {
            res->set_status(400, "Bad Request");
            res->set_body("Title and content are required");
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

        res->set_status(302, "Found");
        res->add_header("Location", "/admin/dashboard");
        return hh_web::exit_code::EXIT;
    }
    catch (const std::exception &e)
    {
        res->set_status(500, "Internal Server Error");
        res->set_body("Error creating blog");
        return hh_web::exit_code::EXIT;
    }
}

// POST /admin/blogs/:id/edit
hh_web::exit_code update_blog_controller(std::shared_ptr<hh_web::web_request> req, std::shared_ptr<hh_web::web_response> res)
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
            res->set_status(400, "Bad Request");
            res->set_body("Invalid blog ID");
            return hh_web::exit_code::EXIT;
        }

        auto form_data = parse_form_data(req->get_body());
        std::string title = form_data["title"];
        std::string content = form_data["content"];

        if (title.empty() || content.empty())
        {
            res->set_status(400, "Bad Request");
            res->set_body("Title and content are required");
            return hh_web::exit_code::EXIT;
        }

        // Load existing blogs
        auto blogs = Blog::get_blogs_from_file(CPP_PROJECT_SOURCE_DIR + std::string("/blogs.db"));

        // Find and update the blog
        bool found = false;
        for (auto &blog : blogs)
        {
            if (blog.get_id() == blog_id)
            {
                blog.set_title(title);
                blog.set_content(content);
                // Update preview
                std::string preview = content.length() > 150 ? content.substr(0, 150) + "..." : content;
                blog.set_preview_content(preview);
                found = true;
                break;
            }
        }

        if (!found)
        {
            res->set_status(404, "Not Found");
            res->set_body("Blog not found");
            return hh_web::exit_code::EXIT;
        }

        // Save blogs
        save_blogs_to_file(blogs);

        res->set_status(302, "Found");
        res->add_header("Location", "/admin/dashboard");
        return hh_web::exit_code::EXIT;
    }
    catch (const std::exception &e)
    {
        res->set_status(500, "Internal Server Error");
        res->set_body("Error updating blog");
        return hh_web::exit_code::EXIT;
    }
}

// DELETE /admin/blogs/:id/delete
hh_web::exit_code delete_blog_controller(std::shared_ptr<hh_web::web_request> req, std::shared_ptr<hh_web::web_response> res)
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
            res->set_status(400, "Bad Request");
            res->set_body("Invalid blog ID");
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
            res->set_status(404, "Not Found");
            res->set_body("Blog not found");
            return hh_web::exit_code::EXIT;
        }

        blogs.erase(it, blogs.end());

        // Save blogs
        save_blogs_to_file(blogs);

        res->set_status(204, "No Content");
        return hh_web::exit_code::EXIT;
    }
    catch (const std::exception &e)
    {
        res->set_status(500, "Internal Server Error");
        res->set_body("Error deleting blog");
        return hh_web::exit_code::EXIT;
    }
}

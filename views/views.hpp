#pragma once
#include "../definentions.hpp"
#include "../library/libs/html-builder/html-builder.hpp"
#include "../models/models.hpp"

std::string index_view(const std::vector<Blog> &blogs = {})
{
    using namespace hh_html_builder;

    auto blogs_elm = std::make_shared<element>("main");

    for (const auto &blog : blogs)
    {
        auto article_elm = std::make_shared<element>("article");
        article_elm->add_child(std::make_shared<element>("h2", blog.get_title()));
        article_elm->add_child(std::make_shared<element>("p", blog.get_preview_content()));
        article_elm->add_child(std::make_shared<element>("a", "Read more",
                                                         std::map<std::string, std::string>({{"href", "blogs/" + std::to_string(blog.get_id())}})));
        article_elm->add_child(std::make_shared<element>("div", "Published on: " + blog.get_created_at(),
                                                         std::map<std::string, std::string>({{"class", "created-at"}})));
        blogs_elm->add_child(article_elm);
    }

    document doc;

    std::string index_path = CPP_PROJECT_SOURCE_DIR + std::string("views/html/index.html");

    std::string html;

    std::ifstream index_file(index_path);
    if (index_file)
    {
        html.assign((std::istreambuf_iterator<char>(index_file)),
                    (std::istreambuf_iterator<char>()));
    }

    auto elms = parse_html_string(html);

    for (auto &elm : elms)
    {
        doc.add_child(elm);
        elm->set_params_recursive({{"articles_html", blogs_elm->to_string()}});
    }

    return doc.to_string();
}

std::string admin_login_view()
{
    std::string index_path = CPP_PROJECT_SOURCE_DIR + std::string("views/html/admin-login.html");

    std::string html;

    std::ifstream index_file(index_path);
    if (index_file)
    {
        html.assign((std::istreambuf_iterator<char>(index_file)),
                    (std::istreambuf_iterator<char>()));
    }

    return html;
}

std::string admin_dashboard_view(const std::vector<Blog> &blogs = {})
{
    using namespace hh_html_builder;

    std::string index_path = CPP_PROJECT_SOURCE_DIR + std::string("views/html/admin-dashboard.html");

    std::string html;

    std::ifstream index_file(index_path);
    if (index_file)
    {
        html.assign((std::istreambuf_iterator<char>(index_file)),
                    (std::istreambuf_iterator<char>()));
    }
    document doc;

    element section("section");
    section.add_child(std::make_shared<element>("h2", "Current Blogs"));

    auto elms = parse_html_string(html);

    element ul("ul");

    for (const auto &blog : blogs)
    {
        element li("li");
        li.add_child(std::make_shared<element>("a", "Edit " + blog.get_title(),
                                               std::map<std::string, std::string>({{"href", "/admin/blogs/" + std::to_string(blog.get_id()) + "/edit"}})));
        li.add_child(std::make_shared<element>("button", "Delete",
                                               std::map<std::string, std::string>({{"onclick", "deleteBlog(" + std::to_string(blog.get_id()) + ")"}})));
        ul.add_child(std::make_shared<element>(li));
    }

    section.add_child(std::make_shared<element>(ul));

    for (auto &elm : elms)
    {
        elm->set_params_recursive({{"current_blogs_html", section.to_string()}});
        doc.add_child(elm);
    }

    return doc.to_string();
}

std::string admin_edit_blog_view(const Blog &blog)
{
    using namespace hh_html_builder;

    document doc;

    std::string index_path = CPP_PROJECT_SOURCE_DIR + std::string("views/html/admin-edit-blog.html");

    std::string html;

    std::ifstream index_file(index_path);
    if (index_file)
    {
        html.assign((std::istreambuf_iterator<char>(index_file)),
                    (std::istreambuf_iterator<char>()));
    }

    auto elms = parse_html_string(html);

    for (auto &elm : elms)
    {
        elm->set_params_recursive({{"blog_id", std::to_string(blog.get_id())},
                                   {"blog_title", blog.get_title()},
                                   {"blog_content", blog.get_content()}});
        doc.add_child(elm);
    }

    return doc.to_string();
}

std::string get_single_blog_view(const Blog &blog)
{

    using namespace hh_html_builder;

    document doc;

    std::string index_path = CPP_PROJECT_SOURCE_DIR + std::string("views/html/single-blog.html");

    std::string html;

    std::ifstream index_file(index_path);
    if (index_file)
    {
        html.assign((std::istreambuf_iterator<char>(index_file)),
                    (std::istreambuf_iterator<char>()));
    }

    auto elms = parse_html_string(html);
    for (auto &elm : elms)
    {
        elm->set_params_recursive({{"blog_id", std::to_string(blog.get_id())},
                                   {"blog_title", blog.get_title()},
                                   {"blog_content", blog.get_content()},
                                   {"blog_created_at", blog.get_created_at()}});
        doc.add_child(elm);
    }

    return doc.to_string();
}
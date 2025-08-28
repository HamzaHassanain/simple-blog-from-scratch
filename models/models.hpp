
#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class Blog
{
    static int id_counter;
    int id;
    std::string title;
    std::string content;
    std::string preview_content;
    std::string created_at;

public:
    // Constructor for new blogs (auto-assigns ID)
    Blog(std::string title, std::string content, std::string preview_content, std::string created_at)
        : title(title), content(content), preview_content(preview_content), created_at(created_at)
    {
        id = ++id_counter;
    }

    // Constructor for loading existing blogs from file (with specific ID)
    Blog(int id, std::string title, std::string content, std::string preview_content, std::string created_at)
        : id(id), title(title), content(content), preview_content(preview_content), created_at(created_at)
    {
        // Update counter if this ID is higher
        if (id > id_counter)
        {
            id_counter = id;
        }
    }

    int get_id() const { return id; }
    std::string get_title() const { return title; }
    std::string get_content() const { return content; }
    std::string get_preview_content() const { return preview_content; }
    std::string get_created_at() const { return created_at; }

    void set_title(const std::string &new_title) { title = new_title; }
    void set_content(const std::string &new_content) { content = new_content; }
    void set_preview_content(const std::string &new_preview_content) { preview_content = new_preview_content; }
    void set_created_at(const std::string &new_created_at) { created_at = new_created_at; }

    std::string to_string_for_file() const
    {
        return std::to_string(id) + "|" + title + "|" + content + "|" + preview_content + "|" + created_at;
    }

    static std::vector<Blog> get_blogs_from_file(const std::string &file_path)
    {
        std::vector<Blog> blogs;
        std::ifstream file(file_path);
        if (!file.is_open())
        {
            // File doesn't exist yet, return empty vector
            return blogs;
        }

        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty())
                continue; // Skip empty lines

            std::istringstream iss(line);
            std::string id_str, title, content, preview_content, created_at;

            // Parse: id|title|content|preview_content|created_at
            if (std::getline(iss, id_str, '|') &&
                std::getline(iss, title, '|') &&
                std::getline(iss, content, '|') &&
                std::getline(iss, preview_content, '|') &&
                std::getline(iss, created_at))
            {
                try
                {
                    int id = std::stoi(id_str);
                    blogs.emplace_back(id, title, content, preview_content, created_at);
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error parsing blog ID: " << id_str << std::endl;
                    continue;
                }
            }
        }
        file.close();
        return blogs;
    }
};

// Initialize static counter
int Blog::id_counter = 0;
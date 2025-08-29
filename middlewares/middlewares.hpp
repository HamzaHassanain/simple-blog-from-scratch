#pragma once
#include "../library/web-lib.hpp"
// Middleware for admin authentication
hh_web::exit_code admin_auth(std::shared_ptr<hh_web::web_request> req, std::shared_ptr<hh_web::web_response> res)
{
    auto cookies = req->get_header("Cookie");

    bool authenticated = false;
    for (const auto &cookie : cookies)
    {
        if (cookie.find("admin_session=authenticated") != std::string::npos)
        {
            authenticated = true;
            break;
        }
    }

    if (!authenticated)
    {
        res->set_status(302, "Found");
        res->add_header("Location", "/admin/login");
        return hh_web::exit_code::EXIT;
    }

    return hh_web::exit_code::CONTINUE;
}

hh_web::exit_code check_body(std::shared_ptr<hh_web::web_request> req, std::shared_ptr<hh_web::web_response> res)
{
    // Check for common XSS attack patterns
    const std::vector<std::string> xss_patterns = {
        "<script>", "</script>",
        "javascript:", "javascript%3A",
        "onerror=", "onload=", "onclick=", "onmouseover=",
        "eval(", "document.cookie",
        "fromCharCode", "String.fromCharCode",
        "alert(", "prompt(", "confirm("};

    // Check for SQL injection patterns
    const std::vector<std::string> sql_patterns = {
        "SELECT", "UPDATE", "DELETE", "INSERT", "DROP",
        "UNION", "JOIN", "WHERE",
        "--", "/*", "*/",
        "1=1", "OR 1=1", "' OR '1'='1",
        "SLEEP(", "BENCHMARK(",
        "information_schema"};

    // Check for command injection patterns
    const std::vector<std::string> cmd_patterns = {
        "`", "&&", "||", ";", "|",
        "$(", ">${",
        "/etc/passwd", "/bin/sh", "/bin/bash",
        "curl", "wget", "nc ", "netcat"};

    bool XSS = true;
    bool SQL = true;
    bool CMD = true;

    auto body = req->get_body();
    std::transform(body.begin(), body.end(), body.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });

    // tokenize the lowercase body
    std::istringstream iss(body);
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(iss, token, ' '))
    {
        tokens.push_back(token);
    }
    for (auto &t : tokens)
    {
        std::transform(t.begin(), t.end(), t.begin(),
                       [](unsigned char c)
                       { return std::tolower(c); });
    }

    for (auto &t : tokens)
    {
        if (XSS)
            for (const auto &pattern : xss_patterns)
            {
                if (t.find(pattern) != std::string::npos)
                {
                    res->set_status(400, "Bad Request");
                    res->send_text("Invalid input detected");
                    return hh_web::exit_code::EXIT;
                }
            }
        if (SQL)
            for (const auto &pattern : sql_patterns)
            {
                if (t == pattern)
                {
                    res->set_status(400, "Bad Request");
                    res->send_text("Invalid input detected");
                    return hh_web::exit_code::EXIT;
                }
            }
        if (CMD)
            for (const auto &pattern : cmd_patterns)
            {
                if (t == pattern)
                {
                    res->set_status(400, "Bad Request");
                    res->send_text("Invalid input detected");
                    return hh_web::exit_code::EXIT;
                }
            }

        // Check for unusual character sequences that might be encoded attacks
        int consecutive_special_chars = 0;
        for (char c : body)
        {
            if (c == '%' || c == '\\' || c == '+' || c == '&' || c == '<' || c == '>' || c == '=' || c == '"' || c == '\'')
            {
                consecutive_special_chars++;
                if (consecutive_special_chars > 5)
                {
                    res->set_status(400, "Bad Request");
                    res->send_text("Invalid input detected");
                    return hh_web::exit_code::EXIT;
                }
            }
            else
            {
                consecutive_special_chars = 0;
            }
        }
    }
    return hh_web::exit_code::CONTINUE;
};

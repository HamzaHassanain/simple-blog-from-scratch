# Quick Setup Guide

## For Developers Who Want to Try the Backend Web Library

This guide will get you up and running with our custom C++ web framework in just a few minutes.

## Prerequisites

- **C++17 Compiler**: GCC 7+, Clang 5+, or MSVC 2017+
- **CMake**: Version 3.10 or higher
- **Git**: For cloning and submodule management
- **Make**: For building (usually comes with your compiler)

### Ubuntu/Debian

```bash
sudo apt update
sudo apt install build-essential cmake git
```

### CentOS/RHEL/Fedora

```bash
sudo yum install gcc-c++ cmake git make
# or for newer versions:
sudo dnf install gcc-c++ cmake git make
```

### macOS

```bash
# Install Xcode command line tools
xcode-select --install

# Install CMake (if not already installed)
brew install cmake
```

### Windows

- Install Visual Studio 2017+ with C++ support
- Install CMake from https://cmake.org/download/
- Install Git from https://git-scm.com/

## Quick Start

### 1. Clone the Repository

```bash
git clone https://github.com/HamzaHassanain/simple-blog-from-scratch.git
cd simple-blog-from-scratch
```

### 2. Build the Project

```bash
chmod +x build.sh
# Linux/macOS
./build.sh # this will automatically initialize submodules and build the project

# Windows (from Developer Command Prompt)
mkdir build
cd build
cmake ..
cmake --build .
```

### 4. Run the Application

```bash
# Linux/macOS
./build/simple_blog

# Windows
.\build\Debug\simple_blog.exe
```

You should see:

```
Server is listening at: http://localhost:8080
```

### 5. Test the Application

Open your browser and visit:

- **Homepage**: http://localhost:8080
- **Admin Panel**: http://localhost:8080/admin/login (admin/password)
- **API**: http://localhost:8080/api/blogs

## Testing the Framework

### Run the Simple API Tests

```bash
cd test
npm install    # Install test dependencies
node test_api.js
```

The test suite will:

- Test all API endpoints
- Run security tests
- Measure performance metrics
- Test concurrent request handling

### Manual API Testing

```bash
# Get all blogs
curl http://localhost:8080/api/blogs

# Get specific blog
curl http://localhost:8080/api/blogs/1

# Create new blog (requires auth)
curl -X POST http://localhost:8080/api/blogs \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer admin-token-123" \
  -d '{"title": "Test Blog", "content": "This is a test blog post!"}'
```

## Exploring the Code

### Framework Integration

Look at `main.cpp` to see how simple it is to set up our web framework:

```cpp
#include "library/web-lib.hpp"

int main() {

    // these are the configuration settings,
    hh_web::logger::absolute_path_to_logs = "/home/hamza/Documents/Learnings/Projects/simple-blog-from-scratch/logs/";
    hh_web::logger::enabled_logging = true;
    hh_http::config::MAX_BODY_SIZE = 1024 * 64; // Max body size to accept,
    hh_http::config::MAX_HEADER_SIZE = 1024 * 4; // Max header size to accept
    hh_http::config::MAX_IDLE_TIME_SECONDS = std::chrono::seconds(5); // Max idle time

    // ...... code after
}
```

```cpp
    auto server = std::make_unique<hh_web::web_server<>>(port);

    // Set up the router
    auto router = make_views_router();
    auto api = make_apis_router();

    server->use_router(router);
    server->use_router(api);

    server->use_static("static");

    server->listen(....);
```

### Routing System

Check `routes/routes.hpp` to see our routing system in action:

```cpp
router->get("/", Route_V(index_controller));
router->get("/blogs/:id", Route_V(get_single_blog_controller));
router->post("/api/blogs", Route_V(api_auth_admin, api_create_blog_controller));
```

### JSON API

Look at `controllers/api_controllers.hpp` for JSON API implementation:

```cpp
// Using our custom JSON library
auto parsed_json = parse(req->get_body());
std::string title = getter::get_string(parsed_json["title"]);
auto response_json = blog_to_json(blog);
send_json_response(res, response_json);
```

### HTML Generation

See `views/views.hpp` for dynamic HTML generation:

```cpp
// Using our custom HTML builder
auto article_elm = std::make_shared<element>("article");
article_elm->add_child(std::make_shared<element>("h2", blog.get_title()));
```

## Common Issues & Solutions

### Build Errors

**Issue**: `fatal error: library/web-lib.hpp: No such file or directory`  
**Solution**: Make sure you ran `git submodule update --init --recursive`

**Issue**: CMake version too old  
**Solution**: Install CMake 3.10+ or use a newer system

### Runtime Errors

**Issue**: `Address already in use`  
**Solution**: Kill any process using port 8080 or change the port in `main.cpp`

**Issue**: Permission denied  
**Solution**: Run with `sudo` on Linux/macOS if binding to port < 1024

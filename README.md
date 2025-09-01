# Simple Blog From Scratch

## A Showcase Project for Backend Web Library That I built (C++)

This project is a **simple blog application built from scratch** using our custom-built **Backend Web Library for C++**. It serves as a demonstration of the my web library capabilities, including HTTP server functionality, routing, JSON handling, HTML templating, and more.

### 🎯 **Project Purpose**

This simple blog application is designed to showcase the features and capabilities of our custom web framework stack:

- **[Backend Web Library](https://github.com/HamzaHassanain/hamza-backend-web-library-cpp)** - The main web framework
- **[HTTP Server Library](https://github.com/HamzaHassanain/hamza-http-server-lib)** - Custom HTTP server implementation
- **[Socket Library](https://github.com/HamzaHassanain/hamza-socket-lib)** - Low-level networking
- **[JSON Parser Library](https://github.com/HamzaHassanain/hamza-json-parser)** - Custom JSON parsing and generation
- **[HTML Builder Library](https://github.com/HamzaHassanain/hamza-html-builder)** - Dynamic HTML generation

**All libraries used in this project are custom-built and developed by us.** No external web frameworks (like Express.js, Flask, etc.) were used.

---

## 🏗️ **Project Architecture**

### **Framework Stack**

```
┌─────────────────────────────────────────┐
│          Simple Blog Application        │
├─────────────────────────────────────────┤
│         Backend Web Library             │
├─────────────────────────────────────────┤
│  ┌─────────────┬──────────────────────┐ │
│  │ JSON Parser │   HTML Builder       │ │
│  └─────────────┴──────────────────────┘ │
├─────────────────────────────────────────┤
│           HTTP Server Library           │
├─────────────────────────────────────────┤
│            Socket Library               │
├─────────────────────────────────────────┤
│              C++ / OS                   │
└─────────────────────────────────────────┘
```

### **Application Structure**

```
simple-blog-from-scratch/
├── controllers/           # Request handlers
│   ├── controllers.hpp   # Web page controllers
│   └── api_controllers.hpp # JSON API controllers
├── models/               # Data models
│   └── models.hpp        # Blog model and file I/O
├── views/                # View layer
│   ├── views.hpp         # HTML view generation
│   └── html/             # HTML templates
├── routes/               # Routing configuration
│   └── routes.hpp        # Route definitions
├── middlewares/          # Custom middleware
│   └── middlewares.hpp   # Authentication middleware
├── static/               # Static assets (CSS)
├── test/                 # Comprehensive test suite
├── utils/                # Utility functions
├── library/              # Backend Web Library (submodule)
└── main.cpp              # Application entry point
```

---

## 🚀 **Features Demonstrated**

### **Web Framework Features**

- ✅ **HTTP Server** - Custom HTTP/1.1 server implementation
- ✅ **Routing System** - Pattern-based URL routing with parameters
- ✅ **Middleware Support** - Authentication and request processing
- ✅ **Static File Serving** - CSS, images, and other assets
- ✅ **JSON API** - RESTful API with JSON parsing/generation
- ✅ **HTML Templating** - Dynamic HTML generation
- ✅ **Session Management** - Cookie-based authentication
- ✅ **Error Handling** - Comprehensive error responses
- ✅ **Performance** - Multi-threaded request handling

### **Blog Application Features**

- ✅ **Public Blog Viewing** - Homepage with blog list and individual blog pages
- ✅ **Admin Panel** - Secure admin interface for blog management
- ✅ **CRUD Operations** - Create, Read, Update, Delete blog posts
- ✅ **Authentication** - Admin login with session management
- ✅ **REST API** - JSON API for programmatic access
- ✅ **Responsive Design** - Modern CSS with mobile support
- ✅ **File-based Storage** - Simple file-based blog storage

---

## 🛠️ **Technology Stack**

### **Core Technologies**

- **Language**: C++17
- **Build System**: CMake 3.10+
- **Compiler**: GCC 7+ / Clang 5+ / MSVC 2017+

### **Custom Libraries** (All Built by Me)

1. **[Backend Web Library](https://github.com/HamzaHassanain/hamza-backend-web-library-cpp)**

   - Main web framework with routing, middleware, and server management
   - HTTP request/response handling
   - Static file serving
   - Error handling and logging

2. **[HTTP Server Library](https://github.com/HamzaHassanain/hamza-http-server-lib)**

   - Custom HTTP/1.1 server implementation
   - Multi-threaded connection handling
   - Request parsing and response generation

3. **[Socket Library](https://github.com/HamzaHassanain/hamza-socket-lib)**

   - Cross-platform socket implementation
   - TCP server and client functionality
   - Connection management

4. **[JSON Parser Library](https://github.com/HamzaHassanain/hamza-json-parser)**

   - Complete JSON parser and generator
   - Type-safe JSON object manipulation
   - Support for all JSON data types

5. **[HTML Builder Library](https://github.com/HamzaHassanain/hamza-html-builder)**

   - Dynamic HTML generation
   - Template parsing and parameter substitution
   - DOM-like element construction

---

## 🏃‍♂️ **Quick Start**

### **Prerequisites**

- CMake 3.10 or higher
- C++17 compatible compiler
- Git (for submodules)

### **1. Clone the Repository**

```bash
git clone <repository-url>
cd simple-blog-from-scratch
```

### **2. Initialize Submodules**

```bash
git submodule update --init --recursive
```

### **3. Build the Project**

```bash
./build.sh
```

### **4. Run the Application**

```bash
./build/simple_blog
```

The server will start on `http://localhost:8080`

### **5. Access the Application**

- **Homepage**: http://localhost:8080
- **Individual Blog**: http://localhost:8080/blogs/1
- **Admin Login**: http://localhost:8080/admin/login
- **Admin Dashboard**: http://localhost:8080/admin/dashboard (after login)
- **API Endpoint**: http://localhost:8080/api/blogs

**Admin Credentials**: username: `admin`, password: `password`

---

## 🔌 **API Documentation**

Our custom web framework provides a complete REST API:

### **Public Endpoints**

```http
GET /api/blogs              # Get all blogs
GET /api/blogs/{id}         # Get specific blog
```

### **Admin Endpoints** (Require Authentication)

```http
POST /api/blogs             # Create new blog
PUT /api/blogs/{id}         # Update existing blog
DELETE /api/blogs/{id}      # Delete blog
```

### **Authentication**

- **Web Interface**: Cookie-based sessions
- **API**: Bearer token (`Authorization: Bearer admin-token-123`)

### **Example API Usage**

**Get All Blogs:**

```bash
curl http://localhost:8080/api/blogs
```

**Create New Blog:**

```bash
curl -X POST http://localhost:8080/api/blogs \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer admin-token-123" \
  -d '{"title": "My Blog Post", "content": "Blog content here..."}'
```

---

## 🧪 **Testing**

We've included a simple tests for the APIs:

### **Test Categories**

- **API Performance Testing** - Concurrent request handling
- **Web Page Testing** - HTML endpoint verification
- **Security Testing** - SQL injection, XSS, and other attack vectors
- **Authentication Testing** - Login and session management
- **Error Handling** - Invalid requests and edge cases

### **Running Tests**

```bash
cd test
npm install              # Install test dependencies
npm test                # Run complete test suite
```

### **Test Results Example**

```
API CREATE OPERATIONS:
Total Requests: 50
Success: 50 (100.00%)
Failed: 0
Average Response Time: 45.23 ms

SECURITY OPERATIONS:
Total Requests: 100
Success: 98 (98.00%)     # 98% of attacks properly rejected
Failed: 2               # 2% security concerns
```

---

## 📁 **Project Structure Deep Dive**

### **Controllers** (`controllers/`)

Demonstrate the framework's request handling capabilities:

- **controllers.hpp**: HTML page controllers using the HTML builder
- **api_controllers.hpp**: JSON API controllers using the JSON parser

### **Models** (`models/`)

Show data modeling and persistence:

- **Blog class**: Complete blog data model with file I/O
- **Static methods**: File-based database operations

### **Views** (`views/`)

Showcase dynamic HTML generation:

- **Template loading**: Reading HTML templates from files
- **Parameter substitution**: Dynamic content injection
- **Component composition**: Building complex pages

### **Routes** (`routes/`)

Demonstrate routing system:

- **Pattern matching**: URL patterns with parameters (`/blogs/:id`)
- **HTTP methods**: GET, POST, PUT, DELETE support
- **Middleware chains**: Authentication and request processing

### **Static Assets** (`static/`)

Show static file serving:

- **CSS files**: Modern responsive design
- **Automatic MIME types**: Content-type detection
- **Performance**: Efficient static file delivery

---

## 🔧 **Framework Capabilities Demonstrated**

### **1. HTTP Server**

```cpp
auto server = std::make_unique<hh_web::web_server<>>(8080);
server->use_static("static");
server->listen(callback);
```

### **2. Routing System**

```cpp
router->get("/blogs/:id", Route_V(get_single_blog_controller));
router->post("/api/blogs", Route_V(auth_middleware, create_blog_controller));
```

### **3. JSON Handling**

```cpp
auto parsed_json = parse(req->get_body());
std::string title = getter::get_string(parsed_json["title"]);
auto blog_json = blog_to_json(blog);
```

### **4. HTML Generation**

```cpp
auto article_elm = std::make_shared<element>("article");
article_elm->add_child(std::make_shared<element>("h2", blog.get_title()));
```

### **5. Middleware**

```cpp
hh_web::exit_code admin_auth(req, res) {
    // Authentication logic
    return authenticated ? exit_code::CONTINUE : exit_code::EXIT;
}
```

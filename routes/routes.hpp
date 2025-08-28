#pragma once
#include "../definentions.hpp"
#include "../library/web-lib.hpp"
#include "../controllers/controllers.hpp"
#include "../controllers/api_controllers.hpp"
#include "../middlewares/middlewares.hpp"
#include <memory>

std::shared_ptr<hh_web::web_router<>> make_views_router()
{
    auto router = std::make_shared<hh_web::web_router<>>();

    router->get("/", Route_V(index_controller));
    router->get("/blogs/:id", Route_V(get_single_blog_controller));
    router->get("/admin/login", Route_V(admin_login_controller));
    router->get("/admin/dashboard", Route_V(admin_auth, admin_dashboard_controller));
    router->get("/admin/blogs/:id/edit", Route_V(admin_auth, admin_edit_blog_controller));

    router->post("/admin/login", Route_V(admin_login_post_controller));

    router->post("/admin/blogs/create", Route_V(admin_auth, body_sanatizer_form_and_json, create_blog_controller));
    router->post("/admin/blogs/:id/edit", Route_V(admin_auth, body_sanatizer_form_and_json, update_blog_controller));
    router->delete_("/admin/blogs/:id/delete", Route_V(admin_auth, delete_blog_controller));

    return router;
}

std::shared_ptr<hh_web::web_router<>> make_apis_router()
{
    auto router = std::make_shared<hh_web::web_router<>>();

    router->get("/api/blogs", Route_V(api_get_all_blogs_controller));
    router->get("/api/blogs/:id", Route_V(api_get_single_blog_controller));

    router->post("/api/blogs", Route_V(api_auth_admin, body_sanatizer_form_and_json, api_create_blog_controller));
    router->put("/api/blogs/:id", Route_V(api_auth_admin, body_sanatizer_form_and_json,api_update_blog_controller));
    router->delete_("/api/blogs/:id", Route_V(api_auth_admin, api_delete_blog_controller));

    return router;
}

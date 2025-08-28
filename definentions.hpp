#pragma once
#ifndef CPP_PROJECT_SOURCE_DIR
#define CPP_PROJECT_SOURCE_DIR "/home/hamza/Documents/Learnings/Projects/simple-blog-from-scratch"
#endif

#define Route_V(...) \
    std::vector<hh_web::web_request_handler_t<>> { __VA_ARGS__ }
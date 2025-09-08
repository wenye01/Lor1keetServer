#pragma once

#include <string>
#include <string_view>

namespace netFrame
{
    struct request_meta
    {
        std::string http_header_;
        std::string_view method_;
        std::string_view url_;
        std::string_view version_;

        void reset()
        { }
    };
}



#pragma once

#include <cstdint>
#include <string_view>

#include "request_meta.h"

namespace netFrame
{
    enum class parse_state :std::uint8_t
    {
        valid,
        invalid,
        unknow
    };

    class http_parser
    {
    public:
        http_parser(std::string_view&& header);

        void parser(request_meta& meta);

    private:
        void parser_meta(request_meta& meta);



    private:
        std::string_view header_;
    };

}
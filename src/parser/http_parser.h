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
        static std::pair<parse_state, bool> parser(std::string_view&& header, request_meta& meta);

    private:
        static void parser_meta(request_meta& meta);
    };

}
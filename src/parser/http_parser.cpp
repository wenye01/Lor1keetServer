#include "http_parser.h"

#include <algorithm>
#include <string_view>
#include <functional>

#include <iostream>

namespace netFrame
{
    http_parser::http_parser(std::string_view&& header)
        :header_(std::move(header))
    {
       
    }

    void http_parser::parser(request_meta& meta)
    {
        std::string_view key("\r\n\r\n");

        auto it = std::search(header_.begin(), header_.end(), std::boyer_moore_searcher(key.begin(), key.end()));
        if (it != header_.end())
        {
            std::string_view request(header_.begin(), it + key.size());
            meta.http_header_ = request;
        }
        std::cout << "test meta data \n" << meta.http_header_ << std::endl;
    }

    void http_parser::parser_meta(request_meta& meta)
    {
//  GET / HTTP/1.1
//  Host: 127.0.0.1 : 8080
//  Connection : keep - alive
//  sec - ch - ua : "Not;A=Brand"; v = "99", "Google Chrome"; v = "139", "Chromium"; v = "139"
//  sec - ch - ua - mobile: ? 0
//  sec - ch - ua - platform : "Windows"
//  Upgrade - Insecure - Requests : 1
//  User - Agent : Mozilla / 5.0 (Windows NT 10.0; Win64; x64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 139.0.0.0 Safari / 537.36
//  Accept : text / html, application / xhtml + xml, application / xml; q = 0.9, image / avif, image / webp, image / apng, */*;q=0.8,application/signed-exchange;v=b3;q=0.7
//  Sec-Fetch-Site: none
//  Sec-Fetch-Mode: navigate
//  Sec-Fetch-User: ?1
//  Sec-Fetch-Dest: document
//  Accept-Encoding: gzip, deflate, br, zstd
//  Accept-Language: zh-CN,zh;q=0.9
        const std::string_view header = meta.http_header_;
        size_t start_line_end = header.find("\r\n");
        if (start_line_end == std::string_view::npos) {
            return;
        }
        std::string_view start_line = header.substr(0, start_line_end);

        // parse method
        size_t method_end = start_line.find(' ');
        if (method_end == std::string_view::npos) {
            return;
        }
        meta.method_ = start_line.substr(0, method_end);

        // parser url
        size_t url_start = method_end + 1;
        size_t url_end = start_line.find(' ', url_start);
        if (url_end == std::string_view::npos) {
            return;
        }
        meta.url_ = start_line.substr(url_start, url_end - url_start);

        // parser version
        size_t version_start = url_end + 1;
        if (version_start >= start_line.size()) {
            return;
        }
        meta.version_ = start_line.substr(version_start);
    }

}
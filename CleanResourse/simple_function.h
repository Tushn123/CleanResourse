#ifndef _SIMPLE_FUNCTION_H__
#define _SIMPLE_FUNCTION_H__

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include "Curl\curl.h"
#include "json\json.h"

#pragma comment(lib, "libcurl.lib")

namespace xml {
bool Xml2Json(const std::string& xml_str, Json::Value& json_data);
}  // namespace xml

namespace net {
std::string UrlEncode(const std::string& str);

namespace http {
bool Get(const std::string& url, std::string& response, CURLcode* p_ret = NULL,
         int* p_http_code = NULL);

bool Post(const std::string& url, const std::string& post_data,
          std::string& response, CURLcode* p_ret = NULL,
          int* p_http_code = NULL);
}  // namespace http
}  // namespace net

#endif  // _SIMPLE_FUNCTION_H__

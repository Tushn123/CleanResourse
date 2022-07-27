#include "stdafx.h"
#include "simple_function.h"
#include "xml/tinyxml2.h"

namespace xml {
void ConvertElement(tinyxml2::XMLElement* node, Json::Value& json_data,
                    bool root_is_array) {
    if (!node) {
    json_data = Json::Value::null;
    return;
  }

  const char* node_name = node->Name();
  tinyxml2::XMLElement* child_node = node->FirstChildElement();
  if (child_node) {
    // 比较前两个节点名称，判断其子节点是否为数组
    std::string child_node_name = child_node->Name();
    tinyxml2::XMLElement* next_node = child_node->NextSiblingElement();
    if (next_node) {
      bool is_array = false;
      if (child_node_name == next_node->Name()) {
        is_array = true;
      }

      unsigned int index = 0;
      tinyxml2::XMLElement* i_node = node->FirstChildElement();
      for (; i_node != NULL; i_node = i_node->NextSiblingElement()) {
        const char* i_node_name = i_node->Name();
        if (is_array) {
          ConvertElement(i_node, json_data[node_name][i_node_name][index++],
                         true);
        } else {
          if (root_is_array) {
            ConvertElement(i_node, json_data, false);
          } else {
            ConvertElement(i_node, json_data[node_name], false);
          }
        }
      }
    } else {
      // 该node只有一个子节点
      if (root_is_array) {
        ConvertElement(child_node, json_data, false);
      } else {
        ConvertElement(child_node, json_data[node_name], false);
      }
    }
  } else {
    // 独立节点，无子节点
    if (root_is_array) {
      json_data =
          (node->GetText() != NULL) ? node->GetText() : Json::Value::null;
    } else {
      json_data[node_name] =
          (node->GetText() != NULL) ? node->GetText() : Json::Value::null;
    }
  }
}

bool Xml2Json(const std::string& xml_str, Json::Value& json_data) {
  tinyxml2::XMLDocument doc;
  json_data = Json::Value::null;
  if (doc.Parse(xml_str.c_str()) != tinyxml2::XML_SUCCESS) {
    return false;
  }

  // 首节点不为数组
  ConvertElement(doc.RootElement(), json_data, false);

  ////TRACE(json_data.toStyledString().c_str());

 return true;
}

}  // namespace xml

namespace net {

std::string UrlEncode(const std::string& str) {
    std::string url_str = "";
    CURL* curl = curl_easy_init();
    char* p_out = curl_easy_escape(curl, str.c_str(), str.size());
    if (p_out == NULL) {
        return url_str;
    }

    url_str = p_out;
    curl_free(p_out);
    curl_easy_cleanup(curl);
    return url_str;
}

namespace http {
size_t WriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
  std::string* str = static_cast<std::string*>(userdata);
  if (NULL == str || NULL == ptr) {
    return -1;
  }

  str->append(ptr, size * nmemb);
  return nmemb;
}

bool Get(const std::string& url, std::string& response,
         CURLcode* p_ret /*  = NULL */, int* p_http_code /*  = NULL */) {
  CURL* hnd = curl_easy_init();
  std::string write_data;

  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
  curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());

  struct curl_slist* headers = NULL;
  headers = curl_slist_append(headers, "cache-control: no-cache");
  curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(hnd, CURLOPT_WRITEDATA, static_cast<void*>(&write_data));
  curl_easy_setopt(hnd, CURLOPT_NOSIGNAL, 1);
  curl_easy_setopt(hnd, CURLOPT_CONNECTTIMEOUT, 10);
  curl_easy_setopt(hnd, CURLOPT_TIMEOUT, 10);

  int http_code = 0;
  CURLcode ret = curl_easy_perform(hnd);
  if (ret == CURLE_OK) {
    ret = curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &http_code);
  }

  if (p_ret) *p_ret = ret;
  if (p_http_code) *p_http_code = http_code;

  bool result = false;
  if (ret == CURLE_OK && http_code == 200) {
    // url编码转化
    int out_len = 0;
    char* p_out = curl_easy_unescape(hnd, write_data.c_str(), write_data.size(),
                                     &out_len);
    response = std::string(p_out);
    curl_free(p_out);
    result = true;
  }
  curl_easy_cleanup(hnd);
  return result;
}

bool Post(const std::string& url, const std::string& post_data,
          std::string& response, CURLcode* p_ret /*  = NULL */,
          int* p_http_code /*  = NULL */) {
  CURL* hnd = curl_easy_init();
  std::string write_data;

  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());

  struct curl_slist* headers = NULL;
  headers = curl_slist_append(headers, "cache-control: no-cache");
  headers = curl_slist_append(headers, "Content-Type: application/json");
  curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, post_data.c_str());
  curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(hnd, CURLOPT_WRITEDATA, static_cast<void*>(&write_data));
  curl_easy_setopt(hnd, CURLOPT_NOSIGNAL, 1);
  curl_easy_setopt(hnd, CURLOPT_CONNECTTIMEOUT, 10);
  curl_easy_setopt(hnd, CURLOPT_TIMEOUT, 10);

  int http_code = 0;
  CURLcode ret = curl_easy_perform(hnd);
  if (ret == CURLE_OK) {
    ret = curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &http_code);
  }

  if (p_ret) *p_ret = ret;
  if (p_http_code) *p_http_code = http_code;

  bool result = false;
  if (ret == CURLE_OK && http_code == 200) {
    // url编码转化
    int out_len = 0;
    char* p_out = curl_easy_unescape(hnd, write_data.c_str(), write_data.size(),
                                     &out_len);
    response = std::string(p_out);
    curl_free(p_out);
    result = true;
  }
  curl_easy_cleanup(hnd);
  return result;
}

}  // namespace http
}  // namespace net
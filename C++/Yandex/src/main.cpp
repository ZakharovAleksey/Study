#include <time_logger.h>
#include <unit_test.h>

// using namespace std;
using namespace unit_test;
using namespace log_time;

#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

// Look through this one: https://pastebin.com/x1rV2hQn

struct HttpRequest
{
  std::string method, path, body;
  std::map<std::string, std::string> get_params;
};

std::pair<std::string, std::string> SplitBy(const std::string& what,
                                            const std::string& by)
{
  size_t pos = what.find(by);
  if (by.size() < what.size() && pos < what.size() - by.size())
  {
    return { what.substr(0, pos), what.substr(pos + by.size()) };
  } else
  {
    return { what, {} };
  }
}

template<typename T>
T FromString(const std::string& s)
{
  T x;
  std::istringstream is(s);
  is >> x;
  return x;
}

std::pair<size_t, std::string> ParseIdAndContent(const std::string& body)
{
  auto [id_string, content] = SplitBy(body, " ");
  return { FromString<size_t>(id_string), content };
}

struct LastCommentInfo
{
  size_t user_id, consecutive_count;
};

enum class HttpCode
{
  Ok = 200,
  NotFound = 404,
  Found = 302,
};

#define CodeToInt(httpCode) static_cast<int>(httpCode)

using Message = std::string;

class HttpResponse {
  public:
  explicit HttpResponse(HttpCode i_code) : d_code(i_code) {}

  HttpResponse& AddHeader(Message i_header, Message i_value)
  {
    d_headers.emplace_back(std::make_pair(i_header, i_value));
    return *this;
  }
  HttpResponse& SetContent(Message i_content)
  {
    d_content = i_content;
    return *this;
  }
  HttpResponse& SetCode(HttpCode i_code)
  {
    d_code = i_code;
    return *this;
  }

  friend ostream& operator<<(ostream& io_os, const HttpResponse& i_resp);

  private:
  HttpCode d_code{ HttpCode::NotFound };
  std::vector<std::pair<Message, Message>> d_headers;
  Message d_content;

  // HttpCode -> corresponding string
  std::unordered_map<HttpCode, std::string> HttpComment = {
    { HttpCode::Ok, "OK" },
    { HttpCode::NotFound, "Not found" },
    { HttpCode::Found, "Found" }
  };
};

std::ostream& operator<<(std::ostream& io_os,
                         const std::pair<Message, Message>& i_msg)
{
  io_os << i_msg.first << " : " << i_msg.second;
  return io_os;
}

std::ostream& operator<<(std::ostream& io_os, const HttpResponse& i_resp)
{
  // First line
  io_os << "HTTP/1.1 " << static_cast<int>(i_resp.d_code) << " "
        << i_resp.HttpComment.at(i_resp.d_code) << "\n";

  // Print all headers
  auto printHeader = [&io_os](const std::pair<Message, Message>& p) {
    io_os << p.first << ": " << p.second << "\n";
  };

  std::for_each(i_resp.d_headers.begin(), i_resp.d_headers.end(), printHeader);

  // Print skip line
  io_os << "\n";
  // Print content
  if (!i_resp.d_content.empty())
  {
    // io_os << "Content-Length: " << i_resp.d_content.size() << "\n";
    io_os << i_resp.d_content << "\n";
  }

  return io_os;
}

class CommentServer {
  private:
  vector<vector<string>> comments_;
  std::optional<LastCommentInfo> last_comment;
  unordered_set<size_t> banned_users;

  public:
  void ServeRequest(const HttpRequest& req, ostream& os)
  {
    HttpResponse httpResp(HttpCode::NotFound);
    if (req.method == "POST")
    {
      if (req.path == "/add_user")
      {
        comments_.emplace_back();
        auto response = to_string(comments_.size() - 1);

        httpResp.SetCode(HttpCode::Ok)
            .AddHeader("Content-Length", std::to_string(response.size()))
            .SetContent(response);

      } else if (req.path == "/add_comment")
      {
        auto [user_id, comment] = ParseIdAndContent(req.body);

        if (!last_comment || last_comment->user_id != user_id)
          last_comment = LastCommentInfo{ user_id, 1 };
        else if (++last_comment->consecutive_count > 3)
          banned_users.insert(user_id);

        if (banned_users.count(user_id) == 0)
        {
          comments_[user_id].push_back(string(comment));
          httpResp.SetCode(HttpCode::Ok);
        } else
        {
          httpResp.SetCode(HttpCode::Found).AddHeader("Location", "/captcha");
        }

      } else if (req.path == "/checkcaptcha")
      {
        if (auto [id, response] = ParseIdAndContent(req.body); response == "42")
        {
          banned_users.erase(id);
          if (last_comment && last_comment->user_id == id) last_comment.reset();
          httpResp.SetCode(HttpCode::Ok);
        } else
        {
          httpResp.SetCode(HttpCode::Found).AddHeader("Location", "/captcha");
        }
      } else
        httpResp.SetCode(HttpCode::NotFound);

    } else if (req.method == "GET")
    {
      if (req.path == "/user_comments")
      {
        auto user_id = FromString<size_t>(req.get_params.at("user_id"));
        string response;
        for (const string& c : comments_[user_id])
          response += c + '\n';

        httpResp.SetCode(HttpCode::Ok)
            .AddHeader("Content-Length", std::to_string(response.size()))
            .SetContent(response);

      } else if (req.path == "/captcha")
      {
        // Note: here was 80
        httpResp.SetCode(HttpCode::Ok)
            .AddHeader("Content-Length", "82")
            .SetContent(
                "What's the answer for The Ultimate Question of Life, the "
                "Universe, and Everything?");
      } else
        httpResp.SetCode(HttpCode::NotFound);
    }
    os << httpResp;
  }

  HttpResponse ServeRequest(const HttpRequest& req)
  {
    HttpResponse httpResp(HttpCode::NotFound);
    if (req.method == "POST")
    {
      if (req.path == "/add_user")
      {
        comments_.emplace_back();
        auto response = to_string(comments_.size() - 1);

        httpResp.SetCode(HttpCode::Ok)
            .AddHeader("Content-Length", std::to_string(response.size()))
            .SetContent(response);

      } else if (req.path == "/add_comment")
      {
        auto [user_id, comment] = ParseIdAndContent(req.body);

        if (!last_comment || last_comment->user_id != user_id)
          last_comment = LastCommentInfo{ user_id, 1 };
        else if (++last_comment->consecutive_count > 3)
          banned_users.insert(user_id);

        if (banned_users.count(user_id) == 0)
        {
          comments_[user_id].push_back(string(comment));
          httpResp.SetCode(HttpCode::Ok);
        } else
        {
          httpResp.SetCode(HttpCode::Found).AddHeader("Location", "/captcha");
        }

      } else if (req.path == "/checkcaptcha")
      {
        if (auto [id, response] = ParseIdAndContent(req.body); response == "42")
        {
          banned_users.erase(id);
          if (last_comment && last_comment->user_id == id) last_comment.reset();
          httpResp.SetCode(HttpCode::Ok);
        } else
        {
          httpResp.SetCode(HttpCode::Found).AddHeader("Location", "/captcha");
        }
      } else
        httpResp.SetCode(HttpCode::NotFound);

    } else if (req.method == "GET")
    {
      if (req.path == "/user_comments")
      {
        auto user_id = FromString<size_t>(req.get_params.at("user_id"));
        string response;
        for (const string& c : comments_[user_id])
          response += c + '\n';

        httpResp.SetCode(HttpCode::Ok)
            .AddHeader("Content-Length", std::to_string(response.size()))
            .SetContent(response);

      } else if (req.path == "/captcha")
      {
        // Note: here was 80
        httpResp.SetCode(HttpCode::Ok)
            .AddHeader("Content-Length", "82")
            .SetContent(
                "What's the answer for The Ultimate Question of Life, the "
                "Universe, and Everything?");
      } else
        httpResp.SetCode(HttpCode::NotFound);
    }
    return httpResp;
  }
};

struct HttpHeader
{
  string name, value;
};

ostream& operator<<(ostream& output, const HttpHeader& h)
{
  return output << h.name << ": " << h.value;
}

bool operator==(const HttpHeader& lhs, const HttpHeader& rhs)
{
  return lhs.name == rhs.name && lhs.value == rhs.value;
}

struct ParsedResponse
{
  int code;
  vector<HttpHeader> headers;
  string content;
};

istream& operator>>(istream& input, ParsedResponse& r)
{
  string line;
  getline(input, line);

  {
    istringstream code_input(line);
    string dummy;
    code_input >> dummy >> r.code;
  }

  size_t content_length = 0;

  r.headers.clear();
  while (getline(input, line) && !line.empty())
  {
    if (auto [name, value] = SplitBy(line, ": "); name == "Content-Length")
    {
      istringstream length_input(value);
      length_input >> content_length;
    } else
    {
      r.headers.push_back({ std::move(name), std::move(value) });
    }
  }

  r.content.resize(content_length);
  input.read(r.content.data(), r.content.size());
  return input;
}

#pragma region Tests

void Test(CommentServer& srv, const HttpRequest& request,
          const ParsedResponse& expected)
{
  stringstream ss;
  srv.ServeRequest(request, ss);
  ParsedResponse resp;
  ss >> resp;
  ASSERT_EQUAL(resp.code, expected.code);
  ASSERT_EQUAL(resp.headers, expected.headers);
  ASSERT_EQUAL(resp.content, expected.content);
}

template<typename CommentServer>
void TestServer()
{
  CommentServer cs;

  const ParsedResponse ok{ 200 };
  const ParsedResponse redirect_to_captcha{ 302,
                                            { { "Location", "/captcha" } },
                                            {} };
  const ParsedResponse not_found{ 404 };

  Test(cs, { "POST", "/add_user" }, { 200, {}, "0" });
  Test(cs, { "POST", "/add_user" }, { 200, {}, "1" });
  Test(cs, { "POST", "/add_comment", "0 Hello" }, ok);
  Test(cs, { "POST", "/add_comment", "1 Hi" }, ok);
  Test(cs, { "POST", "/add_comment", "1 Buy my goods" }, ok);
  Test(cs, { "POST", "/add_comment", "1 Enlarge" }, ok);
  Test(cs, { "POST", "/add_comment", "1 Buy my goods" }, redirect_to_captcha);
  Test(cs, { "POST", "/add_comment", "0 What are you selling?" }, ok);
  Test(cs, { "POST", "/add_comment", "1 Buy my goods" }, redirect_to_captcha);
  Test(cs, { "GET", "/user_comments", "", { { "user_id", "0" } } },
       { 200, {}, "Hello\nWhat are you selling?\n" });
  Test(cs, { "GET", "/user_comments", "", { { "user_id", "1" } } },
       { 200, {}, "Hi\nBuy my goods\nEnlarge\n" });
  Test(cs, { "GET", "/captcha" },
       { 200,
         {},
         { "What's the answer for The Ultimate Question of Life, the Universe, "
           "and Everything?" } });
  Test(cs, { "POST", "/checkcaptcha", "1 24" }, redirect_to_captcha);
  Test(cs, { "POST", "/checkcaptcha", "1 42" }, ok);
  Test(cs, { "POST", "/add_comment", "1 Sorry! No spam any more" }, ok);
  Test(cs, { "GET", "/user_comments", "", { { "user_id", "1" } } },
       { 200, {}, "Hi\nBuy my goods\nEnlarge\nSorry! No spam any more\n" });

  Test(cs, { "GET", "/user_commntes" }, not_found);
  Test(cs, { "POST", "/add_uesr" }, not_found);
}

#pragma endregion

int main()
{
  TestRunner tr;
  RUN_TEST(tr, TestServer<CommentServer>);
}

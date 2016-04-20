//
//  regex.cpp
//
//  Modified by 罗铮 on 04/18/19.
//  http://www.cplusplus.com/reference/regex/
//  http://www.onlamp.com/pub/a/onlamp/2006/04/06/boostregex.html?page=1

#include <iostream>
#include <string>
#include <regex>

int main ()
{
  const char cstr[] = "subject";
  std::string s ("subject");
  std::regex e ("(sub)(.*)");

  // test return value
  if (std::regex_match ("subject", std::regex("(sub)(.*)") ))
    std::cout << "string literal matched\n";

  if (std::regex_match (s,e))
    std::cout << "string object matched\n";

  // test cmatch and smatch
  std::cmatch cm;    // same as std::match_results<const char*> cm;
  std::regex_match (cstr,cm,e);
  std::cout << "string literal with " << cm.size() << " matches\n";

  std::smatch sm;    // same as std::match_results<string::const_iterator> sm;
  std::regex_match (s,sm,e);
  std::cout << "string object with " << sm.size() << " matches\n";

  // test value in cmatch and smatch
  std::regex_match ( s.cbegin(), s.cend(), sm, e);
  std::cout << "the matches in sm were: ";
  for (const auto &x:sm) {
    std::cout << "[" << x << "] ";
  }
  std::cout << std::endl;

  // using explicit flags:
  std::regex_match ( cstr, cm, e, std::regex_constants::match_default );
  std::cout << "the matches in cm were: ";
  for (const auto &x:cm) {
    std::cout << "[" << x << "] ";
  }
  std::cout << std::endl;

  // The hard-coded regular expression in Example contains lots of backslashes.
  // This is necessary because I am escaping certain characters twice: once for the compiler,
  // and once for the regular expression engine.
  std::string ss = "http://www.foo.com/bar";
  std::string re_raw     = "(ftp|http|https):\\/\\/((\\w+\\.)*(\\w*))\\/([\\w\\d]+\\/{0,1})+";
  std::cout << re_raw << std::endl;
  std::string re_literal = R"((ftp|http|https):\/\/((\w+\.)*(\w*))\/([\w\d]+\/{0,1})+)";
  std::cout << re_literal << std::endl;
  std::regex re(re_literal);

  std::cmatch matches;
  if (regex_match(ss.c_str(), matches, re))
  {
     // matches[0] contains the original string.  matches[n]
     // contains a sub_match object for each matching
     // subexpression
     for (size_t i = 1; i < matches.size(); i++)
     {
        // sub_match::first and sub_match::second are iterators that
        // refer to the first and one past the last chars of the
        // matching subexpression
        std::string match(matches[i].first, matches[i].second);
        std::cout << "\tmatches[" << i << "] = " << match << std::endl;
     }
  }

  return 0;
}

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

  return 0;
}
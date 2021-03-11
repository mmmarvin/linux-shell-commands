#include "path.h"

#include <pwd.h>
#include <unistd.h>
using namespace std;

string combine_path(string p1, string p2)
{
  if(p1.empty()) {
    return p2;
  }

  if(p2.empty()) {
    return p1;
  }

  if(p1.back() == '/') {
    p1.pop_back();
  }

  if(p2[0] == '/') {
    if(p2.size() > 1) {
      p2 = p2.substr(1, p2.size() - 1);
    } else {
      p2 = string();
    }
  }

  return p1 + "/" + p2;
}

string get_templates_path()
{
  return combine_path(get_home_path(), "Templates");
}

string get_home_path()
{
  return getpwuid(getuid())->pw_dir;
}

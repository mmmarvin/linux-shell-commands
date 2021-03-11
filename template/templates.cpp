#include "templates.h"

#include <fstream>
#include <dirent.h>
#include "path.h"
using namespace std;

void create_template(const std::string& template_filename, const std::string& output_filename)
{
  ifstream in(combine_path(get_templates_path(), template_filename), ios_base::in);
  ofstream out(output_filename, ios_base::out);

  if(in.is_open() && out.is_open()) {
    char buff[255];
    in.read(&buff[0], 255);
    streamsize r = in.gcount();
    while(r) {
      out.write(&buff[0], r);

      in.read(&buff[0], 255);
      r = in.gcount();
    }
  }
}

vector<string> get_templates()
{
  vector<string> ret;

  DIR*    d = opendir(get_templates_path().c_str());
  dirent* e;

  if(d) {
    while((e = readdir(d)) != nullptr) {
      string file = e->d_name;
      if(file != ".." && file != ".") {
        ret.push_back(e->d_name);
      }
    }
  }

  return ret;
}

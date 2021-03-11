#include <iostream>
#include "templates.h"
using namespace std;

int main(int argc, char** argv)
{
  vector<string> templates = get_templates();
  if(argc == 2) {
    string template_chosen = argv[1];
    for(vector<string>::iterator it = templates.begin(), it_end = templates.end();
        it != it_end;
        ++it) {
      if(*it == template_chosen) {
        create_template(template_chosen, template_chosen);
        break;
      }
    }
  } else if(argc == 3) {
    string template_chosen = argv[1];
    string output_template_chosen = argv[2];
    for(vector<string>::iterator it = templates.begin(), it_end = templates.end();
        it != it_end;
        ++it) {
      if(*it == template_chosen) {
        create_template(template_chosen, output_template_chosen);
        break;
      }
    }
  }

  return EXIT_SUCCESS;
}

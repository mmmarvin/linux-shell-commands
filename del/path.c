/**********
 *
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 *
 **********/
#include "path.h"

#include <sys/stat.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

size_t path_len(const path_t* p)
{
  assert(p);
  return strnlen(&p->value[0], PATH_MAX * 2);
}

const char* get_path_str(const path_t* p)
{
  return &p->value[0];
}

bool is_dir(const path_t* p)
{
  struct stat ps;
  stat(&p->value[0], &ps);

  return S_ISDIR(ps.st_mode);
}

path_t create_path(const char* p)
{
  path_t ret;
  strcpy(&ret.value[0], p);
  return ret;
}

path_t copy_path(const path_t* p)
{
  path_t ret;
  strcpy(&ret.value[0], &p->value[0]);

  return ret;
}

path_t combine_path(const path_t* p1, const path_t* p2)
{
  path_t v1, v2, ret;
  strcpy(&v1.value[0], &p1->value[0]);
  strcpy(&v2.value[0], &p2->value[0]);

  if(!path_len(&v1)) {
    strcpy(&ret.value[0], &v2.value[0]);
    return ret;
  }

  if(!path_len(&v2)) {
    strcpy(&ret.value[0], &v1.value[0]);
    return ret;
  }

  if(path_len(&v1) >= PATH_MAX) {
    strncpy(&ret.value[0], &v1.value[0], PATH_MAX);
    ret.value[PATH_MAX] = '\0';
    return ret;
  }

  if(v1.value[path_len(&v1)] == '/') {
    v1.value[path_len(&v1)] = '\0';
  }

  if(v2.value[0] == '/') {
    // shift left
    for(size_t i = 0, isize = path_len(&v2) - 1; i < isize; ++i) {
      v2.value[i] = v2.value[i + 1];
    }
  }
  size_t combined_len = path_len(&v1) + path_len(&v2);
  if(combined_len >= PATH_MAX) {
    size_t diff = combined_len - PATH_MAX - 1;
    strcpy(&ret.value[0], &v1.value[0]);
    ret.value[path_len(&v1)] = '/';
    memcpy(&ret.value[0] + path_len(&v1) + 1, &v2.value[0], diff);
    ret.value[PATH_MAX] = '\0';
  } else {
    strcpy(&ret.value[0], &v1.value[0]);
    ret.value[path_len(&v1)] = '/';
    strcpy(&ret.value[0] + path_len(&v1) + 1, &v2.value[0]);
    ret.value[combined_len + 1] = '\0';
  }

  return ret;
}

path_t combine_path_string(const path_t* p1, const char* p2)
{
  path_t ret;

  if(!path_len(p1)) {
    strcpy(&ret.value[0], p2);
    return ret;
  }

  if(!strnlen(p2, PATH_MAX)) {
    strcpy(&ret.value[0], &p1->value[0]);
    return ret;
  }

  size_t combined_len = path_len(p1) + strnlen(p2, PATH_MAX);
  if(combined_len >= PATH_MAX) {
    size_t diff = combined_len - PATH_MAX;
    strcpy(&ret.value[0], &p1->value[0]);
    memcpy(&ret.value[0] + path_len(&p1), p2, sizeof(char) * strlen(p2));
    ret.value[PATH_MAX] = '\0';
  } else {
    strcpy(&ret.value[0], &p1->value[0]);
    strcpy(&ret.value[0] + path_len(p1), p2);
    ret.value[combined_len + 1] = '\0';
  }

  return ret;
}

path_t remove_extension(const path_t* p)
{
  path_t ret;
  size_t len = path_len(p);

  if(len) {
    while(len) {
      char c = p->value[len];
      if(c == '.') {
        memcpy(&ret.value[0], &p->value[0], sizeof(char) * len);
        ret.value[len] = '\0';
        return ret;
      } else {
        --len;
      }

      // if p == ".file", then . on index 0 is not an extension
      // so return no extension at this point
    }
  }

  ret.value[0] = '\0';
  return ret;
}

path_t get_root_path(const path_t* p)
{
  path_t ret;
  size_t len = path_len(p);

  if(len) {
    while(len) {
      char c = p->value[len];
      if(c == '/') {
        memcpy(&ret.value[0], &p->value[0], len);
        ret.value[len] = '\0';
        return ret;
      } else {
        --len;
      }
    }

    if(p->value[0] == '/') {
      strcpy(&ret.value[0], "/");
      return ret;
    }

    strcpy(&ret.value[0], ".");
    return ret;
  }

  ret.value[0] = '\0';
  return ret;
}

path_t get_filename(const path_t* p)
{
  path_t ret;
  size_t len = path_len(p);

  if(len) {
    while(len) {
      char c = p->value[len];
      if(c == '/') {
        memcpy(&ret.value[0], &p->value[0] + len + 1, sizeof(char) * (path_len(p) - len - 1));
        ret.value[(path_len(p) - len - 1)] = '\0';
        return ret;
      } else {
        --len;
      }
    }

    if(p->value[0] == '/') {
      memcpy(&ret.value[0], &p->value[0] + 1, sizeof(char) * (path_len(p) - 1));
      ret.value[path_len(p) - 1] = '\0';
      return ret;
    }

    strcpy(&ret.value[0], &p->value[0]);
    return ret;
  }

  ret.value[0] = '\0';
  return ret;
}

path_t get_file_extension(const path_t* p)
{
  path_t ret;
  size_t len = path_len(p);

  if(len) {
    while(len) {
      char c = p->value[len];
      if(c == '.') {
        memcpy(&ret.value[0], &p->value[0] + len + 1, sizeof(char) * (path_len(p) - len - 1));
        ret.value[(path_len(p) - len - 1)] = '\0';
        return ret;
      } else {
        --len;
      }

      // if p == ".file", then . on index 0 is not an extension
      // so return no extension at this point
    }
  }

  ret.value[0] = '\0';
  return ret;
}

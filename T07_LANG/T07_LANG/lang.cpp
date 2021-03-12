#include "lang.h"

int mk5::frontend::read_file(const char* name)
{
  char file_name[MAX_STR] = "";
  strcat(file_name, "..\\PROGRAMS\\");
  strcat(file_name, name);
  FILE *file = fopen(file_name, "rb");
  if (file == nullptr)
    return 0;

  fseek(file, 0, SEEK_END);
  int buf_len = ftell(file) + 1;

  rewind(file);

  buf = new char[buf_len];
  fread(buf, sizeof(char), buf_len - 1, file);

  fclose(file);
  buf[buf_len - 1] = 0;

  tok = new token[buf_len];
  int pos_count = 0, str_count = 0;

  for (int i = 0; i < buf_len; i++) {

    if (isspace(buf[i])) {
      pos_count++;

      if (buf[i] == '\r')
      {
        i++;
        pos_count = 0;
        str_count++;
      }
      continue;
    }

    if (isdigit(buf[i])) {
      int add_pos = 0;
      double num = NAN;
      sscanf(buf + i, "%lg%n", &num, &add_pos);
      i += add_pos - 1;
      tok[tok_size++] = token(num, str_count + 1, pos_count + 1);

      pos_count += add_pos;

      continue;
    }

    if (isalpha(buf[i])) {
      int new_pos = 0;

      do
      {
        new_pos++;
      } while (isalpha((unsigned char)buf[i + new_pos]) || isdigit(buf[i + new_pos]));

      tok[tok_size++] = token(buf + i, str_count + 1, pos_count + 1, TOK_WORD, new_pos);
      pos_count += new_pos;
      i += new_pos - 1;

      continue;
    }

    tok[tok_size++] = token(buf + i, str_count + 1, pos_count + 1, TOK_SYM, 1);
    pos_count++;
  }

  tok[tok_size++] = token(nullptr, str_count + 1, pos_count + 1, TOK_NUL, buf_len);

  return 1;
}
char *strtok(char *str, const char *delim) {
  static char *last = NULL;
  char *token_start;

  if (str != NULL) {
    last = str;
  }
  if (last == NULL || *last == '\0') {
    return NULL;
  }
  while (*last && strchr(delim, *last)) {
    last++;
  }

  if (*last == '\0') {
    return NULL;
  }

  token_start = last;

  while (*last && !strchr(delim, *last)) {
    last++;
  }

  if (*last) {
    *last = '\0';
    last++;
  }
  return token_start;
}
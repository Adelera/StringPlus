#include <math.h>
#include <stdarg.h>
#include <stdint.h>

#include "s21_string.h"

typedef struct {
  int minus;
  int plus;
  int space;
  int hash;
  int zero;

  int width;
  int precision;
  int is_precision_set;

  char length;
} FormatSpec;

typedef struct {
  int len;
  int zeros;
  int spaces;
  char sign;
} IntSizes;

static void parse_flags(const char **format, FormatSpec *spec);
static void parse_width(const char **format, va_list args, FormatSpec *spec);
static void parse_precision(const char **format, va_list args,
                            FormatSpec *spec);
static void parse_length(const char **format, FormatSpec *spec);
static void parse_specifier(const char **format, char **str, va_list args,
                            FormatSpec *spec);
static void handle_char(char **str, va_list args, FormatSpec *spec);
static void handle_string(char **str, va_list args, FormatSpec *spec);
static void handle_int(char **str, va_list args, FormatSpec *spec);
static void handle_unsigned(char **str, va_list args, FormatSpec *spec,
                            int base, int uppercase);
static void handle_pointer(char **str, va_list args, FormatSpec *spec);
static void handle_float(char **str, va_list args, FormatSpec *spec);
static void handle_scientific(char **str, va_list args, FormatSpec *spec,
                              int uppercase);
static void handle_general(char **str, va_list args, FormatSpec *spec,
                           int uppercase);
static void read_int_arg(va_list args, FormatSpec *spec, long long *num);
static void read_uint_arg(va_list args, FormatSpec *spec,
                          unsigned long long *num);
static void read_float_arg(va_list args, FormatSpec *spec, long double *num);
static int int_to_str(unsigned long long num, char *out, int base,
                      int uppercase);
static void calc_int_sizes(long long num, FormatSpec *spec, IntSizes *sizes);
static void calc_uint_sizes(unsigned long long num, FormatSpec *spec,
                            IntSizes *sizes, int base);
static char get_float_sign(long double num, FormatSpec *spec);
static int normalize_float_precision(FormatSpec *spec, int is_general);
static int format_float_special(long double num, int uppercase, char *out);
static long double pow10_ld(int precision);
static int int_part_to_str(long double num, char *out, int out_cap);
static int format_fixed_abs(long double num, int precision, int hash, char *out,
                            int out_cap);
static int format_scientific_abs(long double num, int precision, int hash,
                                 int uppercase, char *out, int out_cap);
static int get_decimal_exponent(long double num);
static int get_rounded_exponent_for_g(long double num, int precision);
static int append_exponent(char *out, int pos, int exponent, int uppercase,
                           int out_cap);
static int trim_trailing_zeros(char *out, int len);
static void write_number_with_padding(char **str, FormatSpec *spec, char sign,
                                      const char *num_buf, int num_len);
static void put_uint_prefix(char **str, int has_prefix, int base, int uppercase,
                            int zeros);
static void put_char(char **str, char c);

int s21_sprintf(char *str, const char *format, ...) {
  va_list args;
  va_start(args, format);

  char *start = str;

  while (*format) {
    if (*format == '%') {
      format++;

      FormatSpec spec = {0};
      spec.precision = 1;
      spec.is_precision_set = 0;

      parse_flags(&format, &spec);
      parse_width(&format, args, &spec);
      parse_precision(&format, args, &spec);
      parse_length(&format, &spec);

      parse_specifier(&format, &str, args, &spec);
    } else {
      put_char(&str, *format);
      format++;
    }
  }

  put_char(&str, '\0');
  va_end(args);

  return str - start - 1;
}

static void parse_flags(const char **format, FormatSpec *spec) {
  while (1) {
    if (**format == '-') {
      spec->minus = 1;
      (*format)++;
    } else if (**format == '+') {
      spec->plus = 1;
      (*format)++;
    } else if (**format == ' ') {
      spec->space = 1;
      (*format)++;
    } else if (**format == '0') {
      spec->zero = 1;
      (*format)++;
    } else if (**format == '#') {
      spec->hash = 1;
      (*format)++;
    } else {
      break;
    }
  }
}

static void parse_width(const char **format, va_list args, FormatSpec *spec) {
  if (**format == '*') {
    spec->width = va_arg(args, int);
    if (spec->width < 0) {
      spec->minus = 1;
      spec->width = -spec->width;
    }
    (*format)++;
  } else if (**format >= '0' && **format <= '9') {
    spec->width = 0;
    while (**format >= '0' && **format <= '9') {
      spec->width = spec->width * 10 + (**format - '0');
      (*format)++;
    }
  }
}

static void parse_precision(const char **format, va_list args,
                            FormatSpec *spec) {
  if (**format == '.') {
    spec->is_precision_set = 1;
    spec->precision = 0;
    (*format)++;

    if (**format == '*') {
      spec->precision = va_arg(args, int);
      if (spec->precision < 0) {
        spec->is_precision_set = 0;
        spec->precision = 0;
      }
      (*format)++;
    } else if (**format >= '0' && **format <= '9') {
      while (**format >= '0' && **format <= '9') {
        spec->precision = spec->precision * 10 + (**format - '0');
        (*format)++;
      }
    }
  }
}

static void parse_length(const char **format, FormatSpec *spec) {
  if (**format == 'h') {
    spec->length = 'h';
    (*format)++;
  } else if (**format == 'l') {
    spec->length = 'l';
    (*format)++;
  } else if (**format == 'L') {
    spec->length = 'L';
    (*format)++;
  }
}

static void parse_specifier(const char **format, char **str, va_list args,
                            FormatSpec *spec) {
  if (**format == '\0') {
    put_char(str, '%');
    return;
  }

  switch (**format) {
    case 'c':
      handle_char(str, args, spec);
      break;
    case 's':
      handle_string(str, args, spec);
      break;
    case 'd':
    case 'i':
      handle_int(str, args, spec);
      break;
    case 'u':
      handle_unsigned(str, args, spec, 10, 0);
      break;
    case 'o':
      handle_unsigned(str, args, spec, 8, 0);
      break;
    case 'x':
      handle_unsigned(str, args, spec, 16, 0);
      break;
    case 'X':
      handle_unsigned(str, args, spec, 16, 1);
      break;
    case 'p':
      handle_pointer(str, args, spec);
      break;
    case 'f':
      handle_float(str, args, spec);
      break;
    case 'e':
      handle_scientific(str, args, spec, 0);
      break;
    case 'E':
      handle_scientific(str, args, spec, 1);
      break;
    case 'g':
      handle_general(str, args, spec, 0);
      break;
    case 'G':
      handle_general(str, args, spec, 1);
      break;
    case '%':
      put_char(str, '%');
      break;
    default:
      // неизвестный спецификатор — просто копируем как есть
      put_char(str, '%');
      put_char(str, **format);
      break;
  }

  (*format)++;
}

static void handle_char(char **str, va_list args, FormatSpec *spec) {
  char c = (char)va_arg(args, int);

  if (spec->width > 1) {
    if (spec->minus == 1) {
      put_char(str, c);
      s21_memset(*str, ' ', spec->width - 1);
      *str += spec->width - 1;
    } else {
      s21_memset(*str, ' ', spec->width - 1);
      *str += spec->width - 1;
      put_char(str, c);
    }
  } else {
    put_char(str, c);
  }
}

static void handle_string(char **str, va_list args, FormatSpec *spec) {
  char *s = va_arg(args, char *);
  if (s == S21_NULL) s = "(null)";

  s21_size_t len_s = s21_strlen(s);

  if (spec->is_precision_set == 1 && spec->precision < (int)len_s) {
    len_s = spec->precision;
  }

  if (spec->width > (int)len_s) {
    if (spec->minus == 1) {
      s21_strncpy(*str, s, len_s);
      *str += len_s;
      s21_memset(*str, ' ', spec->width - len_s);
      *str += spec->width - len_s;
    } else {
      s21_memset(*str, ' ', spec->width - len_s);
      *str += spec->width - len_s;
      s21_strncpy(*str, s, len_s);
      *str += len_s;
    }
  } else {
    s21_strncpy(*str, s, len_s);
    *str += len_s;
  }
}

// Обработка целого числа со знаком
static void handle_int(char **str, va_list args, FormatSpec *spec) {
  long long num = 0;
  read_int_arg(args, spec, &num);

  IntSizes sizes;
  char buf[64];
  unsigned long long abs_num = (num < 0)
                                   ? (unsigned long long)(-(num + 1)) + 1ULL
                                   : (unsigned long long)num;

  sizes.len = int_to_str(abs_num, buf, 10, 0);
  if (num == 0 && spec->is_precision_set && spec->precision == 0) {
    sizes.len = 0;
  }

  calc_int_sizes(num, spec, &sizes);

  int use_zero = (spec->zero && !spec->minus && !spec->is_precision_set);

  if (spec->minus == 0) {
    if (use_zero) {
      if (sizes.sign) put_char(str, sizes.sign);
      s21_memset(*str, '0', sizes.spaces);
      *str += sizes.spaces;
    } else {
      s21_memset(*str, ' ', sizes.spaces);
      *str += sizes.spaces;
      if (sizes.sign) put_char(str, sizes.sign);
    }
    s21_memset(*str, '0', sizes.zeros);
    *str += sizes.zeros;
  } else {
    if (sizes.sign) put_char(str, sizes.sign);
    s21_memset(*str, '0', sizes.zeros);
    *str += sizes.zeros;
  }

  for (int i = 0; i < sizes.len; i++) put_char(str, buf[i]);

  if (spec->minus) {
    s21_memset(*str, ' ', sizes.spaces);
    *str += sizes.spaces;
  }
}

// Обработка беззнакового числа
static void handle_unsigned(char **str, va_list args, FormatSpec *spec,
                            int base, int uppercase) {
  unsigned long long num = 0;
  read_uint_arg(args, spec, &num);

  IntSizes sizes;
  char buf[64];

  sizes.len = int_to_str(num, buf, base, uppercase);
  if (num == 0 && spec->is_precision_set && spec->precision == 0) {
    sizes.len = 0;
  }

  calc_uint_sizes(num, spec, &sizes, base);
  int has_prefix = 0;
  if (spec->hash && base == 8) {
    has_prefix = (num != 0 && sizes.zeros == 0) || sizes.len == 0;
  } else if (spec->hash && base == 16 && num != 0) {
    has_prefix = 1;
  }

  int use_zero = (spec->zero && !spec->minus && !spec->is_precision_set);

  if (spec->minus == 0) {
    if (use_zero) {
      put_uint_prefix(str, has_prefix, base, uppercase, sizes.zeros);
      s21_memset(*str, '0', sizes.spaces);
      *str += sizes.spaces;
    } else {
      s21_memset(*str, ' ', sizes.spaces);
      *str += sizes.spaces;
      put_uint_prefix(str, has_prefix, base, uppercase, sizes.zeros);
    }
    s21_memset(*str, '0', sizes.zeros);
    *str += sizes.zeros;
  } else {
    put_uint_prefix(str, has_prefix, base, uppercase, sizes.zeros);
    s21_memset(*str, '0', sizes.zeros);
    *str += sizes.zeros;
  }

  for (int i = 0; i < sizes.len; i++) put_char(str, buf[i]);

  if (spec->minus) {
    s21_memset(*str, ' ', sizes.spaces);
    *str += sizes.spaces;
  }
}

static void handle_float(char **str, va_list args, FormatSpec *spec) {
  long double num = 0.0L;
  read_float_arg(args, spec, &num);

  char sign = get_float_sign(num, spec);
  long double abs_num = fabsl(num);
  char num_buf[1024];
  int num_len = format_float_special(num, 0, num_buf);

  if (num_len == 0) {
    int precision = normalize_float_precision(spec, 0);
    num_len = format_fixed_abs(abs_num, precision, spec->hash, num_buf,
                               (int)sizeof(num_buf));
  }

  write_number_with_padding(str, spec, sign, num_buf, num_len);
}

static void handle_scientific(char **str, va_list args, FormatSpec *spec,
                              int uppercase) {
  long double num = 0.0L;
  read_float_arg(args, spec, &num);

  char sign = get_float_sign(num, spec);
  long double abs_num = fabsl(num);
  char num_buf[1024];
  int num_len = format_float_special(num, uppercase, num_buf);

  if (num_len == 0) {
    int precision = normalize_float_precision(spec, 0);
    num_len = format_scientific_abs(abs_num, precision, spec->hash, uppercase,
                                    num_buf, (int)sizeof(num_buf));
  }

  write_number_with_padding(str, spec, sign, num_buf, num_len);
}

static void handle_general(char **str, va_list args, FormatSpec *spec,
                           int uppercase) {
  long double num = 0.0L;
  read_float_arg(args, spec, &num);

  char sign = get_float_sign(num, spec);
  long double abs_num = fabsl(num);
  char num_buf[1024];
  int num_len = format_float_special(num, uppercase, num_buf);

  if (num_len == 0) {
    int precision = normalize_float_precision(spec, 1);

    int exponent =
        (abs_num == 0.0L) ? 0 : get_rounded_exponent_for_g(abs_num, precision);
    int use_scientific = (exponent < -4 || exponent >= precision);

    if (use_scientific) {
      int sci_precision = precision - 1;
      if (sci_precision < 0) sci_precision = 0;
      num_len = format_scientific_abs(abs_num, sci_precision, spec->hash,
                                      uppercase, num_buf, (int)sizeof(num_buf));
    } else {
      int frac_precision = precision - (exponent + 1);
      if (frac_precision < 0) frac_precision = 0;
      num_len = format_fixed_abs(abs_num, frac_precision, spec->hash, num_buf,
                                 (int)sizeof(num_buf));
    }

    if (!spec->hash) {
      num_len = trim_trailing_zeros(num_buf, num_len);
    }
  }

  write_number_with_padding(str, spec, sign, num_buf, num_len);
}

static void handle_pointer(char **str, va_list args, FormatSpec *spec) {
  void *ptr = va_arg(args, void *);
  char buf[128];
  int len = 0;

  if (ptr == S21_NULL) {
    const char *nil = "(nil)";
    for (int i = 0; nil[i] != '\0'; i++) buf[len++] = nil[i];
  } else {
    uintptr_t addr = (uintptr_t)ptr;
    char hex_buf[64];
    int hex_len = int_to_str((unsigned long long)addr, hex_buf, 16, 0);

    buf[len++] = '0';
    buf[len++] = 'x';
    for (int i = 0; i < hex_len; i++) buf[len++] = hex_buf[i];
  }

  int spaces = (spec->width > len) ? spec->width - len : 0;
  int use_zero = (spec->zero && !spec->minus && ptr != S21_NULL);

  if (!spec->minus) {
    if (use_zero) {
      put_char(str, '0');
      put_char(str, 'x');
      s21_memset(*str, '0', spaces);
      *str += spaces;
      for (int i = 2; i < len; i++) put_char(str, buf[i]);
    } else {
      s21_memset(*str, ' ', spaces);
      *str += spaces;
      for (int i = 0; i < len; i++) put_char(str, buf[i]);
    }
  } else {
    for (int i = 0; i < len; i++) put_char(str, buf[i]);
    s21_memset(*str, ' ', spaces);
    *str += spaces;
  }
}

// Обработка аргумента длины для int
static void read_int_arg(va_list args, FormatSpec *spec, long long *num) {
  switch (spec->length) {
    case 'h':
      *num = (short)va_arg(args, int);
      break;

    case 'l':
      *num = va_arg(args, long);
      break;

    default:
      *num = va_arg(args, int);
      break;
  }
}

// Обработка аргумента длины для беззнакового числа
static void read_uint_arg(va_list args, FormatSpec *spec,
                          unsigned long long *num) {
  switch (spec->length) {
    case 'h':
      *num = (unsigned short)va_arg(args, int);
      break;

    case 'l':
      *num = va_arg(args, unsigned long);
      break;

    default:
      *num = va_arg(args, unsigned int);
      break;
  }
}

static void read_float_arg(va_list args, FormatSpec *spec, long double *num) {
  if (spec->length == 'L') {
    *num = va_arg(args, long double);
  } else {
    *num = va_arg(args, double);
  }
}

static char get_float_sign(long double num, FormatSpec *spec) {
  char sign = '\0';
  if (signbit(num)) {
    sign = '-';
  } else if (spec->plus) {
    sign = '+';
  } else if (spec->space) {
    sign = ' ';
  }
  return sign;
}

static int normalize_float_precision(FormatSpec *spec, int is_general) {
  int precision = spec->is_precision_set ? spec->precision : 6;
  if (is_general && precision == 0) precision = 1;
  if (precision < 0) precision = is_general ? 1 : 0;
  if (precision > 512) precision = 512;
  return precision;
}

static int format_float_special(long double num, int uppercase, char *out) {
  if (isnan(num)) {
    out[0] = uppercase ? 'N' : 'n';
    out[1] = uppercase ? 'A' : 'a';
    out[2] = uppercase ? 'N' : 'n';
    return 3;
  }
  if (isinf(num)) {
    out[0] = uppercase ? 'I' : 'i';
    out[1] = uppercase ? 'N' : 'n';
    out[2] = uppercase ? 'F' : 'f';
    return 3;
  }
  return 0;
}

static long double pow10_ld(int precision) {
  long double pow10 = 1.0L;
  for (int i = 0; i < precision; i++) pow10 *= 10.0L;
  return pow10;
}

static int int_part_to_str(long double num, char *out, int out_cap) {
  if (out_cap <= 0) return 0;
  if (num < 1.0L) {
    out[0] = '0';
    return 1;
  }

  char temp[1024];
  int len = 0;
  while (num >= 1.0L && len < (int)sizeof(temp)) {
    long double div = floorl(num / 10.0L);
    int digit = (int)(num - div * 10.0L);
    if (digit < 0) digit = 0;
    if (digit > 9) digit = 9;
    temp[len++] = (char)('0' + digit);
    num = div;
  }

  int out_len = (len < out_cap) ? len : out_cap;
  for (int i = 0; i < out_len; i++) out[i] = temp[len - 1 - i];
  return out_len;
}

static int format_fixed_abs(long double num, int precision, int hash, char *out,
                            int out_cap) {
  if (out_cap <= 0) return 0;
  if (precision < 0) precision = 0;

  int pos = 0;
  long double int_part_ld = floorl(num);
  long double frac_part = num - int_part_ld;
  long double scale = pow10_ld(precision);
  long double rounded_frac = rintl(frac_part * scale);

  if (rounded_frac >= scale && precision > 0) {
    int_part_ld += 1.0L;
    rounded_frac = 0.0L;
  } else if (precision == 0 && rounded_frac >= 1.0L) {
    int_part_ld += 1.0L;
    rounded_frac = 0.0L;
  }

  char int_buf[1024];
  int int_len = int_part_to_str(int_part_ld, int_buf, (int)sizeof(int_buf));
  for (int i = 0; i < int_len && pos < out_cap; i++) out[pos++] = int_buf[i];

  if ((precision > 0 || hash) && pos < out_cap) out[pos++] = '.';

  if (precision > 0) {
    char frac_buf[1024];
    int frac_len =
        int_part_to_str(rounded_frac, frac_buf, (int)sizeof(frac_buf));
    int leading_zeros = precision - frac_len;
    if (rounded_frac < 1.0L) leading_zeros = precision;

    for (int i = 0; i < leading_zeros && pos < out_cap; i++) out[pos++] = '0';
    if (rounded_frac >= 1.0L) {
      for (int i = 0; i < frac_len && pos < out_cap; i++)
        out[pos++] = frac_buf[i];
    }
  }

  return pos;
}

static int format_scientific_abs(long double num, int precision, int hash,
                                 int uppercase, char *out, int out_cap) {
  if (out_cap <= 0) return 0;
  if (precision < 0) precision = 0;

  int exponent = 0;
  long double mantissa = 0.0L;
  if (num != 0.0L) {
    exponent = get_decimal_exponent(num);
    mantissa = num / powl(10.0L, exponent);

    while (mantissa >= 10.0L) {
      mantissa /= 10.0L;
      exponent++;
    }
    while (mantissa < 1.0L) {
      mantissa *= 10.0L;
      exponent--;
    }
  }

  int pos = 0;
  long double scale = pow10_ld(precision);
  long double rounded = rintl(mantissa * scale);
  long double limit = 10.0L * scale;
  if (rounded >= limit) {
    rounded /= 10.0L;
    exponent++;
  }

  char digits[1024];
  int digits_len = int_part_to_str(rounded, digits, (int)sizeof(digits));
  int required = precision + 1;
  int pad = required - digits_len;

  if (pad > 0) {
    if (pos < out_cap) out[pos++] = '0';
    if ((precision > 0 || hash) && pos < out_cap) out[pos++] = '.';
    for (int i = 0; i < precision && pos < out_cap; i++) out[pos++] = '0';
  } else {
    if (digits_len > 0 && pos < out_cap) out[pos++] = digits[0];
    if ((precision > 0 || hash) && pos < out_cap) out[pos++] = '.';

    for (int i = 1; i < digits_len && i <= precision && pos < out_cap; i++) {
      out[pos++] = digits[i];
    }
    for (int i = digits_len - 1; i < precision && pos < out_cap; i++) {
      out[pos++] = '0';
    }
  }

  pos = append_exponent(out, pos, exponent, uppercase, out_cap);
  return pos;
}

static int get_decimal_exponent(long double num) {
  int exp = 0;
  if (num == 0.0L) return 0;

  while (num >= 10.0L) {
    num /= 10.0L;
    exp++;
  }
  while (num < 1.0L) {
    num *= 10.0L;
    exp--;
  }
  return exp;
}

static int get_rounded_exponent_for_g(long double num, int precision) {
  int exponent = get_decimal_exponent(num);
  long double mantissa = num / powl(10.0L, exponent);
  long double scale = pow10_ld(precision - 1);
  long double rounded = rintl(mantissa * scale);
  long double limit = pow10_ld(precision);
  if (rounded >= limit) exponent++;
  return exponent;
}

static int append_exponent(char *out, int pos, int exponent, int uppercase,
                           int out_cap) {
  if (pos < out_cap) out[pos++] = uppercase ? 'E' : 'e';
  if (pos < out_cap) out[pos++] = (exponent < 0) ? '-' : '+';

  unsigned int exp_abs = (unsigned int)((exponent < 0) ? -exponent : exponent);
  char exp_buf[32];
  int exp_len = int_to_str(exp_abs, exp_buf, 10, 0);

  if (exp_len < 2 && pos < out_cap) out[pos++] = '0';
  for (int i = 0; i < exp_len && pos < out_cap; i++) out[pos++] = exp_buf[i];
  return pos;
}

static int trim_trailing_zeros(char *out, int len) {
  int exp_pos = -1;
  for (int i = 0; i < len; i++) {
    if (out[i] == 'e' || out[i] == 'E') {
      exp_pos = i;
      break;
    }
  }

  int main_end = (exp_pos == -1) ? len : exp_pos;
  int dot_pos = -1;
  for (int i = 0; i < main_end; i++) {
    if (out[i] == '.') {
      dot_pos = i;
      break;
    }
  }
  if (dot_pos == -1) return len;

  int i = main_end - 1;
  while (i > dot_pos && out[i] == '0') i--;
  if (i == dot_pos) i--;
  int new_main_end = i + 1;

  if (exp_pos != -1) {
    int k = new_main_end;
    for (int j = exp_pos; j < len; j++) out[k++] = out[j];
    return k;
  }
  return new_main_end;
}

static void write_number_with_padding(char **str, FormatSpec *spec, char sign,
                                      const char *num_buf, int num_len) {
  int total_len = num_len + (sign != '\0');
  int spaces = (spec->width > total_len) ? spec->width - total_len : 0;
  int use_zero = (spec->zero && !spec->minus);

  if (!spec->minus) {
    if (use_zero) {
      if (sign) put_char(str, sign);
      s21_memset(*str, '0', spaces);
      *str += spaces;
    } else {
      s21_memset(*str, ' ', spaces);
      *str += spaces;
      if (sign) put_char(str, sign);
    }
  } else if (sign) {
    put_char(str, sign);
  }

  for (int i = 0; i < num_len; i++) put_char(str, num_buf[i]);

  if (spec->minus) {
    s21_memset(*str, ' ', spaces);
    *str += spaces;
  }
}

// Преобразовываем число в строку
static int int_to_str(unsigned long long num, char *out, int base,
                      int uppercase) {
  const char *digits_lower = "0123456789abcdef";
  const char *digits_upper = "0123456789ABCDEF";
  const char *digits = uppercase ? digits_upper : digits_lower;

  if (num == 0) {
    out[0] = '0';
    return 1;
  }

  char temp[64];
  int i = 0;
  while (num > 0) {
    temp[i++] = digits[num % base];
    num /= base;
  }

  for (int j = 0; j < i; j++) {
    out[j] = temp[i - 1 - j];
  }
  return i;
}

static void put_uint_prefix(char **str, int has_prefix, int base, int uppercase,
                            int zeros) {
  if (has_prefix) {
    if (base == 8 && zeros == 0) {
      put_char(str, '0');
    } else if (base == 16) {
      put_char(str, '0');
      put_char(str, uppercase ? 'X' : 'x');
    }
  }
}

static void put_char(char **str, char c) { *(*str)++ = c; }

// Считаем кол-во пробелов, нулей для int
static void calc_int_sizes(long long num, FormatSpec *spec, IntSizes *sizes) {
  int is_neg = (num < 0);

  if (is_neg) {
    sizes->sign = '-';
  } else if (spec->plus) {
    sizes->sign = '+';
  } else if (spec->space) {
    sizes->sign = ' ';
  } else {
    sizes->sign = '\0';
  }

  sizes->zeros = 0;
  if (spec->is_precision_set && spec->precision > sizes->len) {
    sizes->zeros = spec->precision - sizes->len;
  }
  int total = sizes->len + sizes->zeros + (sizes->sign != '\0');
  sizes->spaces = (spec->width > total) ? spec->width - total : 0;
}

// Считаем кол-во пробелов, нулей для unsigned
static void calc_uint_sizes(unsigned long long num, FormatSpec *spec,
                            IntSizes *sizes, int base) {
  sizes->sign = '\0';  // У беззнаковых знака НЕТ
  sizes->zeros = 0;

  if (spec->is_precision_set && spec->precision > sizes->len) {
    sizes->zeros = spec->precision - sizes->len;
  }

  int prefix_len = 0;
  if (spec->hash && base == 8) {
    if ((num != 0 && sizes->zeros == 0) || sizes->len == 0) {
      prefix_len = 1;
    }
  } else if (spec->hash && base == 16 && num != 0) {
    prefix_len = 2;
  }

  int total = sizes->len + sizes->zeros + prefix_len;
  sizes->spaces = (spec->width > total) ? spec->width - total : 0;
}
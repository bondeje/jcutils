#ifndef STRINGS_H
#define STRINGS_H

// For all function returns, nul-terminator is excluded.

// WARNING: except where otherwise noted, all pointers passed should be non-null
#include <stddef.h>
#include <assert.h>
#include <limits.h>

#ifndef CAT
    #define CAT_(x, y) x##y
    #define CAT(x, y) CAT_(x, y)
#endif

#ifndef VARIADIC_SIZE
    #define VARIADIC_SIZE(...) VARIADIC_SIZE_(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, UNUSED)
    #define VARIADIC_SIZE_(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, size, ...) size
#endif

#ifndef APPEND_NARGS
    #define APPEND_NARGS(FUNCTION_BASE, ...) CAT(FUNCTION_BASE, VARIADIC_SIZE(__VA_ARGS__))
#endif

#define CONST_STRING_1(string) (String){.str = string, .size = strlen(string), .capacity = 0}
#define CONST_STRING_2(string, len) (String){.str = string, .size = len, .capacity = 0}
#define CONST_STRING(...) APPEND_NARGS(CONST_STRING_, __VA_ARGS__)(__VA_ARGS__)

#define AUTO_STRING_1(string) (String){.str = string, .size = strlen(string), .capacity = -strlen(string)}
#define AUTO_STRING_2(string, len) (String){.str = string, .size = len, .capacity = (assert(len <= PTRDIFF_MAX), -(len))}
#define AUTO_STRING(...) APPEND_NARGS(AUTO_STRING_, __VA_ARGS__)(__VA_ARGS__)

#define RESIZABLE_STRING() (String){.str = NULL, .size = 0, .capacity = 0}

typedef struct String {
	char * str;
	ptrdiff_t size;
	// capacity determines mutability
	// capacity == 0 means contents are immutable
	// capacity < 0 means contents are not allocated
	// capacity == 0 and str != NULL means contents are read-only
	ptrdiff_t capacity;
} String;

// TODO: not a full implementation of Array(String)
typedef struct Array_String {
	String * strings;
	ptrdiff_t size;
} Array_String;

extern String const WHITESPACE;
extern String const NEWLINE;
extern String const PATH_SEPARATOR;

/**
 * @brief Get the length of a string. This is the number of characters in the 
 * array that are available excluding any null terminators.
 * 
 * @param str 
 * @return ptrdiff_t 
 */
 
static inline ptrdiff_t String_len(String const * str) { return str->size; }

/**
 * @brief Get the length of the string. Alias for String_len. 
 * 
 * @param str 
 * @return ptrdiff_t 
 */
#define String_size(str) String_len(str)

/**
 * @brief Get the capacity of a string. This is the number of characters that 
 * may be written to. Note that this is semantically different from the underlying
 * storage. You should generally not access the capacity directly.
 * 
 * @param str 
 * @return size_t
 */
static inline size_t String_capacity(String const * str) { return (size_t)(str->capacity < 0 ? -str->capacity : str->capacity); }

/**
 * @brief Get the string pointer of a string.
 * 
 * @param str 
 * @return char* 
 */
static inline char * String_str(String const * str) { return str->str; }

/**
 * @brief Get whether the string is mutable. includes NULL strings that are allocatable
 * 
 * @param str 
 * @return _Bool 
 */
static inline _Bool String_is_mutable(String const * str) { return str && (str->capacity != 0 || !str->str); }

/**
 * @brief Get whether the string is allocatable.
 * 
 * @param str 
 * @return _Bool 
 */
static inline _Bool String_is_allocatable(String const * str) { return str->capacity > 0 || (!str->capacity && !str->str); }

/**
 * @brief Get the hash of a string. This is a hash function that can be used to
 * hash a string into a bin.
 * 
 * @param a 
 * @param bin_size 
 * @return size_t 
 */
size_t String_hash(String a, size_t bin_size);

/**
 * @brief Check if a string is empty.
 * 
 * @param str 
 * @return _Bool 
 */
static inline _Bool String_is_empty(String const * str) { return str->size == 0; }

/**
 * @brief Clear the string. This sets the size to 0 and does not free the buffer.
 * @note If this is applied to an immutable string, the underlying buffer will no
 * longer be usable.
 * 
 * @param str 
 */
static inline void String_clear(String * str) { str->size = 0; }

/**
 * @brief Compare two strings. Follows semantics of strcmp or used in e.g. qsort.
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int String_compare(String const * a, String const * b);
// this is for compatibility with e.g. qsort. cannot be a macro because an address needs to be taken
static inline int String_comp(void const * a, void const * b) {
	return String_compare((String *)a, (String *)b);
}

/**
 * @brief Check if a string is in another string.
 * 
 * @param str 
 * @param other 
 * @return _Bool 
 */
_Bool String_in(String const * str, String const * other);

/**
 * @brief Check if a character is in a string.
 * 
 * @param str 
 * @param val 
 * @return _Bool 
 */
_Bool String_char_in(String const * str, char val);

/**
 * @brief Convert a string to lowercase.
 * @note This is in place and only works on standard ASCII, i.e. assumes standard ASCII separation of 'A' - 'Z' and 'a' - 'z'
 * 
 * @param str 
 */
void String_lower(String * str);

/**
 * @brief Convert a string to uppercase.
 * @note This is in place and only works on standard ASCII, i.e. assumes standard ASCII separation of 'A' - 'Z' and 'a' - 'z'
 * 
 * @param str 
 */
void String_upper(String * str);

/**
 * @brief Get a character from a string. Returns -1 if the location is out of bounds
 * 
 * @param str 
 * @param loc - This is circular, e.g. -1 will return the last character
 * @return int
 */
int String_get(String const * str, ptrdiff_t loc);

/**
 * @brief Set a character in a string. Returns 0 on success, -1 on failure
 * 
 * @param str 
 * @param loc - This is circular, e.g. -1 will change the last character
 * @param val 
 * @return int
 */
int String_set(String * str, ptrdiff_t loc, char val);

/**
 * @brief Check if a string starts with another string.
 * 
 * @param str 
 * @param prefix 
 * @return _Bool 
 */
_Bool String_starts_with(String const * str, String const * prefix);

/**
 * @brief Check if a string ends with another string.
 * 
 * @param str 
 * @param suffix 
 * @return _Bool 
 */
_Bool String_ends_with(String const * str, String const * suffix);

/**
 * @brief Count the number of occurrences of a substring in a string.

 * 
 * @param str 
 * @param sub - The substring to count
 * @param start - the location to starting counting from
 * @param end - the location to stop counting at. 0 means the end of the string
 * @return int
 */
int String_count(String const * str, String const * sub, ptrdiff_t start, ptrdiff_t end);

/**
 * @brief Find the first occurrence of a substring in a string relative to the start of the string, not the start value
 * returns -1 if the substring is not found
 * 
 * @param str 
 * @param sub - The substring to find
 * @param start - the location to start searching from
 * @param end - the location to stop searching at
 * @return ptrdiff_t
 */
ptrdiff_t String_find(String const * str, String const * sub, ptrdiff_t start, ptrdiff_t end);

/**
 * @brief Find the last occurrence of a substring in a string.
 * returns -1 if the substring is not found
 * 
 * @param str 
 * @param sub - The substring to find
 * @param start - the location to start searching from
 * @param end - the location to stop searching at
 * @return ptrdiff_t
 */
ptrdiff_t String_rfind(String const * str, String const * sub, ptrdiff_t start, ptrdiff_t end);

/**
 * @brief Destroy a string, i.e. frees allocated buffers. 
 * After this, str compares equal to EMPTY_STRING
 * 
 * @param str 
 */
void String_dest(String * str); 

/**
 * @brief Deletes a string, i.e. frees allocated buffers and frees the String itself
 * 
 * @param str 
 */
void String_del(String * str);

/**
 * @brief Strip characters from the beginning and end of the string
 * result is as if rstrip and lstrip were applied in succession
 * 
 * @param str 
 * @param chars - The characters to strip. If NULL, whitespace is used
 * @return int - 0 on success, -1 on failure (string is immutable)
 */
int String_strip(String * str, String const * chars);

/**
 * @brief Strip characters from the beginning of the string
 * 
 * @param str 
 * @param chars - The characters to strip. If NULL, whitespace is used
 * @return int - 0 on success, -1 on failure (string is immutable)
 */
int String_lstrip(String * str, String const * chars);

/**
 * @brief Strip characters from the end of the string
 * 
 * @param str 
 * @param chars - The characters to strip. If NULL, whitespace is used
 * @return int - 0 on success, -1 on failure (string is immutable)	
 */
int String_rstrip(String * str, String const * chars);

/**
 * @brief Partition a string into two strings. The first string is the part of the
 * string before the first occurrence of the separator. The second string is the 
 * part of the string after the first occurrence of the separator.
 * If the separator is not found, the first string is the entire string and the second
 * string is empty.
 * returns 0 on success. size of the required buffer on failure
 * failure conditions:
 * - suffix is NULL
 * - suffix is immutable
 * - suffix is of insufficient capacity and not allocatable

 * @param str 
 * @param sep - The separator
 * @param suffix - The suffix. 
 * @return ptrdiff_t 
 */
ptrdiff_t String_partition(String * str, String const * sep, String * suffix);
/**
 * @brief Partition a string into two strings. The first string is the part of the
 * string before the last occurrence of the separator. The second string is the 
 * part of the string after the last occurrence of the separator.
 * If the separator is not found, the first string is the entire string and the second
 * string is empty.
 * returns 0 on success. size of the required buffer on failure
 * failure conditions:
 * - suffix is NULL
 * - suffix is immutable
 * - suffix is of insufficient capacity and not allocatable

 * @param str 
 * @param sep - The separator
 * @param suffix - The suffix. 
 * @return ptrdiff_t 
 */
 ptrdiff_t String_rpartition(String * str, String const * sep, String * suffix);
/**
 * @brief Copy a string into dest. dest must be mutable
 * returns 0 on success or the size of the dest buffer on failure
 * failure conditions:
 * - dest is immutable
 * - dest is of insufficient capacity and not allocatable
 * 
 * @param dest 
 * @param src 
 * @return ptrdiff_t 
 */
ptrdiff_t String_copy(String * dest, String const * src);

/**
 * @brief Expand tabs in a string.
 * returns 0 on success or the required buffers size on failure
 * failure conditions:
 * - str is immutable and at least one tab needs to be replaced
 * - str is of insufficient capacity and not allocatable
 * 
 * @param str 
 * @param tabsize - The number of spaces to replace each tab with
 * @return ptrdiff_t 
 */
ptrdiff_t String_expand_tabs(String * str, unsigned char tabsize);

/**
 * @brief Append a character to a string.
 * returns 0 on success or the required buffers size on failure
 * failure conditions:
 * - str is immutable
 * - str is of insufficient capacity and not allocatable
 */
ptrdiff_t String_append(String * str, char chr);

/**
 * @brief Extend a string with another string.
 * returns 0 on success or the required buffers size on failure
 * failure conditions:
 * - str is immutable
 * - str is of insufficient capacity and not allocatable
 */
ptrdiff_t String_extend(String * str, String const * other);

/**
 * @brief Replace a substring in a string.
 * returns 0 on success, -1 on failure, or the required buffer size
 * -1 failure conditions:
 * - old or new are NULL
 * - count < -1
 * required buffer size return conditions:
 * - str is immutable and at least one replacement needs to be made
 * - str is of insufficient capacity and not allocatable
 * - count is 0
 
 * @param str 
 * @param old - the substring to be replaced
 * @param new - the substring to replace with
 * @param count - the maximum number of replacements to make, -1 means no limit
 * @return ptrdiff_t 
 */
ptrdiff_t String_replace(String * str, String const * old, String const * new, 
	int count);

/**
 * @brief Get a null-terminated c-string from a string.
 * returns NULL if it could not be null-terminated. 
 * @note that this always fails on CONST_STRINGs
 * 
 * @param str 
 * @return char* 
 */
char * String_cstr(String * str);

/**
 * @brief Split a string into a array of strings, splitting on the separator 
 * returns the number of elements of dest filled on success.
 * on failure, returns the negative index of the first String in Array_String that failed.
 *    in this case, the indices below have been filled
 * if dest->size is -1, the underlying array will be allocated.
 *    in this case, a negative return is an allocation failure
 *
 * Assuming all buffers are of sufficient capacity,
 * Array_String dest = {.size = -1};
 * String out = {0};
 * String_split(&dest, str, sep);
 * String_join(&out, sep, &dest);
 * String_compare(&out, str) == 0; // out and str are now equal copies
 * 
 * @param dest 
 * @param str 
 * @param sep 
 * @return ptrdiff_t 
 */
ptrdiff_t String_split(Array_String * dest, String * str, String const * sep);
/**
 * @brief Join a array of strings into a single string.
 * returns 0 on success, size of the required buffer on failure
 * failure conditions:
 * - dest is immutable
 * - dest is of insufficient capacity and not allocatable
 *
 * @param dest
 * @param sep - The separator to join on. If NULL, EMPTY_STRING is used
 * @param strings
 * @return ptrdiff_t
 */
ptrdiff_t String_join(String * dest, String const * sep, Array_String * strings);
// step == 0, step is set to 1. if step > 0 and end == 0, String_len is used as end
ptrdiff_t String_slice(String * dest, String const * str, ptrdiff_t start, ptrdiff_t end, ptrdiff_t step);

// allocates upon return
String * String_new(char const * buf, ptrdiff_t size);

#endif

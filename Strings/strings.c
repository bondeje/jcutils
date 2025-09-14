#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "strings.h"

#ifndef STRING_GROWTH_FACTOR
	#define STRING_GROWTH_FACTOR 2
#endif

#ifndef STRING_MIN_ALLOC
	#define STRING_MIN_ALLOC 64
#endif

String const WHITESPACE = {" \t\f\n\r\v", 6, 0};

String const EMPTY_STRING = {"", 0, 0}; // note that this is NOT allocatable and not mutable

#ifdef _WIN32
	String const PATH_SEPARATOR = {"\\", 1, 0};
	String const NEWLINE = {"\r\n", 2, 0};
#else
	String const PATH_SEPARATOR = {"/", 1, 0};
	String const NEWLINE = {"\n", 1, 0};
#endif

#define UPPER_TO_LOWER_OFFSET 32

int String_compare(String const * a, String const * b) {
	ptrdiff_t size_a = String_len(a);
	ptrdiff_t size_b = String_len(b);
	int result = 0;
	if (size_a < size_b) {
		if (!size_a) {
			return -1;
		}
		result = strncmp(a->str, b->str, size_a);
		if (!result) { // the first size_a characters match. a is a substring of b
			result = -1;
		}
	} else if (size_a > size_b) {
		if (!size_b) {
			return 1;
		}
		result = strncmp(a->str, b->str, size_b);
		if (!result) { // the first size_b characters match. b is a substring of a
			result = 1;
		}
	} else {
		if (!size_a) {
			return 0;
		}
		result = strncmp(a->str, b->str, size_a);
	}
	return result;
}

_Bool String_in(String const * str, String const * other) {
	return 0 < String_count(str, other, 0, 0);
}

_Bool String_char_in(String const * str, char val) {
	if (str->size <= 0) {
		return false;
	}
	char const * str_ = str->str;
	for (ptrdiff_t i = 0; i < str->size; i++) {
		if (str_[i] == val) {
			return true;
		}
	}
	return false;
}

void String_lower(String * str) {
	for (ptrdiff_t i = 0; i < str->size; i++) {
		char c = str->str[i];
		if (c >= 'A' && c <= 'Z') {
			str->str[i] = c + UPPER_TO_LOWER_OFFSET;
		}
	}
}
void String_upper(String * str) {
	for (ptrdiff_t i = 0; i < str->size; i++) {
		char c = str->str[i];
		if (c >= 'a' && c <= 'z') {
			str->str[i] = c - UPPER_TO_LOWER_OFFSET;
		}
	}
}
int String_get(String const * str, ptrdiff_t loc) {
	if (str->size <= 0) {
		return -1;
	}
	if (loc < 0) {
		loc = str->size - 1 + ((loc + 1) % str->size);
	}
	if (loc < str->size) {
		return str->str[loc];
	}
	return -1;
}
int String_set(String * str, ptrdiff_t loc, char val) {
	if (str->size <= 0) {
		return -1;
	}
	if (loc < 0) {
		loc = str->size - 1 + ((loc + 1) % str->size);
	}
	int out = -1;
	if (loc < str->size) {
		out = str->str[loc];
		str->str[loc] = val;
	}
	return out;
}
_Bool String_starts_with(String const * str, String const * prefix) {
	if (str->size < prefix->size || prefix->size < 0) {
		return false;
	}
	char const * str_ = str->str;
	char const * pre_ = prefix->str;
	for (ptrdiff_t i = 0; i < prefix->size; i++) {
		if (str_[i] != pre_[i]) {
			return false;
		}
	}
	return true;
}
_Bool String_ends_with(String const * str, String const * suffix) {
	if (str->size < suffix->size || suffix->size < 0) {
		return false;
	}
	char const * str_ = str->str + (str->size - suffix->size);
	char const * pre_ = suffix->str;
	for (ptrdiff_t i = 0; i < suffix->size; i++) {
		if (str_[i] != pre_[i]) {
			return false;
		}
	}
	return true;
}
int String_count(String const * str, String const * sub, ptrdiff_t start, ptrdiff_t end) {
	int ct = 0;
	if (0 >= sub->size || 0 >= str->size) {
		return ct;
	}
	if (start < 0) {
		start += str->size * (start / str->size) - (start % str->size);
	}
	if (end < 0) {
		end += str->size * (end / str->size) - (end % str->size);
	}
	if (0 == end) {
		end += str->size;
	}
	if (start >= end) {
		return 0;
	}
	ptrdiff_t loc = String_find(&(String){.str = str->str + start, .size = end - start}, 
								sub, 0, end - start);
	while (loc >= 0) {
		loc += start;
		ct++;
		if (loc < end - sub->size) {
			start = loc + sub->size;
			loc = String_find(&(String){.str = str->str + start, 
					.size = end - start}, sub, 0, end - start);
		} else {
			loc = -1;
		}
	}
	return ct;
}
// returns -1 if not found
ptrdiff_t String_find(String const * str, String const * sub, ptrdiff_t start, ptrdiff_t end) {
	if (0 >= sub->size || 0 >= str->size || str->size < sub->size) {
		return -1;
	}
	if (start < 0) {
		start += str->size * (start / str->size) - (start % str->size);
	}
	if (end < 0) {
		end += str->size * (end / str->size) - (end % str->size);
	}
	if (0 == end) {
		end += str->size;
	}
	if (start >= end) {
		return -1;
	}
	char init = sub->str[0];
	char * str_ = str->str + start;
	char const * end_ = str->str + end;
	while (str_ < end_) {
		if (String_starts_with(&(String){.str=str_, .size=end_-str_}, sub)) {
			return str_ - str->str;
		}
		str_++;
		while (str_ < end_ && *str_ != init) {
			str_++;
		}
	}
	return -1;
}
ptrdiff_t String_rfind(String const * str, String const * sub, ptrdiff_t start, ptrdiff_t end) {
	if (0 >= sub->size || 0 >= str->size || sub->size > str->size) {
		return -1;
	}
	if (start < 0) {
		start += str->size * (start / str->size) - (start % str->size);
	}
	if (end < 0) {
		end += str->size * (end / str->size) - (end % str->size);
	}
	if (0 == end) {
		end += str->size;
	}
	if (start + sub->size > end) {
		return -1;
	}
	char init = sub->str[0];
	char * str_ = str->str + start;
	ptrdiff_t final_end = sub->size + start;
	String test = {.str = str_, .size = 0};
	while (end >= final_end) {
		test.size = end - start;
		if (String_ends_with(&test, sub)) {
			return end - sub->size;
		}
		end--;
		while (end >= final_end && str->str[end - sub->size] != init) {
			end--;
		}
	}
	return -1;
}
void String_dest(String * str) {
	// frees buffer if allocated and resets to compare equal to EMPTY_STRING
	if (String_is_allocatable(str)) {
		free(str->str);
		// setting str to NULL and capacity to 0 ensures string still evaluates to allocatable
		str->str = NULL;
		str->capacity = 0;
		assert(String_is_allocatable(str));
	}
	// AUTO_STRING and CONST_STRINGS cannot change capacity, only set size to 0 to ensure compare to empty string succeeds
	str->size = 0;
}

void String_del(String * str) { 
	// calls *_dest and then frees str
	String_dest(str);
	free(str);
}
int String_strip(String * str, String const * chars) {
	if (chars == NULL) {
		chars = &WHITESPACE;
	}
	// run rstrip first so that the potential move in lstrip is moving less memory
	if (String_rstrip(str, chars)) {
		return -1;
	}
	return String_lstrip(str, chars);
}
int String_lstrip(String * str, String const * chars) {
	if (String_is_empty(str)) {
		return 0;
	}
	// whitespace if chars == NULL
	if (chars == NULL) {
		chars = &WHITESPACE;
	}
	if (String_is_empty(chars)) {
		return 0;
	}
	if (!String_is_mutable(str)) {
		return -1;
	}
	ptrdiff_t itest = 0;
	char * str_ = str->str;
	while (itest < str->size && String_char_in(chars, str_[itest])) {
		itest++;
	}
	if (itest) {
		str->size -= itest;
		memmove(str->str, str->str + itest, str->size * sizeof(char));
	}
	return 0;
}
int String_rstrip(String * str, String const * chars) {
	// whitespace if chars == NULL
	if (String_is_empty(str)) {
		return 0;
	}
	if (chars == NULL) {
		chars = &WHITESPACE;
	}
	if (String_is_empty(chars)) {
		return 0;
	}
	if (!String_is_mutable(str)) {
		return -1;
	}
	char * str_ = str->str;
	while (str->size > 0 && String_char_in(chars, str_[str->size - 1])) {
		str->size--;
	}
	return 0;
}

/**
 * @brief Resize the string.
 * 
 * @param str 
 * @param new_capacity 
 * @return int - 0 on success, -1 on failure to reallocate, -2 on unallocatable string
 */
int String_resize(String * str, ptrdiff_t new_capacity) {
	if (!String_is_allocatable(str)) {
		return -2;
	}
	if (new_capacity < STRING_MIN_ALLOC) {
		new_capacity = STRING_MIN_ALLOC;
	} else if (new_capacity < str->capacity * STRING_GROWTH_FACTOR) {
		new_capacity = str->capacity * STRING_GROWTH_FACTOR;
	}
	char * str_ = realloc(str->str, new_capacity);
	if (!str_) {
		return -1;
	}
	str->str = str_;
	str->capacity = new_capacity;
	return 0;
}

/**
 * @brief Initialize the string.
 * 
 * @param str 
 * @param buf 
 * @param size 
 * @param capacity - see typedef for semantics
 */
void String_init(String * str, char * buf, ptrdiff_t size, ptrdiff_t capacity) {
	str->str = buf;
	str->size = size;
	str->capacity = capacity;
}

ptrdiff_t String_partition(String * str, const String * sep, String * suffix) {
	if (!sep || !sep->size ||str->size < sep->size) {
		if (!suffix) {
			return 0;
		}
		suffix->size = 0;
		return 0;
	}
	char first_sep = sep->str[0];
	char * str_current = str->str;
	char * str_end = str->str + str->size - sep->size;
	
	while (str_current < str_end) {
		while (str_current < str_end && *str_current != first_sep) {
			str_current++;
		}
		if (str_current == str_end || !strncmp(str_current, sep->str, sep->size)) {
			break;
		}
		str_current++;
	}

	// check for valid suffix copying
	ptrdiff_t suffix_size = str->size - sep->size - (str_current - str->str);
	if (!suffix || !String_is_mutable(suffix)) {
		return suffix_size;
	}

	// ensure suffix has sufficient capacity
	if ((ptrdiff_t)String_capacity(suffix) < suffix_size) {
		if (String_is_allocatable(suffix)) {
			if (String_resize(suffix, suffix_size)) {
				// failed to allocate a larger buffer
				return suffix_size;
			}
		} else { // cannot allocate a larger buffer
			return suffix_size;
		}		
	}

	// suffix has sufficient capacity
	// perform copy. str_current is the location of the separator
	if (suffix_size) {
		// only copy and modify str if separator is found
		memmove(suffix->str, str_current + sep->size, suffix_size * sizeof(*(str->str)));
		str->size -= suffix_size + sep->size;
	}
	suffix->size = suffix_size;
	return 0;
}
ptrdiff_t String_rpartition(String * str, String const * sep, String * suffix) {
	if (!sep || !sep->size || str->size < sep->size) {
		if (!suffix) {
			return 0;
		}
		suffix->size = 0;
		return 0;
	}
	char first_sep = sep->str[0];
	char * str_current = str->str + str->size - sep->size + 1;
	
	while (str_current > str->str) {
		while (str_current > str->str && (*(--str_current) != first_sep)) {
			
		}
		if (!strncmp(str_current, sep->str, sep->size)) {
			break;
		}
	}

	if (str_current == str->str) { // separator not found, don't partition
		if (!suffix) {
			return 0;
		}
		suffix->size = 0;
		return 0;
	}

	// check for valid suffix copying
	ptrdiff_t suffix_size = str->size - sep->size - (str_current - str->str);
	if (!suffix || !String_is_mutable(suffix)) {
		return suffix_size;
	}

	// ensure suffix has sufficient capacity
	if ((ptrdiff_t)String_capacity(suffix) < suffix_size) {
		if (String_is_allocatable(suffix)) {
			if (String_resize(suffix, suffix_size)) {
				// failed to allocate a larger buffer
				return suffix_size;
			}
		} else { // cannot allocate a larger buffer
			return suffix_size;
		}		
	}

	// suffix has sufficient capacity
	// perform copy. str_current is the location of the separator
	if (suffix_size) {
		// only copy and modify str if separator is found
		memmove(suffix->str, str_current + sep->size, suffix_size * sizeof(*(str->str)));
		str->size -= suffix_size + sep->size;
	}
	suffix->size = suffix_size;	
	return 0;
}
ptrdiff_t String_copy(String * dest, String const * src) {
	if (!dest || !String_is_mutable(dest)) {
		return src->size;
	}
	if ((ptrdiff_t)String_capacity(dest) < src->size) {
		if (String_is_allocatable(dest)) {
			if (String_resize(dest, src->size)) {
				return src->size;
			}
		} else {
			return src->size;
		}
	}
	if (src->size) {
		memcpy(dest->str, src->str, src->size * sizeof(char));
	}
	dest->size = src->size;
	return 0;
}
ptrdiff_t String_expand_tabs(String * str, unsigned char tabsize) {
	char * str_ = str->str;
	ptrdiff_t N = str->size;
	ptrdiff_t new_size = str->size;
	for (ptrdiff_t i = 0; i < N; i++) {
		if (str_[i] == '\t') {
			new_size += tabsize - 1;
		}		
	}
	if (new_size == N) { // no tabs to expand
		return 0;
	}

	if (!String_is_mutable(str)) {
		return new_size;
	}
	if ((ptrdiff_t)String_capacity(str) < new_size) {
		if (String_is_allocatable(str)) {
			if (String_resize(str, new_size)) {
				return new_size;
			}
		} else {
			return new_size;
		}
	}
	// at this point, the str has sufficient capacity
	str->size = new_size;
	new_size--; // new_size is now the write pointer while N is the read
	str_ = str->str; // reset in case realloc'd
	N--;
	while (N > -1) {
		if (str_[N] == '\t') {
			for (unsigned char i = 0; i < tabsize; i++) {
				str_[new_size--] = ' ';
			}
		} else {
			str_[new_size--] = str_[N];
		}
		N--;
	}
	return 0;
}
ptrdiff_t String_append(String * str, char chr) {
	if (!String_is_mutable(str)) {
		return str->size + 1;
	}
	if ((ptrdiff_t)String_capacity(str) < str->size + 1) {
		if (String_is_allocatable(str)) {
			if (String_resize(str, str->size + 1)) {
				return str->size + 1;
			}
		} else {
			return str->size + 1;
		}
	}
	
	// at this point, str has sufficient capacity
	str->str[str->size++] = chr;
	return 0;
}
ptrdiff_t String_extend(String * str, String const * other) {
	ptrdiff_t new_size = str->size + other->size;
	if (!String_is_mutable(str)) {
		return new_size;
	}
	if ((ptrdiff_t)String_capacity(str) < new_size) {
		if (String_is_allocatable(str)) {
			if (String_resize(str, new_size)) {
				return new_size;
			}
		} else {
			return new_size;
		}
	}
	// at this point, str has sufficient capacity
	if (other->size) {
		memcpy(str->str + str->size, other->str, other->size * sizeof(char));
	}
	str->size = new_size;
	return 0;
}

ptrdiff_t String_replace(String * str, String const * old, 
	String const * new, int count) {

	if (!old || !new || count < -1) {
		return -1;
	}

	ptrdiff_t read = str->size;
	ptrdiff_t old_size = old->size;
	ptrdiff_t new_size = new->size;

	int nold = String_count(str, old, 0, read);
	if (!nold) {
		return 0;
	}
	
	if (!count) {
		return nold * (new_size - old_size) + read;
	} else if (count == -1) {
		count = nold;
	}
	ptrdiff_t delta = count * (new_size - old_size);
	ptrdiff_t write = read + delta;

	if ((ptrdiff_t)String_capacity(str) < write) {
		if (String_is_allocatable(str)) {
			if (String_resize(str, write)) {
				return write;
			}
		} else {
			return write;
		}
	}
	// str should have sufficient capacity
	// write; // this is now the location before which bytes are being written to
	// read; // this is now the location before which bytes are read from
	ptrdiff_t next = read;
	for (int i = 0; i < nold - count; i++) {
		next = String_rfind(str, old, 0, next);
	}
	write -= read - next;
	memmove(str->str + write, str->str + next, (read - next) * sizeof(char));
	read = next;
	for (int i = 0; i < count; i++) {
		next = String_rfind(str, old, 0, next);
		ptrdiff_t copy = read - next - old_size;

		// copy everything up to next instance of old string to new buffer
		write -= copy;
		memmove(str->str + write, str->str + next + old_size, copy * sizeof(char));

		// copy in new string where old was to be placed
		write -= new_size;
		memcpy(str->str + write, new->str, new_size * sizeof(char));
		read = next;
	}

	return 0;
}

// allocates upon return
String * String_new(char const * buf, ptrdiff_t size) {
	if (size < 0) {
		return NULL;
	}
	String * str = malloc(sizeof(*str));
	ptrdiff_t capacity = 0;
	if (buf) {
		if (size) {
			capacity = size + 1; // always allocate for a potential null-terminator
		} else {
			size = strlen(buf);
			capacity = size + 1;
		}
	} else {
		if (size) { // size is the capacity
			capacity = size;
			size = 0;
		} else { // create just an allocatable String with no initial contents
			capacity = 0;
		}
	}
	char * string_buffer = NULL;
	if (capacity) {
		string_buffer = malloc(capacity * sizeof(char));
		if (!string_buffer) {
			free(str);
			return NULL;
		}
	}
	if (buf) { // copy buffer into String
		memcpy(string_buffer, buf, size * sizeof(char));
	}
	String_init(str, string_buffer, size, capacity);
	return str;
}
// separate on whitespace if sep is NULL. Returns number of elements of dest filled
ptrdiff_t String_split(Array_String * dest, String * str, String const * sep) {
	if (dest->size == 0 || !dest->strings) {
		return 0;
	}
	if (!sep) {
		sep = &WHITESPACE;
	}

	ptrdiff_t nsep = String_count(str, sep, 0, str->size);
	//TODO: fix this
	if (nsep < 1) {
		return 0;
	}

	if (dest->size < 0) {
		dest->strings = malloc((nsep + 1) * sizeof(*(dest->strings)));
		if (!dest->strings) {
			return -1;
		}
		dest->size = nsep + 1;
	}

	if (nsep > dest->size - 1) {
		nsep = dest->size - 1;
	}

	ptrdiff_t start = 0;
	ptrdiff_t index = 0;
	for (; index < nsep; index++) {
		ptrdiff_t loc = String_find(str, sep, start, str->size);
		const String src = AUTO_STRING(str->str + start, loc - start);
		if (String_copy(dest->strings + index, &src)) {
			return index;
		}
		start = loc + sep->size;
	}
	const String src = AUTO_STRING(str->str + start, str->size - start);
	if (String_copy(dest->strings + index, &src)) {
		return index;
	}
	return index + 1;
}
ptrdiff_t String_join(String * dest, String const * sep, Array_String * strings) {
	if (!strings->size || !strings->strings) {
		dest->size = 0;
		return 0;
	}

	if (!sep) {
		sep = &EMPTY_STRING;
	}

	ptrdiff_t new_size = strings->strings[0].size;
	for (ptrdiff_t i = 1; i < strings->size; i++) {
		new_size += sep->size + strings->strings[i].size;
	}

	if (!String_is_mutable(dest)) {
		return new_size;
	}
	if ((ptrdiff_t)String_capacity(dest) < new_size) {
		if (String_is_allocatable(dest)) {
			if (String_resize(dest, new_size)) {
				return new_size;
			}
		} else {
			return new_size;
		}
	}
	// dest is now mutable and has sufficient capacity
	String_clear(dest);
	String_extend(dest, strings->strings + 0);
	for (ptrdiff_t i = 1; i < strings->size; i++) {
		String_extend(dest, sep);
		String_extend(dest, strings->strings + i);
	}
	return 0;
}
char * String_cstr(String * str) {
	if (String_append(str, '\0')) {
		return NULL;
	}
	str->size--; // the null-terminator that was append is not part of the string itself
	return str->str;
}

ptrdiff_t String_slice(String * dest, String const * str, ptrdiff_t start, ptrdiff_t end, ptrdiff_t step) {
	if (step == 0) {
		step = 1;
	}
	ptrdiff_t N = String_len(str);
	int dir = step > 0 ? 1 : -1;
	if (dir > 0 && end == 0) {
		end = N;
	}
	if (start < 0) {
		start = N - 1 + ((start + 1) % N);
	}
	if (end < 0) {
		end = N + ((end + 1) % N);
	}
	// special case of end originally being -1
	if (dir < 0) {
		if (end == N) {
			end = -1;
		}
	}
	if ((dir < 0 && end >= start) || (dir > 0 && end <= start)) {
		return -1;
	}
	
	ptrdiff_t nchars = (end - start - dir) / step + 1;

	if (!String_is_mutable(dest)) {
		return nchars;
	}
	if ((ptrdiff_t)String_capacity(dest) < nchars) {
		if (String_is_allocatable(dest)) {
			if (String_resize(dest, nchars)) {
				return nchars;
			}
		} else {
			return nchars;
		}
	}
	// dest is now mutable and has sufficient capacity
	String_clear(dest);
	char const * str_ = str->str;
	for ( ; 0 < (end - start) * dir; start += step) {
		String_append(dest, str_[start]);
	}
	return 0;
}

size_t String_hash(String a, size_t bin_size) {
	unsigned long long hash = 5381;
	ptrdiff_t i = 0;
	unsigned char * str = (unsigned char *) a.str;

	while (i < a.size) {
		hash = ((hash << 5) + hash) + *str; /* hash * 33 + c */
		str++;
		i++;
	}
	return hash % bin_size;
}


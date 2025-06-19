/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** string_tests
*/

#include "utils/string.h"
#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Test(string_utils, to_lowercase_basic)
{
    char str[] = "HELLO WORLD";
    to_lowercase(str);
    cr_assert_str_eq(
        str, "hello world", "String should be converted to lowercase");
}

Test(string_utils, to_lowercase_mixed)
{
    char str[] = "HeLLo WoRLd 123!@#";
    to_lowercase(str);
    cr_assert_str_eq(str, "hello world 123!@#",
        "Mixed case string should be converted to lowercase, preserving "
        "numbers and symbols");
}

Test(string_utils, to_lowercase_already_lower)
{
    char str[] = "already lowercase";
    to_lowercase(str);
    cr_assert_str_eq(str, "already lowercase",
        "Already lowercase string should remain unchanged");
}

Test(string_utils, to_lowercase_empty)
{
    char str[] = "";
    to_lowercase(str);
    cr_assert_str_eq(str, "", "Empty string should remain empty");
}

Test(string_utils, to_lowercase_special_chars)
{
    char str[] = "123!@#$%^&*()";
    to_lowercase(str);
    cr_assert_str_eq(str, "123!@#$%^&*()",
        "String with only special characters should remain unchanged");
}

Test(string_utils, trim_leading_spaces)
{
    char str[] = "   hello world";
    trim(str);
    cr_assert_str_eq(str, "hello world", "Leading spaces should be removed");
}

Test(string_utils, trim_trailing_spaces)
{
    char str[] = "hello world   ";
    trim(str);
    cr_assert_str_eq(str, "hello world", "Trailing spaces should be removed");
}

Test(string_utils, trim_both_spaces)
{
    char str[] = "   hello world   ";
    trim(str);
    cr_assert_str_eq(str, "hello world",
        "Both leading and trailing spaces should be removed");
}

Test(string_utils, trim_mixed_whitespace)
{
    char str[] = "\t\n  hello world  \n\t";
    trim(str);
    cr_assert_str_eq(str, "hello world",
        "Mixed whitespace (tabs, newlines, spaces) should be removed");
}

Test(string_utils, trim_only_whitespace)
{
    char str[] = "   \t\n   ";
    trim(str);
    cr_assert_str_eq(
        str, "", "String with only whitespace should become empty");
}

Test(string_utils, trim_empty_string)
{
    char str[] = "";
    trim(str);
    cr_assert_str_eq(str, "", "Empty string should remain empty");
}

Test(string_utils, trim_no_whitespace)
{
    char str[] = "hello";
    trim(str);
    cr_assert_str_eq(
        str, "hello", "String without whitespace should remain unchanged");
}

Test(string_utils, dyn_strcat_basic)
{
    char *dest = malloc(6);
    strcpy(dest, "hello");
    char *result = dyn_strcat(&dest, " world");

    cr_assert_not_null(result, "dyn_strcat should not return NULL");
    cr_assert_str_eq(
        result, "hello world", "Strings should be concatenated correctly");

    free(result);
}

Test(string_utils, dyn_strcat_empty_dest)
{
    char *dest = malloc(1);
    strcpy(dest, "");
    char *result = dyn_strcat(&dest, "hello");

    cr_assert_not_null(result, "dyn_strcat should not return NULL");
    cr_assert_str_eq(
        result, "hello", "Should concatenate to empty destination");

    free(result);
}

Test(string_utils, dyn_strcat_empty_src)
{
    char *dest = malloc(6);
    strcpy(dest, "hello");
    char *result = dyn_strcat(&dest, "");

    cr_assert_not_null(result, "dyn_strcat should not return NULL");
    cr_assert_str_eq(result, "hello", "Should handle empty source string");

    free(result);
}

Test(string_utils, dyn_strcat_null_dest)
{
    char *result = dyn_strcat(NULL, "hello");

    cr_assert_null(
        result, "dyn_strcat should return NULL for NULL destination");
}

Test(string_utils, dyn_strcat_null_src)
{
    char *dest = malloc(6);
    strcpy(dest, "hello");
    char *result = dyn_strcat(&dest, NULL);

    cr_assert_null(result, "dyn_strcat should return NULL for NULL source");
    // dest should still be valid since the function returns early on NULL src
    free(dest);
}

Test(string_utils, dyn_strcat_multiple)
{
    char *dest = malloc(6);
    strcpy(dest, "hello");

    dest = dyn_strcat(&dest, " ");
    cr_assert_not_null(dest, "First concatenation should succeed");

    dest = dyn_strcat(&dest, "world");
    cr_assert_not_null(dest, "Second concatenation should succeed");
    cr_assert_str_eq(
        dest, "hello world", "Multiple concatenations should work");

    free(dest);
}

Test(string_utils, empty_string_basic)
{
    char *result = empty_string(0);

    cr_assert_not_null(result, "empty_string should not return NULL");
    cr_assert_str_eq(result, "", "empty_string should return an empty string");
    cr_assert_eq(strlen(result), 0, "empty_string should have length 0");

    free(result);
}

Test(string_utils, empty_string_modifiable)
{
    char *result = empty_string(0);

    cr_assert_not_null(result, "empty_string should not return NULL");

    // Test that we can modify the string (it should be writable memory)
    result = realloc(result, 10);
    strcpy(result, "test");
    cr_assert_str_eq(
        result, "test", "empty_string result should be modifiable");

    free(result);
}

# Coding Guidelines

Style guidelines and formatting rules are not covered here, as they are specified in the `.clang-format` file. Be sure to use ClangFormat to format your code before committing.

All the below guidelines are **not strictly enforced**, but they are **highly recommended** to maintain code quality.

Style consistency matters far more than picking the best and the most readable style. Following a consistent style will make your code automatically more readable :).

## Naming Convention

- Use `UPPER_SNAKE_CASE` for macros.
- Use `lower_snake_case` for everything else.
- Don't use acronyms or abbreviate names unless they are widely known.

### Naming Files

- Use `lower_snake_case` for files and directory names.
- Use specificity-first convention for file names, e.g., `project_logo.png` instead of `logo_project.png`.

## Doxygen Comments

- Use `///` or `///<` style comments for Doxygen documentation.
- Prefer limiting the documentation to 80 characters per line, though this is not strictly enforced.
- Avoid overuse of `@` tags. Use them only when necessary, such as important params, noted return, general notes, etc.
- Do not over document. You do not need to document every single function or variable. Moreover, you do not need to document every single parameter.

## Niche Styles

Below are some niche styles that I believe you should keep in mind.

### Use of `auto`

- Avoid using `auto` unless the type is obvious from the context but verbose.
- Same rule applies for use in `for` loops or lambda parameters.

### Range-based for loops

- Prefer using range-based for loops over traditional for loops when iterating over containers and index is not needed.

### Lambda Functions

- Prefer using reference capture for all lambdas unless you need to copy the variable.

### Static Functions

- Mark your function with `static` if it is meant to be used only in a specific source file.

### Structs and Classes

- Prefer using `struct` over `class`.
- Avoid using `private` and `protected` access specifiers. Use `public` for everything.

Note: Even though I use `struct`, I refer to them as "classes" in the documentation.

### Aggregate Types

- Prefer making your types aggregate types (i.e., no constructors, no private members).

### Enums

- Prefer using `enum class` over `enum`.

### Shared Pointers

- Prefer using `std::shared_ptr` over `std::unique_ptr` unless you are sure that the object will not be shared.

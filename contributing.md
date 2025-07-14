# Contributing to Crystalgui

Welcome, contributors! Thank you for showing your interest in contributing to Crystalgui! I welcome your contributions, whether it is a bug report, feature request, or code contribution.

There is not much strict structure of contributing to this project (e.g., no specific format for commit messages, no specific branch naming conventions, etc.). Feel free to contribute in any way you like or makes sense to you.

However, I do have some [guidelines for coding standards](coding_guidelines.md) that I would like you to follow. This will help ensure that the code is consistent and easy to read.

TL;DR: Feel free to contribute in any meaningful way, and be sure to run the tests.

## How Can You Help?

You can help me in several ways:

- **Report bugs**: If you find a bug, please report it. You can do this by opening an issue on [GitHub](https://github.com/anstropleuton/crystalgui/issues). Please provide as much detail as possible, including steps to reproduce the bug, your operating system, and any error messages you received. This will be very helpful in fixing the bug quickly.
- **Request features**: If you have an idea for a new feature or improvement, please let me know! You can do this by opening an issue on [GitHub](https://github.com/anstropleuton/crystalgui/issues). It will be helpful if you can provide a detailed description of the feature, why you think it is important, and how you think it should work.
- **Contribute code**: If you want to contribute code, fork the repository and make your changes. Be sure to follow the [coding style and guidelines](coding_guidelines.md). Once you have made your changes, and finally create a pull request. In the pull request description, please provide a detailed description of the changes you made and why you made them. This will help me review your changes quickly.

Ideas that you can contribute code in:
- Fixing bugs.
- Implementing features.

## Coding Style and Guidelines

You can find the coding style and guidelines in the [coding guidelines](coding_guidelines.md) document. If you have ClangFormat installed, you can run `clang-format` on the code to format it according to the guidelines, using the included `.clang-format` file. This will help ensure that the code is consistent and easy to read.

## Code Contribution Steps

Here are some rough steps that you can follow when contributing code:

1. Fork the repository from the [Crystalgui Repo](https://github.com/anstropleuton/crystalgui).
2. Clone and build the repo once, just to ensure that it builds before changes.
3. Optionally, set up your development environment using CMake and ClangFormat. Clangd is also recommended.
4. Make your proposed changes and commit them.
5. Run all the existing tests and verify they all pass. Feel free to add your own tests.
5. Create a pull request.

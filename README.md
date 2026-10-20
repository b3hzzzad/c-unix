# C-UNIX

An evolving collection of Unix command-line utilities written in C, focused on low-level I/O, memory management, and systems programming.

This project aims to recreate commonly used Unix commands from scratch to gain a deeper understanding of how they work internally.

1. cat

A custom implementation of the Unix cat command.

**Supported features:**

| Command | Description |
| cat | Read from standard input and print to standard output. |
| cat <file> | Display the contents of a file. |
| cat -n <file> | Display file contents with line numbers. |
| cat -e <file> | Display `$` at the end of each newline-terminated line. |

**Usage:**

```bash
./cunix cat
./cunix cat file.txt
./cunix cat -n file.txt
./cunix cat -e file.txt
```

2. grep 

A custom implementation of the Unix grep command for searching text within files.

**Supported features:**

| Command | Description |
| grep <pattern> <file> | Search for a substring and print matching lines. |
| grep <pattern> <file1> <file2> | Search for a substring across multiple files. |
| grep -n <patterm> <file> | Display matching lines prefixed with their line numbers. |
| grep -v <patterm> <file> | Print lines that do not match. |

**Usage:**

```bash
./cunix grep hello file.txt
./cunix grep error log1.txt log2.txt
./cunix grep "hello world" file.txt
./cunix grep -n "test" file.txt
./cunix grep -v "test" file.txt
```

3. tail

A custom implementation of the Unix tail command using backward file scanning.

**Supported features:**

| Command | Description |
| tail <file> | Display the last 10 lines of a file. |
| tail -n <N> <file> | Display the last N lines of a file. |
| tail <file1> <file2> | Display the last 10 lines of multiple files with filename headers. |
| tail -n <N> <file1> <file2> | Display the last N lines of multiple files with filename headers. |

**Usage:**

```bash
./cunix tail file.txt
./cunix tail -n 5 file.txt
./cunix tail -n 20 large_file.txt
./cunix tail file1.txt file2.txt
./cunix tail -n 5 file1.txt file2.txt
```

## Project Structure

All commands are accessible through the `cunix` executable:

```bash
./cunix <command> [options] [arguments]
```

## Project Status

**Work in Progress**

The project is actively being developed. Additional Unix commands, options, error handling improvements, and tests will be added progressively.
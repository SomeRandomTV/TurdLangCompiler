# TurdLangCompiler

Starting off a simple compiler for my custom language

This is built in C++26 and has a custom lexer and syntax parser

## Lexer

The lexer tokenizes source code into a stream of tokens for parsing.

### Token Types

**Keywords:**
| Lexeme | Token |
|--------|-------|
| print | KEY_PRINT |
| read | KEY_READ |
| if | KEY_IF |
| else | KEY_ELSE |
| for | KEY_FOR |
| while | KEY_WHILE |
| function | KEY_FUNCTION |
| var | KEY_VAR |
| return | KEY_RETURN |
| true | KEY_TRUE |
| false | KEY_FALSE |

**Data Types:**
| Lexeme | Token |
|--------|-------|
| int | DATATYPE_INT |
| float | DATATYPE_FLOAT |
| string | DATATYPE_STRING |
| bool | DATATYPE_BOOL |
| char | DATATYPE_CHAR |

**Operators:**
| Lexeme | Token |
|--------|-------|
| = | ASSIGN_OP |
| == | EQUAL_OP |
| != | NOT_EQUAL_OP |
| ! | NOT_OP |
| < | LESSER_OP |
| > | GREATER_OP |
| <= | LEQUAL_OP |
| >= | GEQUAL_OP |
| \|\| | OR_OP |
| && | AND_OP |
| + | ADD_OP |
| - | SUB_OP |
| * | MUL_OP |
| / | DIV_OP |
| % | MOD_OP |
| ** | POW_OP |
| // | INT_DIV_OP |

**Delimiters:**
| Lexeme | Token |
|--------|-------|
| [ | LEFT_BRACKET |
| ] | RIGHT_BRACKET |
| { | LEFT_BRACE |
| } | RIGHT_BRACE |
| ( | LEFT_PAREN |
| ) | RIGHT_PAREN |
| ; | SEMICOLON |
| : | COLON |
| , | COMMA |

**Literals & Identifiers:**
| Type | Token |
|------|-------|
| Variable names | IDENTIFIER |
| Integer literals | INT_LIT |
| Float literals | FLOAT_LIT |
| String literals | STR_LIT |

**Special Tokens:**
| Token | Description |
|-------|-------------|
| END_OF_FILE | End of input |
| UNKNOWN | Unrecognized token |

## Lexer Implementation

The lexer is implemented in C++ with the following key components:

### Token Structure
```cpp
struct Token {
    std::string lexeme;  // The actual text
    TokenType type;      // Token classification
    int line;           // Line number in source
    int column;         // Column position
};
```

### Lexer Class Features
- **File-based input**: Reads source code from files
- **Position tracking**: Tracks line and column numbers for error reporting
- **Lookup tables**: Uses hash maps for efficient keyword and operator recognition
- **String literal support**: Handles both single and double quoted strings
- **Number parsing**: Recognizes integer and floating-point literals
- **Error handling**: Provides detailed error messages with location information

### Key Methods
- `tokenize()`: Main tokenization method that processes the entire file
- `print_tokens()`: Debug utility to display all tokens
- `skip_whitespace()`: Handles whitespace and maintains position tracking
- `read_string_literal()`: Processes quoted string literals
- `read_identifier()`: Handles variable names and keywords
- `read_number()`: Parses numeric literals (int/float)

## Usage

```cpp
Lexer lexer("source_file.turd");
std::vector<Token> tokens = lexer.tokenize();
lexer.print_tokens();  // Debug output
```

## Build Requirements

- C++26 compatible compiler
- Standard library support for:
  - `<string>`
  - `<vector>`
  - `<fstream>`
  - `<unordered_map>`

## Future Development

- Parser implementation for syntax analysis
- AST (Abstract Syntax Tree) generation
- Semantic analysis
- Code generation/interpretation

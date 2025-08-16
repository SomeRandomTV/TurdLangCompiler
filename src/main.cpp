// =============================================================================
// main.cpp - Test driver for the lexer
// =============================================================================
#include <iostream>
#include <fstream>
#include "Lexer/lexer.hpp"

void create_test_file(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << content;
        file.close();
        std::cout << "Created test file: " << filename << std::endl;
    } else {
        std::cerr << "Failed to create test file: " << filename << std::endl;
    }
}

int main() {
    // Create test files with different scenarios
    
    // Test 1: Basic tokens and string literals
    create_test_file("test1.txt", R"(
var x = 42;
var name = "Hello World";
var ch = 'A';
if (x > 10) {
    print("x is greater than 10");
}
)");

    // Test 2: Various operators and numbers
    create_test_file("test2.txt", R"(
var a = 3.14;
var b = 100;
var result = a ** b + (5 * 2);
if (a >= 3.0 && b <= 200) {
    print("Condition met");
}
var division = a // 2;
)");

    // Test 3: String and character escapes
    create_test_file("test3.txt", R"(
var str1 = "Line 1\nLine 2\tTabbed";
var str2 = "Quote: \"Hello\"";
var char1 = '\n';
var char2 = '\'';
var char3 = '\\';
print("Testing escape sequences");
)");

    // Test 4: Functions and keywords
    create_test_file("test4.txt", R"(
function factorial(n) {
    if (n <= 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

var result = factorial(5);
print(result);

while (true) {
    read(input);
    if (input == "quit") break;
}
)");

    // Test 5: Arrays and complex expressions
    create_test_file("test5.txt", R"(
var numbers[10];
for (var i = 0; i < 10; i++) {
    numbers[i] = i * 2;
}

var found = false;
var target = 8;
for (var j = 0; j < 10; j++) {
    if (numbers[j] == target) {
        found = true;
    }
}
)");

    // Test 6: Edge cases and potential errors
    create_test_file("test6.txt", R"(
var empty_string = "";
var single_quote_in_string = "It's working";
var number_with_decimal = 123.456;
var just_decimal = .789;
var operators = ! != == >= <= && || **;
)");

    // Run tests on each file
    std::vector<std::string> test_files = {
        "test1.txt", "test2.txt", "test3.txt", 
        "test4.txt", "test5.txt", "test6.txt"
    };

    for (const auto& filename : test_files) {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "TESTING FILE: " << filename << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        
        try {
            Lexer lexer(filename);
            auto tokens = lexer.tokenize();
            
            std::cout << "Successfully tokenized " << tokens.size() << " tokens:" << std::endl;
            lexer.print_tokens();
            
        } catch (const std::exception& e) {
            std::cerr << "Error processing " << filename << ": " << e.what() << std::endl;
        }
    }

    // Test error cases
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "TESTING ERROR CASES" << std::endl;
    std::cout << std::string(50, '=') << std::endl;

    // Test unterminated string
    create_test_file("error1.txt", R"(var str = "unterminated string)");
    
    // Test unterminated character literal
    create_test_file("error2.txt", R"(var ch = 'unterminated)");
    
    // Test empty character literal
    create_test_file("error3.txt", R"(var ch = '';)");

    std::vector<std::string> error_files = {"error1.txt", "error2.txt", "error3.txt"};
    
    for (const auto& filename : error_files) {
        std::cout << "\nTesting error case: " << filename << std::endl;
        try {
            Lexer lexer(filename);
            auto tokens = lexer.tokenize();
            std::cout << "Unexpected success for " << filename << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Expected error caught: " << e.what() << std::endl;
        }
    }

    return 0;
}


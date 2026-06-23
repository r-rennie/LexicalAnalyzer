// SCW Lexical Analyzer using regex
// Rachael Rennie and Maria Lyons
// Used geeks4geeks lexical analyzer as a starting point and ChatGPT to help figure out how to implement features
// 12/7/25
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <regex>

using namespace std;

// Creating the token types
// Wanted to see how to differentiate between different elements of the same token, so we split it
// up instead of just keeping it to the broad categories
enum class TokenType {
    // Keywords
    IF, ELSE, WHILE, PRINT, INTEGER, FLOAT, GET,

    // Operators
    ADD, SUBTRACT, MULTIPLY, DIVIDE, // basic operators
    ASSIGN, // assignment
    EQUAL, NOT_EQUAL, GREATER_EQUAL, LESS_EQUAL, GREATER, LESS, // comparators

    // Identifiers
    IDENTIFIER,

    // Numbers
    INTEGER_LITERAL, FLOAT_LITERAL,

    // Symbols/Punctuators
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE, SEMICOLON,

    // End / unknown
    END_OF_FILE, UNKNOWN
};

// Create actual tokens
struct Token {
    TokenType type;
    string value;
    Token(TokenType t, const string& v) : type(t), value(v) {}
};

// Converting token type to string for printing
string getTokenTypeName(TokenType type) {
    switch(type) {
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::PRINT: return "PRINT";
        case TokenType::GET: return "GET";
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::ADD: return "ADD";
        case TokenType::SUBTRACT: return "SUBTRACT";
        case TokenType::MULTIPLY: return "MULTIPLY";
        case TokenType::DIVIDE: return "DIVIDE";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::NOT_EQUAL: return "NOT_EQUAL";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::LESS: return "LESS";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::INTEGER_LITERAL: return "INTEGER_LITERAL";
        case TokenType::FLOAT_LITERAL: return "FLOAT_LITERAL";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::END_OF_FILE: return "EOF";
        default: return "UNKNOWN";
    }
}

// Lexical Analyzer class; main functionality implemented here
class LexicalAnalyzer {
private:
    string input; // stores whole source code
    size_t position; // tracks position
    vector<string> errors; // keeps track of any errors to print list at the end
    unordered_map<TokenType, int> tokenCounts; // counts how many tokens
    unordered_map<string, TokenType> keywordMap; // links keywords to token type


    // floatRegex: character before . is optional; accepts inputs like 0.5 and .5 equally
    regex floatRegex = regex(R"([0-9]*\.[0-9]+)");
    regex intRegex = regex(R"([0-9]+)");
    // idRegex: accepts identifiers with underscores
    regex idRegex = regex(R"([A-Za-z_][A-Za-z0-9_]*)");
    regex multiOpRegex = regex(R"(==|!=|<=|>=)");
    regex singleOpRegex = regex(R"([\+\*/=<>-])");
    regex punctRegex = regex(R"([\(\)\{\};])");

    // initializing keywords
    void initKeywords() {
        keywordMap["IF"] = TokenType::IF;
        keywordMap["ELSE"] = TokenType::ELSE;
        keywordMap["WHILE"] = TokenType::WHILE;
        keywordMap["PRINT"] = TokenType::PRINT;
        keywordMap["GET"] = TokenType::GET;
        keywordMap["INTEGER"] = TokenType::INTEGER;
        keywordMap["FLOAT"] = TokenType::FLOAT;
    }

    // converts all input to uppercase so errors aren't thrown based on case sensitivity
    string toUpperCase(const string& s) {
        string res = s;
        for (char &c : res) c = toupper(c);
        return res;
    }

    // counts how many token there are
    void countToken(TokenType t) {
        tokenCounts[t]++;
    }

public:
    // Constructor
    LexicalAnalyzer(const string& src) : input(src), position(0) {
        initKeywords();
    }

    // return list of tokens in the input code
    vector<Token> tokenize() {
        // stores all discovered tokens
        vector<Token> tokens;

        // while there is still code to parse
        while (position < input.size()) {
            continue_loop:

            // identifies space
            if (isspace(input[position])) { position++; continue; }

            // illegal number detection (multiple dots)
            // couldn't really think of any other invalid number input so this is the only implementation
            // we currently have. any other invalid inputs would result in an "unknown" error
            if (isdigit(input[position])) {
                int dotCount = 0;
                size_t temp = position;

                // collects the full number
                while (temp < input.size() && (isdigit(input[temp]) || input[temp] == '.')) {
                    if (input[temp] == '.') dotCount++;
                    // if there is more than one '.' it's ILLEGAL
                    if (dotCount > 1) {
                        errors.push_back("Illegal number format near '" + input.substr(position, temp - position + 1) + "'");
                        tokens.emplace_back(TokenType::UNKNOWN, input.substr(position, temp - position + 1));
                        countToken(TokenType::UNKNOWN);
                        position = temp + 1;
                        goto continue_loop;
                    }
                    temp++;
                }
            }

            // gives remaining portion of input
            string remaining = input.substr(position);
            // stores details of pattern recognized
            smatch match;

            // identifies floats
            if (regex_search(remaining, match, floatRegex) && match.position() == 0) {
                tokens.emplace_back(TokenType::FLOAT_LITERAL, match.str());
                countToken(TokenType::FLOAT_LITERAL);
                position += match.length();
            }

            // identifies integers
            else if (regex_search(remaining, match, intRegex) && match.position() == 0) {
                tokens.emplace_back(TokenType::INTEGER_LITERAL, match.str());
                countToken(TokenType::INTEGER_LITERAL);
                position += match.length();
            }

            // identifies identifiers/keywords
            else if (regex_search(remaining, match, idRegex) && match.position() == 0) {
                string word = match.str();
                string upper = toUpperCase(word);

                // matches against keywords
                if (keywordMap.find(upper) != keywordMap.end()) {
                    tokens.emplace_back(keywordMap[upper], word);
                    countToken(keywordMap[upper]);
                } else {
                    // anything that isn't a keyword is saved as an identifier
                    tokens.emplace_back(TokenType::IDENTIFIER, word);
                    countToken(TokenType::IDENTIFIER);
                }
                position += match.length();
            }

            // identifies multi-character operators
            else if (regex_search(remaining, match, multiOpRegex) && match.position() == 0) {
                string op = match.str();
                if (op == "==") { tokens.emplace_back(TokenType::EQUAL, op); countToken(TokenType::EQUAL); }
                else if (op == "!=") { tokens.emplace_back(TokenType::NOT_EQUAL, op); countToken(TokenType::NOT_EQUAL); }
                else if (op == "<=") { tokens.emplace_back(TokenType::LESS_EQUAL, op); countToken(TokenType::LESS_EQUAL); }
                else if (op == ">=") { tokens.emplace_back(TokenType::GREATER_EQUAL, op); countToken(TokenType::GREATER_EQUAL); }
                position += match.length();
            }

            // identifies single-character operators
            else if (regex_search(remaining, match, singleOpRegex) && match.position() == 0) {
                char op = match.str()[0];
                switch(op) {
                    case '+': tokens.emplace_back(TokenType::ADD, "+"); countToken(TokenType::ADD); break;
                    case '-': tokens.emplace_back(TokenType::SUBTRACT, "-"); countToken(TokenType::SUBTRACT); break;
                    case '*': tokens.emplace_back(TokenType::MULTIPLY, "*"); countToken(TokenType::MULTIPLY); break;
                    case '/': tokens.emplace_back(TokenType::DIVIDE, "/"); countToken(TokenType::DIVIDE); break;
                    case '=': tokens.emplace_back(TokenType::ASSIGN, "="); countToken(TokenType::ASSIGN); break;
                    case '<': tokens.emplace_back(TokenType::LESS, "<"); countToken(TokenType::LESS); break;
                    case '>': tokens.emplace_back(TokenType::GREATER, ">"); countToken(TokenType::GREATER); break;
                }
                position += 1;
            }

            // identifies punctuators (symbols)
            else if (regex_search(remaining, match, punctRegex) && match.position() == 0) {
                char p = match.str()[0];
                switch(p) {
                    case '(': tokens.emplace_back(TokenType::LEFT_PAREN, "("); countToken(TokenType::LEFT_PAREN); break;
                    case ')': tokens.emplace_back(TokenType::RIGHT_PAREN, ")"); countToken(TokenType::RIGHT_PAREN); break;
                    case '{': tokens.emplace_back(TokenType::LEFT_BRACE, "{"); countToken(TokenType::LEFT_BRACE); break;
                    case '}': tokens.emplace_back(TokenType::RIGHT_BRACE, "}"); countToken(TokenType::RIGHT_BRACE); break;
                    case ';': tokens.emplace_back(TokenType::SEMICOLON, ";"); countToken(TokenType::SEMICOLON); break;
                }
                position += 1;
            }
            // identifies unknown characters (not part of the language)
            else {
                errors.push_back("Invalid character '" + string(1, input[position]) + "'");
                tokens.emplace_back(TokenType::UNKNOWN, string(1, input[position]));
                countToken(TokenType::UNKNOWN);
                position++;
            }
        }

        // identifies the end of the file; there are no more tokens
        tokens.emplace_back(TokenType::END_OF_FILE, "");
        countToken(TokenType::END_OF_FILE);
        return tokens;
    }

    // counts number of tokens and errors
    unordered_map<TokenType, int> getTokenCounts() { return tokenCounts; }
    vector<string> getErrors() { return errors; }
};

// categorizing the token type

// keywords
bool isKeyword(TokenType t) {
    switch(t) { case TokenType::IF: case TokenType::ELSE: case TokenType::WHILE:
                 case TokenType::PRINT: case TokenType::GET: case TokenType::INTEGER:
                 case TokenType::FLOAT: return true; default: return false; }
}

// operators
bool isOperator(TokenType t) {
    switch(t) { case TokenType::ADD: case TokenType::SUBTRACT: case TokenType::MULTIPLY:
                 case TokenType::DIVIDE: case TokenType::ASSIGN: case TokenType::EQUAL:
                 case TokenType::NOT_EQUAL: case TokenType::LESS: case TokenType::GREATER:
                 case TokenType::LESS_EQUAL: case TokenType::GREATER_EQUAL: return true;
                 default: return false; }
}

// symbols
bool isPunct(TokenType t) {
    switch(t) { case TokenType::LEFT_PAREN: case TokenType::RIGHT_PAREN:
                 case TokenType::LEFT_BRACE: case TokenType::RIGHT_BRACE:
                 case TokenType::SEMICOLON: return true; default: return false; }
}

// errors
bool isError(TokenType t) { return t == TokenType::UNKNOWN; }

// main body
int main() {

    // test text files are nested in the cmake-build-debug folder
    // there are text files 1-5 to test different scenario inputs
    // hardcoded in; to test different file, update filename input below
    string filename = "text4.txt";
    ifstream file(filename);

    // if file doesn't exist
    if (!file) {
        cerr << "Error: Cannot open file " << filename << endl;
        return 1;
    }

    // reads contents of a file into a single string so the lexer can process it
    stringstream buffer;
    buffer << file.rdbuf();
    string code = buffer.str();

    // runs lexer with input
    LexicalAnalyzer lexer(code);
    vector<Token> tokens = lexer.tokenize();

    // outputting tokens
    for (auto &t : tokens)
        cout << "Token: " << getTokenTypeName(t.type)
             << ", Value: " << t.value << endl;

    // print summary counts
    unordered_map<TokenType,int> counts = lexer.getTokenCounts();

    // initialize each token category
    int totalTokens = tokens.size();
    int countKeywords = 0, countIdentifiers = 0, countIntegers = 0, countFloats = 0;
    int countOperators = 0, countPuncts = 0, countErrors = 0;

    // categorizes and counts tokensit innit
    for (auto &t : tokens) {
        if (isKeyword(t.type)) countKeywords++;
        else if (t.type == TokenType::IDENTIFIER) countIdentifiers++;
        else if (t.type == TokenType::INTEGER_LITERAL) countIntegers++;
        else if (t.type == TokenType::FLOAT_LITERAL) countFloats++;
        else if (isOperator(t.type)) countOperators++;
        else if (isPunct(t.type)) countPuncts++;
        else if (isError(t.type)) countErrors++;
    }

    // prints total and itemized token counts
    cout << "\n===== TOKEN COUNTS =====\n";
    cout << "Total Tokens: " << totalTokens << "\n";
    cout << "Keywords: " << countKeywords << "\n";
    cout << "Identifiers: " << countIdentifiers << "\n";
    cout << "Integer Literals: " << countIntegers << "\n";
    cout << "Float Literals: " << countFloats << "\n";
    cout << "Operators: " << countOperators << "\n";
    cout << "Punctuators: " << countPuncts << "\n";
    cout << "Errors: " << countErrors << "\n";

    // prints error messages
    vector<string> errs = lexer.getErrors();
    if (!errs.empty()) {
        cout << "\n===== ERRORS =====\n";
        for (auto &e : errs) cout << e << endl;
    }

    return 0;
}

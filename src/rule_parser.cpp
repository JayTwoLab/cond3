#include "rule_parser.hpp"

#include <cctype>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

namespace cond3 {

enum class TokenType { Number, And, Or, Not, LParen, RParen, End };

struct Token {
    TokenType type{};
    std::string text{};
};

static std::string to_upper_copy(const std::string& s) {
    std::string r(s);
    std::transform(r.begin(), r.end(), r.begin(), [](unsigned char c){ return static_cast<char>(std::toupper(c)); });
    return r;
}

class Lexer {
public:
    explicit Lexer(const std::string& src) : src_(src), pos_(0) {}

    Token next() {
        skip_ws();
        if (pos_ >= src_.size()) return {TokenType::End, {}};

        char c = src_[pos_];
        if (std::isdigit(static_cast<unsigned char>(c))) {
            std::size_t start = pos_;
            while (pos_ < src_.size() && std::isdigit(static_cast<unsigned char>(src_[pos_]))) ++pos_;
            return {TokenType::Number, src_.substr(start, pos_ - start)};
        }

        if (c == '(') { ++pos_; return {TokenType::LParen, "("}; }
        if (c == ')') { ++pos_; return {TokenType::RParen, ")"}; }

        // read an identifier/keyword
        if (std::isalpha(static_cast<unsigned char>(c))) {
            std::size_t start = pos_;
            while (pos_ < src_.size() && std::isalpha(static_cast<unsigned char>(src_[pos_]))) ++pos_;
            std::string word = src_.substr(start, pos_ - start);
            std::string up = to_upper_copy(word);
            if (up == "AND") return {TokenType::And, word};
            if (up == "OR") return {TokenType::Or, word};
            if (up == "NOT") return {TokenType::Not, word};
            throw std::invalid_argument("unexpected identifier: " + word);
        }

        throw std::invalid_argument(std::string("unexpected character: ") + c);
    }

private:
    void skip_ws() {
        while (pos_ < src_.size() && std::isspace(static_cast<unsigned char>(src_[pos_]))) ++pos_;
    }

    const std::string& src_;
    std::size_t pos_;
};

class Parser {
public:
    explicit Parser(const std::string& s) : lexer_(s) { advance(); }

    rule_node parse() {
        rule_node result = parse_or();
        if (cur_.type != TokenType::End) throw std::invalid_argument("unexpected token after expression");
        return result;
    }

private:
    // OR level (lowest precedence)
    rule_node parse_or() {
        std::vector<rule_node> items;
        items.push_back(parse_and());
        while (cur_.type == TokenType::Or) {
            advance();
            items.push_back(parse_and());
        }
        if (items.size() == 1) return std::move(items.front());
        return rule_node::make_any_of(std::move(items));
    }

    // AND level
    rule_node parse_and() {
        std::vector<rule_node> items;
        items.push_back(parse_unary());
        while (cur_.type == TokenType::And) {
            advance();
            items.push_back(parse_unary());
        }
        if (items.size() == 1) return std::move(items.front());
        return rule_node::make_all_of(std::move(items));
    }

    // Unary: NOT or primary
    rule_node parse_unary() {
        if (cur_.type == TokenType::Not) {
            advance();
            rule_node child = parse_unary();
            return rule_node::make_not(std::move(child));
        }
        return parse_primary();
    }

    // Primary: number or parenthesized expression
    rule_node parse_primary() {
        if (cur_.type == TokenType::Number) {
            // convert number token to uint64_t
            std::uint64_t id = 0;
            try {
                id = static_cast<std::uint64_t>(std::stoull(cur_.text));
            } catch (...) {
                throw std::invalid_argument("invalid condition id: " + cur_.text);
            }
            advance();
            return rule_node::make_leaf(id);
        }

        if (cur_.type == TokenType::LParen) {
            advance();
            rule_node inner = parse_or();
            if (cur_.type != TokenType::RParen) throw std::invalid_argument("expected ')'");
            advance();
            return inner;
        }

        throw std::invalid_argument("expected number or '('");
    }

    void advance() { cur_ = lexer_.next(); }

    Lexer lexer_;
    Token cur_;
};

rule_node parse_rule(const std::string& expr) {
    Parser p(expr);
    return p.parse();
}

} // namespace cond3
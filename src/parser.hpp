#ifndef FLEUR_PARSER_HPP
#define FLEUR_PARSER_HPP

#include <common.hpp>
#include <tokenizer.hpp>

#include <vector>

namespace Fleur
{
    namespace Keyword
    {
        // Note: String literals live in static storage.
        constexpr std::string_view FUNCTION = "fn";
    }

    enum class ASTNodeType : u64
    {
        FUNCTION_PROTOTYPE
    };

    struct ASTNode
    {
        // Allow destruction of derived with base pointer.
        virtual ~ASTNode() = default;

        ASTNodeType type;
    };

    struct ASTNodeFunctionPrototype : ASTNode
    {
        Token const *identifier;
        std::vector<Parser::FunctionParameter> parameterList;
        Token const *returnTypeIdentifier;
    };

    struct ASTNodeFunction : ASTNode
    {
        Token const *identifier;
        std::vector<Parser::FunctionParameter> parameterList;
        Token const *returnTypeIdentifier;
        // TODO: Add body.
    };

    class ParserData
    {
    public:
        ParserData(TokenizerData const *tokenizerData);
        ~ParserData();
        ParserData(ParserData &&other);

        std::vector<ASTNode*> nodes;
        TokenizerData const *tokenizerData;

    private:
        ParserData(ParserData const&) = delete;
        ParserData& operator=(ParserData const&) = delete;
        ParserData& operator=(ParserData&&) = delete;

    };

    enum class ParseStatus : i8
    {
        OK = 0,
        UNEXPECTED = -1,
        END = -2
    };

    class Parser
    {
    public:
        struct FunctionParameter
        {
            Token const *identifier;
            Token const *typeIdentifier;
        };

        ParserData Parse(TokenizerData const *tokenizerData);

    private:
        TokenizerData const *tokenizerData;
        u64 index;
        Token const *token;
        Token const *lookahead;

        // `token` or `lookahead` are set to `nullptr` if we are at the end of the token stream.
        void Eat();

        ParseStatus ParseFunctionBody();
        // Note: `typeIdentifier` and `hasBody` are set iff return value is `OK`.
        ParseStatus ParseFunctionReturnType(Token const **typeIdentifier, bool *hasBody);
        ParseStatus ParseFunctionParameter(FunctionParameter *parameter);
        ParseStatus ParseFunctionParameterList(std::vector<FunctionParameter> *parameterList);
        ParseStatus ParseTopLevelFunction(ParserData *data);
        ParseStatus ParseTopLevel(ParserData *data);
    };
}

#endif
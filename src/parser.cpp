#include <parser.hpp>

// Macros to be used in functions with return type `ParseStatus`.
/*
#define EXPECT_LOOKAHEAD(x)                                \
    if (lookahead == nullptr)                              \
        return Fleur::ParseStatus::END;                    \
    if (lookahead->type != (x))                            \
        return Fleur::ParseStatus::UNEXPECTED;

#define EXPECT_TOKEN(x)                                    \
    if (token == nullptr)                                  \
        return Fleur::ParseStatus::END;                    \
    if (token->type != (x))                                \
        return Fleur::ParseStatus::UNEXPECTED;
*/

namespace Fleur
{
    ParserData::ParserData(TokenizerData const *tokenizerData)
        : nodes(16), // Pick some sensible capacity.
          tokenizerData(tokenizerData)
    {
    }

    ParserData::~ParserData()
    {
        for (ASTNode *node : nodes)
        {
            delete node;
        }
    }

    ParserData::ParserData(ParserData &&other)
    {
    }

    void Parser::Eat()
    {
        [[likely]]
        if (index + 2 < tokenizerData->tokens.size())
        {
            token = &tokenizerData->tokens[index + 1];
            lookahead = &tokenizerData->tokens[index + 2];
            index++;
        }
        else if (index + 1 < tokenizerData->tokens.size())
        {
            token = &tokenizerData->tokens[index + 1];
            lookahead = nullptr;
            index++;
        }
        else
        {
            token = nullptr;
            lookahead = nullptr;
        }
    }

    ParseStatus Parser::ParseFunctionBody()
    {
        // TODO: Implement this.
        return ParseStatus::UNEXPECTED;
    }

    ParseStatus Parser::ParseFunctionReturnType(Token const **typeIdentifier, bool *hasBody)
    {
        if (token->type == TokenType::RIGHT_ARROW) // Return type.
        {
            // Eat right arrow.
            Eat();

            // Eat return type identifier.
            if (token == nullptr) // TODO: Error message.
                return ParseStatus::END;
            if (token->type != TokenType::IDENTIFIER) // TODO: Error message.
                return ParseStatus::UNEXPECTED;

            *typeIdentifier = token;
            Eat();

            if (token == nullptr) // TODO: Error message.
                return ParseStatus::END;
        }

        if (token->type == TokenType::SEMICOLON) // No function body.
        {
            // Eat semicolon.
            Eat();
            *hasBody = false;
            return ParseStatus::OK;
        }
        else if (token->type == TokenType::OPEN_CURLY_BRACKET) // Function body.
        {
            // Eat open curly bracket.
            Eat();
            *hasBody = true;
            return ParseStatus::OK;
        }
        else // Unexpected token.
        {
            // TODO: Error message.
            // Eat unexpected token.
            Eat();
            return ParseStatus::UNEXPECTED;
        }
    }

    ParseStatus Parser::ParseFunctionParameter(FunctionParameter *parameter)
    {
        FunctionParameter _parameter;
        _parameter.identifier = token;

        // Eat function parameter identifier.
        Eat();

        // Expect and eat colon.
        if (token == nullptr) // TODO: Error message.
            return ParseStatus::END;
        if (token->type != TokenType::COLON) // TODO: Error message.
            return ParseStatus::UNEXPECTED;

        Eat();

        // Expect and eat function parameter type identifier.
        if (token == nullptr) // TODO: Error message.
            return ParseStatus::END;
        if (token->type != TokenType::IDENTIFIER) // TODO: Error message.
            return ParseStatus::UNEXPECTED;

        _parameter.typeIdentifier = token;
        Eat();

        *parameter = _parameter;
        return ParseStatus::OK;
    }

    ParseStatus Parser::ParseFunctionParameterList(std::vector<FunctionParameter> *parameterList)
    {
        while (true)
        {
            FunctionParameter parameter;

            ParseStatus status = ParseFunctionParameter(&parameter);
            if (status != ParseStatus::OK)
                return status;

            parameterList->push_back(parameter);

            if (token == nullptr) // TODO: Error message.
                return ParseStatus::END;

            if (token->type == TokenType::COMMA)
            {
                // Eat comma.
                Eat();

                // Expect function parameter identifier.
                if (token == nullptr) // TODO: Error message.
                    return ParseStatus::END;
                if (token->type != TokenType::IDENTIFIER) // TODO: Error message.
                    return ParseStatus::UNEXPECTED;
            }
            else if (token->type == TokenType::CLOSE_ROUND_BRACKET)
            {
                // Eat close round bracket.
                Eat();
                return ParseStatus::OK;
            }
            else // Unexpected token.
            {
                // TODO: Error message.
                // Eat unexpected token.
                Eat();
                return ParseStatus::UNEXPECTED;
            }
        }
    }

    ParseStatus Parser::ParseTopLevelFunction(ParserData *data)
    {
        Token const *identifier;
        std::vector<FunctionParameter> parameterList;
        Token const *returnTypeIdentifier;
        bool hasBody;

        // Eat function keyword.
        Eat();

        // Expect and eat function identifier.
        if (token == nullptr) // TODO: Error message.
            return ParseStatus::END;
        if (token->type != TokenType::IDENTIFIER) // TODO: Error message.
            return ParseStatus::UNEXPECTED;

        identifier = token;
        Eat();

        // Expect and eat open round bracket.
        if (token == nullptr) // TODO: Error message.
            return ParseStatus::END;
        if (token->type != TokenType::OPEN_ROUND_BRACKET) // TODO: Error message.
            return ParseStatus::UNEXPECTED;

        Eat();

        if (token == nullptr) // TODO: Error message.
            return ParseStatus::END;

        if (token->type == TokenType::CLOSE_ROUND_BRACKET) // No parameter list.
        {
            // Eat close round bracket.
            Eat();
        }
        else if (token->type == TokenType::IDENTIFIER) // Parameter list.
        {
            // Eat parameter list and close round bracket.
            ParseStatus status = ParseFunctionParameterList(&parameterList);
            if (status != ParseStatus::OK)
                return status;
        }
        else // Unexpected token.
        {
            // TODO: Error message.
            // Eat unexpected token.
            Eat();
            return ParseStatus::UNEXPECTED;
        }

        if (token == nullptr) // TODO: Error message.
            return ParseStatus::END;

        ParseStatus status = ParseFunctionReturnType(&returnTypeIdentifier, &hasBody);
        if (status != ParseStatus::OK)
            return status;

        if (hasBody)
        {
            status = ParseFunctionBody(); // TODO: Parse function body.
            return status;
        }
        else
        {
            auto *node = new ASTNodeFunctionPrototype;

            node->type = ASTNodeType::FUNCTION_PROTOTYPE;
            node->identifier = identifier;
            node->parameterList = std::move(parameterList);
            node->returnTypeIdentifier = returnTypeIdentifier;

            data->nodes.push_back(node);
        }
    }

    ParseStatus Parser::ParseTopLevel(ParserData *data)
    {
        if (token->type == TokenType::IDENTIFIER)
        {
            if (token->string == Keyword::FUNCTION)
            {
                ParseTopLevelFunction(data);
                return ParseStatus::OK;
            }
        }

        // TODO: Error message.
        // Eat unexpected token.
        Eat();
        return ParseStatus::UNEXPECTED;
    }

    ParserData Parser::Parse(TokenizerData const *tokenizerData)
    {
        ParserData data{ tokenizerData };

        this->tokenizerData = tokenizerData;
        index = static_cast<u64>(-1);

        // Get first token.
        // No need to check if it's the END because
        // we only invoke the parser with non-empty TokenizerData.
        Eat();

        while (true)
        {
            ParseStatus status = ParseTopLevel(&data);
            if (status == ParseStatus::END)
                break;
        }

        return data;
    }
}

/*
#undef EXPECT_TOKEN(x)
#undef EXPECT_LOOKAHEAD(x)
*/
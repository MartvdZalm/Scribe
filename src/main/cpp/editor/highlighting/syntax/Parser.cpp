#include "editor/highlighting/syntax/Parser.hpp"

Parser::Parser(const std::string& filePath) : filePath(filePath)
{
    FILE* file = fopen(filePath.c_str(), "r");

    if (!file) {
        std::cerr << "File Not Found" << std::endl;
        return;
    }

    char buffer[65536];
    rapidjson::FileReadStream inputStream(file, buffer, sizeof(buffer));
    document.ParseStream(inputStream);

    fclose(file);

    if (document.HasParseError()) {
        std::cerr << "Error offeset: " << document.GetErrorOffset() << std::endl;
        std::cerr << "Parse error: " <<  rapidjson::GetParseErrorFunc(document.GetParseError()) << std::endl;
    }
}

Parser::~Parser() {}

bool Parser::parse()
{
    if (!document.IsObject()) {
        std::cerr << "Invalid JSON format" << std::endl;
        return false;
    }

    if (document.HasMember("keywords") && document["keywords"].IsArray()) {
        const auto& keywords = document["keywords"];
        
        for (rapidjson::SizeType i = 0; i < keywords.Size(); i++) {
            keywordSet.insert(keywords[i].GetString());
        }
    }

    if (document.HasMember("types") && document["types"].IsArray()) {
        const auto& types = document["types"];

        for (rapidjson::SizeType i = 0; i < types.Size(); i++) {
            typeSet.insert(types[i].GetString());
        }
    }

    if (document.HasMember("comments") && document["comments"].IsObject()) {
        const auto& comments = document["comments"];

        if (comments.HasMember("line_comment") && comments["line_comment"].IsString()) {
            lineCommentStart = comments["line_comment"].GetString();
        }

        if (comments.HasMember("block_comment_start") && comments["block_comment_start"].IsString()) {
            blockCommentStart = comments["block_comment_start"].GetString();
        }

        if (comments.HasMember("block_comment_end") && comments["block_comment_end"].IsString()) {
            blockCommentEnd = comments["block_comment_end"].GetString();
        }
    }

    if (document.HasMember("strings") && document["strings"].IsArray()) {
        const auto& strings = document["strings"];

        for (rapidjson::SizeType i = 0; i < strings.Size(); i++) {
            stringDelimiters.push_back(strings[i].GetString());
        }
    }

    return true;
}
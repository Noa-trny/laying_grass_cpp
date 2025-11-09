#include "../include/TileParser.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>

static std::string readFileToString(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Impossible d'ouvrir: " + path);
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

// utilitaire pour trouver le ']' correspondant a un '[' (gere imbrication)
static size_t findMatchingBracket(const std::string& s, size_t openPos) {
    if (openPos >= s.size() || s[openPos] != '[') return std::string::npos;
    int depth = 0;
    for (size_t i = openPos; i < s.size(); ++i) {
        if (s[i] == '[') ++depth;
        else if (s[i] == ']') {
            --depth;
            if (depth == 0) return i;
        }
    }
    return std::string::npos;
}

namespace TileParser {

std::vector<Tile> loadTilesFromJson(const std::string& filepath) {
    std::string content = readFileToString(filepath);
    std::vector<Tile> result;

    size_t pos = 0;
    while (true) {
        pos = content.find('{', pos);
        if (pos == std::string::npos) break;
        size_t end = content.find('}', pos);
        if (end == std::string::npos) break;
        std::string obj = content.substr(pos, end - pos + 1);

        int id = -1;
        size_t idPos = obj.find("\"id\"");
        if (idPos != std::string::npos) {
            size_t colon = obj.find(':', idPos);
            if (colon != std::string::npos) {
                std::stringstream ss(obj.substr(colon+1));
                ss >> id;
            }
        }

        std::vector<std::vector<bool>> shape;
        size_t shapePos = obj.find("\"shape\"");
        if (shapePos != std::string::npos) {
            size_t arrStart = obj.find('[', shapePos);
            if (arrStart != std::string::npos) {
                size_t arrEnd = findMatchingBracket(obj, arrStart);
                if (arrEnd != std::string::npos) {
                    std::string arr = obj.substr(arrStart, arrEnd - arrStart + 1);
                    size_t rpos = 0;
                    while (true) {
                        rpos = arr.find('[', rpos);
                        if (rpos == std::string::npos) break;
                        if (rpos == 0) { rpos = rpos + 1; continue; }
                        size_t rend = findMatchingBracket(arr, rpos);
                        if (rend == std::string::npos) break;
                        std::string row = arr.substr(rpos+1, rend - rpos - 1);
                        std::vector<bool> rowVec;
                        std::stringstream rss(row);
                        int val;
                        char ch;
                        while (rss >> std::ws) {
                            if (!(rss >> val)) break;
                            rowVec.push_back(val != 0);
                            rss >> std::ws;
                            if (rss.peek() == ',') rss.get(ch);
                        }
                        if (!rowVec.empty()) shape.push_back(rowVec);
                        rpos = rend + 1;
                    }
                }
            }
        }

        if (id >= 0 && !shape.empty()) {
            result.emplace_back(id, shape);
        }
        pos = end + 1;
    }

    if (result.empty()) throw std::runtime_error("Aucune tuile chargee depuis " + filepath);
    return result;
}

} // namespace TileParser

#include "../include/GameUtils.hpp"
#include <random>
#include <algorithm>
#include <iostream>

namespace GameUtils {

std::vector<Tile> pickRandomTiles(const std::vector<Tile>& available, int n) {
    std::vector<Tile> result;
    if (available.empty() || n <= 0) return result;
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<int> indices(available.size());
    for (size_t i = 0; i < indices.size(); ++i) indices[i] = static_cast<int>(i);
    std::shuffle(indices.begin(), indices.end(), g);
    for (int i = 0; i < n && i < static_cast<int>(indices.size()); ++i) {
        result.push_back(available[indices[i]]);
    }
    return result;
}

void showTileConsole(const Tile& tile) {
    int h = tile.getHeight();
    int w = tile.getWidth();
    std::cout << "Tuile #" << tile.getId() << "\n";
    auto shape = tile.getShape();
    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            bool cell = false;
            if (r >= 0 && r < static_cast<int>(shape.size()) && c >= 0 && c < static_cast<int>(shape[r].size())) {
                cell = shape[r][c];
            }
            std::cout << (cell ? '#' : '.');
        }
        std::cout << '\n';
    }
}

void showTileConsoleColored(const Tile& tile, const std::string& color, const std::string& reset) {
    int h = tile.getHeight();
    int w = tile.getWidth();
    std::cout << color << "Tuile #" << tile.getId() << reset << "\n";
    auto shape = tile.getShape();
    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            bool cell = false;
            if (r >= 0 && r < static_cast<int>(shape.size()) && c >= 0 && c < static_cast<int>(shape[r].size())) {
                cell = shape[r][c];
            }
            if (cell) std::cout << color << '#' << reset;
            else std::cout << '.';
        }
        std::cout << '\n';
    }
}

void showTileConsoleColored(const Tile& tile, const std::string& color, const std::string& reset, int displayNumber) {
    int h = tile.getHeight();
    int w = tile.getWidth();
    if (displayNumber >= 0) std::cout << color << "(" << displayNumber << ")" << reset << "\n";
    else std::cout << color << "Tuile #" << tile.getId() << reset << "\n";
    auto shape = tile.getShape();
    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            bool cell = false;
            if (r >= 0 && r < static_cast<int>(shape.size()) && c >= 0 && c < static_cast<int>(shape[r].size())) {
                cell = shape[r][c];
            }
            if (cell) std::cout << color << '#' << reset;
            else std::cout << '.';
        }
        std::cout << '\n';
    }
}

void showTilesConsole(const std::vector<Tile>& tiles) {
    if (tiles.empty()) return;
    const int gap = 3;
    int count = std::min(3, static_cast<int>(tiles.size()));

    std::vector<int> widths(count), heights(count), displayWidths(count);
    std::vector<std::vector<std::string>> rows(count);
    int maxHeight = 0;

    for (int i = 0; i < count; ++i) {
        auto shape = tiles[i].getShape();
        int h = static_cast<int>(shape.size());
        int w = 0;
        for (const auto& r : shape) w = std::max(w, static_cast<int>(r.size()));
        if (h == 0) h = 1;
        if (w == 0) w = 1;
        widths[i] = w;
        heights[i] = h;
        std::string label = "Tuile #" + std::to_string(tiles[i].getId());
        displayWidths[i] = std::max(widths[i], static_cast<int>(label.size()));
        maxHeight = std::max(maxHeight, heights[i]);

        rows[i].reserve(heights[i]);
        for (int r = 0; r < static_cast<int>(shape.size()); ++r) {
            std::string line;
            line.reserve(displayWidths[i]);
            for (int c = 0; c < widths[i]; ++c) {
                bool cell = false;
                if (r >= 0 && r < static_cast<int>(shape.size()) && c >= 0 && c < static_cast<int>(shape[r].size())) {
                    cell = shape[r][c];
                }
                line.push_back(cell ? '#' : '.');
            }
            if (static_cast<int>(line.size()) < displayWidths[i])
                line.append(displayWidths[i] - static_cast<int>(line.size()), '.');
            rows[i].push_back(line);
        }
        for (int r = static_cast<int>(shape.size()); r < heights[i]; ++r) {
            rows[i].emplace_back(displayWidths[i], '.');
        }
    }

    for (int i = 0; i < count; ++i) {
        std::string label = "Tuile #" + std::to_string(tiles[i].getId());
        int padTotal = displayWidths[i] - static_cast<int>(label.size());
        int padLeft = padTotal > 0 ? padTotal / 2 : 0;
        int padRight = padTotal > 0 ? padTotal - padLeft : 0;
        std::cout << std::string(padLeft, ' ') << label << std::string(padRight, ' ');
        if (i + 1 < count) std::cout << std::string(gap, ' ');
    }
    std::cout << '\n';

    for (int r = 0; r < maxHeight; ++r) {
        for (int i = 0; i < count; ++i) {
            if (r < heights[i]) std::cout << rows[i][r];
            else std::cout << std::string(displayWidths[i], '.');
            if (i + 1 < count) std::cout << std::string(gap, ' ');
        }
        std::cout << '\n';
    }

    std::cout << '\n';
}

void showTilesConsoleColored(const std::vector<Tile>& tiles, const std::string& color, const std::string& reset) {
    if (tiles.empty()) return;
    const int gap = 3;
    int count = std::min(3, static_cast<int>(tiles.size()));

    std::vector<int> widths(count), heights(count), displayWidths(count);
    std::vector<std::vector<std::string>> rows(count);
    int maxHeight = 0;

    for (int i = 0; i < count; ++i) {
        auto shape = tiles[i].getShape();
        int h = static_cast<int>(shape.size());
        int w = 0;
        for (const auto& r : shape) w = std::max(w, static_cast<int>(r.size()));
        if (h == 0) h = 1;
        if (w == 0) w = 1;
        widths[i] = w;
        heights[i] = h;
        std::string label = "Tuile #" + std::to_string(tiles[i].getId());
        displayWidths[i] = std::max(widths[i], static_cast<int>(label.size()));
        maxHeight = std::max(maxHeight, heights[i]);

        rows[i].reserve(heights[i]);
        for (int r = 0; r < static_cast<int>(shape.size()); ++r) {
            std::string line;
            line.reserve(displayWidths[i]);
            for (int c = 0; c < widths[i]; ++c) {
                bool cell = false;
                if (r >= 0 && r < static_cast<int>(shape.size()) && c >= 0 && c < static_cast<int>(shape[r].size())) {
                    cell = shape[r][c];
                }
                line.push_back(cell ? '#' : '.');
            }
            if (static_cast<int>(line.size()) < displayWidths[i])
                line.append(displayWidths[i] - static_cast<int>(line.size()), '.');
            rows[i].push_back(line);
        }
        for (int r = static_cast<int>(shape.size()); r < heights[i]; ++r) {
            rows[i].emplace_back(displayWidths[i], '.');
        }
    }

    for (int i = 0; i < count; ++i) {
        std::string label = "Tuile #" + std::to_string(tiles[i].getId());
        int padTotal = displayWidths[i] - static_cast<int>(label.size());
        int padLeft = padTotal > 0 ? padTotal / 2 : 0;
        int padRight = padTotal > 0 ? padTotal - padLeft : 0;
        std::cout << std::string(padLeft, ' ') << color << label << reset << std::string(padRight, ' ');
        if (i + 1 < count) std::cout << std::string(gap, ' ');
    }
    std::cout << '\n';

    for (int r = 0; r < maxHeight; ++r) {
        for (int i = 0; i < count; ++i) {
            if (r < heights[i]) {
                // print each char with color if '#'
                const std::string &line = rows[i][r];
                for (char ch : line) {
                    if (ch == '#') std::cout << color << ch << reset;
                    else std::cout << ch;
                }
            } else std::cout << std::string(displayWidths[i], '.');
            if (i + 1 < count) std::cout << std::string(gap, ' ');
        }
        std::cout << '\n';
    }

    std::cout << '\n';
}

void showTilesConsoleColored(const std::vector<Tile>& tiles, const std::string& color, const std::string& reset, const std::vector<int>& displayNumbers) {
    if (tiles.empty()) return;
    const int gap = 3;
    int count = std::min(3, static_cast<int>(tiles.size()));

    std::vector<int> widths(count), heights(count), displayWidths(count);
    std::vector<std::vector<std::string>> rows(count);
    int maxHeight = 0;

    for (int i = 0; i < count; ++i) {
        auto shape = tiles[i].getShape();
        int h = static_cast<int>(shape.size());
        int w = 0;
        for (const auto& r : shape) w = std::max(w, static_cast<int>(r.size()));
        if (h == 0) h = 1;
        if (w == 0) w = 1;
        widths[i] = w;
        heights[i] = h;
        std::string label;
        if (!displayNumbers.empty() && i < static_cast<int>(displayNumbers.size())) label = "(" + std::to_string(displayNumbers[i]) + ")";
        else label = "Tuile #" + std::to_string(tiles[i].getId());
        displayWidths[i] = std::max(widths[i], static_cast<int>(label.size()));
        maxHeight = std::max(maxHeight, heights[i]);

        rows[i].reserve(heights[i]);
        for (int r = 0; r < static_cast<int>(shape.size()); ++r) {
            std::string line;
            line.reserve(displayWidths[i]);
            for (int c = 0; c < widths[i]; ++c) {
                bool cell = false;
                if (r >= 0 && r < static_cast<int>(shape.size()) && c >= 0 && c < static_cast<int>(shape[r].size())) {
                    cell = shape[r][c];
                }
                line.push_back(cell ? '#' : '.');
            }
            if (static_cast<int>(line.size()) < displayWidths[i])
                line.append(displayWidths[i] - static_cast<int>(line.size()), '.');
            rows[i].push_back(line);
        }
        for (int r = static_cast<int>(shape.size()); r < heights[i]; ++r) {
            rows[i].emplace_back(displayWidths[i], '.');
        }
    }

    for (int i = 0; i < count; ++i) {
        std::string label;
        if (!displayNumbers.empty() && i < static_cast<int>(displayNumbers.size())) label = "(" + std::to_string(displayNumbers[i]) + ")";
        else label = "Tuile #" + std::to_string(tiles[i].getId());
        int padTotal = displayWidths[i] - static_cast<int>(label.size());
        int padLeft = padTotal > 0 ? padTotal / 2 : 0;
        int padRight = padTotal > 0 ? padTotal - padLeft : 0;
        std::cout << std::string(padLeft, ' ') << color << label << reset << std::string(padRight, ' ');
        if (i + 1 < count) std::cout << std::string(gap, ' ');
    }
    std::cout << '\n';

    for (int r = 0; r < maxHeight; ++r) {
        for (int i = 0; i < count; ++i) {
            if (r < heights[i]) {
                const std::string &line = rows[i][r];
                for (char ch : line) {
                    if (ch == '#') std::cout << color << ch << reset;
                    else std::cout << ch;
                }
            } else std::cout << std::string(displayWidths[i], '.');
            if (i + 1 < count) std::cout << std::string(gap, ' ');
        }
        std::cout << '\n';
    }

    std::cout << '\n';
}

void showBoardWithOverlay(const Board& board, const Tile& tile, Position pos, const std::string& overlayColor, const std::string& reset) {
    int size = board.getSize();
    // header letters
    std::cout << "   ";
    for (int col = 0; col < size; ++col) {
        std::cout << static_cast<char>('A' + (col % 26));
    }
    std::cout << '\n';

    auto shape = tile.getShape();
    int th = static_cast<int>(shape.size());
    int tw = th ? static_cast<int>(shape[0].size()) : 0;

    for (int row = 0; row < size; ++row) {
        std::cout << static_cast<char>('A' + (row % 26)) << "  ";
        for (int col = 0; col < size; ++col) {
            // check if overlay covers this cell
            int tr = row - pos.row;
            int tc = col - pos.col;
            bool overlayCell = false;
            if (tr >= 0 && tr < th && tc >= 0 && tc < tw) {
                if (static_cast<int>(shape.size()) > tr && static_cast<int>(shape[tr].size()) > tc) {
                    overlayCell = shape[tr][tc];
                }
            }
            if (overlayCell) {
                std::cout << overlayColor << '#' << reset;
            } else {
                int cell = board.getCell(row, col);
                if (cell == 0) std::cout << '.';
                else std::cout << '#';
            }
        }
        std::cout << '\n';
    }
}

} // namespace GameUtils

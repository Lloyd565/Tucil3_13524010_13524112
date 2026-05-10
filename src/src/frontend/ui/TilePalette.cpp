#include "frontend/ui/TilePalette.hpp"

#include <cctype>

static Color getTileColor(char tile) {
    if (tile == 'X') return Color{10, 10, 12, 255};
    if (tile == 'L') return Color{220, 45, 45, 255};
    if (tile == 'O') return Color{24, 150, 48, 255};
    if (tile == 'N') return Color{250, 210, 60, 255};
    if (isdigit(static_cast<unsigned char>(tile))) return Color{250, 210, 60, 255};

    return WHITE;
}

static char getTileAtIndex(int index) {
    const char tiles[] = {'*', 'X', 'L', 'N', 'O', 'Z'};
    return tiles[index];
}

static const char* getLabelAtIndex(int index) {
    const char* labels[] = {"Floor", "Wall", "Lava", "Number", "Goal", "Player"};
    return labels[index];
}

static void drawPlayerSwatch(Rectangle swatchBounds) {
    const float centerX = swatchBounds.x + swatchBounds.width / 2.0f;
    const float centerY = swatchBounds.y + swatchBounds.height / 2.0f;
    const float radius = swatchBounds.width * 0.26f;

    DrawRectangleRec(swatchBounds, WHITE);
    DrawCircle(static_cast<int>(centerX), static_cast<int>(centerY), radius, Color{33, 125, 220, 255});
    DrawCircle(static_cast<int>(centerX), static_cast<int>(centerY - radius * 0.15f), radius * 0.72f, Color{245, 248, 255, 255});
    DrawCircle(static_cast<int>(centerX - radius * 0.22f), static_cast<int>(centerY - radius * 0.18f), radius * 0.09f, Color{12, 24, 38, 255});
    DrawCircle(static_cast<int>(centerX + radius * 0.22f), static_cast<int>(centerY - radius * 0.18f), radius * 0.09f, Color{12, 24, 38, 255});
    DrawTriangle(
        Vector2{centerX + radius * 0.55f, centerY - radius * 0.9f},
        Vector2{centerX + radius * 1.0f, centerY - radius * 0.25f},
        Vector2{centerX + radius * 0.35f, centerY - radius * 0.36f},
        Color{255, 132, 64, 255}
    );
}

TilePalette::TilePalette(Rectangle bounds)
    : bounds(bounds) {}

Rectangle TilePalette::getTileBounds(int index) const {
    return Rectangle{
        bounds.x + 28.0f,
        bounds.y + 82.0f + index * 72.0f,
        bounds.width - 56.0f,
        54.0f
    };
}

char TilePalette::getClickedTile() const {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return '\0';

    for (int i = 0 ; i < 6 ; i++) {
        if (CheckCollisionPointRec(GetMousePosition(), getTileBounds(i))) {
            return getTileAtIndex(i);
        }
    }

    return '\0';
}

void TilePalette::draw(char selectedTile) const {
    const char* title = "Tiles";
    const int titleFontSize = 28;
    const int titleWidth = MeasureText(title, titleFontSize);

    DrawRectangleRounded(bounds, 0.06f, 12, Color{34, 38, 48, 255});
    DrawRectangleRoundedLines(bounds, 0.06f, 12, Color{85, 94, 112, 255});
    DrawText(
        title,
        static_cast<int>(bounds.x + (bounds.width - titleWidth) / 2.0f),
        static_cast<int>(bounds.y + 30.0f),
        titleFontSize,
        Color{246, 248, 252, 255}
    );

    for (int i = 0 ; i < 6 ; i++) {
        drawTileOption(getLabelAtIndex(i), getTileAtIndex(i), i, selectedTile);
    }
}

void TilePalette::drawTileOption(const char* label, char tile, int index, char selectedTile) const {
    const Rectangle optionBounds = getTileBounds(index);
    const Rectangle swatchBounds = {
        optionBounds.x + 14.0f,
        optionBounds.y + 10.0f,
        34.0f,
        34.0f
    };
    const bool isSelected = tile == selectedTile;
    const bool isHovered = CheckCollisionPointRec(GetMousePosition(), optionBounds);
    const Color fillColor = isSelected ? Color{73, 88, 116, 255} :
                            isHovered ? Color{45, 51, 64, 255} :
                                        Color{39, 44, 56, 255};

    DrawRectangleRounded(optionBounds, 0.12f, 8, fillColor);
    DrawRectangleRoundedLines(optionBounds, 0.12f, 8, Color{91, 102, 124, 255});
    if (tile == 'Z') drawPlayerSwatch(swatchBounds);
    else DrawRectangleRec(swatchBounds, getTileColor(tile));

    DrawRectangleLinesEx(swatchBounds, 1.0f, Color{160, 168, 184, 255});
    DrawText(label, static_cast<int>(optionBounds.x + 62.0f), static_cast<int>(optionBounds.y + 16.0f), 22, Color{246, 248, 252, 255});

    if (tile == 'N') {
        DrawText("#", static_cast<int>(swatchBounds.x + 10.0f), static_cast<int>(swatchBounds.y + 7.0f), 20, Color{24, 29, 39, 255});
    }
    else if (isdigit(static_cast<unsigned char>(tile))) {
        const char text[2] = {tile, '\0'};
        DrawText(text, static_cast<int>(swatchBounds.x + 10.0f), static_cast<int>(swatchBounds.y + 7.0f), 20, Color{24, 29, 39, 255});
    }
}

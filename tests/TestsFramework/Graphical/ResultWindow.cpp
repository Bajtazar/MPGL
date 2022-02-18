#include "ResultWindow.hpp"

#include <ranges>

namespace mpgl::tests {

    ResultWindow::TextVector
        ResultWindow::generateHeader(Font& font) noexcept
    {
        return TextVector{
            Text{font, 18, 20_x + getHeight(0),  "Test name",
                Color::Black, Font::Type::BoldItalic},
            Text{font, 18, 200_x + getHeight(0), "Passed tests",
                Color::Black, Font::Type::BoldItalic},
            Text{font, 18, 400_x + getHeight(0),  "Failed tests",
                Color::Black, Font::Type::BoldItalic},
            Text{font, 18, 580_x + getHeight(0),  "Status",
                Color::Black, Font::Type::BoldItalic}
        };
    }

    ResultWindow::ResultWindow(Font& font,
        Results const& results) noexcept : columns{4, TextVector{}},
            header{generateHeader(font)}, begin{0}, end{
            clampEnd(0, results.size())}
    {
        uint i = 1;
        for (auto const& [name, positives, all] : results) {
            addName(font, i, name);
            addPassed(font, i, positives);
            addFailed(font, i, all - positives);
            addStatus(font, i, positives == all);
            ++i;
        }
    }

    Vector2u ResultWindow::getHeight(uint index) noexcept {
        return 1_y * context.windowDimensions[1] - 20_y * (
            1 + index);
    }

    void ResultWindow::addName(Font& font, uint index,
        std::string const& name) noexcept
    {
        columns[Names].emplace_back(font, 18,
            20_x + getHeight(index), name,
            Color::Black);
    }

    void ResultWindow::addPassed(Font& font, uint index,
        std::size_t passed) noexcept
    {
        columns[Passed].emplace_back(font, 18,
            200_x + getHeight(index), std::to_string(passed),
            Color::Green);
    }

    void ResultWindow::addFailed(Font& font, uint index,
        std::size_t failed) noexcept
    {
        columns[Failed].emplace_back(font, 18,
            400_x + getHeight(index), std::to_string(failed),
            Color::Red, Font::Type::Bold);
    }

    void ResultWindow::addStatus(Font& font, uint index,
        bool status) noexcept
    {
        columns[Status].emplace_back(font, 18,
            580_x + getHeight(index), status ? "Passed" : "Failed",
            status ? Color::Green : Color::Red, Font::Type::BoldItalic);
    }

    void ResultWindow::draw(void) const noexcept {
        for (auto const& column : columns)
            column.draw(begin, end);
        header.draw();
    }

    void ResultWindow::copyToGPU(void) const noexcept {
        columns.copyToGPU();
        header.copyToGPU();
    }

    std::size_t ResultWindow::clampEnd(uint begin, uint max) noexcept {
        return std::min(begin + std::max(context.windowDimensions[1]
            / 20u, 4u), max);
    }

    void ResultWindow::onScreenTransformation(
        Vector2u const& oldDimensions) noexcept
    {
        int32 drop = (int32) context.windowDimensions[1]
            - oldDimensions[1];
        columns.onScreenTransformation(oldDimensions);
        header.onScreenTransformation(oldDimensions);
        auto lambda = [&drop](auto& text){ text.translate({0, drop}); };
        std::ranges::for_each(columns | std::views::join, lambda);
        std::ranges::for_each(header, lambda);
        end = clampEnd(begin, columns.front().size());
    }

}

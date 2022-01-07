#pragma once

#include "../../../src/Engine/2DGraphics.hpp"
#include "../Tester.hpp"

namespace ge::tests {

    class ResultWindow : public Drawable,
        public ScreenTransformationEvent
    {
    public:
        typedef typename Tester::ResultVector   Results;

        explicit ResultWindow(Font& font,
            Results const& results) noexcept;

        void copyToGPU(void) const noexcept final;
        void draw(void) const noexcept final;

        void onScreenTransformation(
            Vector2u const& oldDimmensions) noexcept final;
    private:
        typedef DrawableCollection<Text<>>      TextVector;
        typedef DrawableCollection<TextVector>  Columns;

        enum {
            Names =                             0x00,
            Passed =                            0x01,
            Failed =                            0x02,
            Status =                            0x03
        };

        void addName(Font& font, uint index,
            std::string const& name) noexcept;
        void addPassed(Font& font, uint index,
            std::size_t passed) noexcept;
        void addFailed(Font& font, uint index,
            std::size_t failed) noexcept;
        void addStatus(Font& font, uint index,
            bool status) noexcept;

        static Vector2u getHeight(uint index) noexcept;
        static TextVector generateHeader(Font& font) noexcept;
        static std::size_t clampEnd(uint begin, uint max) noexcept;

        Columns                                 columns;
        TextVector                              header;
        std::size_t                             begin;
        std::size_t                             end;
    };

}

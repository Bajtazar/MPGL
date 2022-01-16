#pragma once

#include "../../Traits/Types.hpp"

namespace ge {

    struct Options {
        explicit Options(
            uint16 openGlMajorVersion = 3,
            uint16 openGlMinorVersion = 3,
            uint8 antiAliasingSamples = 4,
            bool floating = false,
            bool maximised = false,
            bool resizable = true,
            bool clean = false);

        uint16                          openGLMajorVersion;
        uint16                          openGLMinorVersion;
        uint8                           antiAliasingSamples : 4;
        bool                            floating : 1;
        bool                            maximised : 1;
        bool                            resizable : 1;
        bool                            clean : 1;
        // bool                            transparent : 1;
    };

}

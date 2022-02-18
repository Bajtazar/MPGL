#pragma once

#include "../../Traits/Types.hpp"

namespace mpgl {

    struct Options {
        uint16              openGLMajorVersion        = 3;
        uint16              openGLMinorVersion        = 3;
        uint8               antiAliasingSamples : 4   = 4;
        bool                floating : 1              = false;
        bool                maximised : 1             = false;
        bool                resizable : 1             = true;
        bool                clean : 1                 = false;
        // bool                            transparent : 1;
    };

}

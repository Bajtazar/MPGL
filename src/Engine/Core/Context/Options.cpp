#include "Options.hpp"

namespace ge {

    Options::Options(uint16 openGLMajorVersion,
        uint16 openGLMinorVersion, uint8 antiAliasingSamples,
        bool floating, bool maximised, bool resizable,
        bool clean) noexcept
            : openGLMajorVersion{openGLMajorVersion},
            openGLMinorVersion{openGLMinorVersion},
            antiAliasingSamples{antiAliasingSamples},
            floating{floating}, maximised{maximised},
            resizable{resizable}, clean{clean}
    {}

}

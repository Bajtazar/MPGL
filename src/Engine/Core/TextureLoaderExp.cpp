#include "TextureLoaderExp.hpp"

namespace ge {

    void TextureLoaderBase::tryLoad(void) {
        if (loadingStatus() != 1.)
            load();
    }

    void TextureLoaderBase::loadAll(void) {
        while (loadingStatus() != 1.)
            load();
    }

}

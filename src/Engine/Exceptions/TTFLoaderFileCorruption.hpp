#pragma once

#include <exception>
#include <string>

namespace ge {

    class TTFLoaderFileCorruption : public std::exception {
    public:
        explicit TTFLoaderFileCorruption(std::string const& fileName) noexcept : std::exception{},
            message{fileName + " - file is corrupted"} {}
        virtual const char* what (void) const noexcept final { return message.c_str(); }
    private:
        std::string message;
    };

}
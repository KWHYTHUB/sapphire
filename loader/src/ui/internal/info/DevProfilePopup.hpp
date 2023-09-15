#pragma once

#include <Sapphire/ui/Popup.hpp>

using namespace sapphire::prelude;

class DevProfilePopup : public Popup<std::string const&> {
protected:
    bool setup(std::string const& developer) override;

public:
    static DevProfilePopup* create(std::string const& developer);
};

#include <Sapphire/DefaultInclude.hpp>

#ifdef GEODE_IS_ANDROID

using namespace sapphire::prelude;

#include <Sapphire/utils/cocos.hpp>
#include <Sapphire/loader/Dirs.hpp>
#include <Sapphire/utils/web.hpp>
#include <ghc/filesystem.hpp>

ghc::filesystem::path dirs::getGameDir() {
    return ghc::filesystem::path(CCFileUtils::sharedFileUtils()->getWritablePath().c_str());
}

ghc::filesystem::path dirs::getSaveDir() {
    return ghc::filesystem::path(CCFileUtils::sharedFileUtils()->getWritablePath().c_str());
}

void utils::web::openLinkInBrowser(std::string const& url) {
    CCApplication::sharedApplication()->openURL(url.c_str());
}

bool utils::file::openFolder(ghc::filesystem::path const&) {
    return false;
}

sapphire::Result<ghc::filesystem::path> utils::file::
    pickFile(sapphire::utils::file::PickMode, sapphire::utils::file::FilePickOptions const&) {
    return sapphire::Err("This function is currently unimplemented");
}

#endif

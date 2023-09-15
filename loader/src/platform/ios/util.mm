
#include <Sapphire/DefaultInclude.hpp>

#ifdef GEODE_IS_IOS

using namespace sapphire::prelude;
#include <Sapphire/loader/Dirs.hpp>
#include <UIKit/UIKit.h>
#include <iostream>
#include <sstream>
#include <Sapphire/utils/web.hpp>

bool utils::clipboard::write(std::string const& data) {
    [UIPasteboard generalPasteboard].string = [NSString stringWithUTF8String:data.c_str()];
    return true;
}

std::string utils::clipboard::read() {
    return std::string([[UIPasteboard generalPasteboard].string UTF8String]);
}

void utils::web::openLinkInBrowser(std::string const& url) {
    [[UIApplication sharedApplication]
        openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]]];
}

void sapphire_nslog(uintptr_t x) {
    NSLog(@"sapphire %lx", x);
}


ghc::filesystem::path dirs::getGameDir() {
    return ghc::filesystem::current_path();
}

ghc::filesystem::path dirs::getSaveDir() {
    return weaklyCanonical(CCFileUtils::sharedFileUtils()->getWritablePath().c_str());
}

#endif

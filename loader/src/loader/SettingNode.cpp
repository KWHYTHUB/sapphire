#include <Sapphire/loader/SettingNode.hpp>
#include <Sapphire/utils/cocos.hpp>

using namespace sapphire::prelude;

void SettingNode::dispatchChanged() {
    if (m_delegate) {
        m_delegate->settingValueChanged(this);
    }
}

void SettingNode::dispatchCommitted() {
    if (m_delegate) {
        m_delegate->settingValueCommitted(this);
    }
}

bool SettingNode::init(SettingValue* value) {
    m_value = value;
    return true;
}

void SettingNode::setDelegate(SettingNodeDelegate* delegate) {
    m_delegate = delegate;
}

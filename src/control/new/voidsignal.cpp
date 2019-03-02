#include "control/new/voidsignal.h"

VoidSignal::~VoidSignal() {}

VoidSlot::VoidSlot(std::function<void(void)> fCallback)
    : m_fCallback(fCallback) {}

VoidSlot::~VoidSlot() {}

void VoidSlot::slot() {
    m_fCallback();
}

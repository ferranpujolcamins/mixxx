#include <gtest/gtest.h>

#include "control/new/controldefinitions.h"
#include "control/new/controlobject.h"

namespace {

using namespace NewControl;

class NewControlTest : public testing::Test {};

TEST_F(NewControlTest, CreateAndGetDefaultValue) {
    std::unique_ptr<ControlObject<bool, bool> > co = Channel(1).play().create();
    EXPECT_EQ(co->defaultValue(), false);
}

TEST_F(NewControlTest, CreateAndGetInitialValue) {
    std::unique_ptr<ControlObject<bool, bool> > co = Channel(1).play().create();
    EXPECT_EQ(co->getValue(), true);
}

TEST_F(NewControlTest, CreateSetAndGet) {
    std::unique_ptr<ControlObject<bool, bool> > co = Channel(1).play().create();
    co->setValue(true);
    EXPECT_EQ(co->getValue(), true);
}

TEST_F(NewControlTest, CreateAndGetDefaultValueInt) {
    Group group = Channel(1);
    std::unique_ptr<ControlObject<int, int>> co = ControlFactory<int, int>(std::move(group), "co", false, 1, 2).create();
    EXPECT_EQ(co->defaultValue(), 2);
}

TEST_F(NewControlTest, CreateAndGetInitialValueInt) {
    Group group = Channel(1);
    std::unique_ptr<ControlObject<int, int>> co = ControlFactory<int, int>(std::move(group), "co", false, 1, 2).create();
    EXPECT_EQ(co->getValue(), 1);
}

TEST_F(NewControlTest, CreateSetAndGetInt) {
    Group group = Channel(1);
    std::unique_ptr<ControlObject<int, int>> co = ControlFactory<int, int>(std::move(group), "co", false, 1, 2).create();
    co->setValue(23);
    EXPECT_EQ(co->getValue(), 23);
}

}  // namespace

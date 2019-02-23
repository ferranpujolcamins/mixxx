#include <gtest/gtest.h>

#include "control/new/controldefinitions.h"
#include "control/new/controlobject.h"

namespace {

using namespace NewControl;

class NewControlTest : public testing::Test {};

TEST_F(NewControlTest, CreateAndGetDefaultValue) {
    std::unique_ptr<ControlObject<bool, bool> > co = Channel(1).play().create();
//    EXPECT_EQ(co.defaultValue(), false);
}

}  // namespace

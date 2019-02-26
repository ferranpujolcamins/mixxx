#include <gtest/gtest.h>

#include "control/new/controldefinitions.h"
#include "control/new/controlobject.h"

namespace {

using namespace NewControl;

class NewControlTest : public testing::Test {
};

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
    std::unique_ptr<ControlObject<int, int>> co = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).create();
    EXPECT_EQ(co->defaultValue(), 2);
}

TEST_F(NewControlTest, CreateAndGetInitialValueInt) {
    std::unique_ptr<ControlObject<int, int>> co = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).create();
    EXPECT_EQ(co->getValue(), 1);
}

TEST_F(NewControlTest, CreateSetAndGetInt) {
    std::unique_ptr<ControlObject<int, int>> co = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).create();
    co->setValue(23);
    EXPECT_EQ(co->getValue(), 23);
}

TEST_F(NewControlTest, Ownership) {
    {
        std::unique_ptr<ControlObject<int, int>> co = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).create();
        int count = ControlValueStore<int, int>::count();
        EXPECT_EQ(count, 1) << "A ControlValue must be created when a ControlObject is created.";;
    }
    int count = ControlValueStore<int, int>::count();
    EXPECT_EQ(count, 0) << "After the last ControlObject is destroyed, the ControlValue must be destroyed.";

    std::unique_ptr<ControlProxy<int, int>> proxy = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).getProxy();
    EXPECT_EQ(proxy, nullptr) << "A proxy must not be created after the CO is destroyed.";
}

TEST_F(NewControlTest, Ownership2) {
    {
        std::unique_ptr<ControlProxy<int, int>> proxy;
        {
            std::unique_ptr<ControlObject<int, int>> co = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).create();
            int count = ControlValueStore<int, int>::count();
            EXPECT_EQ(count, 1) << "A ControlValue must be created when a ControlObject is created.";

            proxy = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).getProxy();
            EXPECT_EQ(count, 1) << "Creating a ControlProxy must not increase nor delete the number of ControlValues.";
        }
        int count = ControlValueStore<int, int>::count();
        EXPECT_EQ(count, 1) << "While a ControlProxy is in scope, the ControlValue must not be destroyed.";
    }
    int count = ControlValueStore<int, int>::count();
    EXPECT_EQ(count, 0) << "After the last ControlProxy is destroyed, the ControlValue must be destroyed.";
}

TEST_F(NewControlTest, ConflictingKeys) {
    {
        std::unique_ptr<ControlObject<int, int>> co1 = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).create();
        std::unique_ptr<ControlObject<bool, bool>> co2 = ControlFactory<bool, bool>(Channel(1), "co", false, false, false).create();
        EXPECT_EQ(co2, nullptr) << "The control must not be created when another is already created with the same key,"
                                   "even if the controls have different type.";
    }
    std::unique_ptr<ControlObject<bool, bool>> co3 = ControlFactory<bool, bool>(Channel(1), "co", false, false, false).create();
    EXPECT_NE(co3, nullptr) << "The control must be created if the key is free again.";
}

TEST_F(NewControlTest, CreateSetAndGetProxy) {
    std::unique_ptr<ControlObject<int, int>> co = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).create();
    co->setValue(66);
    std::unique_ptr<ControlProxy<int, int>> proxy = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).getProxy();
    int value = proxy->getValue();
    EXPECT_EQ(value, 66);
    proxy->setValue(33);
    value = co->getValue();
    EXPECT_EQ(value, 33);
}

}  // namespace

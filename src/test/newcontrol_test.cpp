#include "test/mixxxtest.h"
#include "control/new/controldefinitions.h"
#include "control/new/controlobject.h"

namespace {

using namespace NewControl;

class NewControlTest: public MixxxTest {
};

TEST_F(NewControlTest, CreateAndGetDefaultValue) {
    std::unique_ptr<ControlObject<bool, bool> > co = Channel(1).play().create();
    EXPECT_EQ(false, co->defaultValue());
}

TEST_F(NewControlTest, CreateAndGetInitialValue) {
    std::unique_ptr<ControlObject<bool, bool> > co = Channel(1).play().create();
    EXPECT_EQ(true, co->getValue());
}

TEST_F(NewControlTest, CreateSetAndGet) {
    std::unique_ptr<ControlObject<bool, bool> > co = Channel(1).play().create();
    co->setValue(true);
    EXPECT_EQ(true, co->getValue());
}

TEST_F(NewControlTest, CreateAndGetDefaultValueInt) {
    std::unique_ptr<ControlObject<int, int>> co = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).create();
    EXPECT_EQ(2, co->defaultValue());
}

TEST_F(NewControlTest, CreateAndGetInitialValueInt) {
    std::unique_ptr<ControlObject<int, int>> co = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).create();
    EXPECT_EQ(1, co->getValue());
}

TEST_F(NewControlTest, CreateSetAndGetInt) {
    std::unique_ptr<ControlObject<int, int>> co = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).create();
    co->setValue(23);
    EXPECT_EQ(23, co->getValue());
}

TEST_F(NewControlTest, Ownership) {
    {
        std::unique_ptr<ControlObject<int, int>> co = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).create();
        int count = ControlValueStore<int, int>::count();
        EXPECT_EQ(1, count) << "A ControlValue must be created when a ControlObject is created.";;
    }
    int count = ControlValueStore<int, int>::count();
    EXPECT_EQ(0, count) << "After the last ControlObject is destroyed, the ControlValue must be destroyed.";

    std::unique_ptr<ControlProxy<int, int>> proxy = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).getProxy();
    EXPECT_EQ(nullptr, proxy) << "A proxy must not be created after the CO is destroyed.";
}

TEST_F(NewControlTest, Ownership3) {
    {
        std::unique_ptr<ControlProxy<int, int>> proxy;
        {
            std::unique_ptr<ControlObject<int, int>> co = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).create();
            int count = ControlValueStore<int, int>::count();
            EXPECT_EQ(1, count) << "A ControlValue must be created when a ControlObject is created.";

            proxy = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).getProxy();
            EXPECT_EQ(1, count) << "Creating a ControlProxy must not increase nor delete the number of ControlValues.";
        }
        int count = ControlValueStore<int, int>::count();
        EXPECT_EQ(1, count) << "While a ControlProxy is in scope, the ControlValue must not be destroyed.";

        std::unique_ptr<ControlProxy<int, int>> proxy2 = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).getProxy();
        EXPECT_EQ(nullptr, proxy2) << "A proxy must not be created after the CO is destroyed, even if there are live proxies.";
    }
    int count = ControlValueStore<int, int>::count();
    EXPECT_EQ(0, count) << "After the last ControlProxy is destroyed, the ControlValue must be destroyed.";
}

TEST_F(NewControlTest, ConflictingKeys) {
    {
        std::unique_ptr<ControlObject<int, int>> co1 = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).create();
        std::unique_ptr<ControlObject<bool, bool>> co2 = ControlFactory<bool, bool>(Channel(1), "co", false, false, false).create();
        EXPECT_EQ(nullptr, co2) << "The control must not be created when another is already created with the same key,"
                                   "even if the controls have different type.";
    }
    std::unique_ptr<ControlObject<bool, bool>> co3 = ControlFactory<bool, bool>(Channel(1), "co", false, false, false).create();
    EXPECT_NE(nullptr, co3) << "The control must be created if the key is free again.";
}

TEST_F(NewControlTest, CreateSetAndGetProxy) {
    std::unique_ptr<ControlObject<int, int>> co = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).create();
    co->setValue(66);
    std::unique_ptr<ControlProxy<int, int>> proxy = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).getProxy();
    int value = proxy->getValue();
    EXPECT_EQ(66, value);
    proxy->setValue(33);
    value = co->getValue();
    EXPECT_EQ(33, value);
}

TEST_F(NewControlTest, TestConnection1) {
    bool slotCalled = false;
    VoidSlot slot([&](){
        slotCalled = true;
    });
    std::unique_ptr<ControlObject<int, int>> co = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).create();
    co->connect(&slot, &VoidSlot::slot);
    co->setValue(7);
    application()->processEvents();
    EXPECT_TRUE(slotCalled);
    int value = co->getValue();
    EXPECT_EQ(7, value);
}

TEST_F(NewControlTest, TestConnection2) {
    bool slotCalled = false;
    VoidSlot slot([&](){
        slotCalled = true;
    });
    std::unique_ptr<ControlObject<int, int>> co = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).create();
    std::unique_ptr<ControlProxy<int, int>> proxy = ControlFactory<int, int>(Channel(1), "co", false, 1, 2).getProxy();
    proxy->connect(&slot, &VoidSlot::slot);
    co->setValue(7);
    application()->processEvents();
    EXPECT_TRUE(slotCalled);
    int value = proxy->getValue();
    EXPECT_EQ(7, value);
}

}  // namespace

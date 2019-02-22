#include "control/new/controlobject.h"
#include "control/new/controldefinitions.h"

namespace NewControl {

void a() {
    ControlObject<bool, bool> co = Channel(1).play().create(false);
    ControlProxy<bool, bool> cp = Channel(1).play().get();
    bool value = cp.value();
    bool value2 = Channel(1).play().getValue();


    //Subscription subscription = Control::Channel(1).play().subscribe([](bool value){});
    //Channel(1).play().subscribe([](bool value){}).disposedBy(/*disposeBag*/);
    // Does cpp warn here? NO! But we can add a log on unsubscribe
    //Channel(1).play().subscribe([](bool value){});
}

} // namespace

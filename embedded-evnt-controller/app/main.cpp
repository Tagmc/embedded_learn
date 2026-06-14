#include "BlockingEventQueue.hpp"
#include "Button.hpp"
#include "ButtonTask.hpp"
#include "ControllerTask.hpp"
#include "GpioPin.hpp"
#include "GpioPort.hpp"
#include "Led.hpp"
#include "SystemState.hpp"
#include "TimerTask.hpp"
#include "UartTask.hpp"
#include "VehicleController.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <thread>

namespace {
constexpr std::uint32_t LedPinNumber = 5;
constexpr std::uint32_t ButtonPinNumber = 3;
constexpr std::size_t EventQueueCapacity = 16;

const char* toString(SystemState state) {
    switch (state) {
        case SystemState::Idle:
            return "Idle";

        case SystemState::Active:
            return "Active";

        case SystemState::Warning:
            return "Warning";

        case SystemState::Fault:
            return "Fault";
    }

    return "Unknown";
}

void printTaskStats(const char* name, const TaskStats& stats) {
    std::cout << name
              << ": posted=" << stats.eventsPosted
              << ", postFailures=" << stats.postFailures
              << ", handled=" << stats.eventsHandled
              << ", timeouts=" << stats.timeouts
              << '\n';
}
}

int main() {
    GpioPort portA;

    GpioPin ledPin(portA, LedPinNumber);
    GpioPin buttonPin(portA, ButtonPinNumber);

    Led statusLed(ledPin);
    Button userButton(buttonPin);

    if (!userButton.initialize()) {
        std::cout << "Failed to initialize Button\n";
        return 1;
    }

    VehicleController controller(statusLed);

    if (!controller.initialize()) {
        std::cout << "Failed to initialize VehicleController\n";
        return 1;
    }

    BlockingEventQueue<EventQueueCapacity> eventQueue;

    ControllerTask<EventQueueCapacity> controllerTask(eventQueue, controller);
    TimerTask<EventQueueCapacity> timerTask(
        eventQueue,
        std::chrono::milliseconds(100)
    );
    UartTask<EventQueueCapacity> uartTask(eventQueue);
    ButtonTask<EventQueueCapacity> buttonTask(eventQueue);

    std::cout << "Starting RTOS-style simulation...\n";
    std::cout << "Initial state: " << toString(controller.state())
              << ", LED=" << std::boolalpha << statusLed.isOn()
              << '\n';

    controllerTask.start();
    timerTask.start();
    uartTask.start();
    buttonTask.start();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "\nStopping tasks...\n";

    timerTask.stop();
    uartTask.stop();
    buttonTask.stop();
    controllerTask.stop();

    std::cout << "\nFinal state: "
              << toString(controller.state())
              << ", LED=" << std::boolalpha << statusLed.isOn()
              << '\n';

    std::cout << "Controller counters: "
              << "handled=" << controller.handledEvents()
              << ", ignored=" << controller.ignoredEvents()
              << ", ticks=" << controller.timerTicks()
              << ", warnings=" << controller.warningCount()
              << ", faults=" << controller.faultCount()
              << '\n';

    std::cout << "\nTask stats:\n";
    printTaskStats("TimerTask", timerTask.stats());
    printTaskStats("UartTask", uartTask.stats());
    printTaskStats("ButtonTask", buttonTask.stats());
    printTaskStats("ControllerTask", controllerTask.stats());

    std::cout << "\nQueue final size: "
              << eventQueue.size()
              << '\n';

    return 0;
}
#include "VehicleController.hpp"

VehicleController::VehicleController(Led &statusLed)
    : statusLed_(statusLed) {}

bool VehicleController::initialize()
{
    const bool ok = statusLed_.initialize();

    if (!ok)
    {
        return false;
    }

    transitionTo(SystemState::Idle);
    return true;
}

void VehicleController::handle(const Event &event)
{
    ++handledEvents_;

    switch (event.type)
    {
    case EventType::ButtonPressed:
        handleButtonPressed(event);
        break;

    case EventType::ButtonReleased:
        handleButtonReleased(event);
        break;

    case EventType::TimerTick:
        handleTimerTick(event);
        break;

    case EventType::UartCommandRecieved:
        handleUartCommand(event);
        break;

    case EventType::SensorError:
        handleSensorError(event);
        break;

    case EventType::FaultDetected:
        handleFaultDetected(event);
        break;
    }
}

SystemState VehicleController::state() const
{
    return state_;
}

std::uint32_t VehicleController::handledEvents() const
{
    return handledEvents_;
}

std::uint32_t VehicleController::ignoredEvents() const
{
    return ignoredEvents_;
}

std::uint32_t VehicleController::timerTicks() const
{
    return timerTicks_;
}

std::uint32_t VehicleController::warningCount() const
{
    return warningCount_;
}

std::uint32_t VehicleController::faultCount() const
{
    return faultCount_;
}

void VehicleController::handleButtonPressed(const Event & /*event*/)
{
    switch (state_)
    {
    case SystemState::Idle:
        transitionTo(SystemState::Active);
        break;

    case SystemState::Active:
        transitionTo(SystemState::Idle);
        break;

    case SystemState::Warning:
        // Trong warning, button thường không nên toggle hệ thống lung tung.
        // Ta ignore để tránh hành vi khó đoán.
        ignoreEvent();
        break;

    case SystemState::Fault:
        // Fault là trạng thái nghiêm trọng.
        // Button bình thường không được clear fault.
        ignoreEvent();
        break;
    }
}

void VehicleController::handleButtonReleased(const Event & /*event*/)
{
    // Hiện tại ButtonReleased chưa làm gì.
    // Nhưng ta vẫn nhận event để sau này dùng cho debounce/long press.
    ignoreEvent();
}

void VehicleController::handleTimerTick(const Event & /*event*/)
{
    ++timerTicks_;

    switch (state_)
    {
    case SystemState::Idle:
        // Idle không blink.
        break;

    case SystemState::Active:
        // Active giữ LED ON.
        break;

    case SystemState::Warning:
        ++warningBlinkTicks_;

        if (warningBlinkTicks_ % 2 == 0)
        {
            statusLed_.toggle();
        }

        break;

    case SystemState::Fault:
        // Fault giữ LED ON cố định.
        statusLed_.turnOn();
        break;
    }
}

void VehicleController::handleUartCommand(const Event &event)
{
    const auto command = commandFromRaw(event.data);

    if (!command.has_value())
    {
        ignoreEvent();
        return;
    }

    switch (*command)
    {
    case CommandId::LedOn:
        if (state_ != SystemState::Fault)
        {
            statusLed_.turnOn();
        }
        else
        {
            ignoreEvent();
        }
        break;

    case CommandId::LedOff:
        if (state_ == SystemState::Idle)
        {
            statusLed_.turnOff();
        }
        else
        {
            ignoreEvent();
        }
        break;

    case CommandId::LedToggle:
        if (state_ == SystemState::Active)
        {
            statusLed_.toggle();
        }
        else
        {
            ignoreEvent();
        }
        break;

    case CommandId::ClearWarning:
        if (state_ == SystemState::Warning)
        {
            transitionTo(SystemState::Active);
        }
        else
        {
            ignoreEvent();
        }
        break;

    case CommandId::Reset:
        transitionTo(SystemState::Idle);
        break;

    case CommandId::ForceFault:
        transitionTo(SystemState::Fault);
        break;
    }
}

void VehicleController::handleSensorError(const Event & /*event*/)
{
    switch (state_)
    {
    case SystemState::Idle:
    case SystemState::Active:
        ++warningCount_;
        transitionTo(SystemState::Warning);
        break;

    case SystemState::Warning:
        ++warningCount_;
        break;

    case SystemState::Fault:
        ignoreEvent();
        break;
    }
}

void VehicleController::handleFaultDetected(const Event & /*event*/)
{
    if (state_ != SystemState::Fault)
    {
        ++faultCount_;
        transitionTo(SystemState::Fault);
        return;
    }

    ++faultCount_;
}

void VehicleController::transitionTo(SystemState nextState)
{
    if (state_ == nextState)
    {
        return;
    }

    state_ = nextState;

    switch (state_)
    {
    case SystemState::Idle:
        onEnterIdle();
        break;

    case SystemState::Active:
        onEnterActive();
        break;

    case SystemState::Warning:
        onEnterWarning();
        break;

    case SystemState::Fault:
        onEnterFault();
        break;
    }
}

void VehicleController::onEnterIdle()
{
    warningBlinkTicks_ = 0;
    statusLed_.turnOff();
}

void VehicleController::onEnterActive()
{
    warningBlinkTicks_ = 0;
    statusLed_.turnOn();
}

void VehicleController::onEnterWarning()
{
    warningBlinkTicks_ = 0;
    statusLed_.turnOn();
}

void VehicleController::onEnterFault()
{
    warningBlinkTicks_ = 0;
    statusLed_.turnOn();
}

void VehicleController::ignoreEvent()
{
    ++ignoredEvents_;
}
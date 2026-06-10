#include <string>
#include <iostream>
#include <vector>
class Logger
{
public:
    virtual ~Logger() = default;
    virtual void info(const std::string &message) = 0;
    virtual void error(const std::string &message) = 0;
};

class FakeLogger : public Logger
{
public:
    void info(const std::string &message) override
    {
        messages_.push_back("[INFO] " + message);
    }
    void error(const std::string &message) override
    {
        messages_.push_back("[ERROR] " + message);
    }

    const std::vector<std::string> &messages() const
    {
        return messages_;
    }

private:
    std::vector<std::string> messages_;
};

class PaymentService
{
public:
    explicit PaymentService(Logger &logger) : logger_(logger)
    {
    }
    bool pay(double amount)
    {
        if (amount <= 0)
        {
            logger_.error("Invalid payment amount");
            return false;
        }
        logger_.info("Start payment");
        logger_.info("Payment successfully");
        return true;
    }

private:
    Logger &logger_;
};
int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    FakeLogger logger;
    PaymentService service(logger);
    service.pay(-10.00);
    for (const std::string &message : logger.messages())
    {
        std::cout << message << '\n';
    }
    return 0;
}
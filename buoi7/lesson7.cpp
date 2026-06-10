class BankAccount
{
public:
    explicit BankAccount(int balance) : balance_(balance)
    {
        if (balance_ < 0)
        {
            balance_ = 0;
        }
    };
    bool withdraw(double amount)
    {
        if (amount <= 0 || amount > balance_)
        {
            return false;
        }
        balance_ -= amount;
        return true;
    }

    double balance() const
    {
        return balance_;
    }

private:
    double balance_;
};
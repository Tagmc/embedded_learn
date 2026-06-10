#include <iostream>
#include <optional>
#include <string>
#include <vector>

class Product
{
public:
    static std::optional<Product> create(int id, std::string name, double price)
    {
        if (id <= 0 || name.empty() || price <= 0)
        {
            return std::nullopt;
        }
        return Product{id, name, price};
    }

    const std::string name() const
    {
        return name_;
    }

    double price() const
    {
        return price_;
    }

    int id() const
    {
        return id_;
    }

private:
    Product(int id, std::string name, double price) : id_(id), name_(std::move(name)), price_(price) {}
    int id_;
    std::string name_;
    double price_;
};

class CartItem
{
public:
    CartItem(Product product, int quantity) : product_(product), quantity_(quantity)
    {
        if (quantity_ < 0)
        {
            quantity_ = 0;
        }
    }
    double subtotal() const
    {
        return quantity_ * product_.price();
    }

    int quantity() const
    {
        return quantity_;
    }

    const Product product() const
    {
        return product_;
    }

private:
    Product product_;
    int quantity_;
};

class ShoppingCart
{
public:
    bool addProduct(Product product, int quantity)
    {
        if (quantity <= 0)
            return false;
        items.emplace_back(std::move(product), quantity);
        return true;
    }
    double total() const
    {
        double total = 0;
        for (const CartItem& item : items)
        {
            total += item.subtotal();
        }
        return total;
    }

    void print(std::ostream &output) const
    {
         for (const CartItem& item : items) {
            output << item.product().name()
                   << " x "
                   << item.quantity()
                   << " = "
                   << item.subtotal()
                   << '\n';
        }

        output << "Total: " << total() << '\n';
    }

private:
    std::vector<CartItem> items;
};

int main()
{
    std::optional<Product> laptop = Product::create(1, "Laptop", 1000.0);
    std::optional<Product> mouse = Product::create(2, "Mouse", 25.0);

    if (!laptop.has_value() || !mouse.has_value())
    {
        std::cout << "Invalid product\n";
        return 1;
    }

    ShoppingCart cart;

    cart.addProduct(*laptop, 1);
    cart.addProduct(*mouse, 2);

    cart.print(std::cout);
    return 0;
}

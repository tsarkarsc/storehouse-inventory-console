#include <cstring>
#include <iomanip>
#include "Product.h"
#include "general.h"

using namespace std;

namespace sict
{
    Product::Product()
    {
        sku_[0] = '\0';
        name_ = nullptr;
        price_ = 0;
        taxed_ = false;
        quantity_ = 0;
        qtyNeeded_ = 0;
    }

    Product::Product(const char* sku, const char* name, bool taxed, double price, int qtyNeeded)
    {
        strncpy(sku_, sku, MAX_SKU_LEN);
        sku_[MAX_SKU_LEN] = '\0';
        name_ = new char[strlen(name) + 1];
        strcpy(name_, name);
        quantity_ = 0;
        taxed_ = taxed;
        price_ = price;
        qtyNeeded_ = qtyNeeded;
    }

    Product::Product(const Product& src)
    {
        strncpy(sku_, src.sku_, MAX_SKU_LEN);
        sku_[MAX_SKU_LEN] = '\0';
        price_ = src.price_;
        taxed_ = src.taxed_;
        quantity_ = src.quantity_;
        qtyNeeded_ = src.qtyNeeded_;

        if(src.name_ != nullptr)
        {
            name_ = new char[strlen(src.name_) + 1];
            strcpy(name_, src.name_);
        }
        else
        {
            name_ = nullptr;
        }
    }

    Product& Product::operator=(const Product& src)
    {
        if(this != &src)
        {
            strncpy(sku_, src.sku_, MAX_SKU_LEN);
            sku_[MAX_SKU_LEN] = '\0';
            price_ = src.price_;
            taxed_ = src.taxed_;
            quantity_ = src.quantity_;
            qtyNeeded_ = src.qtyNeeded_;

            delete [] name_;

            if(src.name_ != nullptr)
            {
                name_ = new char[strlen(src.name_) + 1];
                strcpy(name_, src.name_);
            }
            else
            {
                name_ = nullptr;
            }

        }
        return *this;

    }

    Product::~Product()
    {
        delete [] name_;
        name_ = nullptr;
    }

    void Product::sku(const char* value)
    {
        strcpy(sku_, value);
    }

    void Product::price(double value)
    {
        price_ = value;
    }

    void Product::name(const char* value)
    {
        delete [] name_;
        name_ = new char[strlen(value) + 1];
        strcpy(name_, value);
    }

    void Product::taxed(bool value)
    {
        taxed_ = value;
    }

    void Product::quantity(int value)
    {
        quantity_ = value;
    }

    void Product::qtyNeeded(int value)
    {
        qtyNeeded_ = value;
    }

    const char* Product::sku() const
    {
        return sku_;
    }

    double Product::price() const
    {
        return price_;
    }

    const char* Product::name() const
    {
        return name_;
    }

    bool Product::taxed() const
    {
        return taxed_;
    }

    int Product::quantity() const
    {
        return quantity_;
    }

    int Product::qtyNeeded() const
    {
        return qtyNeeded_;
    }

    bool Product::isEmpty() const
    {
        return (sku_[0]=='\0' && name_==nullptr && price_==0 && quantity_==0 && taxed_==false && qtyNeeded_==0);
    }

    double Product::cost() const
    {
        double c = 0;

        if(taxed_)
        {
            c = price_ * (1 + TAX);
        }
        else
        {
            c = price_;
        }

        return c;
    }

    bool Product::operator==(const char* ch) const
    {
        return (strcmp(sku_, ch)==0);
    }

    int Product::operator+=(int value)
    {
        quantity_ += value;
        return quantity_;
    }

    int Product::operator-=(int value)
    {
        quantity_ -= value;
        return quantity_;
    }

    double operator+=(double& d, const Product& p)
    {
        double total_cost;
        total_cost = p.cost() * p.quantity();
        d += total_cost;
        return d;
    }

    std::ostream& operator<<(std::ostream& ostr, const Product& p)
    {
        return p.write(ostr, true);
    }

    std::istream& operator>>(std::istream& istr, Product& p)
    {
        return p.read(istr);
    }

}
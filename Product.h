#ifndef SICT_PRODUCT_H_
#define SICT_PRODUCT_H_

#include "Streamable.h"
#include "general.h"

namespace sict
{
    class Product: public Streamable
    {
    private:
       
        char sku_[MAX_SKU_LEN + 1];
        char* name_;
        double price_;
        bool taxed_;
        int quantity_;
        int qtyNeeded_;

    public:

        Product();
        Product(const char*, const char*, bool = true, double = 0, int = 0);
        Product(const Product&);
        Product& operator=(const Product&);
        virtual ~Product();

        void sku(const char*);
        void price(double);
        void name(const char*);
        void taxed(bool);
        void quantity(int);
        void qtyNeeded(int);

        const char* sku() const;
        double price() const;
        const char* name() const;
        bool taxed() const;
        int quantity() const;
        int qtyNeeded() const;
        bool isEmpty() const;
        double cost() const;

        bool operator==(const char*) const;
        int operator+=(int);
        int operator-=(int);

        virtual std::ostream& write(std::ostream& os, bool linear) const = 0;
        virtual std::istream& read(std::istream& is) = 0;
        
    };
    
    double operator+=(double&, const Product&);
    std::ostream& operator<<(std::ostream& ostr, const Product&);
    std::istream& operator>>(std::istream& istr, Product&);
   
}

#endif
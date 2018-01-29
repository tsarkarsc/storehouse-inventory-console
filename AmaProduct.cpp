#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "AmaProduct.h"

using namespace std;

namespace sict
{
    AmaProduct::AmaProduct(char ch)
    {
        fileTag_ = ch;
    }

    const char* AmaProduct::unit() const
    {
        return unit_;
    }

    void AmaProduct::unit(const char* value)
    {
        strncpy(unit_, value, 10);
    }

    std::fstream& AmaProduct::store(std::fstream& file, bool addNewLine) const
    {
        if(file.is_open())
        {
            file << fileTag_ << ',' << sku() << ',' << name() << ','
                << price() << ',' << taxed() << ',' << quantity() 
                << ',' << unit() << ',' << qtyNeeded();

            if(addNewLine)
            {
                file << std::endl;
            }
        }

        return file;
    }

    std::fstream& AmaProduct::load(std::fstream& file)
    {
        double dtemp;
        int itemp;
        char stemp[MAX_SKU_LEN+1];
        char ntemp[21];
        char utemp[11];

        if(file.is_open())
        {
            file.getline(stemp, MAX_SKU_LEN+1, ',');
            sku(stemp);

            file.getline(ntemp, 21, ',');
            name(ntemp);

            file >> dtemp;
            price(dtemp);

            file.ignore();
            file >> itemp;
            taxed(itemp);

            file.ignore();
            file >> itemp;
            quantity(itemp);

            file.ignore();
            //file >> utemp;
            file.getline(utemp, 11, ',');
            unit(utemp);

            file >> itemp;
            qtyNeeded(itemp);

            file.ignore(); 
        }
        return file; 
    }

    std::ostream& AmaProduct::write(std::ostream& os, bool linear) const
    {
        if(!err_.isClear())
        {
            os << err_;
            return os; 
        }
        else
        {
            if(linear)
            {
                os << setw(MAX_SKU_LEN) << left << sku() << '|'
                    << setw(20) << left << name() << '|'
                    << setw(7) << right << fixed << setprecision(2) << cost() << '|'
                    << setw(4) << right << quantity() << '|'
                    << setw(10) << left << unit() << '|'
                    << setw(4) << right << qtyNeeded() << '|';
            }
            else
            {
                os << "Sku: " << sku() << endl;
                os << "Name: " << name() << endl;
                os << "Price: " << price() << endl;

                if(taxed())
                {
                    os << "Price after tax: " << cost() << endl;
                }
                else
                {
                    os << "Price after tax: N/A" << endl;
                }

                os << "Quantity On Hand: " << quantity() << ' ' << unit() << endl;
                os << "Quantity Needed: " << qtyNeeded(); 

            }
        }

        return os;
    }

    std::istream& AmaProduct::read(std::istream& istr)
    {
        double dtemp;
        int itemp;
        char stemp[MAX_SKU_LEN+1];
        char ntemp[21];
        char utemp[11];
        char ch;

        if(!istr.fail())
        {
            cout << "Sku: ";

            istr.getline(stemp, MAX_SKU_LEN+1, '\n');
            sku(stemp);

            cout << "Name: ";
    
            istr.getline(ntemp, 21, '\n');
            name(ntemp);

            cout << "Unit: ";
        
            istr.getline(utemp, 11, '\n');
            unit(utemp);

            cout << "Taxed? (y/n): ";
            
            istr >> ch;
            if(ch=='y' || ch=='Y')
            {
                taxed(true);
                istr.ignore();
                istr.clear();
            }
            else if(ch=='n' || ch=='N')
            {
                taxed(false);
                istr.ignore();
                istr.clear();
            }
            else
            {
                err_.message("Only (Y)es or (N)o are acceptable");
                istr.setstate(ios::failbit);
            }
            
            if (err_.isClear()) 
            {
                cout << "Price: ";
                istr >> dtemp;

                if (istr.fail()) 
                {
                    err_.message("Invalid Price Entry");
                }
                else 
                {
                    price(dtemp);
                }

            }

            if (err_.isClear())
            {
                cout << "Quantity On hand: ";
                istr >> itemp;

                if (istr.fail()) 
                {
                    err_.message("Invalid Quantity Entry");
                }
                else 
                {
                    quantity(itemp);
                }

            }

            if (err_.isClear()) 
            {
                cout << "Quantity Needed: ";
                istr >> itemp;

                if (istr.fail()) 
                {
                    err_.message("Invalid Quantity Needed Entry");
                }
                else 
                {
                    qtyNeeded(itemp);
                }
            }

        }

        return istr;
    }

}
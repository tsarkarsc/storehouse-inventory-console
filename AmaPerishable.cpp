#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "AmaPerishable.h"

using namespace std;

namespace sict
{
    AmaPerishable::AmaPerishable() : AmaProduct('P')
    {}

    const Date& AmaPerishable::expiry() const
    {
        return expiry_;
    }

    void AmaPerishable::expiry(const Date &value)
    {
        expiry_ = value;
    }

    std::fstream& AmaPerishable::store(std::fstream& file, bool addNewLine) const
    {
        AmaProduct::store(file, false);
        file << ',' << expiry();
        if(addNewLine)
        {
            file << std::endl;
        }   

        return file;
    }

    std::fstream& AmaPerishable::load(std::fstream& file)
    {
        AmaProduct::load(file);
        expiry_.read(file);

        file.ignore();

        return file;
    }

    std::ostream& AmaPerishable::write(std::ostream& ostr, bool linear) const
    {
        AmaProduct::write(ostr, linear);
        if(err_.isClear() && !this->isEmpty())
        {
            if(linear)
            {
                ostr << expiry();
            }
            else
            {
                ostr << endl << "Expiry date: " << expiry();
            }
        }
        return ostr;
    }

    std::istream& AmaPerishable::read(std::istream& istr)
    {
        Date tempdate;
        AmaProduct::read(istr);

        if(err_.isClear())
        {
            cout << "Expiry date (YYYY/MM/DD): ";
            std::cin >> tempdate;

            if(tempdate.bad())
            {
                if(tempdate.errCode() == 1)
                {
                    err_.message("Invalid Date Entry");
                }
                else if(tempdate.errCode() == 2)
                {
                    err_.message("Invalid Year in Date Entry");
                }
                else if(tempdate.errCode() == 3)
                {
                    err_.message("Invalid Month in Date Entry");
                }
                else if(tempdate.errCode() == 4)
                {
                    err_.message("Invalid Day in Date Entry");
                }

                istr.setstate(ios::failbit);
            }
            else
            {
                expiry(tempdate);
            }
        }
        else
        {
            cout << err_.message();
        }

        return istr;
    }

}

#include <iomanip>
#include <iostream>
#include "Date.h"
#include "general.h"
using namespace std;


namespace sict
{
    Date::Date()
    {
        year_ = 0;
        mon_ = 0;
        day_ = 0;
        readErrorCode_ = 0;
    }

    Date::Date(int year, int mon, int day)
    {
        year_ = year;
        mon_ = mon;
        day_ = day;
        readErrorCode_ = 0;
    }

    int Date::value() const
    {
        return year_ * 372 + mon_ * 31 + day_;
    }
    
    int Date::mdays() const
    {
        int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, -1 };
        int mon = mon_ >= 1 && mon_ <= 12 ? mon_ : 13;
        mon--;
        return days[mon] + int((mon == 1)*((year_ % 4 == 0) && (year_ % 100 != 0)) || (year_ % 400 == 0));
    }

    void Date::errCode(int errorCode)
    {
        readErrorCode_ = errorCode;
    }

    int Date::errCode() const
    {
        return readErrorCode_;
    }

    bool Date::bad() const
    {
        return readErrorCode_ != 0;
    }

    bool Date::operator==(const Date& D) const
    {
        return this->value() == D.value();
    }

    bool Date::operator!=(const Date& D) const
    {
        return this->value() != D.value();
    }

    bool Date::operator<(const Date& D) const
    {
        return this->value() < D.value();
    }

    bool Date::operator>(const Date& D) const
    {
        return this->value() > D.value();
    }

    bool Date::operator<=(const Date& D) const
    {
        return this->value() <= D.value();
    }

    bool Date::operator>=(const Date& D) const
    {
        return this->value() >= D.value();
    }

    std::istream& Date::read(std::istream& istr)
    {
        char c;

        istr >> year_ >> c >> mon_ >> c >> day_;

        if( istr.fail() )
        {
            readErrorCode_ = CIN_FAILED;
        }
        else
        {
            if( year_ < MIN_YEAR || year_ > MAX_YEAR ) 
            {
                readErrorCode_ = YEAR_ERROR;
            }
            else
            {
                if( mon_ < 1 || mon_ > 12) 
                {
                    readErrorCode_ = MON_ERROR;
                }
                else
                {
                    if(day_ <= 0 || day_ > this->mdays() )
                    {
                        readErrorCode_ = DAY_ERROR;
                    }
                }
            }
        }

        return istr;
    }

    std::ostream& Date::write(std::ostream& ostr) const
    {
        ostr << year_ << '/' << setw(2) << setfill('0') << mon_ << '/' << setw(2) << setfill('0') << day_ <<setfill(' ');
        return ostr;
    }

    std::istream& operator>>(std::istream& is, Date& d)
    {
        d.read(is);
        return is;
    }

    std::ostream& operator<<(std::ostream& os, const Date& d)
    {
        d.write(os);
        return os;
    }
}

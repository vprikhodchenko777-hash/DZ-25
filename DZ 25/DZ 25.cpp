
#include <iostream>
#include <windows.h>
using namespace std;

class Date
{
private:
    int day, month, year;

    bool isLeap() const
    {
        return (year % 4 == 0 && year % 100 != 0)
            || year % 400 == 0;
    }

    int daysInMonth() const
    {
        if (month == 2)
            return isLeap() ? 29 : 28;

        if (month == 4 || month == 6 ||
            month == 9 || month == 11)
            return 30;

        return 31;
    }

public:
    Date(int day, int month, int year)
        : day(day), month(month), year(year)
    {
        if (this->year < 1)
            this->year = 1;

        if (this->month < 1 || this->month > 12)
            this->month = 1;

        if (this->day < 1 || this->day > daysInMonth())
            this->day = 1;
    }

    void print() const
    {
        if (day < 10) cout << "0";
        cout << day << ".";

        if (month < 10) cout << "0";
        cout << month << "." << year << endl;
    }

    Date& operator++()
    {
        day++;

        if (day > daysInMonth())
        {
            day = 1;
            month++;

            if (month > 12)
            {
                month = 1;
                year++;
            }
        }
        return *this;
    }

    Date operator++(int)
    {
        Date old = *this;
        ++(*this);
        return old;
    }

    Date& operator--()
    {
        if (day > 1)
            day--;
        else if (month > 1)
        {
            month--;
            day = daysInMonth();
        }
        else if (year > 1)
        {
            year--;
            month = 12;
            day = 31;
        }
        return *this;
    }

    Date operator--(int)
    {
        Date old = *this;
        --(*this);
        return old;
    }

    Date& operator+=(int days)
    {
        for (int i = 0; i < days; i++)
            ++(*this);
        return *this;
    }

    Date operator+(int days) const
    {
        Date result = *this;
        result += days;
        return result;
    }

    Date& operator-=(int days)
    {
        for (int i = 0; i < days; i++)
            --(*this);
        return *this;
    }

    Date operator-(int days) const
    {
        Date result = *this;
        result -= days;
        return result;
    }

    bool operator==(const Date& other) const
    {
        return day == other.day &&
            month == other.month &&
            year == other.year;
    }

    bool operator<(const Date& other) const
    {
        if (year != other.year)
            return year < other.year;

        if (month != other.month)
            return month < other.month;

        return day < other.day;
    }

    bool operator!=(const Date& other) const
    {
        return !(*this == other);
    }

    bool operator>(const Date& other) const
    {
        return other < *this;
    }

    bool operator<=(const Date& other) const
    {
        return !(other < *this);
    }

    bool operator>=(const Date& other) const
    {
        return !(*this < other);
    }

    int operator-(const Date& other) const
    {
        if (*this < other)
            return -(other - *this);

        Date temp = other;
        int days = 0;

        while (temp < *this)
        {
            ++temp;
            days++;
        }

        return days;
    }
};

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "УРОВЕНЬ 1\n";

    Date a(28, 2, 2026);
    a.print();
    ++a;
    a.print();

    Date e(31, 12, 2026);
    Date old = e++;

    cout << "Старая дата: ";
    old.print();
    cout << "Новая дата: ";
    e.print();

    Date b(17, 10, 2026);
    cout << "17.10.2026 + 41 = ";
    (b + 41).print();

    Date d1(10, 5, 2026);
    Date d2(3, 2, 2026);
    Date d3(25, 12, 2025);

    Date earliest = d1;
    if (d2 < earliest) earliest = d2;
    if (d3 < earliest) earliest = d3;

    cout << "Самая ранняя дата: ";
    earliest.print();

    Date f(10, 10, 2026);
    (f += 5) += 2;
    cout << "После += 5 и += 2: ";
    f.print();

    Date wrong(31, 4, 2026);
    cout << "Неверная дата: ";
    wrong.print();

    cout << "\nУРОВЕНЬ 2\n";

    Date newYear(1, 1, 2027);
    cout << "01.01.2027 - 1 = ";
    (newYear - 1).print();

    Date today(25, 10, 2026);
    Date courseEnd(27, 11, 2026);

    cout << "До конца курса: "
        << courseEnd - today << " дней\n";

    cout << "До Нового года: "
        << newYear - today << " дней\n";

    cout << "\nУРОВЕНЬ 3\n";

    Date leap(28, 2, 2028);
    ++leap;
    cout << "После 28.02.2028: ";
    leap.print();

    Date invalid(29, 2, 2100);
    cout << "29.02.2100: ";
    invalid.print();

    return 0;
}

/*

1. Префиксный ++ возвращает Date&, потому что
   изменяет текущий объект и возвращает ссылку на него.
   Постфиксный ++ возвращает Date, потому что должен
   сохранить и вернуть старое значение.
   Если вернуть Date&, нельзя безопасно вернуть
   ссылку на локальную копию старой даты.

2. Параметр int в operator++(int) нужен, чтобы
   компилятор отличал постфиксный ++ от префиксного.
   Сам параметр обычно не используется.

3. operator+ константный, потому что создаёт новую
   дату и не изменяет исходную.
   operator+= изменяет текущий объект, поэтому
   не может быть константным.

4. Если !=, >, <= и >= реализованы через == и <,
   не нужно повторять одну и ту же логику.
   Код становится короче и уменьшается риск ошибок.

5. Компилятор выбирает operator- по типу аргумента.
   В d - 5 аргумент int, поэтому вызывается
   operator-(int).
   В d - other аргумент Date, поэтому вызывается
   operator-(const Date&).

6. Date + int означает прибавление определённого
   количества дней к дате.
   Date - Date означает количество дней между датами.
   Date + Date не имеет понятного смысла,
   поэтому такой оператор обычно не создают.
*/

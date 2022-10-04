#include "date.h"
#include <cstdio>
#include <ostream>
#include <istream>
#include <exception>


namespace {
	const int pmon_days[][12]{ {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
								{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} };
	bool is_valid_date(int d, int m, int y);
	const char *const week_names[]{ "Pazar", "Pazartesi", "Sali", "Carsamba", "Persembe", "Cuma", "Cumartesi" };
	const char *const mon_names[]{ "Ocak", "Subat", "Mart", "Nisan", "Mayis", "Haziran", "Temmuz", "Agustos", "Eylul", "Ekim", "Kasim", "Aralik" };

	class date_exception : public std::exception {
	public:
		const char* what()const
		{
			return "Bad Date Exeption\n";
		}
	};
}

namespace project {
	Date::Date() : m_day{ 1 }, m_mon{ 1 }, m_year{ year_base } {}//5
	Date::Date(int d, int m, int y) : m_day{ d }, m_mon{ m }, m_year{ y } //6
	{
		if (!is_valid_date(m_day, m_mon, m_year)) {
			std::printf("invalid date...\n");
			exit(EXIT_FAILURE);
		}
	}
	Date::Date(const char *p) //7
	{
		std::sscanf(p, "%d/%d/%d", &m_day, &m_mon, &m_year);

		if (!is_valid_date(m_day, m_mon, m_year)) {
			std::printf("invalid date...\n");
			exit(EXIT_FAILURE);
		}
	}
	Date::Date(std::time_t timer)
	{
		tm *s_tm = std::localtime(&timer);

		m_day = s_tm->tm_mday;
		m_mon = s_tm->tm_mon + 1;
		m_year = s_tm->tm_year + 1900;

		if (!is_valid_date(m_day, m_mon, m_year)) {
			std::printf("invalid date...\n");
			exit(EXIT_FAILURE);
		}
	}

	int Date::get_month_day()const //9
	{
		return m_day;
	}

	int Date::get_month()const //10
	{
		return m_mon;
	}

	int Date::get_year()const //11
	{
		return m_year;
	}

	int Date::get_year_day()const //12
	{
		
		int year_day{};

		int leap_check = static_cast<int>(isleap(m_year));

		for (int i = 0; i < m_mon - 1; ++i)
			year_day += pmon_days[leap_check][i];

		year_day += m_day;

		return year_day;
	}

	Date::Weekday Date::get_week_day()const //13
	{
		int d = m_day;
		int m = m_mon;
		int y = m_year;
		static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
		y -= m < 3;
		return static_cast<Weekday>(static_cast<int>(Weekday::Sunday) + (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7);
	}

	Date &Date::set_month_day(int day) //14
	{
		m_day = day;

		if (!is_valid_date(m_day, m_mon, m_year)) {
			std::printf("invalid date...\n");
			exit(EXIT_FAILURE);
		}
		return *this;
	}
	 
	Date &Date::set_month(int month) //15
	{
		m_mon = month;

		if (!is_valid_date(m_day, m_mon, m_year)) {
			std::printf("invalid date...\n");
			exit(EXIT_FAILURE);
		}
		return *this;
	}

	Date &Date::set_year(int year) //16
	{
		m_year = year;

		if (!is_valid_date(m_day, m_mon, m_year)) {
			std::printf("invalid date...\n");
			exit(EXIT_FAILURE);
		}
		return *this;
	}

	Date &Date::set(int day, int mon, int year) //17
	{
		m_day = day;
		m_mon = mon;
		m_year = year;

		if (!is_valid_date(m_day, m_mon, m_year)) {
			std::printf("invalid date...\n");
			exit(EXIT_FAILURE);
		}
		return *this;
	}

	Date Date::operator-(int day)const //18
	{
		int total = get_total_datys_from_base();

		total -= day;

		return extrat_date_from_total_days(total);		
	}

	Date &Date::operator+=(int day) //19
	{
		int total = get_total_datys_from_base();

		total += day;

		return *this = extrat_date_from_total_days(total);
	}

	Date &Date::operator-=(int day) //20
	{
		int total = get_total_datys_from_base();

		total -= day;

		return *this = extrat_date_from_total_days(total);
	}

	Date &Date::operator++() //21
	{
		return *this += 1;
	}

	Date Date::operator++(int) //22
	{
		Date temp{ *this };
		++ *this;
		return temp;
	}

	Date &Date::operator--() //23
	{
		return *this -= 1;
	}

	Date Date::operator--(int) //24
	{
		Date temp{ *this };
		-- *this;
		return temp;
	}

	Date Date::random_date() //25
	{
		int y = (std::rand() % (random_max_year - random_min_year + 1)) + random_min_year;
		int m = (std::rand() % 12) + 1;

		int leap_check = isleap(y);

		int d = (std::rand() % pmon_days[leap_check][m - 1]) + 1;

		return Date{ d, m, y };
	}

	bool operator<(const Date &d1, const Date &d2) //27
	{
		if (d1.m_year < d2.m_year)
			return true;

		if (d1.m_mon < d2.m_mon && d1.m_year == d2.m_year)
			return true;

		if (d1.m_day < d2.m_day && d1.m_mon == d2.m_mon && d1.m_year && d2.m_year)
			return true;

		return false;
	}

	bool operator==(const Date &d1, const Date &d2) //27
	{
		if (d1.m_year == d2.m_year && d1.m_mon == d2.m_mon && d1.m_day == d2.m_day)
			return true;

		return false;
	}

	bool operator<=(const Date &d1, const Date &d2) //27
	{
		return !(d2 < d1);
	}

	bool operator>(const Date &d1, const Date &d2) //27
	{
		return !(d1 <= d2);
	}

	bool operator>=(const Date &d1, const Date &d2) //27
	{
		return d2 <= d1;
	}

	bool operator!=(const Date &d1, const Date &d2) //27
	{
		return !(d1 == d2);
	}

	int operator-(const Date &d1, const Date &d2) //28
	{
		int t1 = d1.get_total_datys_from_base();
		int t2 = d2.get_total_datys_from_base();

		return t1 - t2;
	}

	Date operator+(const Date &date, int n) //29
	{
		Date temp{ date };
		temp += n;
		return temp;
	}

	Date operator+(int n, const Date &date) //29
	{
		Date temp{ date };
		temp += n;
		return temp;
	}

	Date::Weekday &operator++(Date::Weekday &r) //30
	{
		return r = r == Date::Weekday::Saturday ? Date::Weekday::Sunday : static_cast<Date::Weekday>(static_cast<int>(r) + 1);
	}
	Date::Weekday operator++(Date::Weekday &r, int) //30
	{
		Date::Weekday temp{ r };
		++r;
		return temp;
	}
	Date::Weekday &operator--(Date::Weekday &r) //30
	{
		return r = r == Date::Weekday::Sunday ? Date::Weekday::Saturday: static_cast<Date::Weekday>(static_cast<int>(r) - 1);
	}
	Date::Weekday operator--(Date::Weekday &r, int) //30
	{
		Date::Weekday temp{ r };
		--r;
		return temp;
	}

	std::ostream &operator<<(std::ostream &os, const Date &date) //31
	{

		std::printf("%02d %-7s %4d %-9s", date.get_month_day(), mon_names[date.get_month() - 1], date.get_year(), week_names[static_cast<int>(date.get_week_day())]);
		return os;
	}

	std::istream &operator>>(std::istream &is, Date &date) //32
	{
		return is >> date.m_day >> date.m_mon >> date.m_year;
	}


	int Date::get_total_datys_from_base()const
	{
		int total_days{};

		for (int i = year_base; i < m_year; ++i)
			total_days += isleap(i) ? 366 : 365;

		int leap_check = static_cast<int>(isleap(m_year));

		for (int i = 0; i < m_mon - 1; ++i) {
			total_days += pmon_days[leap_check][i];
		}

		total_days += m_day;

		return total_days;
	}

	Date Date::extrat_date_from_total_days(int total)const
	{
		int y{ year_base }, m{ 1 }, d{ 0 };

		while (total > (isleap(y) ? 366 : 365)) {
			total -= isleap(y) ? 366 : 365;
			++y;
		}

		int leap_check = static_cast<int>(isleap(y));

		for (int i = 0; total > pmon_days[leap_check][i]; ++i) {
			total -= pmon_days[leap_check][i];
			++m;
		}

		d = total;

		return Date{ d, m, y };
	}
}

namespace {
	bool is_valid_date(int d, int m, int y)
	{
		if (y < project::Date::Date::year_base)
			return false;

		if (m < 1 || m > 12)
			return false;

		int leap_check = static_cast<int>(project::Date::isleap(y));

		if (d < 1 || d > pmon_days[leap_check][m - 1])
			return false;

		return true;
	}
}

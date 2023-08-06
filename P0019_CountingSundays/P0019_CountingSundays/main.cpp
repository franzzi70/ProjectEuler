
#include <iostream>
#include <time.h>

int daysOfMonth(int year, int month)
{
    int numDays = 31;

    if (month == 2)
    {
        if (year % 4 == 0)
        {
            numDays = 29;
        }
        else
        {
            numDays = 28;
        }
    }
    else
    {
        if (month == 4
            || month == 6
            || month == 9
            || month == 11
            )
        {
            numDays = 30;
        }
        else
        {
            numDays = 31;
        }
    }
    return numDays;
}

int dayDistance190001(int year, int month, int day)
{
    int startYear = 1900;
    int startMonth = 1;
    int startDay = 1;
    int dayCount = 0;
    for (int diffYear = startYear ; diffYear < year ; diffYear++)
    {
        for (int i = 1; i <= 12; i++)
        {
            dayCount += daysOfMonth(diffYear, i);
        }
    }
    for (int diffMonth = startMonth; diffMonth < month; diffMonth++)
    {
        dayCount += daysOfMonth(year, diffMonth);
    }
    dayCount += (day - 1);
    return dayCount;
}

int getWeekDay(int year, int month, int day)
{
    int startDayDist = dayDistance190001(1901, 1, 1);
    int firstSundayDist = 6;    // startDayDist - 1;

    int dist = dayDistance190001(year, month, 1);
    int weekDay = ((dist + firstSundayDist + 1) % 7);
    return weekDay + 1;
}

int solve()
{
    int startDayDist = dayDistance190001(1901, 1, 1);
    int sundayCount = 0;

    for (int year = 1901; year <= 2000; year++)
    {
        for (int month = 1; month <= 12; month++)
        {
            int day = getWeekDay(year, month, 1);
            if (day == 1)
            {
                sundayCount += 1;
            }
        }
    }

    return sundayCount;
}


int main()
{
    time_t t1 = clock();
    int solution = solve();
    time_t t2 = clock();
    int ms = (int)(t2 - t1) * 1000 / CLOCKS_PER_SEC;

    std::cout << "solution: " << solution << std::endl << "duration: " << ms << " ms" << std::endl;
}

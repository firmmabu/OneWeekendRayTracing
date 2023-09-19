#ifndef INTERVAL_H
#define INTERVAL_H

class interval{
    public:
    double min;
    double max;

    interval();
    interval(double,double);

    bool contains(double) const;

    bool surrounds(double) const;

    double clamp(double) const;

    //空区间和实数区间R
    static const interval empty,universe;
};

#endif
#ifndef TRACK_H
#define TRACK_H
#include <string>
using namespace std;
class Track
{
public:
    int aid;
    string title;
    string artist;
    string url;
    double weight;
    Track(int aid, string title, string artist, string url, double weight);
};

#endif // TRACK_H

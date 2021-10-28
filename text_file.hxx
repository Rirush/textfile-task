//
// Created by Rirush on 28.10.2021.
//

#ifndef TEXTFILE_CLASS_TEXT_FILE_HXX
#define TEXTFILE_CLASS_TEXT_FILE_HXX


#include <string>
#include <vector>
#include <iostream>
#include <fstream>

enum FileMode {
    READ, WRITE
};

class Entry {
public:
    int n;
    std::string time;
    double hours;
    int freq;
    double amp, kk, r, antAz, antEl;
    int norad;
    std::string name;

    Entry();

    bool operator==(const Entry &other) const;

    friend std::istream &operator>>(std::istream &in, Entry &e);

    friend std::ostream &operator<<(std::ostream &out, Entry &e);

    bool operator<(const Entry &e) const;
};

class TextFile {
    std::vector<Entry> contents;
    std::ifstream input;
    FileMode mode;
public:
    std::string path;

    TextFile();

    TextFile(std::string path, FileMode mode);

    void copyTo(TextFile &target);

    void join(const TextFile &source);

    void flush();

    void sort();

    void sortBy(bool cmp(const Entry &l, const Entry &r));

    template<typename T>
    T sumBy(T val(const Entry &l));

    TextFile &operator+=(const TextFile &b);

    TextFile operator+(const TextFile &b) const;

    TextFile &operator-=(const TextFile &b);

    TextFile operator-(const TextFile &b) const;
};

template<typename T>
T TextFile::sumBy(T (*val)(const Entry &)) {
    T sum = 0;
    for(const auto &v : this->contents) {
        sum += val(v);
    }
    return sum;
}

#endif //TEXTFILE_CLASS_TEXT_FILE_HXX

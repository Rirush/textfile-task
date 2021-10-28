//
// Created by Rirush on 28.10.2021.
//

#include "text_file.hxx"

std::istream &operator>>(std::istream &in, Entry &e) {
    return in >> e.n >> e.time >> e.hours >> e.freq >> e.amp >> e.kk >> e.r >> e.antAz >> e.antEl >> e.norad >> e.name;
}

std::ostream &operator<<(std::ostream &out, Entry &e) {
    return out << e.n << '\t' << e.time << '\t' << e.hours << '\t' << e.freq << '\t' << e.amp << '\t' << e.kk << '\t'
               << e.r << '\t' << e.antAz << '\t' << e.antEl << '\t' << e.norad << '\t' << e.name << std::endl;
}

bool Entry::operator==(const Entry &other) const {
    return this->n == other.n && this->time == other.time && this->hours == other.hours && this->freq == other.freq &&
           this->amp == other.amp && this->kk == other.kk && this->r == other.r && this->antAz == other.antAz &&
           this->antEl == other.antEl && this->norad == other.norad && this->name == other.name;
}

Entry::Entry() {
    n = 0;
    hours = 0;
    freq = 0;
    amp = 0;
    kk = 0;
    r = 0;
    antAz = 0;
    antEl = 0;
    norad = 0;
}

bool Entry::operator<(const Entry &e) const {
    int h1,m1,s1,ms1;
    int h2,m2,s2,ms2;
    sscanf(this->time.c_str(), "%d:%d:%d.%d", &h1, &m1, &s1, &ms1);
    sscanf(e.time.c_str(), "%d:%d:%d.%d", &h2, &m2, &s2, &ms2);
    if (n != e.n) {
        return n < e.n;
    } else {
        if(h1 != h2) {
            return h1 < h2;
        }
        if(m1 != m2) {
            return m1 < m2;
        }
        if(s1 != s2) {
            return s1 < s2;
        }
        return ms1 < ms2;
    }
}

TextFile::TextFile() {
    mode = FileMode::WRITE;
}

TextFile::TextFile(std::string path, FileMode mode) {
    this->path = path;
    this->mode = mode;
    if(mode == FileMode::READ) {
        input = std::ifstream(path);
        std::string f;
        // Skip the header
        input >> f >> f >> f >> f >> f >> f >> f >> f >> f >> f >> f;

        while(true) {
            Entry e;
            if(input >> e) {
                contents.push_back(e);
            } else {
                break;
            }
        }
    }
}

void TextFile::copyTo(TextFile &target) {
    for(auto v : this->contents) {
        target.contents.push_back(v);
    }
}

void TextFile::join(const TextFile &source) {
    for(auto v : source.contents) {
        if (std::any_of(this->contents.begin(),  this->contents.end(), [&v](const Entry& e){ return v == e; })) {
            continue;
        } else {
            this->contents.push_back(v);
        }
    }
    sort();
}

void TextFile::flush() {
    std::ofstream output(path);
    output << "N\tTime\tHours\tFreq(kHz)\tAmp\tKK\tR(km)\tAntAz(grad)\tAntEl(grad)\tNORAD\tName" << std::endl;
    for(auto &v : this->contents) {
        output << v;
    }
}

TextFile &TextFile::operator+=(const TextFile &b) {
    this->join(b);
    sort();
    return *this;
}

TextFile TextFile::operator+(const TextFile &b) const {
    TextFile result;
    for(auto v : this->contents) {
        result.contents.push_back(v);
    }
    result.join(b);
    return result;
}

TextFile &TextFile::operator-=(const TextFile &b) {
    std::vector<Entry> r;
    for(auto v : this->contents) {
        if (std::any_of(b.contents.begin(),  b.contents.end(), [&v](const Entry& e){ return v == e; })) {
            continue;
        } else {
            r.push_back(v);
        }
    }
    this->contents = r;
    sort();
    return *this;
}

TextFile TextFile::operator-(const TextFile &b) const {
    TextFile r;
    for(auto v : this->contents) {
        if (std::any_of(b.contents.begin(),  b.contents.end(), [&v](const Entry& e){ return v == e; })) {
            continue;
        } else {
            r.contents.push_back(v);
        }
    }

    for(auto v : b.contents) {
        if (std::any_of(this->contents.begin(),  this->contents.end(), [&v](const Entry& e){ return v == e; })) {
            continue;
        } else {
            r.contents.push_back(v);
        }
    }
    r.sort();
    return r;
}

void TextFile::sort() {
    std::sort(this->contents.begin(),  this->contents.end());
}

void TextFile::sortBy(bool cmp(const Entry &l, const Entry &r)) {
    std::sort(this->contents.begin(),  this->contents.end(), cmp);
}

#include <iostream>
#include "text_file.hxx"

int main() {
    TextFile f("chirp_20210317_124100_003_out.dat", FileMode::READ);
    TextFile f2("chirp_20210317_124700_003_out.dat", FileMode::READ);
    TextFile o = f + f2;
    o.path = "test.dat";
    auto sum = o.sumBy<double>([](const Entry &e) { return e.hours; });
    std::cout << sum;
    o.flush();
    return 0;
}

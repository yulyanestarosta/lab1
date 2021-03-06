#include <iostream>
#include <random>
#include <set>
#include<fstream>

using namespace std;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


class State {
public:
    virtual bool contains(int s) const {
        return true;
    }
};

class DiscreteState: public State{
private:
    int const state;

public:
    DiscreteState(int state) : state(state) { }
    bool contains(int s) const {
        return s == state;
    }
};

class SegmentState: public State {
private:
    int const beg, end;
public:
    SegmentState() : beg(0), end(-1) { }
    SegmentState(int beg, int end) : beg(beg), end(end) { }

    bool contains(int s) const {
        return s >= beg && s <= end;
    }
};

class SetState: public State {
private:
    std::set<int> states;
public:
    SetState() : states() { }
    SetState(std::set<int> const& src) : states(src) { }

    bool contains(int s) const {
        return states.count(s) > 0;
    }

    void SetLengh(unsigned int n) {   //заполнение рандомными
        for (int i = 0; i < n; i++) {
            int ins = 100*rand()/RAND_MAX;
            states.insert(ins);
        }
    }

    void Clear() {   //очистка
        states.clear();
    }
};

class CrossingStates : public State {      //Пересечение
private:
    const State& s1;
    const State& s2;
public:
    CrossingStates(const State& s1, const State& s2) : s1(s1), s2(s2) {};

    bool contains(int s) const {
        return s1.contains(s) && s2.contains(s);
    }

};

class UnionStates : public State {   // Объединение
private:
    const State& s1;
    const State& s2;
public:
    UnionStates(const State& s1, const State& s2) : s1(s1), s2(s2) {};

    bool contains(int s) const {
        return s1.contains(s) or s2.contains(s);
    }

};

class SubstractStates : public State {  // Вычитание
private:
    const State& s1;
    const State& s2;
public:
    SubstractStates(const State& s1, const State& s2) : s1(s1), s2(s2) {};

    bool contains(int s) const {
        return !s1.contains(s) and s2.contains(s);
    }


};

void output(string name, string file) {     //вывод данных в файл для построения графиков
    std::ofstream resout(file, std::ios::app);
    if (resout.is_open()) {
        resout << name << std::endl;
    }
    resout.close();
}
class ProbabilityTest {
private:
    unsigned seed;
    int test_min, test_max;
    unsigned test_count;

public:
    ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count) : seed(seed), test_min(test_min), test_max(test_max), test_count(test_count) { }

    float operator()(State const& s) const {     
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(test_min, test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;

        return static_cast<float>(good) / static_cast<float>(test_count);
    }

    void TestNumberChange(unsigned int n) {
        test_count = n;
    }
};




int main(int argc, const char* argv[]) {
    srand(time(nullptr));
    DiscreteState d(1);
    SegmentState s(0, 10);
    SetState ss({ 1, 3, 5, 7, 23, 48, 57, 60, 90, 99 });

    CrossingStates cds(d, s);

    UnionStates uds(d, s);

    SubstractStates sds(d, s);

    ProbabilityTest pt(10, 0, 100, 100000);


    //vector<unsigned int> vec = {1, 3, 6, 8};
    //ProbabilityTest pt(vec);

    std::cout << pt(d) << std::endl;
    std::cout << pt(s) << std::endl;
    std::cout << pt(ss) << std::endl;

    std::cout << pt(cds) << std::endl;

    std::cout << pt(uds) << std::endl;

    std::cout << pt(sds) << std::endl;

    //для данные для графика(1/3 пункт)
    /*
    ProbabilityTest test1(10, 0, 100, 1000);
    for (int i = 0; i < 1000; i++) {
        output(std::to_string(test1(s)), "segment_out.txt"); //Вывод для сегмента
        output(std::to_string(test1(ss)), "set_out.txt"); //Вывод для сета
        test1.TestNumberChange(1000+1000*(i+1));
    }
    */


    //для данных для графика (2 пункт)
    /*
    SetState set1({});
    for (double i = 1; i <= 10; i++) {
        set1.SetLengh(5 * i);
        cout << "SetLen" << ' ' << "IntLen" << ' ' << "SetLen/IntervalLen" << ' ' << "Probability" << std::endl;
        cout << 5*i << "      " << 101 << "     " << 5 * i / 101 << "             " << pt(set1) << std::endl;
        set1.Clear();
    }
    */
}


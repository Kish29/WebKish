#include <iostream>
#include "memory"

class keeper {
public:

    virtual void show() const = 0;
};

class time_keeper : keeper {
public:
    time_keeper() {
        printf("time_keeper constructor\n");
    }

    explicit time_keeper(int f) : flag(f) {}

    virtual ~time_keeper() {
        printf("time_keeper destructor\n");
    }

    void show() const override {
        printf("in time_keeper!\n");
    }

private:
    int flag{};
};

class atomic_clock : public time_keeper {
public:

    atomic_clock() {
        printf("atomic_clock constructor\n");
    }

    /*atomic_clock() : time_keeper() {
        printf("atomic_clock constructor\n");
    }*/
    explicit atomic_clock(int f) : time_keeper(f) {}

    ~atomic_clock() override {
        printf("atomic_clock destructor\n");
    }

    void show() const override {
        printf("in atomic_clock!\n");
    }
};

void ss() {
    std::shared_ptr<atomic_clock> t1(new atomic_clock(1));
//    std::shared_ptr<time_keeper> t2(dynamic_cast<time_keeper *>(t1.get()));
    std::shared_ptr<time_keeper> t2(t1);
    printf("count %ld\n", t1.use_count());
}

#include "unistd.h"

int main() {
    ss();
    sleep(1);
    return 0;
}

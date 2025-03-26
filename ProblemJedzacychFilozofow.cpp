// ProblemJedzacychFilozofow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <semaphore>
#include <iomanip>

using namespace std;

class Philosopher;

// Possible states of philosophers
enum State { THINKS, WAITS, EATS };

// Class that displays table with philosophers status
class Display {
public:
    static Display display;
    vector<Philosopher *> *philosophers;
    mutex displayMutex;

    // Sets philosphers threads to pointers vector
    void SetPhilosophers(vector<Philosopher *> *philos) {
        philosophers = philos;
    }
    // Displays headers - philosopher number
    void displayHeader() {
        for (int i = 0; i < philosophers->size(); i++)
        {
            cout << setw(7) << i;
        }
        cout << endl;
    }
    void displayTable();
};

Display Display::display;

// Philospher class
class Philosopher {
public:
    int id;
    mutex &leftFork;
    mutex &rightFork;
    
    enum State state = WAITS;

    Philosopher(int id, mutex &left, mutex &right)
        : id(id), leftFork(left), rightFork(right) {
    }

    // Makes philosopher think then eat endlessly
    void dine() {
        while (true) {
            think();
            eat();
        }
    }

    // Changes philosoper state to the given in parameter, 
    // then redisplays table with philosopher status
    void changeState(State s) {
        state = s;
        Display::display.displayTable();
    }

private:
    // Makes philosopher think (rest) for some time
    void think() {
        changeState(THINKS);
        this_thread::sleep_for(chrono::milliseconds(1000 + rand() % 4000));
    }

    // Makes philosopher first wait for both forks, 
    // after he gets both, he starts eating for some time
    void eat() {
        changeState(WAITS);
        lock(leftFork, rightFork);
        lock_guard<mutex> leftLock(leftFork, adopt_lock);
        lock_guard<mutex> rightLock(rightFork, adopt_lock);

        changeState(EATS);
        this_thread::sleep_for(chrono::milliseconds(1000 + rand() % 4000));
    }
};

// Prints status of philosphers
void Display::displayTable() {
    displayMutex.lock();
    cout << "    ";
    for (int i = 0; i < philosophers->size(); i++)
    {
        switch ((*philosophers)[i]->state)
        {
        case THINKS:
            cout << "Thinks ";
            break;
        case WAITS:
            cout << "Waits  ";
            break;
        case EATS:
            cout << "Eats   ";
            break;
        }
    }
    //cout << '\n';
    cout << '\r';
    displayMutex.unlock();
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    // Base philosophers number
    int numPhilosophers = 5;

    // Recieving philosophers number as an argument
    if (argc > 1) {
        try {
            numPhilosophers = stoi(argv[1]);
        }
        catch (const exception &e) {
            cerr << "Błąd konwersji argumentu: " << e.what() << endl;
            return 1;
        }
    }
    cout << "Liczba filozofow: " << numPhilosophers << endl;

    // vector of Forks
    vector<mutex> forks(numPhilosophers);
    // vector of Philosophers
    vector<Philosopher *> philosophers(numPhilosophers);
    // vector of Threads
    vector<thread *> threads(numPhilosophers);

    // Creation of Philosophers with assigned left and right fork
    for (int i = 0; i < numPhilosophers; ++i) {
        philosophers[i] = new Philosopher(i, forks[i], forks[(i + 1) % numPhilosophers]);
    }

    Display::display.SetPhilosophers(&philosophers);
    Display::display.displayHeader();

    // Creation of threads containing Philosophers dining
    for (int i = 0; i < numPhilosophers; ++i) {
        threads[i] = new thread(&Philosopher::dine, philosophers[i]);
    }

    // Execution of threads
    for (auto &t : threads) {
        t->join();
    }

    return 0;
}
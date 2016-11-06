#ifndef PROJECT_SONG_H
#define PROJECT_SONG_H

#include <string>
#include <boost/thread/pthread/mutex.hpp>

class song {

private:
    std::string *songData;
    __volatile bool _stop = false;
    boost::mutex lock;
    int sec;

public:
    song(std::string *song) : songData(song) { };

    void play();

    void stop() { _stop = true; }

    void forward() {
        lock.lock();
        sec += 5;
        lock.unlock();
    }

    void prev() {
        lock.lock();
        sec -= 5;
        lock.unlock();
    }

};


#endif //PROJECT_SONG_H

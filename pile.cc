#include "lib/pool.cc"
#include <thread>

namespace threadMutex{

    void t1(Pool::Req *req) {
        // IO
       std::this_thread::sleep_for(std::chrono::milliseconds(50));
        // MUT
       req->lock.lock();
       req->buffer[req->pointer++] = '1';
       req->lock.unlock();
    }

    void t2(Pool::Req *req) {
        // IO
       std::this_thread::sleep_for(std::chrono::milliseconds(10));
        // MUT
       req->lock.lock();
       req->buffer[req->pointer++] = '0';
       req->lock.unlock();

    }

}

int main() { 

    char *response ;

    Pool::BlockPool *pool = Pool::start(50);
    
    pool->pile(threadMutex::t1);
    pool->pile(threadMutex::t2);

    response = pool->flush();
    
    printf("[ %s ]\n",response);

    pool->pile(threadMutex::t2);
    pool->pile(threadMutex::t1);
    pool->pile(threadMutex::t1);
    pool->pile(threadMutex::t1);
    pool->pile(threadMutex::t2);

    response = pool->flush();
    
    printf("[ %s ]\n",response);
    
    return 0;
}


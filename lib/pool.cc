#include <thread>
#include <mutex>

namespace Pool{

    struct Req {

        char *buffer;
        int pointer = 0;
        std::mutex lock;
    };

    struct BlockPool {

        Req *stream;
        int p = 0;
        std::thread *threadQueue;
        void (*fn[12])(Req*) ;
        int pile(void (*fn)(Req*) ) 
        { 
            this->fn[this->p++] = fn;
            return 0; 
        }
        char * flush() 
        {   
            this->threadQueue = new std::thread[this->p];
            for (int i = 0; i < p; i++)
            {
                this->threadQueue[i] = std::thread(this->fn[i],this->stream);
            }
            for (int i = 0; i < p; i++)
            {
                 this->threadQueue[i].join();
            }
            this->p = 0;
            char *result;
           
            result = (char*) malloc( ( this->stream->pointer - 1 ) * sizeof (char));
        
            memcpy (result,this->stream->buffer, sizeof(result));
            result[this->stream->pointer] = '\0';
            this->stream->pointer = 0;
           
            return result;
        }

    };
 
    BlockPool *start(int len){

        char *buffer;
        buffer = (char*) malloc( len * sizeof (char));

        BlockPool *pool = new BlockPool();
        Req *req = new Req();
        req->buffer = buffer;
        pool->stream = req;
       
        return pool;

    }
}
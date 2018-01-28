#include <thread>
#include <mutex>

namespace Pool{

    struct Req {

        char *buffer;
        int pointer = 0;
        std::mutex lock;
    };

    struct BlockPool {

        int pbloc = 0;
        int *_bloc;
        Req *stream;
        int p = 0;
        std::thread *threadQueue;
        void (*fn[12])(Req*) ;

        int pile(void (*fn)(Req*) ) 
        { 
            this->fn[this->p++] = fn;
            return 0; 
        }

        void bloc(){
            this->_bloc[this->pbloc] = (int)this->p;
            this->pbloc++;
        }

      

        char * flushPrevious() 
        {   

            int block ;
            if(this->pbloc >= 2) {
                 block = this->_bloc[this->pbloc-2];
            }else{
                 block = 0;
            }
            
            char *result = this->flushPos(block);
            
            this->pbloc--;

            return result;

        }

        char * flushPos(int L) 
        {   
            this->threadQueue = new std::thread[this->p];
            for (int i = L; i < p; i++)
            {
                this->threadQueue[i] = std::thread(this->fn[i],this->stream);
            }
            for (int i = L; i < p; i++)
            {
                 this->threadQueue[i].join();
            }
            this->p = L;
            char *result;
           
            result = (char*) malloc( ( this->stream->pointer - 1 ) * sizeof (char));
        
            memcpy (result,this->stream->buffer, sizeof(result));
            result[this->stream->pointer] = '\0';
            this->stream->pointer = 0;

            this->p = L;

            return result;
        }

        char * flushAll() 
        {   
            return this->flushPos(0);
        }

    };
 
    BlockPool *start(int len){

        int *bloc;
        bloc = (int*) malloc( 20 * sizeof (int));

        char *buffer;
        buffer = (char*) malloc( len * sizeof (char));

        BlockPool *pool = new BlockPool();
        pool->_bloc = bloc;

        Req *req = new Req();
        req->buffer = buffer;
        pool->stream = req;
       
        return pool;

    }
}
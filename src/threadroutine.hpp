#ifndef THREADEXECUTION_HPP
#define THREADEXECUTION_HPP

#include <chrono>
#include <thread>

namespace dg {
namespace utils {

class DoNothing
{
    public:
        inline void operator() () {}
};

/**
 * This class models routines that are executed within a thread.
 * <p>
 * These routines call the () operator from a given function object until they are stopped
 * or until the work that has to be done is actually done. These calls can be temporarilly paused, until resumed.
 * The user is also allowed to specify latency durations in order to put the underlying thread to sleep after each
 * call to the () operator of the specified function object or after a pause is detected. It is also possible to specify
 * functions to execute before the routine starts and after it finishes.
 * <p>
 * Basically, if the underlying routine is not stopped, not paused and its work is still not done,
 * it calls the () operator of the specified function object and then sleeps for a given duration. After that,
 * if still not stopped, not paused and if the work is still not done, the () operator of the specified function
 *  object is called again and then put again to sleep, and so on. As soon as the routine is stopped or the work is done,
 * its execution ends. If, at any time, the routine is paused, it sleeps for a given duration and then checks again if it
 * is still paused. As soon as the routine is unpaused, its work is resumed.
 * <p>
 * <h3>Code example</h3>
 * <p>
 * <pre><code>
 struct CountStop //predicate
 {
     int count;
     CountStop() : count(0) {}
     bool operator()() const { return count >= 50; }
 };

 class Count //iterative task incrementing i
 {
     public:
         int i;
         CountStop& s;
         Count(CountStop& s) : i(0), s(s) {}
         void operator() ()
         {
             cout << i << " ";
             cout.flush();

             i++;
             s.count++;
         }
 };

 CountStop stop;
 Count count(stop);

 ThreadRoutine<Count,CountStop> exec(count, stop, 100ms, 2s, false);//sleep duration is sort of useless here
 reference_wrapper<ThreadRoutine<Count,CountStop>> exec_ref(exec);//most of the time, this is mandatory (here, it is not)

 cout << "Thread starting (this will write stuff for 5 seconds)" << endl;
 thread t(exec_ref);
 t.join();
 cout << endl << "Thread quitting" << endl;
 </code></pre>
 * @brief This class models routines that are executed within a thread.
 * @tparam Function the object function that will actually be used during the routine execution (the object function that will actually do the required work).
 * @tparam Condition a predicate telling when the work that has to be done actually is.
 * @author R. Absil
 */
template<class Function, class Condition>
class ThreadRoutine
{    
    Function& f;//function that actually makes a bit of work
    Condition& cond;//tells when the work is done

    std::chrono::milliseconds _sleep_latency;//amount of time to sleep after each little bit of work done
    std::chrono::milliseconds _pause_duration;//amout of time to sleep when the thread is paused (recheck state after)

    bool _stop;//is the thread stopped ?
    bool _pause;//is the thread paused ?

    public:
        /**
         * Builds up a thread execution routine.
         * @brief Builds up a thread execution routine.
         * @param sleep_latency the duration to put the underlying thread to sleep after some work is done
         * @param f the function to iteratively execute within the thread execution
         * @param cond the condition that has to be met for the execution to stop
         * @param pause_duration the sleep duration between two pause-checking
         * @param pause true if the thread is initially paused
         * @param before the function to execute before the main execution of f
         * @param after the function to execute after the main execution of f
         */
        ThreadRoutine(Function& f, Condition& cond, std::chrono::milliseconds sleep_latency,
                        std::chrono::milliseconds pause_duration, bool pause = false);//the default values will not work (reference), hence static dn

        /**
         * Runs the routine.
         * <p>
         * Basically, if the underlying routine is not stopped, not paused and its work is still not done,
         * it calls the () operator of the specified function object and then sleeps for a given duration. After that,
         * if still not stopped, not paused and if the work is still not done, the () operator of the specified function
         *  object is called again and then put again to sleep, and so on. As soon as the routine is stopped or the work is done,
         * its execution ends. If, at any time, the routine is paused, it sleeps for a given duration and then checks again if it
         * is still paused. As soon as the routine is unpaused, its work is resumed.
         * @brief Runs the routine
         * @return the value of the predicate
         */
        bool operator() ();

        /**
         * Stops the routine.
         * <p>
         * Basically, let the () operator of the specified function object finish its work, the returns. After such a call
         * the the () operator of the specified function object is never called again.
         * @brief stop
         */
        inline void stop();

        /**
         * Pauses the routine until resumed.
         * @brief Pauses the routine until resumed.
         */
        inline void pause();

        /**
         * Resumes a paused routine. If already paused, does nothing.
         * @brief Resumes a paused routine. If already paused, does nothing.
         */
        inline void resume();        
};

template<class Function, class Condition>
ThreadRoutine<Function,Condition>::ThreadRoutine(Function &f, Condition& cond, std::chrono::milliseconds sleep_latency,
                                 std::chrono::milliseconds pause_duration, bool pause)
                                : f(f),
                                  cond(cond),
                                  _sleep_latency(sleep_latency),
                                  _pause_duration(pause_duration),
                                  _stop(false),
                                  _pause(pause)
{}

template<class Function, class Condition>
bool ThreadRoutine<Function,Condition>::operator() ()
{
    if(_stop)
        return cond();

    while(! _stop && !cond())
    {
        if(! _pause)
        {
            f();
            std::this_thread::sleep_for(_sleep_latency);
        }
        else
            std::this_thread::sleep_for(_pause_duration);
    }

    return cond();
}

template<class Function, class Condition>
void ThreadRoutine<Function,Condition>::stop()
{
    _stop = true;
}

template<class Function, class Condition>
void ThreadRoutine<Function,Condition>::pause()
{
    _pause = true;
}

template<class Function, class Condition>
void ThreadRoutine<Function,Condition>::resume()
{
    _pause = false;
}

}}//namespace util

#endif // THREADEXECUTION_HPP

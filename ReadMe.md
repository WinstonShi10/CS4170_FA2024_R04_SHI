# Reflection 04, OpenMP III: Producers and Consumers

Winston Shi

## Summary

Use OpenMP to implement a producer consumer program in which some of the threads are producers and others are consumers. The producers read text from a collection of files, one per producer. They insert lines of text into a single shared queue. The consumers take the lines of text and tokenize them. Tokens are “words” separated by white space. When a consumer finds a token, it writes it to `stdout`.

Evaluate the exercise using 1-12 threads for the producer, 1-12 threads for consumer, and 12 text files that you find somewhere or randomly generate (they should be of similar size).

## Key Notes
## Key Code Snippet

## Run Locally

`docker run --rm -v ${PWD}:/tmp -w /tmp/Default rgreen13/alpine-bash-gpp make all`

`docker run --rm -v ${PWD}:/tmp -w /tmp/Default rgreen13/alpine-bash-gpp ./OpenMP`

## Run on OpenMP
## Speedup
## Efficiency
## Karp-Flatt Metric
## Performance

### What is the optimal number of producers and consumers?
### time spent on this reflection
### time spent coding
### time spent writing
### time spent testing
### time spent analyzing







/* NOTES
-> synchronize threads before continuing past statement
    #pragma omp barrier

-> applies to the next line of code only
-> high performance
-> x<op> = expression (x -= 1)
-> + - * / & ^ | << >>
-> Load, modify, store
    #pragma omp atomic

-> 
    #pragma omp critical

omp_init_unlock(omp_lock_t*)
omp_set_unlock(omp_lock_t*)
omp_unset_unlock(omp_lock_t*)
omp_destroy_unlock(omp_lock_t*)
*/

/*
int main() {
    int id;

    omp_lock_t lck;
    omp_init_lock(&lck);

    omp_set_num_threads(4);
    #pragma omp parallel private(id)
    {
        id = omp_get_thread_num();

        omp_set_lock(&lck);
        std::cout << "my thread is: " << id << std::endl;
        omp_unset_lock(&lck);
    }

    return 0;
}
*/




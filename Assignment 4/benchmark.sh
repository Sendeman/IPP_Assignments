for thread_count in 1 2 4 8 16
do
    sh execute.sh $thread_count sieve_of_eratosthenes $1
done
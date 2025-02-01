1) Overview:
This project explores the efficiency of multithreading and multiprocessing in C by implementing two key experiments:
- Parallel Web Scraping – Scraping multiple web pages concurrently using both threads and processes to compare performance.
- Parallel Sorting – Sorting a large dataset using selection sort (sequential) and quick sort (multiprocessing) to evaluate execution time differences.
2) Features:
- Web scraping using libcurl.
- Parallel execution using POSIX threads (pthreads) and fork().
- Performance benchmarking of threads vs processes.
- Sorting 1,000,000 random integers using different algorithms to compare sequential and multiprocessing execution.
3) Technologies Used:
- C Programming Language
- POSIX Threads (pthreads)
- Multiprocessing with fork()
- libcurl for HTTP requests
- GCC for compilation
4) Results:
- The project provides insights into the efficiency of threads vs processes for parallel execution.
- Performance analysis helps determine the best approach based on the nature of the task (I/O-bound vs CPU-bound).

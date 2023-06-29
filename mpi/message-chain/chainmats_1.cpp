#include <iostream>
#include <vector>
#include <mpi.h>

void print_ordered(double t, int size);

int main(int argc, char* argv[])
{
    int i, myid, ntasks;
    constexpr int size = 10000000;
    std::vector<int> message(size);
    std::vector<int> receiveBuffer(size);
    MPI_Status status;

    double t0, t1;

    int source, destination;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    // Initialize buffers
    for (i = 0; i < size; i++) {
        message[i] = myid;
        receiveBuffer[i] = -1;
    }

    // Set source and destination ranks
    source = (myid == 0) ? MPI_PROC_NULL : myid - 1;
    destination = (myid == ntasks - 1) ? MPI_PROC_NULL : myid + 1;

    // Start measuring the time spent in communication
    MPI_Barrier(MPI_COMM_WORLD);
    t0 = MPI_Wtime();

    // Send messages
    MPI_Send(message.data(), size, MPI_INT, destination, myid + 1, MPI_COMM_WORLD);

    std::cout << "Sender: " << myid << ". Sent elements: " << size
              << ". Tag: " << myid + 1 << ". Receiver: " << destination << std::endl;

    // Receive messages
    MPI_Recv(receiveBuffer.data(), size, MPI_INT, source, myid, MPI_COMM_WORLD, &status);
    int receivedSize;
    MPI_Get_count(&status, MPI_INT, &receivedSize);

    std::cout << "Receiver: " << myid << ". First element: " << receiveBuffer[0]
              << ". Received message size: " << receivedSize << std::endl;

    // Finalize measuring the time and print it out
    t1 = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    std::cout << "Time elapsed in rank " << myid << ": " << t1 - t0 << std::endl;
    fflush(stdout);

    print_ordered(t1 - t0, receivedSize);

    MPI_Finalize();
    return 0;
}

void print_ordered(double t, int size)
{
    int i, rank, ntasks;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    if (rank == 0) {
        std::cout << "Time elapsed in rank " << rank << ": " << t << std::endl;
        for (i = 1; i < ntasks; i++) {
            MPI_Recv(&t, 1, MPI_DOUBLE, i, 11, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Status status;
            MPI_Get_count(&status, MPI_DOUBLE, &size);
            std::cout << "Time elapsed in rank " << i << ": " << t
                      << ". Received message size: " << size << std::endl;
        }
    } else {
        MPI_Send(&t, 1, MPI_DOUBLE, 0, 11, MPI_COMM_WORLD);
    }
}

#include <cstdio>
#include <vector>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank_id, ntasks, nrecv;
    constexpr int arraysize = 100000;
    constexpr int msgsize = 100;
    std::vector<int> message(arraysize);
    std::vector<int> receiveBuffer(arraysize);
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);

    // Initialize message and receive buffer
    for (int i = 0; i < arraysize; i++) {
        message[i] = rank_id;
        receiveBuffer[i] = -1;
    }

    // Send and receive messages as defined in exercise
    if (rank_id == 0) {
        MPI_Send(message.data(), msgsize, MPI_INT, 1, 1, MPI_COMM_WORLD);
        MPI_Recv(receiveBuffer.data(), arraysize, MPI_INT, 1, 2, MPI_COMM_WORLD,
                 &status);
        MPI_Get_count(&status, MPI_INT, &nrecv);
        std::cout << "Rank " << rank_id << " received " << nrecv << " element. First " << receiveBuffer[0] << std::endl;
    } else if (rank_id == 1) {
        MPI_Send(message.data(), msgsize, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Recv(receiveBuffer.data(), arraysize, MPI_INT, 0, 1, MPI_COMM_WORLD,
                 &status);
        MPI_Get_count(&status, MPI_INT, &nrecv);
        std::cout << "Rank " << rank_id << " received " << nrecv << " element. First " << receiveBuffer[0] << std::endl;
    }

    MPI_Finalize();
    return 0;
}

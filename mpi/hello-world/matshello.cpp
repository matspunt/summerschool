#include <mpi.h>
#include <iostream>

int main(int argc, char **argv)
{
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
  
    // Get the number of processes associated with the communicator
    int ntasks;
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    // Get the rank of the calling process
    int rank_id;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);

    if (rank_id == 0){
        std::cout << "There are " << ntasks << " tasks total. We report this from rank " << rank_id << "\n" << std::endl;
    }

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    MPI_Barrier(MPI_COMM_WORLD);

    std::cout << "Hello world from process " << processor_name << " with rank " << rank_id << " out of " << ntasks << " processors\n" << std::endl;
    //std::cout.flush();
    
    // Finalize: Any resources allocated for MPI can be freed
    MPI_Finalize();
}

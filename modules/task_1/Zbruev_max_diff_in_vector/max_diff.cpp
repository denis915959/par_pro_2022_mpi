// Copyright 2022 Zbruev Denis

#include <mpi.h>
#include <ctime>
#include <cmath>
#include <vector>
#include <random>
#include <exception>
#include "../../../modules/task_1/Zbruev_max_diff_in_vector/max_diff.h"
#include <cstddef>

std::vector<int> get_rand(int size) {
    std::vector<int> vec(size);
    srand(time(NULL)); //рандомный вектор строим
    for (int i = 0; i < size; i++)
    {
        vec[i] = rand()%500;
    }
    return vec;
}

int get_max_difference_without_mpi(std::vector<int> Vector) {
    int size = Vector.size();
    int res = 0;
    for (int i = 0; i < (size - 1); i++) {
        if (abs(Vector[i] - Vector[i + 1]) > res)
            res = abs(Vector[i] - Vector[i + 1]);
    }
    return res;
}



int get_max_difference_mpi(std::vector<int> Vector) {
    int vect_size = static_cast<int>(Vector.size());
    int size, proc_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int fragment_size = (vect_size - 1) / size;
    int remainder = (vect_size - 1) % size;
    std::vector<int> tmp_vect;

    if (proc_rank == 0) {
        if (fragment_size) {
            for (int proc = 1; proc < size; proc++) {
                MPI_Send(&Vector[0] + proc * fragment_size + remainder, fragment_size + 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
            }
        }
    }

    int result = 0;
    int max_difference = 0;

    if (proc_rank == 0) {
        if (size != 1 || fragment_size) {
            tmp_vect.resize(fragment_size + remainder + 1);
            tmp_vect.assign(Vector.begin(), Vector.begin() + fragment_size + remainder + 1);
            max_difference = get_max_difference_without_mpi(tmp_vect);
        }
        else {
            max_difference = get_max_difference_without_mpi(Vector);
        }
    }
    else if (fragment_size) {
        tmp_vect.resize(fragment_size + 1);
        MPI_Status status;
        MPI_Recv(&tmp_vect[0], fragment_size + 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        max_difference = get_max_difference_without_mpi(tmp_vect);
    }

    MPI_Reduce(&max_difference, &result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    return result;
}
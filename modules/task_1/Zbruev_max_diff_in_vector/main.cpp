// Copyright 2022 Zbruev Denis

#include<gtest/gtest.h>
#include<mpi.h>
#include<vector>
#include<gtest-mpi-listener.hpp>
#include"../../../modules/task_1/Zbruev_max_diff_in_vector/max_diff.h"

TEST(max_diff_in_vector, test_get_max_difference_without_mpi) {
    int vect_size = 4;
    std::vector<int> test_vector(vect_size);
    test_vector[0] = 3;
    test_vector[1] = 0;
    test_vector[2] = 15;
    test_vector[3] = 11;

    EXPECT_EQ(15, get_max_difference_without_mpi(test_vector));
}

TEST(max_diff_in_vector, get_max_difference_mpi_Size_3) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int vect_size = 3;
    std::vector<int> test_vector(vect_size);
    test_vector = get_rand(vect_size);

    int res = get_max_difference_mpi(test_vector);
    if (proc_rank == 0) {
        ASSERT_EQ(get_max_difference_without_mpi(test_vector), res);
    }
}

TEST(max_diff_in_vector, get_max_difference_mpi_Size_60) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int vect_size = 60;
    std::vector<int> test_vector(vect_size);
    test_vector = get_rand(vect_size);
    int res = get_max_difference_mpi(test_vector);
    if (proc_rank == 0) {
        ASSERT_EQ(get_max_difference_without_mpi(test_vector), res);
    }
}

TEST(max_diff_in_vector, get_max_difference_mpi_Size_400) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int vect_size = 400;
    std::vector<int> test_vector(vect_size);

    int res = get_max_difference_mpi(test_vector);
    if (proc_rank == 0)
        ASSERT_EQ(get_max_difference_without_mpi(test_vector), res);
}

TEST(max_diff_in_vector, get_max_difference_mpi_Size_2000) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int vect_size = 2000;
    std::vector<int> test_vector(vect_size);
    int res = get_max_difference_mpi(test_vector);
    if (proc_rank == 0)
        ASSERT_EQ(get_max_difference_without_mpi(test_vector), res);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}

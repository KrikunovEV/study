#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[])
{
	int errCode;

	if ((errCode = MPI_Init(&argc, &argv)) != 0)
	{
		return errCode;
	}

	//cout << "hello world\n";
	
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Status st;
	int N = 3;
	bool* pass = new bool[size];
	int num_pass = 0;
	memset(pass, 0, size);
	pass[rank] = true;

	if (rank + 1 < size)
		MPI_Send(&rank, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
	if (rank - 1 >= 0)
		MPI_Send(&rank, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);

	if (rank + N < size)
		MPI_Send(&rank, 1, MPI_INT, rank + N, 0, MPI_COMM_WORLD);
	if (rank - N >= 0)
		MPI_Send(&rank, 1, MPI_INT, rank - N, 0, MPI_COMM_WORLD);

	if (rank + N*N < size)
		MPI_Send(&rank, 1, MPI_INT, rank + N*N, 0, MPI_COMM_WORLD);
	if (rank - N*N >= 0)
		MPI_Send(&rank, 1, MPI_INT, rank - N*N, 0, MPI_COMM_WORLD);

	int sum = rank;
	while (num_pass != size - 1)
	{
		int got;
		MPI_Recv(&got, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
		
		if (pass[got])
			continue;

		sum += got;
		pass[got] = true;
		num_pass++;

		if (rank + 1 < size)
			MPI_Send(&got, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		if (rank - 1 >= 0)
			MPI_Send(&got, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);

		if (rank + N < size)
			MPI_Send(&got, 1, MPI_INT, rank + N, 0, MPI_COMM_WORLD);
		if (rank - N >= 0)
			MPI_Send(&got, 1, MPI_INT, rank - N, 0, MPI_COMM_WORLD);

		if (rank + N * N < size)
			MPI_Send(&got, 1, MPI_INT, rank + N * N, 0, MPI_COMM_WORLD);
		if (rank - N * N >= 0)
			MPI_Send(&got, 1, MPI_INT, rank - N * N, 0, MPI_COMM_WORLD);
	}

	cout << "num: " << rank + 1 << " sum: " << sum << endl;



	///*int next_rank = rank + 1;
	//if (next_rank == size)
	//	next_rank = 0;

	//int prev_rank = rank - 1;
	//if (rank == 0)
	//	prev_rank = size - 1;

	//double b = a;
	//for (int i = 0; i < size - 1; i++) {
	//	MPI_Send(&b, 1, MPI_DOUBLE, next_rank, 0, MPI_COMM_WORLD);
	//	MPI_Recv(&b, 1, MPI_DOUBLE, prev_rank, 0, MPI_COMM_WORLD, &st);
	//	a += b;
	//}

	//cout << rank << ':' << a << endl;*/

	//if (rank == 0)
	//{
	//	for (int i = 0; i < size-1; i++)
	//	{
	//		double b = 0.0;
	//		MPI_Recv(&b, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &st);
	//		cout << "Got from " << st.MPI_SOURCE << endl;
	//		a += b;
	//	}
	//	//cout << rank << ':' << a << endl;

	//	/*for (int i = 0; i < size - 1; i++)
	//	{
	//		MPI_Send(&a, 1, MPI_DOUBLE, i + 1, 0, MPI_COMM_WORLD);
	//	}*/
	//}
	//else
	//{
	//	MPI_Send(&a, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	//	//MPI_Recv(&a, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &st);
	//	cout << rank << ':' << a << endl;
	//}

	//MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	//cout << rank << ':' << a << endl;

	MPI_Finalize();
	return 0;
}
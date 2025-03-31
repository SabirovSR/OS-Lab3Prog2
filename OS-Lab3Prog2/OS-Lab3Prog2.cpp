#include <windows.h>
#include <stdio.h>

#define NUM_THREADS 20
#define ITERATIONS 100000000

volatile int Var = 0; // ��������� volatile, ����� ���������� �� ������������� ������

DWORD WINAPI IncrementThread(LPVOID lpParam) {
    volatile int* pVar = (volatile int*)lpParam;
    for (int i = 0; i < ITERATIONS; i++) {
        (*pVar)++;
    }
    return 0;
}

DWORD WINAPI DecrementThread(LPVOID lpParam) {
    volatile int* pVar = (volatile int*)lpParam;
    for (int i = 0; i < ITERATIONS; i++) {
        (*pVar)--;
    }
    return 0;
}

int main() {
    HANDLE threads[NUM_THREADS];
    DWORD threadIds[NUM_THREADS];

    printf("Initial value of Var: %d\n", Var);

    for (int i = 0; i < NUM_THREADS; i++) {
        if (i % 2 == 0) {
            threads[i] = CreateThread(
                NULL,            // �������� ������������
                0,               // ������ ����� (�� ���������)
                IncrementThread, // ������� ������
                (LPVOID)&Var,    // �������� (����� Var)
                0,               // ����� ��������
                &threadIds[i]    // ������������� ������
            );
        }
        else {
            threads[i] = CreateThread(
                NULL,
                0,
                DecrementThread,
                (LPVOID)&Var,
                0,
                &threadIds[i]
            );
        }

        if (threads[i] == NULL) {
            printf("CreateThread failed for thread %d \n", i);
            return 1;
        }
    }

    // ������� ���������� ���� �������
    if (WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE) == WAIT_FAILED)
    {
        printf("WaitForSingleObject failed\n");
        return 1;
    }

    // ��������� ����������� �������
    for (int i = 0; i < NUM_THREADS; i++) {
        if (!CloseHandle(threads[i])) {
            printf("CloseHandle failed\n");
            return 1;
        }
    }

    printf("Final value of Var: %d\n", Var);

    return 0;
}
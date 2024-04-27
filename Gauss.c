#include <iostream>
#include <stdlib.h>
using namespace std;


void input(double** m, double** copy_m, int size1, int size2)
{
    int i, j;
    for (i = 0; i < size1; i++)
    {
       cout << "Введите элементы " << i+1 << "-й строки ";
       for (j = 0; j < size2; j++)
       {
           cin >> m[i][j];
           copy_m[i][j] = m[i][j];
        }
    }
}

void output(double** matr, int row, int col)
{
    cout << "\n";
    for (int i = 0; i < row; i++) {
        
        for (int j = 0; j < col; j++)
        {
            cout.width(5);
            cout.setf(ios::left);
            cout << matr[i][j] << "  ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void outputSol(double** matr, int row, int col)
{
    cout << "\n";
    for (int i = 0; i < row; i++) {

        for (int j = 0; j < col; j++)
        {
            cout << "|";
            cout.width(5);
            cout.setf(ios::left);
            cout << matr[i][j] << "|";
            if (i == ((col / 2) + col % 2))
            {
                if (j > 0)
                {

                    cout << "*t" << j;
                    if (j+1 != col) cout << "+";
                }
                else
                {

                    cout << "+   ";
                }
            }
            else
            {
                cout << "    ";
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

void check1(double** matr, double* x, int row, int col)
{
    cout << "Проверка:" << endl;
    double sum;
    for (int i = 0; i < row; i++)
    {
        sum = 0;
        for (int j = 0; j < col; j++)
        {
            sum += matr[i][j] * x[j];
        }
        cout << sum << '=' << matr[i][col] << endl;
    }
}

void check2(double** matr, double** matrSolution, int rowM, int colM, int rowMSolution, int colMSolution)
{
    cout << "Проверка: " << endl;
    double* t, *tt;
    t = new double[colMSolution - 1];
    tt = new double[colM];
    for (int j = 0; j < colMSolution-1; j++)
    {
        cout << "Введите t" << j+1 << ": ";
        cin >> t[j];
        cout << '\n';
    }
    double temp;
    for (int i = 0; i < rowMSolution; i++)
    {
        temp = 0;
        for (int j = 1; j < colMSolution; j++)
        {
            temp += t[j - 1] * matrSolution[i][j];
        }
        temp += matrSolution[i][0];
        tt[i] = temp;
    }
    temp = 0;
    for (int i = 0; i < rowM; i++)
    {
        temp = 0;
        for (int j = 0; j < colM; j++)
        {
            temp += matr[i][j] * tt[j]; //может происходить падение точности из-за дробей: -5.55112e-17 ≈ 0
        }
        cout << temp << '=' << matr[i][colM] << '\n';
    }
    delete[] t;
    delete[] tt;
}

void MatrixCalculation(double** matr, int row, int col, int i ,int k)
{
    double leadElement, matr_ti;
    leadElement = matr[i-k][i];

    for (int j = 0; j < col + 1; j++)
    {
        matr[i-k][j] = matr[i-k][j] / leadElement;
    }
    for (int t = i + 1 - k; t < row; t++)//t-ая строка 
    {
        matr_ti = matr[t][i];
        for (int j = 0; j < col + 1; j++)
        {
            matr[t][j] = matr[t][j] - matr[i-k][j] * matr_ti;
        }
    }
    output(matr, row, col + 1);
}

int Gauss()
{
    double** matr, ** matrCopy;
    double* x, * tmpPoint;
    int row, col, rowFirst, colFirst, * flag, flags = 0;
    do
    {
        cout << "Введите количество строк: ";
        cin >> rowFirst;
        cout << "Введите количество столбцов: ";
        cin >> colFirst;
    } while (rowFirst <= 0 || colFirst <= 0);

    bool rowMore = false;
    bool colMore = false;
    col = colFirst;
    row = rowFirst;

    if (colFirst > rowFirst) {
        colMore = true;
        col = colFirst;
        row = colFirst;
    }
    else if (colFirst < rowFirst) {
        rowMore = true;
    }

    x = (double*)malloc(sizeof(double) * col); //выделение памяти
    flag = (int*)calloc(col, sizeof(int));
    matr = (double**)malloc(sizeof(double*) * row);
    matrCopy = (double**)malloc(sizeof(double*) * row);
    for (int i = 0; i < row; i++)
    {
        matr[i] = (double*)malloc(sizeof(double) * (col + 1));
        matrCopy[i] = (double*)malloc(sizeof(double) * (col + 1));
    }

    input(matr, matrCopy, rowFirst, colFirst);

    cout << "Введите свободные члены уравнения: ";
    for (int i = 0; i < rowFirst; i++)
    {
        cin >> matr[i][col];
        matrCopy[i][col] = matr[i][col];
    }

    if (colMore)//дополнение матрицы нулевыми строчками
    {
        for (int i = rowFirst; i < colFirst; i++)
        {
            for (int j = 0; j < col + 1; j++)
            {
                matr[i][j] = 0;
            }
        }
    }

    int diffirence = 0;
    if (rowMore)
    {
        diffirence = rowFirst - colFirst;
        row = col;
    }

    int k = 0;
    for (int i = 0; i < row; i++) //подсчет матрицы методом Гаусса
    {

        if (matr[i - k][i] != 0)
        {
            MatrixCalculation(matr, row, col, i, k);
        }
        else
        {
            bool other = true;
            for (int j = i - k + 1; j < col; j++) //аналог for-else python
            {
                if (matr[j][i] != 0)
                {
                    tmpPoint = matr[i - k];
                    matr[i - k] = matr[j];
                    matr[j] = tmpPoint;
                    other = false;
                    MatrixCalculation(matr, row, col, i, k);
                    break;
                }
            }
            if (other)
            {
                flag[i] = 1;
                flags += 1;
                k++;
            }
        }
    }

    bool solutionNotExist = false;
    for (int i = 0; i < diffirence; i++)
    {
        if (matr[row + i][col] != 0)
        {
            cout << "Решений нет" << endl;
            solutionNotExist = true;
            break;
        }
    }

    if (flags == 0 && !(solutionNotExist))
    {
        for (int i = row - 1; i >= 0; i--)
        {
            double temp = 0;
            for (int j = i + 1; j < row; j++)
            {
                temp += matr[i][j] * x[j];
            }
            x[i] = (matr[i][col] - temp) / matr[i][i];
        }
        cout << "Решения системы: ";
        for (int i = 0; i < col; i++)
            cout << x[i] << " ";
        cout << "\n\n\n";

        cout << "Исходная матрица:\n";
        output(matrCopy, row, col + 1);

        check1(matrCopy, x, row, col);

    }
    else if (flags > 0 && !(solutionNotExist))
    {
        //проверка на существования решений
        bool solutionExist = true;
        for (int i = 0; i < flags; i++)
        {
            if (matr[row - 1 - i][col] != 0)
            {
                solutionExist = false;
                cout << "Решений нет" << endl;
                break;
            }
        }

        if (solutionExist)
        {
            double** matrSolution; //выделение памяти массива решений
            matrSolution = (double**)malloc(sizeof(double*) * (col));
            for (int i = 0; i < col; i++)
            {
                matrSolution[i] = (double*)(calloc(flags + 1, sizeof(double)));
            }


            int flagsNew = 0;//заполнение t
            for (int j = 0; j < col; j++)
            {
                if (flag[j] == 1)
                {
                    matrSolution[j][flagsNew + 1] = 1;
                    flagsNew++;
                }
            }

            output(matrSolution, col, flags + 1);

            //обратный ход для t 
            double coeff;
            for (int i = row - 1 - flags; i >= 0; i--)
            {
                for (int j = 0; j < col; j++)//нахождение единицы, после выход
                    if (matr[i][j] == 1)
                    {
                        for (int p = j + 1; p < col; p++)
                        {
                            coeff = matr[i][p];
                            for (int x = 0; x < flags + 1; x++)
                            {
                                matrSolution[j][x] += -coeff * matrSolution[p][x];
                            }
                        }
                        matrSolution[j][0] += matr[i][col];
                        output(matrSolution, col, flags + 1);
                        break;
                    }
            }
            cout << "ФСР:" << endl;
            outputSol(matrSolution, col, flags + 1);

            check2(matr, matrSolution, row, col, col, flags + 1);

            if (flags > 0)//очистка памяти 
            {
                for (int i = 0; i < col; i++)
                {
                    free(matrSolution[i]);
                }
                free(matrSolution);
            }
        }
    }

    //очистка памяти 
    for (int i = 0; i < row; i++)
    {
        free(matr[i]);
        free(matrCopy[i]);
    }
    free(matr);
    free(matrCopy);
    free(x);
    free(flag);

    return 0;
}

int main()
{
    setlocale(0, "");
    Gauss();
}

#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <stdlib.h>

using namespace std;

struct ColBox {
    vector<int> Value;
    vector<int> Row;
    vector<int> ColIndex;
    int Len;
    int Num;
};

struct CoordBox {
    vector<int> Value;
    vector<int> Row;
    vector<int> Col;
    int Len; //Размеры матрицы
    int Num; //количество чисел
};

void ReadFromFile(ColBox *col, string path) {
    ifstream inp(path);
    inp >> col->Len;
    inp >> col->Num;
    col->Value.resize(col->Num);
    col->Row.resize(col->Num);
    col->ColIndex.resize(col->Len);
    for (int i = 0; i < col->Num; i++) {
        inp >> col->Value[i];
    }
    for (int i = 0; i < col->Num; i++) {
        inp >> col->Row[i];
    }
    for (int i = 0; i < col->Len; i++) {
        inp >> col->ColIndex[i];
    }
}

void WriteToFile(ColBox *col, string path) {
    ofstream out(path);
    out << col->Len << '\n' << col->Num << '\n';
    for (int i = 0; i < col->Num; i++) {
        out << col->Value[i] << ' ';
    }
    out << '\n';
    for (int i = 0; i < col->Num; i++) {
        out << col->Row[i] << ' ';
    }
    out << '\n';
    for (int i = 0; i < col->Len; i++) {
        out << col->ColIndex[i] << ' ';
    }
    out << '\n';
}

//Перевод в координатный вид(1)
void MakeCoord(CoordBox *coord, ColBox *col) {
  coord->Len = col->Len;
  coord->Num = col->Num;
  coord->Value.resize(col->Num);
  coord->Row.resize(col->Num);
  coord->Col.resize(col->Num);
  for (int i = 0; i < col->Num; i++) {
    coord->Value[i] = col->Value[i];
    coord->Row[i] = col->Row[i];
  }
  for (int j = 0, i = 0; i < coord->Num; i++) {
    if (col->ColIndex[j] == i) {
      coord->Col[i] = j;
      j++;
    } else {
      if (col->ColIndex[j] == col->ColIndex[j - 1]) {
        i--;
        j++;
        continue;
      }
      coord->Col[i] = coord->Col[i - 1];
    }
  }
}

void Rotation(CoordBox *coord_n, CoordBox *coord) {
  coord_n->Len = coord->Len;
  coord_n->Num = coord->Num;
  coord_n->Value.resize(coord->Num);
  coord_n->Row.resize(coord->Num);
  coord_n->Col.resize(coord->Num);
  for (int i = 0; i < coord->Num; i++) {
    coord_n->Value[i] = coord->Value[i];
    coord_n->Col[i] = coord->Row[i];
  }
  for (int i = 0; i < coord->Num; i++) {
    coord_n->Row[i] = coord->Len - (coord->Col[i] + 1);
  }
}

void Sorting(CoordBox *coord_n) {
  int Start, End;
  int save = 0;
  Start = 0;
  End = coord_n->Num;
  while (Start != End) {
    for (int i = Start; i < End - 1; i++) {
      if (coord_n->Col[i] > coord_n->Col[i + 1]) {
        swap(coord_n->Col[i], coord_n->Col[i + 1]);
        swap(coord_n->Row[i], coord_n->Row[i + 1]);
        swap(coord_n->Value[i], coord_n->Value[i + 1]);
        save = i + 1;
      }
    }
    End = save;
    for (int i = End; i != Start; i--) {
      if (coord_n->Col[i] < coord_n->Col[i - 1]) {
        swap(coord_n->Col[i], coord_n->Col[i - 1]);
        swap(coord_n->Row[i], coord_n->Row[i - 1]);
        swap(coord_n->Value[i], coord_n->Value[i - 1]);
        save = i - 1;
      }
    }
    Start = save;
  }
}

void MakeCol(CoordBox *coord, ColBox *col) {
  int len = coord->Len;
  int num = coord->Num;
  vector<int> col_2;
  col_2.resize(len);
  for (int i = 0; i < len; i++) {
    col_2[i] = coord->Col[i];
  }
  vector<int> sup = {0, 0, 0, 0, 0, 0};
  for (int i = 0; i < num; i++) {
    int counter = 0;
    for (int j = i; j < num; j++) {
      if (col_2[j] == col_2[i]) {
        counter++;
      }
    }
    if (sup[col_2[i]] < counter) {
      sup[col_2[i]] = counter;
    }
  }
  for (int i = len - 1; i >= 0; i--) {
    if (sup[i] != 0) {
      sup[i] = 0;
      break;
    }
  }
  vector<int> colIndex_2;
  colIndex_2.resize(len);
  colIndex_2[0] = 0;
  for (int i = 1; i < len; i++) {
    if (sup[i - 1] == 0) {
      colIndex_2[i] = colIndex_2[i - 1];
    } else {
      colIndex_2[i] = colIndex_2[i - 1] + sup[i - 1];
    }
  }
  col->Len = len;
  col->Num = num;
  col->Value.resize(num);
  col->Row.resize(num);
  col->ColIndex.resize(len);
  for (int i = 0; i < num; i++) {
    col->Value[i] = coord->Value[i];
    col->Row[i] = coord->Row[i];
  }
  for (int i = 0; i < len; i++) {
    col->ColIndex[i] = colIndex_2[i];
  }
}

int MemUsed(ColBox *col, CoordBox *coord) {
  int mem = 0;
  mem += sizeof(int) * col->Value.capacity() + sizeof(vector<int>);
  mem += sizeof(int) * col->Row.capacity() + sizeof(vector<int>);
  mem += sizeof(int) * col->ColIndex.capacity() + sizeof(vector<int>);
  mem += sizeof(int) * 2;
  mem += sizeof(int) * coord->Value.capacity() + sizeof(vector<int>);
  mem += sizeof(int) * coord->Row.capacity() + sizeof(vector<int>);
  mem += sizeof(int) * coord->Col.capacity() + sizeof(vector<int>);
  mem += sizeof(int) * 2;
  mem *= 2;
  return mem;
}

void PrintMatrix(CoordBox *coord) {
  int arr[coord->Len][coord->Len];
  for (int i = 0; i < coord->Len; i++) {
    for (int j = 0; j < coord->Len; j++) {
      arr[i][j] = 0;
    }
  }
  for (int i = 0; i < coord->Num; i++) {
    arr[coord->Row[i]][coord->Col[i]] = coord->Value[i];
  }
  for (int i = 0; i < coord->Len; i++) {
    for (int j = 0; j < coord->Len; j++) {
      cout << arr[i][j] << ' ';
    }
    cout << '\n';
  }
}

int main() {
    ColBox *col = new ColBox;
    CoordBox *coord = new CoordBox;
    CoordBox *coord_n = new CoordBox;
    ColBox *col_n = new ColBox;
    ReadFromFile(col, "input.txt");
    auto t1_start = chrono::steady_clock::now();
    MakeCoord(coord, col);
    Rotation(coord_n, coord);
    Sorting(coord_n);
    MakeCol(coord_n, col_n);
    auto t1_end = chrono::steady_clock::now();
    auto t1_duration = chrono::duration_cast<chrono::microseconds>(t1_end - t1_start).count();
    int mem_size = MemUsed(col, coord);
    cout << "Execution time: " << t1_duration << " mksec" << endl;
    cout << "Used memory: " << mem_size << "bytes\n";
    WriteToFile(col_n, "output.txt");
    /*
    _____________________________________________________
    */
    // 1
    cout << "Input data" << '\n';
    for (int i = 0; i < col->Num; i++) {
        cout << col->Value[i] << ' ';
    }
    cout << endl;
    for (int i = 0; i < col->Num; i++) {
        cout << col->Row[i] << ' ';
    }
    cout << endl;
    for (int i = 0; i < col->Len; i++) {
        cout << col->ColIndex[i] << ' ';
    }
    cout << endl;
    cout << '\n';
    PrintMatrix(coord);
    // 4
    cout << endl;
    cout << "Output data" << '\n';
    for (int i = 0; i < col_n->Num; i++) {
        cout << col_n->Value[i] << ' ';
    }
    cout << endl;
    for (int i = 0; i < col_n->Num; i++) {
        cout << col_n->Row[i] << ' ';
    }
    cout << endl;
    for (int i = 0; i < col_n->Len; i++) {
        cout << col_n->ColIndex[i] << ' ';
    }
    cout << endl;
    cout << '\n';
    PrintMatrix(coord_n);
    /*
    _____________________________________________________
    */
    delete col;
    delete coord;
    delete coord_n;
    delete col_n;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "timer.h"
#include "sorter.h"
using namespace std;

void Sorter::fill(int count, string file_name) 
{
    vec.reserve(count);
    vec.clear();
    ifstream file(file_name);
    string line;
    for (int i = 0; i < count && file >> line; i++) {
        vec.push_back(line);
    }
    file.close();
}

void Sorter::print(ostream & out) 
{
    for (unsigned int i = 0; i < vec.size(); i++)
        out << vec.at(i) << " ";
}

bool Sorter::verify_sorted() 
{
    for (unsigned int i = 1; i < vec.size(); ++i) {
        if (vec[i] < vec[i - 1])
            return false;
    }
    return true;
}

void InsertionSorter::insertionsort(vector<string>& vec, int low, int high)
{
    for (int index = 1; index < high; index++) {
        
        string currentvalue = vec[index];
        int position = index;
        
        while (position > low && vec[position - 1] > currentvalue) {
            vec[position] = vec[position - 1];
            position--;
        }
        
        vec[position] = currentvalue;
    }
}

void InsertionSorter::sort() 
{
    InsertionSorter::insertionsort(vec, 0, vec.size());
}

string QuickSorter::select_pivot(vector<string>& vec, int low, int high) 
{
    int mid = low + (high - low) / 2;
    if (vec[mid] < vec[low]) swap(vec[low], vec[mid]);
    if (vec[high] < vec[low]) swap(vec[low], vec[high]);
    if (vec[mid] < vec[high]) swap(vec[mid], vec[high]);
    return vec[high];
}

int QuickSorter::partition(vector<string>& vec, int low, int high) 
{
    string pivot = QuickSorter::select_pivot(vec, low, high);

    for (int j = low; j < high; j++) {
        if (vec[j] <= pivot) {
            swap(vec[low], vec[j]);
            ++low;
        }
    }

    swap(vec[low], vec[high]);
    return low;
}

void QuickSorter::quicksort(vector<string>& vec, int low, int high) 
{
    if (low < high) {
        int pivot_index = QuickSorter::partition(vec, low, high);
        QuickSorter::quicksort(vec, low, pivot_index - 1);
        QuickSorter::quicksort(vec, pivot_index + 1, high);
    }
}

void QuickSorter::sort() 
{
    QuickSorter::quicksort(vec, 0, vec.size() - 1);
}

void HeapSorter::heapify(vector<string>& vec, int high, int root) 
{
    int largest = root;
    int left = 2 * root + 1;
    int right = 2 * root + 2;
    
    if (left < high && vec[left] > vec[largest])
        largest = left;
    
    if (right < high && vec[right] > vec[largest])
        largest = right;
    
    if (largest != root) {
        swap(vec[root], vec[largest]);
        HeapSorter::heapify(vec, high, largest);
    }
}

void HeapSorter::heapsort(vector<string>& vec, int low, int high) 
{
    int size = high;
    
    for (int root = size / 2 - 1; root >= low; --root)
        HeapSorter::heapify(vec, size, root);
    
    for (int end = size - 1; end > low; --end) {
        swap(vec[0], vec[end]);
        heapify(vec, end, 0);
    }
}

void HeapSorter::sort() 
{
    HeapSorter::heapsort(vec, 0, vec.size());
}

void introsort_util(vector<string>& arr, int low, int high, int depth_limit) 
{
    if (high - low < 11) {
        InsertionSorter::insertionsort(arr, low, high);
        return;
    }
    
    if (depth_limit == 0) {
        HeapSorter::heapsort(arr, low, high);
        return;
    }
    
    int p = QuickSorter::partition(arr, low, high - 1);
    introsort_util(arr, low, p - 1, depth_limit - 1);
    introsort_util(arr, p + 1, high, depth_limit - 1);
}

void IntroSorter::introsort(vector<string>& vec, int low, int high) 
{
    int depth_limit = 2 * log(high - low);
    introsort_util(vec, low, high, depth_limit);
}

void IntroSorter::sort() 
{
    IntroSorter::introsort(vec, 0, vec.size());
}

void STLSorter::sort() 
{
    ranges::sort(vec);
}

void StableSorter::sort() 
{
    stable_sort(vec.begin(), vec.end());
}

void ShellSorter::gapInsertionSort(vector<string> & avector, int start, int gap) 
{
    int sz = avector.size();
    for (int i = start + gap; i < sz; i += gap) {
        string currentvalue = avector[i];
        int position = i;
        
        while (position >= gap && avector[position - gap] > currentvalue) {
            avector[position] = avector[position - gap];
            position -= gap;
        }
        avector[position] = currentvalue;
    }
}

void ShellSorter::shellSort(vector<string> & avector) 
{
    int subvectorcount = avector.size() / 2;
    while (subvectorcount > 0) {
        for (int startposition = 0; startposition < subvectorcount; startposition++) {
            ShellSorter::gapInsertionSort(avector, startposition, subvectorcount);
            subvectorcount = subvectorcount / 2;
        }
    }
}

void ShellSorter::sort() 
{
    ShellSorter::shellSort(vec);
}

ostream & operator << (ostream & out, Sorter & L)
{
    L.print(out);
    return out;
}

void error(string word, string msg)
{
    cout << "Error: " << word << " " << msg << endl;
}

void measure_partition(int k, string file_name, Sorter & L)
{
    Timer t;
    double eTime;
    ifstream in(file_name);
    L.fill(k * NWORDS / 10, file_name);
    t.start();
    L.sort();
    t.elapsedUserTime(eTime);
    in.close();
    cout << "\t\tI = " << eTime << endl;      
}

void measure_partitions(string file_name, Sorter & L)
{
    cout << L.name << endl;
    for (int K = 1; K <= 10; ++K) {
        cout << "\tK = " << K << endl;
        measure_partition(K, file_name, L);
        if (!L.verify_sorted())
            error(L.name, "is not sorted correctly");
    }
}

void measure_sorters(string input_file)
{
    InsertionSorter tryingInsert = InsertionSorter();
    QuickSorter tryingQuick = QuickSorter();
    HeapSorter tryingHeap = HeapSorter();
    IntroSorter tryingIntro = IntroSorter();
    STLSorter tryingSTL = STLSorter();
    StableSorter tryingStable = StableSorter();
    measure_partitions(input_file, tryingInsert);
    measure_partitions(input_file, tryingQuick);
    measure_partitions(input_file, tryingHeap);
    measure_partitions(input_file, tryingIntro);
    measure_partitions(input_file, tryingSTL);
    measure_partitions(input_file, tryingStable);
}    
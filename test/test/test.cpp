// Radix Sort in C++ Programming

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

const int MAX_SIZE = 1000;

struct NutrientInfo
{
    string food;
    string measure;
    double grams;
    double calories;
    double protein;
    double fat;
    double satFat;
    double fiber;
    double carbs;
    string category;
};

double stringToDouble(const std::string& str) {
    if (str.empty() || str == "t") {
        return 0.0;
    }
    try {
        return std::stod(str);
    }
    catch (const std::exception& e) {
        return 0.0;
    }
}

void readCsv(const std::string& filename, NutrientInfo* data, int& dataSize) {
    std::ifstream file("C:/Users/khoow/OneDrive - Asia Pacific University/DSTR");
    std::string line;

    // Skip the header line
    std::getline(file, line);

    while (std::getline(file, line) && dataSize < MAX_SIZE) {
        std::istringstream s(line);
        NutrientInfo info;
        std::string temp;

        std::getline(s, info.food, ',');
        std::getline(s, info.measure, ',');       
        
        std::getline(s, temp, ',');
        info.grams = stringToDouble(temp);

        std::getline(s, temp, ',');
        info.calories = stringToDouble(temp);

        std::getline(s, temp, ',');
        info.protein = stringToDouble(temp);

        std::getline(s, temp, ',');
        info.fat = stringToDouble(temp);

        std::getline(s, temp, ',');
        info.satFat = stringToDouble(temp);

        std::getline(s, temp, ',');
        info.fiber = stringToDouble(temp);

        std::getline(s, temp, ',');
        info.carbs = stringToDouble(temp);

        // ... Repeat for other numeric fields

        std::getline(s, info.category);

        data[dataSize++] = info;
    }
}



int getMax(int arr[], int n) {
    int mx = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}

void countSort(int arr[], int n, int exp) {
    int* output = new int[n];
    
    int i, count[10] = { 0 };

    for (i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (i = 0; i < n; i++)
        arr[i] = output[i];

    delete[] output;
}

void radixSort(int arr[], int n) {
    int m = getMax(arr, n);

    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}


void printNutrients(const NutrientInfo* data, int dataSize) {
    for (int i = 0; i < dataSize; ++i) {
        std::cout << ", Calories: " << data[i].calories 
            << "Food: " << data[i].food            
            << ", Measure: " << data[i].measure
            << ", Grams: " << data[i].grams
            << ", Protein: " << data[i].protein
            << ", Fat: " << data[i].fat
            << ", Sat Fat: " << data[i].satFat
            << ", Fiber: " << data[i].fiber
            << ", Carbs: " << data[i].carbs
            << ", Category: " << data[i].category
            << std::endl;
    }
}

int main() {
    NutrientInfo nutrients[MAX_SIZE];
    int dataSize = 0;

    readCsv("Nutrients_Info.csv", nutrients, dataSize);

    int calories[MAX_SIZE];
    int indices[MAX_SIZE];
    for (int i = 0; i < dataSize; ++i) {
        calories[i] = static_cast<int>(nutrients[i].calories);
        indices[i] = i;
    }

    // Sort the indices array based on the calories
    std::sort(indices, indices + dataSize, [&](const int& a, const int& b) {
        return calories[a] < calories[b];
        });

    // Create a temporary array to store the sorted data
    NutrientInfo sortedNutrients[MAX_SIZE];
    for (int i = 0; i < dataSize; ++i) {
        sortedNutrients[i] = nutrients[indices[i]];
    }

    printNutrients(sortedNutrients, dataSize);

    return 0;
}
















/*
// Function to get the largest element from an array
int getMax(int array[], int n) {
    int max = array[0];
    for (int i = 1; i < n; i++)
        if (array[i] > max)
            max = array[i];
    return max;
}

// Using counting sort to sort the elements in the basis of significant places
void countingSort(int array[], int size, int place) {
    const int max = 10;
    int output[size];
    int count[max];

    for (int i = 0; i < max; ++i)
        count[i] = 0;

    // Calculate count of elements
    for (int i = 0; i < size; i++)
        count[(array[i] / place) % 10]++;

    // Calculate cumulative count
    for (int i = 1; i < max; i++)
        count[i] += count[i - 1];

    // Place the elements in sorted order
    for (int i = size - 1; i >= 0; i--) {
        output[count[(array[i] / place) % 10] - 1] = array[i];
        count[(array[i] / place) % 10]--;
    }

    for (int i = 0; i < size; i++)
        array[i] = output[i];
}

// Main function to implement radix sort
void radixsort(int array[], int size) {
    // Get maximum element
    int max = getMax(array, size);

    // Apply counting sort to sort elements based on place value.
    for (int place = 1; max / place > 0; place *= 10)
        countingSort(array, size, place);
}

// Print an array
void printArray(int array[], int size) {
    int i;
    for (i = 0; i < size; i++)
        cout << array[i] << " ";
    cout << endl;
}

// Driver code
int main() {
    int array[] = { 121, 432, 564, 23, 1, 45, 788 };
    int n = sizeof(array) / sizeof(array[0]);
    radixsort(array, n);
    printArray(array, n);
}
*/
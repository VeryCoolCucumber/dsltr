#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <functional>

using namespace std;
using namespace chrono;

const int MAX_SIZE = 1000;

struct NutrientsInfo
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

struct Node {
    NutrientsInfo data;
    Node* next;

    Node(const NutrientsInfo& nutrient) : data(nutrient), next(nullptr) {}
};

struct NutrientsLinkedList {
    Node* head;

    NutrientsLinkedList() : head(nullptr) {}

    ~NutrientsLinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    void push_back(const NutrientsInfo& nutrient) {
        Node* newNode = new Node(nutrient);
        if (head == nullptr) {
            head = newNode;
        }
        else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    void printList() const {
        Node* current = head;
        while (current != nullptr) {
            const NutrientsInfo& nutrient = current->data;
            cout << "Food: " << nutrient.food << endl;
            cout << "Measure: " << nutrient.measure << endl;
            cout << "Grams: " << nutrient.grams << endl;
            cout << "Calories: " << nutrient.calories << endl;
            cout << "Protein: " << nutrient.protein << endl;
            cout << "Fat: " << nutrient.fat << endl;
            cout << "SatFat: " << nutrient.satFat << endl;
            cout << "Fiber: " << nutrient.fiber << endl;
            cout << "Carbs: " << nutrient.carbs << endl;
            cout << "Category: " << nutrient.category << endl;
            cout << endl;

            current = current->next;
        }
    }

};

struct NutrientsArray
{
    NutrientsInfo* nutrients;
    int size;
    int capacity;

    NutrientsArray() : nutrients(nullptr), size(0), capacity(0) {}

    ~NutrientsArray()
    {
        delete[] nutrients;
    }

    void push_back(const NutrientsInfo& nutrient)
    {
        if (size == capacity)
        {
            // Double the capacity if needed
            capacity = (capacity == 0) ? 1 : capacity * 2;
            NutrientsInfo* newNutrients = new NutrientsInfo[capacity];

            // Copy existing data
            for (int i = 0; i < size; ++i)
            {
                newNutrients[i] = nutrients[i];
            }

            // Release old memory
            delete[] nutrients;

            // Update pointer to the new array
            nutrients = newNutrients;
        }

        // Add the new nutrient
        nutrients[size++] = nutrient;
    }
};

typedef bool (*ComparisonFunc)(const NutrientsInfo&, const NutrientsInfo&);

struct ScaledNode {
    int scaledValue;
    Node* nodePtr;
};

void addNode(Node*& head, const NutrientsInfo& info) {
    Node* newNode = new Node{ info };
    if (head == nullptr) {
        head = newNode;
    }
    else {
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;

    }
}

double stringToDouble(const string& str) {
    // Handle known non-numeric placeholders
    if (str.empty() || str == "t" || str == "a") {
        return 0.0;
    }

    // Remove commas, quotes, and other non-numeric characters
    string cleanedStr;
    for (size_t i = 0; i < str.length(); ++i) {
        char ch = str[i];
        if (ch == ',') {
            if (i > 0 && isdigit(str[i - 1]) && i < str.length() - 1 && isdigit(str[i + 1])) {
                continue;
            }
        }

        if (ch != ',' && ch != '\'' && ch != '\"' && ch != ' ') {
            cleanedStr += ch;
        }
    }

    // Attempt conversion to double
    try {
        return stod(cleanedStr);
    }
    catch (const exception& e) {
        return 0.0;
    }
}

ScaledNode* extractData(Node* head, int dataSize, int scale, int sortColumn) {
    ScaledNode* data = new ScaledNode[dataSize];
    Node* current = head;
    for (int i = 0; i < dataSize; ++i) {
        if (sortColumn == 1 || sortColumn == 2 || sortColumn == 10) {
            data[i].scaledValue = i;
        }
        else {
            switch (sortColumn) {
            case 3: data[i].scaledValue = static_cast<int>(current->data.grams * scale);
                break;
            case 4: data[i].scaledValue = static_cast<int>(current->data.calories * scale);
                break;
            case 5: data[i].scaledValue = static_cast<int>(current->data.protein * scale);
                break;
            case 6: data[i].scaledValue = static_cast<int>(current->data.fat * scale);
                break;
            case 7: data[i].scaledValue = static_cast<int>(current->data.satFat * scale);
                break;
            case 8: data[i].scaledValue = static_cast<int>(current->data.fiber * scale);
                break;
            case 9: data[i].scaledValue = static_cast<int>(current->data.carbs * scale);
                break;
            default:
                cout << "Enter a valid selection.";
                break;
            }
        }

        data[i].nodePtr = current;
        current = current->next;
    }
    return data;
}

int getListSize(Node* head) {
    int count = 0;
    Node* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

void printLinkedList(const Node* head) {
    const Node* current = head;
    while (current != nullptr) {
        cout << "Food: " << current->data.food << endl;
        cout << "Measure: " << current->data.measure << endl;
        cout << "Grams: " << current->data.grams << endl;
        cout << "Calories: " << current->data.calories << endl;
        cout << "Protein: " << current->data.protein << endl;
        cout << "Fat: " << current->data.fat << endl;
        cout << "SatFat: " << current->data.satFat << endl;
        cout << "Fiber: " << current->data.fiber << endl;
        cout << "Carbs: " << current->data.carbs << endl;
        cout << "Category: " << current->data.category << endl;
        cout << endl;
        current = current->next;
    }
}

void reconstructList(Node*& head, ScaledNode* data, int dataSize) {
    if (dataSize == 0) return;
    head = data[0].nodePtr;
    Node* current = head;
    for (int i = 1; i < dataSize; ++i) {
        current->next = data[i].nodePtr;
        current = current->next;
    }
    current->next = nullptr;
}

void readCsvToLinkedList(const string& filename, Node*& head) {
    ifstream file(filename);
    string line, temp;

    // Skip the header line
    getline(file, line);

    while (getline(file, line)) {
        istringstream s(line);
        NutrientsInfo info;
        getline(s, info.food, ',');
        getline(s, info.measure, ',');

        getline(s, temp, ',');
        info.grams = stringToDouble(temp);

        getline(s, temp, ',');
        info.calories = stringToDouble(temp);

        getline(s, temp, ',');
        info.protein = stringToDouble(temp);

        getline(s, temp, ',');
        info.fat = stringToDouble(temp);

        getline(s, temp, ',');
        info.satFat = stringToDouble(temp);

        getline(s, temp, ',');
        info.fiber = stringToDouble(temp);

        getline(s, temp, ',');
        info.carbs = stringToDouble(temp);

        getline(s, info.category);
        addNode(head, info);
    }
}

void readCsvToArray(const string& filename, NutrientsInfo* data, int& dataSize) {
    ifstream file(filename);
    string line;

    // Skip the header line
    getline(file, line);

    while (getline(file, line) && dataSize < MAX_SIZE) {
        istringstream s(line);
        NutrientsInfo info;
        string temp;

        getline(s, info.food, ',');
        getline(s, info.measure, ',');

        getline(s, temp, ',');
        info.grams = stringToDouble(temp);

        getline(s, temp, ',');
        info.calories = stringToDouble(temp);

        getline(s, temp, ',');
        info.protein = stringToDouble(temp);

        getline(s, temp, ',');
        info.fat = stringToDouble(temp);

        getline(s, temp, ',');
        info.satFat = stringToDouble(temp);

        getline(s, temp, ',');
        info.fiber = stringToDouble(temp);

        getline(s, temp, ',');
        info.carbs = stringToDouble(temp);

        getline(s, info.category);

        data[dataSize++] = info;
    }
}

//DATA CLEANING
bool isNumeric(const string& str)
{
    string temp = str;
    // Remove commas from numeric string
    temp.erase(remove(temp.begin(), temp.end(), ','), temp.end());
    istringstream iss(temp);
    double d;
    return iss >> d >> ws && iss.eof();
}

void trimWhitespace(string& str)
{
    str.erase(0, str.find_first_not_of(" \t\r\n"));
    str.erase(str.find_last_not_of(" \t\r\n") + 1);
}

void removeQuotes(string& str) {
    if (!str.empty() && str.front() == '\"') {
        str.erase(0, 1);
    }
    if (!str.empty() && str.back() == '\"') {
        str.erase(str.size() - 1, 1);
    }
}

void removeCommaFromNumbers(string& str) {
    str.erase(remove(str.begin(), str.end(), ','), str.end());
}

double cleanAndConvertToDouble(const string& str)
{
    string cleanedStr = str;
    removeCommaFromNumbers(cleanedStr);
    try {
        return stod(cleanedStr);
    }
    catch (const exception&) {
        return 0.0;
    }

    if (str.empty() || str == "t" || str == "a")
    {
        return 0.0; // Handle empty or special cases
    }
    else if (str.find('/') != string::npos || isalpha(str[0])) // Check for '/' or an alphabetical character
    {
        return -1.0; // Return a default value for dates or non-numeric values
    }
    else
    {
        // Remove commas and convert to double
        string cleanedStr = str;
        cleanedStr.erase(remove(cleanedStr.begin(), cleanedStr.end(), ','), cleanedStr.end());
        return stod(cleanedStr);
    }
}

void split(const string& str, char delim, string out[], int maxFields) {
    stringstream ss(str);
    string token;
    int fieldCount = 0;

    while (getline(ss, token, delim) && fieldCount < maxFields) {
        out[fieldCount++] = token;
    }
}

void addNutrientToArray(NutrientsInfo* array, NutrientsInfo& nutrient, int& size, int capacity) {
    if (size < capacity) {
        array[size++] = nutrient;
    }
    else {
        cerr << "Array is full." << endl;
    }
}


void preprocessData(NutrientsInfo* data, int n) {
    for (int i = 0; i < n; i++) {
        if (data[i].protein < 0) {
            data[i].protein = 0;  // Set negative protein values to 0
        }
    }
}

void replaceCategory(NutrientsInfo arr[], int n) {
    for (int i = 0; i < n; i++) {
        if (arr[i].category == "t") {  // Check if the category is 't'
            arr[i].category = "undefined";  // Replace with "undefined"
        }
    }
}

void replaceCharacterInCategory(NutrientsLinkedList& list) {
    Node* current = list.head;
    while (current != nullptr) {
        // Check if the category is a single character 't'
        if (current->data.category.length() == 1 && current->data.category[0] == 't') {
            current->data.category = "undefined";
        }
        current = current->next;
    }
}

void replaceNegativeProteinWithZero(NutrientsLinkedList& list) {
    Node* current = list.head;
    while (current != nullptr) {
        // Check if the protein value is negative
        if (current->data.protein < 0) {
            current->data.protein = 0;
        }
        current = current->next;
    }
}
//DATA CLEANING




//SORT ARRAY

double getCalories(const NutrientsInfo& item) {
    return item.calories;
}

double getGrams(const NutrientsInfo& item) {
    return item.grams;
}

double getProtein(const NutrientsInfo& item) {
    return item.protein;
}

double getFat(const NutrientsInfo& item) {
    return item.fat;
}

double getSatFat(const NutrientsInfo& item) {
    return item.satFat;
}

double getFiber(const NutrientsInfo& item) {
    return item.fiber;
}

double getCarbs(const NutrientsInfo& item) {
    return item.carbs;
}

double getMaxValue(const NutrientsInfo* data, int n, function<double(const NutrientsInfo&)> getValue) {
    double maxValue = getValue(data[0]);
    for (int i = 1; i < n; i++) {
        double value = getValue(data[i]);
        if (value > maxValue) {
            maxValue = value;
        }
    }
    return maxValue;
}

int getMaxStringLength(NutrientsInfo arr[], int n, string NutrientsInfo::* field) {
    int maxLength = 0;
    for (int i = 0; i < n; i++) {
        maxLength = max(maxLength, static_cast<int>((arr[i].*field).length()));
    }
    return maxLength;
}

bool extractLeadingNumber(const std::string& str, double& number) {
    std::istringstream iss(str);
    iss >> number;

    return !iss.fail() && !iss.eof();
}

void countSortString(NutrientsInfo arr[], int n, int exp, string NutrientsInfo::* field) {
    vector<NutrientsInfo> output(n);
    int count[256] = { 0 };  // Extended ASCII, including '\0'

    // Count occurrence of each character at exp position
    for (int i = 0; i < n; i++) {
        int index = (arr[i].*field).length() - exp - 1;
        char ch = index >= 0 ? (arr[i].*field)[index] : '\0';
        count[static_cast<unsigned char>(ch)]++;
    }

    // Accumulate count array
    for (int i = 1; i < 256; i++) {
        count[i] += count[i - 1];
    }

    // Build output array
    for (int i = n - 1; i >= 0; i--) {
        int index = (arr[i].*field).length() - exp - 1;
        char ch = index >= 0 ? (arr[i].*field)[index] : '\0';
        output[count[static_cast<unsigned char>(ch)] - 1] = arr[i];
        count[static_cast<unsigned char>(ch)]--;
    }

    // Copy the output array to arr
    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }
}

void radixSortString(NutrientsInfo arr[], int n, string NutrientsInfo::* field) {
    int maxLength = getMaxStringLength(arr, n, field);

    for (int exp = 0; exp < maxLength; exp++) {
        countSortString(arr, n, exp, field);
    }
}

void countSortNumbers(NutrientsInfo* data, int n, int exp, function<double(const NutrientsInfo&)> getFieldValue) {
    vector<NutrientsInfo> output(n);
    int count[10] = { 0 };

    for (int i = 0; i < n; i++) {
        int index = static_cast<int>(getFieldValue(data[i]) / exp) % 10;
        count[index]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int index = static_cast<int>(getFieldValue(data[i]) / exp) % 10;
        output[count[index] - 1] = data[i];
        count[index]--;
    }

    for (int i = 0; i < n; i++) {
        data[i] = output[i];
    }
}

void radixSortNumbers(NutrientsInfo* data, int n, function<double(const NutrientsInfo&)> getFieldValue) {
    double m = getMaxValue(data, n, getFieldValue);

    for (int exp = 1; m / exp > 0; exp *= 10) {
        countSortNumbers(data, n, exp, getFieldValue);
    }
}

void radixSortByMeasure(NutrientsInfo arr[], int n) {
    vector<NutrientsInfo> withNumber, withoutNumber;

    for (int i = 0; i < n; i++) {
        double num;
        if (extractLeadingNumber(arr[i].measure, num)) {
            withNumber.push_back(arr[i]);
        }
        else {
            withoutNumber.push_back(arr[i]);
        }
    }

    int sizeWithNumber = withNumber.size();
    int sizeWithoutNumber = withoutNumber.size();


    // Sort the 'withNumber' array numerically
    radixSortNumbers(withNumber.data(), sizeWithNumber, [](const NutrientsInfo& item) {
        double num;
        extractLeadingNumber(item.measure, num);
        return num;
        });

    // Sort the 'withoutNumber' array alphabetically

    radixSortString(withoutNumber.data(), sizeWithoutNumber, &NutrientsInfo::measure);


    // Merge the arrays
    // ...

    // Merge the arrays back into arr
    int withIndex = 0, withoutIndex = 0, arrIndex = 0;
    while (withIndex < sizeWithNumber && withoutIndex < sizeWithoutNumber) {
        if (withNumber[withIndex].measure.compare(withoutNumber[withoutIndex].measure) < 0) {
            arr[arrIndex++] = withNumber[withIndex++];
        }
        else {
            arr[arrIndex++] = withoutNumber[withoutIndex++];
        }
    }
    // Copy any remaining elements
    while (withIndex < sizeWithNumber) {
        arr[arrIndex++] = withNumber[withIndex++];
    }
    while (withoutIndex < sizeWithoutNumber) {
        arr[arrIndex++] = withoutNumber[withoutIndex++];
    }
}

void mergeArrays(NutrientsInfo* arr, int left, int middle, int right) {
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Create temporary arrays
    NutrientsInfo* leftArr = new NutrientsInfo[n1];
    NutrientsInfo* rightArr = new NutrientsInfo[n2];

    // Copy data to temporary arrays leftArr[] and rightArr[]
    for (int i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = arr[middle + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    int i = 0; // Initial index of left subarray
    int j = 0; // Initial index of right subarray
    int k = left; // Initial index of merged subarray

    while (i < n1 && j < n2) {
        if (leftArr[i].calories <= rightArr[j].calories) {
            arr[k] = leftArr[i];
            i++;
        }
        else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of leftArr[], if there are any
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Copy the remaining elements of rightArr[], if there are any
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }

    // Free the allocated memory
    delete[] leftArr;
    delete[] rightArr;
}

// Merge sort function for array
void mergeSortArray(NutrientsInfo* arr, int left, int right) {
    if (left < right) {
        // Same as (left + right) / 2, but avoids overflow for large left and right
        int middle = left + (right - left) / 2;

        // Sort first and second halves
        mergeSortArray(arr, left, middle);
        mergeSortArray(arr, middle + 1, right);

        // Merge the sorted halves
        mergeArrays(arr, left, middle, right);
    }
}

Node* mergeListsByColumn(Node* left, Node* right, int sortColumn);

// Merge sort function for linked list
Node* mergeSortLinkedList(Node* head, int sortColumn) {
    if (head == nullptr || head->next == nullptr)
        return head;

    // Find the middle of the list
    Node* middle = nullptr;
    Node* fast = head;
    Node* slow = head;

    while (fast != nullptr && fast->next != nullptr) {
        middle = slow;
        slow = slow->next;
        fast = fast->next->next;
    }

    // Split the list into two halves
    Node* left = head;
    Node* right = middle->next;
    middle->next = nullptr;

    // Recursively sort the two halves
    left = mergeSortLinkedList(left, sortColumn);
    right = mergeSortLinkedList(right, sortColumn);

    // Merge the sorted halves
    return mergeListsByColumn(left, right, sortColumn);
}

bool compareNodes(Node* left, Node* right, int sortColumn);

// Merge function for linked list based on chosen column
Node* mergeListsByColumn(Node* left, Node* right, int sortColumn) {
    Node* result = nullptr;

    if (left == nullptr)
        return right;
    if (right == nullptr)
        return left;

    // Compare values and merge
    if (compareNodes(left, right, sortColumn)) {
        result = left;
        result->next = mergeListsByColumn(left->next, right, sortColumn);
    }
    else {
        result = right;
        result->next = mergeListsByColumn(left, right->next, sortColumn);
    }

    return result;
}



bool compareNodes(Node* left, Node* right, int sortColumn) {
    double value1, value2;

    switch (sortColumn) {
    case 1: // Food
        value1 = left->data.food.compare(right->data.food);
        value2 = 0.0;  // Dummy value, as it won't be used
        break;
    case 2: // Measure
        value1 = left->data.measure.compare(right->data.measure);
        value2 = 0.0;
        break;
    case 3: // Grams
        value1 = left->data.grams;
        value2 = right->data.grams;
        break;
    case 4: // Calories
        value1 = left->data.calories;
        value2 = right->data.calories;
        break;
        // Add cases for other columns as needed
    default:
        value1 = 0.0;
        value2 = 0.0;
        break;
    }

    return value1 <= value2;
}

void bubbleSortArrayByColumn(NutrientsInfo* data, int dataSize, int sortColumn) {
    for (int i = 0; i < dataSize - 1; ++i) {
        for (int j = 0; j < dataSize - i - 1; ++j) {
            // Compare elements based on the chosen column
            double value1, value2;

            switch (sortColumn) {
            case 1: // Food
                value1 = data[j].food.compare(data[j + 1].food);
                value2 = 0.0;  // Dummy value, as it won't be used
                break;
            case 2: // Measure
                value1 = data[j].measure.compare(data[j + 1].measure);
                value2 = 0.0;
                break;
            case 3: // Grams
                value1 = data[j].grams;
                value2 = data[j + 1].grams;
                break;
            case 4: // Calories
                value1 = data[j].calories;
                value2 = data[j + 1].calories;
                break;
                // Add cases for other columns as needed
            default:
                value1 = 0.0;
                value2 = 0.0;
                break;
            }

            if (value1 > value2) {
                // Swap data[j] and data[j + 1]
                NutrientsInfo temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}

void bubbleSortLinkedList(Node*& head, int sortColumn) {
    if (head == nullptr || head->next == nullptr) {
        // Empty or single-node list is already sorted
        return;
    }

    bool swapped;
    Node* current;
    Node* lastSorted = nullptr;

    do {
        swapped = false;
        current = head;

        while (current->next != lastSorted) {
            // Compare elements based on the chosen column
            double value1, value2;

            switch (sortColumn) {
            case 1: // Food
                value1 = current->data.food.compare(current->next->data.food);
                value2 = 0.0;  // Dummy value, as it won't be used
                break;
            case 2: // Measure
                value1 = current->data.measure.compare(current->next->data.measure);
                value2 = 0.0;
                break;
            case 3: // Grams
                value1 = current->data.grams;
                value2 = current->next->data.grams;
                break;
            case 4: // Calories
                value1 = current->data.calories;
                value2 = current->next->data.calories;
                break;
                // Add cases for other columns as needed
            default:
                value1 = 0.0;
                value2 = 0.0;
                break;
            }

            if (value1 > value2) {
                // Swap data of adjacent nodes
                NutrientsInfo temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;

                swapped = true;
            }
            current = current->next;
        }

        // Mark the last swapped node as the new lastSorted
        lastSorted = current;
    } while (swapped);
}


//LINKED LIST

int getMaxValueLinked(const NutrientsLinkedList& list, function<int(const NutrientsInfo&)> getFieldFunc) {
    if (list.head == nullptr) return 0;

    int maxValue = getFieldFunc(list.head->data);
    Node* current = list.head->next;

    while (current != nullptr) {
        maxValue = max(maxValue, getFieldFunc(current->data));
        current = current->next;
    }

    return maxValue;
}

int getDigit(int number, int exp) {
    return (number / exp) % 10;
}

// Function to perform counting sort on the linked list for a specific digit
void countingSortForRadixLinked(NutrientsLinkedList& list, int exp, function<int(const NutrientsInfo&)> getDigitFunc) {
    Node* output[10] = { nullptr };  // Array of pointers for each digit
    Node* last[10] = { nullptr };
    Node* current = list.head;

    // Counting occurrences of digits and creating new linked lists
    while (current != nullptr) {
        int index = getDigitFunc(current->data);
        if (output[index] == nullptr) {
            output[index] = current;
        }
        else {
            last[index]->next = current;
        }
        last[index] = current;
        current = current->next;
    }

    // Now combine the linked lists back into the original list
    Node* newList = nullptr;
    Node* lastNode = nullptr;
    for (int i = 0; i < 10; i++) {
        if (output[i] != nullptr) {
            if (newList == nullptr) {
                newList = output[i];
            }
            else {
                lastNode->next = output[i];
            }
            lastNode = last[i];
        }
    }
    if (lastNode != nullptr) {
        lastNode->next = nullptr; // Ensure the last node points to nullptr
    }
    list.head = newList; // Set the head of the list to the new list
}

// Radix Sort function
void radixSortLinkedList(NutrientsLinkedList& list, function<int(const NutrientsInfo&, int)> getDigitFunc, int maxDigit) {
    for (int exp = 1; maxDigit / exp > 0; exp *= 10)
        countingSortForRadixLinked(list, exp, [exp, &getDigitFunc](const NutrientsInfo& item) -> int {
        return getDigitFunc(item, exp);
            });
}


int getMaxStringLengthLinkedList(Node* head, function<string(const NutrientsInfo&)> getStringField) {
    int maxLength = 0;
    Node* current = head;
    while (current != nullptr) {
        maxLength = max(maxLength, static_cast<int>(getStringField(current->data).length()));
        current = current->next;
    }
    return maxLength;
}

void countingSortForRadixLinkedStrings(NutrientsLinkedList& list, int exp, function<char(const NutrientsInfo&)> getCharFunc) {
    // First, calculate the size of the linked list
    int size = 0;
    Node* node = list.head;
    while (node != nullptr) {
        size++;
        node = node->next;
    }

    vector<NutrientsInfo> output(size); // Now use the calculated size
    int count[256] = { 0 }; // ASCII characters

    Node* current = list.head;
    while (current != nullptr) {
        char ch = getCharFunc(current->data);
        count[static_cast<unsigned char>(ch)]++;
        current = current->next;
    }

    for (int i = 1; i < 256; i++)
        count[i] += count[i - 1];

    current = list.head;
    while (current != nullptr) {
        char ch = getCharFunc(current->data);
        output[count[static_cast<unsigned char>(ch)] - 1] = current->data;
        count[static_cast<unsigned char>(ch)]--;
        current = current->next;
    }

    current = list.head;
    for (const auto& item : output) {
        current->data = item;
        current = current->next;
    }
}

void radixSortLinkedListStrings(NutrientsLinkedList& list, function<string(const NutrientsInfo&)> getStringField) {
    int maxLength = getMaxStringLengthLinkedList(list.head, getStringField);

    for (int exp = maxLength - 1; exp >= 0; exp--) {
        countingSortForRadixLinkedStrings(list, exp, [exp, &getStringField](const NutrientsInfo& item) -> char {
            string str = getStringField(item);
            if (exp < str.length()) {
                return str[exp];
            }
            return '\0'; // For strings shorter than the current position
            });
    }
}


void columnSortMenu() {
    cout << "Select a column to sort by: " << endl;
    cout << "1. Food\n";
    cout << "2. Measure\n";
    cout << "3. Grams\n";
    cout << "4. Calories\n";
    cout << "5. Protein\n";
    cout << "6. Fat\n";
    cout << "7. Saturated Fat\n";
    cout << "8. Fiber\n";
    cout << "9. Carbs\n";
    cout << "10. Category\n";
}

int main()
{
    string filename = "C:/Users/Asus/OneDrive - Asia Pacific University/Documents/Degree Year 2/Sem 2/Data Structure(DSTR)/Case Study #1 - 20231220/Nutrients_Info 1.csv";

    const int maxSize = 1000;
    int menuChoice, sortChoice, columnChoice, maxLength;
    NutrientsLinkedList nutrientsList;
    NutrientsInfo nutrientsArray[maxSize];
    NutrientsInfo data[MAX_SIZE];
    int dataSize = 0;
    readCsvToArray(filename, data, dataSize);
    int n = 0;
    ifstream file("C:/Users/khoow/OneDrive - Asia Pacific University/DSTR/Nutrients_Info 1.csv");

    if (!file.is_open())
    {
        cerr << "Error opening file: Nutrients_Info 1.csv" << endl;
        return 1; // Return an error code
    }

    string line;
    // Skip the header line if it exists
    getline(file, line);

    while (getline(file, line))
    {
        const int fields = 10;
        string tokens[fields];
        split(line, ',', tokens, fields);

        if (tokens[0].empty() || tokens[fields - 1].empty())
        {
            cerr << "Error in line: " << line << " - Incorrect number of fields" << endl;
            continue;
        }

        try
        {
            NutrientsInfo nutrient;
            removeQuotes(tokens[0]);
            trimWhitespace(tokens[0]); // Trim leading and trailing whitespaces

            nutrient.food = tokens[0];

            removeQuotes(tokens[1]);
            nutrient.measure = tokens[1];

            nutrient.grams = cleanAndConvertToDouble(tokens[2]);
            nutrient.calories = cleanAndConvertToDouble(tokens[3]);
            nutrient.protein = cleanAndConvertToDouble(tokens[4]);
            nutrient.fat = cleanAndConvertToDouble(tokens[5]);

            // Handle cases where Sat.Fat is 't' or 'a' or null
            nutrient.satFat = cleanAndConvertToDouble(tokens[6]);

            // Handle cases where Fiber and Carbs are 't' or 'a' or null
            nutrient.fiber = cleanAndConvertToDouble(tokens[7]);
            nutrient.carbs = cleanAndConvertToDouble(tokens[8]);

            removeQuotes(tokens[9]);
            trimWhitespace(tokens[9]); // Trim leading and trailing whitespaces
            nutrient.category = tokens[9];

            replaceCategory(nutrientsArray, n);
            preprocessData(nutrientsArray, n);
            addNutrientToArray(nutrientsArray, nutrient, n, maxSize);
            replaceCharacterInCategory(nutrientsList);
            replaceNegativeProteinWithZero(nutrientsList);
            nutrientsList.push_back(nutrient);
        }
        catch (const exception& e)
        {
            cerr << "Error parsing line: " << line << endl;
            cerr << "Exception details: " << e.what() << endl;
            // Skip the line if there is an error in parsing
            continue;
        }
    }

    file.close();

    while (true) {
        cout << "Choose data storage method:\n1. Array\n2. Linked List\n3. Exit\n";
        cin >> menuChoice;

        if (menuChoice == 3) {
            cout << "Program exited." << endl;
            break; // Exit the program
        }

        cout << "Choose sorting algorithm:\n1. Radix Sort\n2. Merge Sort\n3. Bubble Sort\n4. Return to Main Menu\n";
        cin >> sortChoice;

        cout << "Choose which columns to sort: " << endl;
        columnSortMenu();
        cin >> columnChoice;

        if (menuChoice == 1) {
            auto start = chrono::high_resolution_clock::now();

            if (sortChoice == 1) {
                switch (columnChoice) {
                case 1:
                    radixSortString(nutrientsArray, n, &NutrientsInfo::food);
                    break;
                case 2:

                    radixSortByMeasure(nutrientsArray, n);
                    break;
                case 3:
                    radixSortNumbers(nutrientsArray, n, [](const NutrientsInfo& item) {return item.grams; });
                    break;
                case 4:
                    radixSortNumbers(nutrientsArray, n, [](const NutrientsInfo& item) {return item.calories; });
                    break;
                case 5:
                    radixSortNumbers(nutrientsArray, n, [](const NutrientsInfo& item) {return item.protein; });
                    break;
                case 6:
                    radixSortNumbers(nutrientsArray, n, [](const NutrientsInfo& item) {return item.fat; });
                    break;
                case 7:
                    radixSortNumbers(nutrientsArray, n, [](const NutrientsInfo& item) {return item.satFat; });
                    break;
                case 8:
                    radixSortNumbers(nutrientsArray, n, [](const NutrientsInfo& item) {return item.fiber; });
                    break;
                case 9:
                    radixSortNumbers(nutrientsArray, n, [](const NutrientsInfo& item) {return item.carbs; });
                    break;
                case 10:
                    radixSortString(nutrientsArray, n, &NutrientsInfo::category);
                    break;
                }
            }


            if (sortChoice == 2) {
                mergeSortArray(data, 0, dataSize - 1);  // Modify this function to sort NutrientInfo array
                cout << "Merge Sorting Algorithm" << endl;
            }

            if (sortChoice == 3) {
                bubbleSortArrayByColumn(data, dataSize, columnChoice); // Add this line for Bubble Sort
                cout << "Bubble Sorting Algorithm" << endl;
            }

            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> elapsed = end - start;

            cout << "Sorting time: " << elapsed.count() << " ms" << endl;
        }

        else if (menuChoice == 2) {
            Node* head = nullptr;
            readCsvToLinkedList(filename, head);
            auto start = chrono::high_resolution_clock::now();

            if (sortChoice == 1) {
                int maxDigit;
                switch (columnChoice) {
                case 1:
                    radixSortLinkedListStrings(nutrientsList, [](const NutrientsInfo& item) {
                        return item.food;
                        });
                    break;
                case 2:
                    radixSortLinkedListStrings(nutrientsList, [](const NutrientsInfo& item) {
                        return item.measure;
                        });
                    break;
                case 3:
                    maxDigit = getMaxValueLinked(nutrientsList, [](const NutrientsInfo& item) {
                        return item.grams; });
                    radixSortLinkedList(nutrientsList, [&](const NutrientsInfo& item, int exp) {
                        return getDigit(item.grams, exp);
                        }, maxDigit);
                    break;
                case 4:
                    maxDigit = getMaxValueLinked(nutrientsList, [](const NutrientsInfo& item) {
                        return item.calories; });
                    radixSortLinkedList(nutrientsList, [&](const NutrientsInfo& item, int exp) {
                        return getDigit(item.calories, exp);
                        }, maxDigit);
                    break;
                case 5:
                    maxDigit = getMaxValueLinked(nutrientsList, [](const NutrientsInfo& item) {
                        return item.protein; });
                    radixSortLinkedList(nutrientsList, [&](const NutrientsInfo& item, int exp) {
                        return getDigit(item.protein, exp);
                        }, maxDigit);
                    break;
                case 6:
                    maxDigit = getMaxValueLinked(nutrientsList, [](const NutrientsInfo& item) {
                        return item.fat; });
                    radixSortLinkedList(nutrientsList, [&](const NutrientsInfo& item, int exp) {
                        return getDigit(item.fat, exp);
                        }, maxDigit);
                    break;
                case 7:
                    maxDigit = getMaxValueLinked(nutrientsList, [](const NutrientsInfo& item) {
                        return item.satFat; });
                    radixSortLinkedList(nutrientsList, [&](const NutrientsInfo& item, int exp) {
                        return getDigit(item.satFat, exp);
                        }, maxDigit);
                    break;
                case 8:
                    maxDigit = getMaxValueLinked(nutrientsList, [](const NutrientsInfo& item) {
                        return item.fiber; });
                    radixSortLinkedList(nutrientsList, [&](const NutrientsInfo& item, int exp) {
                        return getDigit(item.fiber, exp);
                        }, maxDigit);
                    break;
                case 9:
                    maxDigit = getMaxValueLinked(nutrientsList, [](const NutrientsInfo& item) {
                        return item.carbs; });
                    radixSortLinkedList(nutrientsList, [&](const NutrientsInfo& item, int exp) {
                        return getDigit(item.carbs, exp);
                        }, maxDigit);
                    break;
                case 10:
                    radixSortLinkedListStrings(nutrientsList, [](const NutrientsInfo& item) {
                        return item.category;
                        });
                    break;
                }
            }

            else if (sortChoice == 2) {
                head = mergeSortLinkedList(head, columnChoice);
                cout << "Merge Sorting Algorithm" << endl;
            }

            else if (sortChoice == 3) {
                bubbleSortLinkedList(head, columnChoice); // Add this line for Bubble Sort
                cout << "Bubble Sorting Algorithm" << endl;
            }
            else {
                int dataSize = getListSize(head);
                int scale = 100;
                ScaledNode* scaledData = extractData(head, dataSize, scale, columnChoice);

                reconstructList(head, scaledData, dataSize); // Reconstruct the linked list
                delete[] scaledData; // Don't forget to free the memory
            }
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> elapsed = end - start;


            cout << "Sorting time: " << elapsed.count() << " ms" << endl;
        }
    }

    return 0;
};
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>  


using namespace std;
using namespace chrono;

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

struct Node {
    NutrientInfo data;
    Node* next;
    
};

struct ScaledNode {
    int scaledValue;
    Node* nodePtr;
};

void addNode(Node*& head, const NutrientInfo& info) {
    Node* newNode = new Node{ info, nullptr };
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

int getMaxStringLength(Node* head, int column) {
    int maxLength = 0;
    Node* current = head;
    while (current != nullptr) {
        int currentLength = 0;
        if (column == 1) currentLength = current->data.food.length();
        else if (column == 2) currentLength = current->data.measure.length();
        else if (column == 10) currentLength = current->data.category.length();
        maxLength = max(maxLength, currentLength);
        current = current->next;
    }
    return maxLength;
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


void sortLinkedListByString(Node*& head, int sortColumn) {
    vector<NutrientInfo> data;
    Node* current = head;
    while (current != nullptr) {
        data.push_back(current->data);
        current = current->next;
    }

    if (sortColumn == 1) {  // Assuming 1 is for sorting by 'food'
        sort(data.begin(), data.end(), [](const NutrientInfo& a, const NutrientInfo& b) {
            return a.food < b.food;
            });
    }
    else if (sortColumn == 2) {
        sort(data.begin(), data.end(), [](const NutrientInfo& a, const NutrientInfo& b) {
            return a.measure < b.measure;
            });
    }
    else if (sortColumn == 10) {
        sort(data.begin(), data.end(), [](const NutrientInfo& a, const NutrientInfo& b) {
            return a.category < b.category;
            });
    }
    // Add similar conditions for other string columns

    head = nullptr;
    for (const auto& item : data) {
        addNode(head, item);
    }
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


void readCsvToLinkedList(const string& filename, Node*& head) {
    ifstream file(filename);
    string line, temp;

    // Skip the header line
    getline(file, line);

    while (getline(file, line)) {
        istringstream s(line);
        NutrientInfo info;
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

void countSortString(Node*& head, int exp, int maxLength, int column) {
    vector<Node*> output(getListSize(head));
    Node* current = head;
    int count[256] = { 0 };  // Extended ASCII

    while (current != nullptr) {
        string str = (column == 1) ? current->data.food :
            (column == 2) ? current->data.measure :
            current->data.category;
        int index = (exp < str.length()) ? (str[str.length() - 1 - exp]) : 0;
        count[index]++;
        current = current->next;
    }

    for (int i = 1; i < 256; i++)
        count[i] += count[i - 1];

    current = head;
    while (current != nullptr) {
        string str = (column == 1) ? current->data.food :
            (column == 2) ? current->data.measure :
            current->data.category;
        int index = (exp < str.length()) ? (str[str.length() - 1 - exp]) : 0;
        output[count[index] - 1] = current;
        count[index]--;
        current = current->next;
    }

    head = output[0];
    current = head;
    for (size_t i = 1; i < output.size(); i++) {
        current->next = output[i];
        current = current->next;
    }
    current->next = nullptr;
}

void radixSortStrings(Node*& head, int column) {
    int maxLength = getMaxStringLength(head, column);
    for (int exp = 0; exp < maxLength; exp++) {
        countSortString(head, exp, maxLength, column);
    }
}

void countSortCalorieNode(ScaledNode* data, int n, int exp) {
    vector<ScaledNode> output(n);
    int i, count[10] = { 0 };

    // Count occurrences
    for (i = 0; i < n; i++)
        count[(data[i].scaledValue / exp) % 10]++;

    // Change count[i] to actual position
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // Build the output array
    for (i = n - 1; i >= 0; i--) {
        output[count[(data[i].scaledValue / exp) % 10] - 1] = data[i];
        count[(data[i].scaledValue / exp) % 10]--;
    }

    // Copy the output array to data[]
    for (i = 0; i < n; i++)
        data[i] = output[i];
}

void radixSortForLinkedList(ScaledNode* data, int n) {
    // Find the maximum number
    int m = data[0].scaledValue;
    for (int i = 1; i < n; i++)
        if (data[i].scaledValue > m)
            m = data[i].scaledValue;

    // Perform counting sort for every digit
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSortCalorieNode(data, n, exp);
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



// ARRAY

void readCsvToArray(const string& filename, NutrientInfo* data, int& dataSize) {
    ifstream file(filename);
    string line;

    // Skip the header line
    getline(file, line);

    while (getline(file, line) && dataSize < MAX_SIZE) {
        istringstream s(line);
        NutrientInfo info;
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



int getMaxCalories(const NutrientInfo* data, int n) {
    double maxCalories = data[0].calories;
    for (int i = 1; i < n; i++) {
        if (data[i].calories > maxCalories) {
            maxCalories = data[i].calories;
        }
    }
    return static_cast<int>(maxCalories);
}

void countSortForNutrientInfo(NutrientInfo* data, int n, int exp) {
    vector<NutrientInfo> output(n);
    int count[10] = { 0 };

    for (int i = 0; i < n; i++) {
        int index = static_cast<int>(data[i].calories) / exp % 10;
        count[index]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int index = static_cast<int>(data[i].calories) / exp % 10;
        output[count[index] - 1] = data[i];
        count[index]--;
    }

    for (int i = 0; i < n; i++) {
        data[i] = output[i];
    }
}

void radixSortForArray(NutrientInfo* data, int n) {
    int m = getMaxCalories(data, n);

    for (int exp = 1; m / exp > 0; exp *= 10) {
        countSortForNutrientInfo(data, n, exp);
    }
}


void printArray(const NutrientInfo* data, int dataSize) {

    for (int i = 0; i < dataSize; ++i) {
        cout << "Food: " << data[i].food << endl;
        cout << "Measure: " << data[i].measure << endl;
        cout << "Grams: " << data[i].grams << endl;
        cout << "Calories: " << data[i].calories << endl;
        cout << "Protein: " << data[i].protein << endl;
        cout << "Fat: " << data[i].fat << endl;
        cout << "Sat Fat : " << data[i].satFat << endl;
        cout << "Fiber: " << data[i].fiber << endl;
        cout << "Carbs: " << data[i].carbs << endl;
        cout << "Category: " << data[i].category << endl;
        cout << endl;
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

//Merge sorting algorithm
// Merge function for array
void mergeArrays(NutrientInfo* arr, int left, int middle, int right) {
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Create temporary arrays
    NutrientInfo* leftArr = new NutrientInfo[n1];
    NutrientInfo* rightArr = new NutrientInfo[n2];

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
void mergeSortArray(NutrientInfo* arr, int left, int right) {
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

// Function to compare nodes based on the chosen column
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

// Bubble sort function for array
void bubbleSortArrayByColumn(NutrientInfo* data, int dataSize, int sortColumn) {
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
                NutrientInfo temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}


// Bubble sort function for linked list
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
                NutrientInfo temp = current->data;
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


int main() {

    int menuChoice, sortChoice, columnChoice;
    string filename = "C:/Users/Asus/OneDrive - Asia Pacific University/Documents/Degree Year 2/Sem 2/Data Structure(DSTR)/Case Study #1 - 20231220/Nutrients_Info 1.csv";

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


        if (sortChoice == 4) {
            continue; // Return to the main menu
        }

        // Implement your sorting and data handling logic here
        if (menuChoice == 1) {
            // Handle array
            NutrientInfo data[MAX_SIZE];
            int dataSize = 0;
            readCsvToArray(filename, data, dataSize);

            auto start = chrono::high_resolution_clock::now();


            if (sortChoice == 1) {                
                radixSortForArray(data, dataSize); // Modify this function to sort NutrientInfo array
                cout << "Radix Sorting Algorithm" << endl;
            }

            else if (sortChoice == 2) {
                mergeSortArray(data, 0, dataSize - 1);  // Modify this function to sort NutrientInfo array
                cout << "Merge Sorting Algorithm" << endl;
            }

            else if (sortChoice == 3) {
                bubbleSortArrayByColumn(data, dataSize, columnChoice); // Add this line for Bubble Sort
                cout << "Bubble Sorting Algorithm" << endl;
            }
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> elapsed = end - start;

            printArray(data, dataSize);
            
            cout << "Sorting time: " << elapsed.count() << " ms" << endl;
        }
        else if (menuChoice == 2) {
            // Handle linked list
            Node* head = nullptr;
            readCsvToLinkedList(filename, head);
            auto start = chrono::high_resolution_clock::now();

            if (sortChoice == 1 && (columnChoice == 1 || columnChoice == 2 || columnChoice == 10)) {
                sortLinkedListByString(head, columnChoice);  // Function to sort linked list of strings
                cout << "Radix Sorting Algorithm" << endl;
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
                radixSortForLinkedList(scaledData, dataSize); // Sort CalorieNode array
                reconstructList(head, scaledData, dataSize); // Reconstruct the linked list
                delete[] scaledData; // Don't forget to free the memory
            }

            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> elapsed = end - start;

            printLinkedList(head);
            cout << "Sorting time: " << elapsed.count() << " ms" << endl;
        }
        
        // Implement a way to pause and display results before returning to the menu
        cout << "Press Enter to return to the menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
                
    }

    // Clean up resources if necessary

    return 0;
    
}


//
// Created by Haya on 11/27/2023.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>

using namespace std;

const double M = 1.2345; //average

struct Node { //type of items in excel sheet
    string date;
    double change;

    Node(string d, double c){ date = d; change = c;}
};

void Build_Max_Heap(vector<Node>& nodes, int n, int i) { //build max heap from slides
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && nodes[left].change > nodes[largest].change)
        largest = left;

    if (right < n && nodes[right].change > nodes[largest].change)
        largest = right;

    if (largest != i) {
        swap(nodes[i], nodes[largest]);
        Build_Max_Heap(nodes, n, largest);
    }
}

void Build_Min_Heap(vector<Node>& nodes, int n, int i) { //build min heap from slides
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && nodes[left].change < nodes[smallest].change)
        smallest = left;

    if (right < n && nodes[right].change < nodes[smallest].change)
        smallest = right;

    if (smallest != i) {
        swap(nodes[i], nodes[smallest]);
        Build_Min_Heap(nodes, n, smallest);
    }
}

void FindExtremes(vector<Node>& data, int N) { //finding the most positive and most negative changes
    int size = data.size();

    // Max heap for N highest changes
    for (int i = size / 2 - 1; i >= 0; i--)
        Build_Max_Heap(data, size, i);

    cout << "Top " << N << " positive changes:" << endl;
    for (int i = 0; i < N; i++) {
        cout << "Date: " << data[i].date << ", Change: " << data[i].change << endl;
    }

    // Min heap for N lowest changes
    for (int i = size / 2 - 1; i >= 0; i--)
        Build_Min_Heap(data, size, i);

    cout << "\nTop " << N << " negative changes:" << endl;
    for (int i = 0; i < N; i++) {
        cout << "Date: " << data[i].date << ", Change: " << data[i].change << endl;
    }
}

// Function to find the maximum subsequence sum
void findMaxSubsequenceSum(const vector<Node>& nodes) { //the contiguous period over which the sum of rate changes is maximum using the max subsequence sum algorithm from slides
    double maxSum = 0;
    double currentSum = 0;
    int start = 0;
    int end = 0;
    int temp = 0;

    for (int i = 0; i < nodes.size(); i++) {
        currentSum += nodes[i].change;

        if (currentSum < 0) {
            currentSum = 0;
            temp = i + 1;
        }

        if (currentSum > maxSum) {
            maxSum = currentSum;
            start = temp;
            end = i;
        }
    }

    cout << "Maximum Subsequence Sum:" <<endl;
    cout << "Start Date: " << nodes[start].date << ", End Date: " << nodes[end].date << endl;
    cout << "Sum: " << maxSum << endl;
}

int main() {
    string line;

    ifstream file("exchange rates.csv");
    if (!file.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return 1;
    }

    vector<Node> nodes;
    while (getline(file, line)) { //reading the dates & changes into the heap
        stringstream ss(line);
        string date, eRate;

        getline(ss, date, ',');
        getline(ss, eRate, ',');

        double rate = stod(eRate);
        double change = M - rate;

        Node newNode(date, change);

        nodes.push_back(newNode); //populating the vector
    }

    file.close();

    int N;
    cout << "Enter the amount of exchange rate changes (positive or negative) that you'd like to get the dates for:"
         << endl;
    cin >> N;

    // open the file for append
    ofstream outFile("analysis and results.txt", ios::app);
    if (!outFile.is_open()) {
        cerr << "Error opening the output file." << endl;
        return 1;
    }

    cout.rdbuf(outFile.rdbuf());  // redirect cout to the file stream

    FindExtremes(nodes, N);
    findMaxSubsequenceSum(nodes);

    outFile.close();

    cout.rdbuf(cout.rdbuf());  // reset cout to the original stream buffer

    return 0;
}